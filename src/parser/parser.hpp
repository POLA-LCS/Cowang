#pragma once
#include "../lexer/lexer.hpp"
#include "../error/error.hpp"
#include <functional>
#include <algorithm>

struct Parser {
    Lexer lexer;
    Token next;

    Parser(const std::string& data) noexcept;

    Meta parse();
    Meta Program();
    bool hasNext() const noexcept;

    // EXPRESSIONS

    Token ExpressionStatement();
    Token Statement(const std::string&);
    Token Expression(const std::string&);
    Token ParenthesizedExpression(const std::string&);
    
    // Binary expressions

    Token Level_0_Expression(const std::string&);
    Token Level_1_Expression(const std::string&);
    Token Level_2_Expression(const std::string&);
    Token Level_3_Expression(const std::string&);
    Token Level_4_Expression(const std::string&);
    Token binaryExpression(const std::string&,
        std::function<Token(const std::string&)> next_level,
        std::vector<TokenType> operators
    );

    // Expressions

    Token UnaryExpression(const std::string&);
    Token PostfixExpression(const std::string&);
    Token PrimaryExpression(const std::string&);

    Token VariableDeclaration(const std::string&, const Token& identifier);

    // WORDS

    Token Identifier(const std::string&);    

    // LITERALS

    Token Literal(const std::string&);
    Token LiteralInteger(const std::string&);
    Token LiteralFloat(const std::string&);
    Token LiteralString(const std::string&);

    // CONSUME

    Token consumeToken(TokenType token_type, const std::string& = "");
    Token advanceToken() noexcept;
};