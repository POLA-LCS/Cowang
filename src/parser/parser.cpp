#include "parser.hpp"

// ====== CONSTRUCTOR =========

Parser::Parser(const std::string& data) noexcept {
    this->lexer = Lexer(data);
    this->next = this->lexer.getNextToken();
}

// ====== PROGRAM =========

Meta Parser::parse() {
    return this->Program();
}

Meta Parser::Program() {
    Meta body;
    while(this->hasNext()) {
        Token stmt = this->ExpressionStatement();
        if(stmt.type != TokenType::END_OF_FILE) body.push_back(stmt);
    }
    return body;
}

bool Parser::hasNext() const noexcept {
    return this->next.type != TokenType::END_OF_FILE;
}

Token Parser::ExpressionStatement() {
    if(!this->hasNext())
        return this->next;
    while(this->next.type == TokenType::SEMI_COLON)
        this->advanceToken();
    Token statement = this->Statement("expression statement");
    this->consumeToken(TokenType::SEMI_COLON);
    return statement;
}

Token Parser::Statement(const std::string& where) {
    Token expr = this->Expression(where);
    return expr;
}

Token Parser::Expression(const std::string& where) {
    Token expr = this->Level_0_Expression(where);
    return expr;
}

Token Parser::Level_0_Expression(const std::string& where) {
    return this->binaryExpression(where, [this](const std::string& where) {
        return this->Level_2_Expression(where);
    }, {
        TokenType::AND,
        TokenType::OR
    });
}

Token Parser::Level_2_Expression(const std::string& where) {
    return this->binaryExpression(where, [this](const std::string& where) {
        return this->Level_3_Expression(where);
    }, {
        TokenType::PLUS,
        TokenType::MINUS,
    });
}

Token Parser::Level_3_Expression(const std::string& where) {
    return this->binaryExpression(where, [this](const std::string& where) {
        return this->Level_4_Expression(where);
    }, {
        TokenType::STAR,
        TokenType::SLASH,
    });
}

Token Parser::Level_4_Expression(const std::string& where) {
    return this->binaryExpression(where, [this](const std::string& where) {
        return this->UnaryExpression(where);
    }, {
        TokenType::CAST_AS
    });
}

Token Parser::binaryExpression(const std::string& where, std::function<Token(const std::string&)> next_level, std::vector<TokenType> operators) {
    Token left = next_level(where);
    while(this->hasNext() && std::find(operators.begin(), operators.end(), this->next.type) != operators.end()) {
        Token bin_operator = this->advanceToken();
        bool typed = false;
        if(this->next.type == TokenType::COLON) {
            this->advanceToken();
            typed = true;
        }
        Token right = next_level(where);
        left = Token((typed)
        ? TokenType::P_TYPED_BINARY_EXPRESSION
        : TokenType::P_BINARY_EXPRESSION, {
            left,
            bin_operator,
            right,
        });
    }
    return left;
}

Token Parser::UnaryExpression(const std::string& where) {
    std::vector<TokenType> unaryOperators = {
        TokenType::MINUS
    };
    if(std::find(unaryOperators.begin(), unaryOperators.end(), this->next.type) != unaryOperators.end()) {
        Token unary_operator = this->advanceToken();
        Token right = this->UnaryExpression(where);
        return Token(TokenType::P_UNARY_EXPRESSION, {
            unary_operator,
            right
        });
    }
    return this->PostfixExpression(where);
}

Token Parser::PostfixExpression(const std::string& where) {
    Token primary_expr = this->PrimaryExpression(where);
    return primary_expr;
}

Token Parser::PrimaryExpression(const std::string& where) {
    switch(this->next.type) {
        case TokenType::IDENTIFIER: {
            Token identifier = this->Identifier(where);
            if(this->next.type == TokenType::COLON) {
                this->advanceToken();
                return this->VariableDeclaration(where, identifier);
            }
            return identifier;
        }
        case TokenType::LEFT_PAREN:
            return this->ParenthesizedExpression(where);
        default: break;
    }
        // case TokenType::LEFT_BRACKET:
        //     return this->ContainerExpression()
        // case TokenType::LEFT_BRACE:
        //     return this->BlockStatement()
    return this->Literal("primary expression");
}

Token Parser::VariableDeclaration(const std::string& where, const Token& identifier) {
    /*
    `DECLARATION_TYPED_VAR => id: type = expr`  
    `DECLARATION_UNDEFINED => id: type`  
    `DECLARATION_INTERPRET => id := expr`  
    */
    if(this->next.type == TokenType::ASSIGN) {
        this->advanceToken();
        Token expression = this->Expression(where);
        return Token(TokenType::P_DECLARATION_INTERPRET, {
            identifier,
            expression,
        });
    }

    Token type = this->Identifier(where);
    if(this->next.type == TokenType::ASSIGN) {
        this->advanceToken();
        Token expression = this->Expression(where);
        return Token(TokenType::P_DECLARATION_TYPED_VAR, {
            identifier,
            type,
            expression,
        });
    }

    return Token(TokenType::P_DECLARATION_UNDEFINED, {
        identifier,
        type,
    });
}

Token Parser::ParenthesizedExpression(const std::string& where) {
    this->consumeToken(TokenType::LEFT_PAREN);
    Token expr = this->Expression(where);
    this->consumeToken(TokenType::RIGHT_PAREN);
    return expr;
}


// ====== WORDS =========
Token Parser::Identifier(const std::string& where) {
    Token identifier = this->consumeToken(TokenType::IDENTIFIER, where);
    return identifier;
}

// ====== LITERALS =========

Token Parser::Literal(const std::string& where) {
    switch(this->next.type) {
        case TokenType::LITERAL_INTEGER:
            return this->LiteralInteger(where);
        case TokenType::LITERAL_FLOAT:
            return this->LiteralFloat(where);
        case TokenType::LITERAL_STRING:
            return this->LiteralString(where);
        default:
            throw CowangErrorUnexpectedToken(where, "literal", TokenToString(this->next.type));
    }
}

Token Parser::LiteralInteger(const std::string& where) {
    Token integer = this->consumeToken(TokenType::LITERAL_INTEGER, where);
    return integer;
}

Token Parser::LiteralFloat(const std::string& where) {
    Token floating = this->consumeToken(TokenType::LITERAL_FLOAT, where);
    return floating;
}

Token Parser::LiteralString(const std::string& where) {
    Token string = this->consumeToken(TokenType::LITERAL_STRING, where);
    return string;
}

// ====== CONSUME =========

Token Parser::consumeToken(TokenType expect, const std::string& where) {
    Token token = this->next;
    if(token.type == TokenType::END_OF_FILE) {
        std::vector<std::string> lines;
        if(!this->lexer.lines.empty())
            lines.push_back(this->lexer.lines.back());
        throw CowangErrorSyntax(where, "Unexpected end of file", lines);
    }
    if(token.type != expect) {
        throw CowangErrorUnexpectedToken(where, TokenToString(expect), TokenToString(token.type));
    }
    this->next = this->lexer.getNextToken();
    return token;
}

Token Parser::advanceToken() noexcept {
    return this->consumeToken(this->next.type, "");
}