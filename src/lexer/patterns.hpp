#pragma once

#include <string>
#include <vector>
#include <regex>

// Special regex strings
inline const std::string r_whitespace   = R"(\s+)";
inline const std::string r_comment      = R"(##.*)";

// Individual regex strings
inline const std::string r_string        = R"("([^"\\]|\\.)*")";
inline const std::string r_float         = R"(\d+\.\d+)";
inline const std::string r_integer       = R"(\d+)";
inline const std::string r_identifier    = R"([A-Za-z_]\w*)";