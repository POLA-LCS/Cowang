#include "src/error/error.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include <iostream>
#include <fstream>

std::string get_file_content(const char* path) {
    std::ifstream file(path);
    if(!file.is_open()) {
        throw CowangError("INPUT", "", std::string("File doesn't exists: ") + path, {});
    }

    std::string buffer;
    std::getline(file, buffer, (char)EOF);
    return buffer;
}

void display_help() {
    std::printf("Cowang v1.0 2025 [https://github.com/POLA-LCS/powang]\n\n");
    std::printf("USAGE:\n");
    std::printf("    cowang <input.co>       Interprets the input.\n");
    std::printf("    cowang (--help | -h)    Displays this message.\n");
}

int main(int argc, char* argv[]) {
    std::string file_content;

    try {
        if(argc == 1) {
            display_help();
            return 0;
        }
        
        if(argc > 2) {
            throw CowangError("INPUT", "arguments", "Too many arguments, see \"cowang --help\"", {});
        }
        
        file_content = get_file_content(argv[1]);
    } catch(const CowangError& error) {
        printf("%s\n", error.what());
    }

    Parser parser(file_content);
    
    try {
        Meta parsed_program = parser.parse(); 
        for(const Token& statement : parsed_program) {
            std::cout << std::format("{}\n", statement.toString());
        }
    } catch(const CowangError& error) {
        printf("ln: %i | %s\n", parser.lexer.lines.size(), error.what());
    }
    
    return 0;
}
