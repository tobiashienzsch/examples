#include "strings.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>

namespace mc::strings {

auto ltrim(std::string& s) -> void
{
    auto isNotSpace = [](auto ch) { return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), isNotSpace));
}

auto rtrim(std::string& s) -> void
{
    auto isNotSpace = [](auto ch) { return !std::isspace(ch); };
    s.erase(std::find_if(s.rbegin(), s.rend(), isNotSpace).base(), s.end());
}

auto trim(std::string& s) -> void
{
    ltrim(s);
    rtrim(s);
}

auto toUpper(std::string& str) -> void
{
    std::transform(begin(str), end(str), begin(str), ::toupper);
}
}  // namespace mc::strings
