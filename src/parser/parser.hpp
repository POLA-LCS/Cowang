#pragma once
#include "../lexer/lexer.hpp"
#include "../error/error.hpp"

struct Parser {
    Lexer lexer;
    Token next;

    Parser(const std::string& data) noexcept;

    Meta parse();
    Meta Program();

    Token ExpressionStatement();
    Token Statement(const std::string& where);

    Token Literal(const std::string& where);
    Token LiteralInteger(const std::string& where);
    Token LiteralFloat(const std::string& where);
    Token LiteralString(const std::string& where);

    Token consumeToken(TokenType token_type, const std::string& where = "");
    Token advance(const std::string& where = "");
};