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
        
    // Operators
    ASSIGN, // =
    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    SLASH,  // /

    // PARSER
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
        case TokenType::PLUS           : return "plus"           ;         
        case TokenType::MINUS          : return "minus"          ;        
        case TokenType::STAR           : return "star"           ;        
        case TokenType::SLASH          : return "slash"          ;         
    }
    return "<UNKNOWN TOKEN TYPE>";
}

struct Token;
typedef std::vector<Token> Meta;

struct Token {
    TokenType type;
    std::string value;
    Meta meta;
    
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
    {std::regex(r_string)    , TokenType::LITERAL_STRING},
    {std::regex(r_float)     , TokenType::LITERAL_FLOAT},
    {std::regex(r_integer)   , TokenType::LITERAL_INTEGER},
    {std::regex(r_identifier), TokenType::IDENTIFIER},

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

    // Operators
    {std::regex(R"(=)"), TokenType::ASSIGN},
    {std::regex(R"(\+)"), TokenType::PLUS  },
    {std::regex(R"(-)"), TokenType::MINUS },
    {std::regex(R"(\*)"), TokenType::STAR  },
    {std::regex(R"(\/)"), TokenType::SLASH },
};