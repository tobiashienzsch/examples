#include "spice_resistor.hpp"

#include <format>
#include <sstream>

namespace mc {
auto parseSpiceResistor(std::string const& src) -> SpiceResistor
{
    auto r  = SpiceResistor{};
    auto in = std::istringstream{src};
    in >> r.name;
    in >> r.positive;
    in >> r.negative;
    in >> r.ohm;
    return r;
}

auto operator<<(std::ostream& out, SpiceResistor const& r) -> std::ostream&
{
    out << std::format(
        "SpiceResistor(name: {}, pos: {}, neg: {}, ohm: {})",
        r.name,
        r.positive,
        r.negative,
        r.ohm
    );
    return out;
}

}  // namespace mc