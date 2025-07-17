#include "error.hpp"

CowangError::CowangError(
    std::string name,
    std::string where,
    std::string resume,
    std::vector<std::string> details
) noexcept: name(name), where(where), resume(resume), details(details) {}

const char* CowangError::what() const noexcept {
    cached_what = (!name.empty()) ? std::format("[{} ERROR]", name) : "[COWANG ERROR]";

    if(!where.empty()) cached_what += std::format(" (in {})", where);
    if(!resume.empty()) cached_what += (" " + resume);

    if(!details.empty()) {
        cached_what += ":";
        for(const std::string& det : details)
            cached_what += ("\n\t" + det);
    }

    return cached_what.c_str();
}

CowangErrorSyntax::CowangErrorSyntax(
    std::string where,
    std::string resume,
    std::vector<std::string> details
) noexcept: CowangError("SYNTAX", where, resume, details) {}

CowangErrorUnexpectedToken::CowangErrorUnexpectedToken(
    std::string where,
    std::string expected,
    std::string encountered
) noexcept: CowangErrorSyntax(where, "Unexpected token", {
    std::format("expected: {}", expected),
    std::format("but {} was encountered", encountered) 
}) {}