#pragma once

#include <mc/stream.hpp>
#include <mc/strings.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace mc::detail {

inline auto parseSpiceNumber(std::string const& str) -> double
{
    struct Unit
    {
        std::string_view suffix;
        double multiplier;
    };

    static constexpr auto table = std::array<Unit, 10>{
        Unit{  "T", 1e+12}, //   Tera
        Unit{  "G", 1e+09}, //   Giga
        Unit{  "X", 1e+06}, //   Mega
        Unit{"MEG", 1e+06}, //   Mega
        Unit{  "K", 1e+03}, //   Kilo
        Unit{  "M", 1e-03}, //   Milli
        Unit{  "U", 1e-06}, //   Micro
        Unit{  "N", 1e-09}, //   Nano
        Unit{  "P", 1e-12}, //   Pico
        Unit{  "F", 1e-15}, //   Femto
    };

    auto in     = std::istringstream{str};
    auto val    = readFromStream<double>(in);
    auto suffix = readFromStream<std::string>(in);
    strings::toUpper(suffix);

    auto const found = std::find_if(begin(table), end(table), [&suffix](auto unit) {
        return unit.suffix == suffix;
    });

    if (found == end(table)) { return val; }

    return val * found->multiplier;
}

}  // namespace mc::detail
