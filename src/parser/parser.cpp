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
    while(this->next.type != TokenType::END_OF_FILE) {
        Token stmt = this->ExpressionStatement();
        if(stmt.type != TokenType::END_OF_FILE) body.push_back(stmt);
    }
    return body;
}

Token Parser::ExpressionStatement() {
    if(this->next.type == TokenType::END_OF_FILE)
        return this->next;
    Token statement = this->Statement("expression statement");
    this->consumeToken(TokenType::SEMI_COLON);
    return statement;
}

Token Parser::Statement(const std::string& where) {
    return this->Literal(where);
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

Token Parser::advance(const std::string& where) {
    return this->consumeToken(this->next.type, where);
}