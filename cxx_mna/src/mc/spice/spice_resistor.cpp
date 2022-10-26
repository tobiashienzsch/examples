#include "spice_resistor.hpp"

#include <mc/spice/detail/parse_spice_number.hpp>
#include <mc/stream.hpp>

#include <fmt/format.h>
#include <sstream>

namespace mc {
auto parseSpiceResistor(std::string const& src) -> SpiceResistor
{
    auto in           = std::istringstream{src};
    auto resistor     = SpiceResistor{};
    resistor.name     = readFromStream<std::string>(in);
    resistor.positive = readFromStream<std::string>(in);
    resistor.negative = readFromStream<std::string>(in);
    resistor.ohm      = detail::parseSpiceNumber(readFromStream<std::string>(in));
    return resistor;
}

auto operator<<(std::ostream& out, SpiceResistor const& r) -> std::ostream&
{
    out << fmt::format(
        "Resistor(name: {}, pos: {}, neg: {}, ohm: {})",
        r.name,
        r.positive,
        r.negative,
        r.ohm
    );
    return out;
}

}  // namespace mc