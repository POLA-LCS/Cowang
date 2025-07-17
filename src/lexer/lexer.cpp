#include "lexer.hpp"
#include "patterns.hpp"
#include "../error/error.hpp"

bool Lexer::hasMoreTokens() noexcept {
    return this->pos < this->code.length();
}

Lexer::Lexer(std::string code) noexcept {
    this->code = std::regex_replace(code, std::regex(r_comment), "");
    this->pos = 0;
}

const char& Lexer::actualChar() noexcept {
    return this->code[pos];
}

Token Lexer::getNextToken() {
    while(this->hasMoreTokens() && this->actualChar() == '\n') {
        this->lines.push_back(line_record);
        this->line_record.clear();
        pos++;
    }

    if(!this->hasMoreTokens())
        return Token{TokenType::END_OF_FILE};

    std::string segment(this->code.data() + pos, this->code.size() - pos);

    std::smatch match;
    for(const auto& [regex, tok_type] : token_patterns) {
        if(std::regex_search(segment, match, regex, std::regex_constants::match_continuous)) {
            std::string matched_string = match.str();
            line_record += matched_string;
            pos += match.length();

            if(tok_type == TokenType::WHITESPACE) {
                return this->getNextToken();
            }

            if(tok_type == TokenType::LITERAL_STRING) {
                return Token{tok_type, matched_string.substr(1, matched_string.length() - 2)};
            }

            return Token{tok_type, matched_string};
        }
    }

    int cursor_pos = pos - (pos - lines.back().length());
    lines.push_back(line_record + segment);
    throw CowangErrorSyntax("", "Unknown token was encountered", {
        lines.back(),
        std::string('~', cursor_pos - 1) + '^'
    });
}