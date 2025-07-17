#pragma once

#include <stdexcept>
#include <exception>
#include <vector>
#include <format>

struct CowangError: std::exception {
    std::string name;
    std::string where;
    std::string resume;
    std::vector<std::string> details = {};
    mutable std::string cached_what;

    CowangError(
        std::string name,
        std::string where,
        std::string resume,
        std::vector<std::string> details = {}
    ) noexcept;

    const char* what() const noexcept override;
};

struct CowangErrorSyntax: CowangError {
    CowangErrorSyntax(
        std::string where,
        std::string resume,
        std::vector<std::string> details = {}
    ) noexcept;
};

struct CowangErrorUnexpectedToken: CowangErrorSyntax {
    CowangErrorUnexpectedToken(
        std::string where,
        std::string expected,
        std::string provided
    ) noexcept;
};