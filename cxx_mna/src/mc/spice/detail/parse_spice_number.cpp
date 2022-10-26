#include "parse_spice_number.hpp"

#include <mc/stream.hpp>
#include <mc/strings.hpp>

#include <algorithm>
#include <array>
#include <sstream>
#include <string_view>

namespace mc::detail {

auto parseSpiceNumber(std::string const& str) -> double
{
    struct Magnitude
    {
        std::string_view suffix;
        double multiplier;
    };

    static constexpr auto magnitudes = std::array<Magnitude, 10>{
        Magnitude{  "T", 1e+12}, //   Tera
        Magnitude{  "G", 1e+09}, //   Giga
        Magnitude{  "X", 1e+06}, //   Mega
        Magnitude{"MEG", 1e+06}, //   Mega
        Magnitude{  "K", 1e+03}, //   Kilo
        Magnitude{  "M", 1e-03}, //   Milli
        Magnitude{  "U", 1e-06}, //   Micro
        Magnitude{  "N", 1e-09}, //   Nano
        Magnitude{  "P", 1e-12}, //   Pico
        Magnitude{  "F", 1e-15}, //   Femto
    };

    auto in     = std::istringstream{str};
    auto val    = readFromStream<double>(in);
    auto suffix = readFromStream<std::string>(in);
    strings::toUpper(suffix);

    auto const hasSuffix = [&suffix](auto unit) { return unit.suffix == suffix; };
    auto const found     = std::find_if(begin(magnitudes), end(magnitudes), hasSuffix);
    if (found == end(magnitudes)) { return val; }

    return val * found->multiplier;
}

}  // namespace mc::detail
