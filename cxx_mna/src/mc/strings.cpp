#include "strings.hpp"

namespace mc::strings {
// trim from start (in place)
auto ltrim(std::string& s) -> void
{
    auto isNotSpace = [](auto ch) { return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), isNotSpace));
}

// trim from end (in place)
auto rtrim(std::string& s) -> void
{
    auto isNotSpace = [](auto ch) { return !std::isspace(ch); };
    s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace).base(), s.end());
}

// trim from both ends (in place)
auto trim(std::string& s) -> void
{
    ltrim(s);
    rtrim(s);
}
}  // namespace mc::strings
