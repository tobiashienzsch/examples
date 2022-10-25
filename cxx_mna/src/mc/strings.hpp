#pragma once

#include <string>

namespace mc {
// trim from start (in place)
auto ltrim(std::string& s) -> void;

// trim from end (in place)
auto rtrim(std::string& s) -> void;

// trim from both ends (in place)
auto trim(std::string& s) -> void;
}  // namespace mc