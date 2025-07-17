#pragma once

#include <string>
#include <regex>
#include <vector>
#include "patterns.hpp"

enum class TokenType {
    // WHITESPACE
    WHITESPACE,

    // LEXER
    END_OF_FILE,
    LITERAL_INTEGER,
    LITERAL_FLOAT,
    LITERAL_STRING,
    IDENTIFIER,

    // Punctuations
    DOT,           // .
    COMMA,         // ,
    COLON,         // :
    SEMI_COLON,    // ;
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACKET,  // {
    RIGHT_BRACKET, // }
    LEFT_BRACE,    // [
    RIGHT_BRACE,   // ]

    ASSIGN,  // =
    // Operators
        // boolean
        AND, // &&
        OR,  // ||
    PLUS,    // +
    MINUS,   // -
    STAR,    // *
    SLASH,   // /
    CAST_AS, // as

    // PARSER
    P_BINARY_EXPRESSION,
    P_TYPED_BINARY_EXPRESSION,
    P_UNARY_EXPRESSION,
    P_DECLARATION_INTERPRET,
    P_DECLARATION_UNDEFINED,
    P_DECLARATION_TYPED_VAR,
};

inline std::string TokenToString(TokenType type) noexcept {
    switch(type) {
        case TokenType::WHITESPACE     : return "<whitespace>"   ;
        case TokenType::END_OF_FILE    : return "end of file"    ;
        case TokenType::LITERAL_INTEGER: return "literal integer";
        case TokenType::LITERAL_FLOAT  : return "literal float"  ;
        case TokenType::LITERAL_STRING : return "literal string" ;
        case TokenType::IDENTIFIER     : return "identifier"     ;
        case TokenType::DOT            : return "dot"            ;
        case TokenType::COMMA          : return "comma"          ;
        case TokenType::COLON          : return "colon"          ;
        case TokenType::SEMI_COLON     : return "semi colon"     ;
        case TokenType::LEFT_PAREN     : return "left paren"     ;
        case TokenType::RIGHT_PAREN    : return "right paren"    ;
        case TokenType::LEFT_BRACKET   : return "left bracket"   ;
        case TokenType::RIGHT_BRACKET  : return "right bracket"  ;
        case TokenType::LEFT_BRACE     : return "left brace"     ;
        case TokenType::RIGHT_BRACE    : return "right brace"    ;
        case TokenType::ASSIGN         : return "assign"         ;
        case TokenType::AND            : return "and"            ;
        case TokenType::OR             : return "or"             ;
        case TokenType::PLUS           : return "plus"           ;
        case TokenType::MINUS          : return "minus"          ;
        case TokenType::STAR           : return "star"           ;
        case TokenType::SLASH          : return "slash"          ;
        case TokenType::CAST_AS        : return "cast as"        ;
        // PARSER
        case TokenType::P_BINARY_EXPRESSION       : return "binary expression"      ;
        case TokenType::P_TYPED_BINARY_EXPRESSION : return "typed binary expression";
        case TokenType::P_UNARY_EXPRESSION        : return "unary expression"       ;
        case TokenType::P_DECLARATION_INTERPRET   : return "declaration interpret"  ;
        case TokenType::P_DECLARATION_UNDEFINED   : return "declaration undefined"  ;
        case TokenType::P_DECLARATION_TYPED_VAR   : return "declaration typed var"  ;
    }
    return "<UNKNOWN TOKEN TYPE>";
}

struct Token;
typedef std::vector<Token> Meta;

struct Token {
    TokenType type;
    std::string value;
    Meta meta;

    inline Token() = default;
    inline Token(TokenType type, std::string value = "") noexcept: type(type), value(value) {}
    inline Token(TokenType type, Meta meta) noexcept: type(type), meta(meta) {}

    inline std::string toString() const noexcept {
        return "<" + TokenToString(type) + ": " + value + ">";
    }
};

struct Lexer {
    std::string code;
    size_t pos;

    // Error tracking lines
    std::vector<std::string> lines;
    // Error tracking line accumulator
    std::string line_record;

    inline Lexer() = default;
    Lexer(std::string code) noexcept;

    bool hasMoreTokens() noexcept;
    const char& actualChar() noexcept;
    Token getNextToken();
};

inline const std::vector<std::pair<std::regex, TokenType>> token_patterns = {
    {std::regex(r_whitespace), TokenType::WHITESPACE},

    {std::regex(r_string)    , TokenType::LITERAL_STRING },
    {std::regex(r_float)     , TokenType::LITERAL_FLOAT  },
    {std::regex(r_integer)   , TokenType::LITERAL_INTEGER},

    // Punctuations
    {std::regex(R"(\.)"), TokenType::DOT          },
    {std::regex(R"(\,)"), TokenType::COMMA        },
    {std::regex(R"(\:)"), TokenType::COLON        },
    {std::regex(R"(\;)"), TokenType::SEMI_COLON   },
    {std::regex(R"(\()"), TokenType::LEFT_PAREN   },
    {std::regex(R"(\))"), TokenType::RIGHT_PAREN  },
    {std::regex(R"(\{)"), TokenType::LEFT_BRACKET },
    {std::regex(R"(\})"), TokenType::RIGHT_BRACKET},
    {std::regex(R"(\[)"), TokenType::LEFT_BRACE   },
    {std::regex(R"(\])"), TokenType::RIGHT_BRACE  },

    // Boolean operators
    {std::regex(R"(&&)"  ), TokenType::AND},
    {std::regex(R"(\|\|)"), TokenType::OR },

    // Operators
    {std::regex(R"(=)"     ), TokenType::ASSIGN },
    {std::regex(R"(\+)"    ), TokenType::PLUS   },
    {std::regex(R"(-)"     ), TokenType::MINUS  },
    {std::regex(R"(\*)"    ), TokenType::STAR   },
    {std::regex(R"(\/)"    ), TokenType::SLASH  },
    {std::regex(R"(\sas\s)"), TokenType::CAST_AS},

    // IDENTIFIER
    {std::regex(r_identifier), TokenType::IDENTIFIER},
};