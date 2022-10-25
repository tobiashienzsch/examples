#pragma once

#include <string>

namespace mc::strings {
// trim from start (in place)
auto ltrim(std::string& s) -> void;

// trim from end (in place)
auto rtrim(std::string& s) -> void;

// trim from both ends (in place)
auto trim(std::string& s) -> void;

auto toUpper(std::string& str) -> void;

}  // namespace mc::strings