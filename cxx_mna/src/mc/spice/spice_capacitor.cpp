#include "spice_capacitor.hpp"

#include <format>
#include <sstream>

namespace mc {
auto parseSpiceCapacitor(std::string const& src) -> SpiceCapacitor
{
    auto r  = SpiceCapacitor{};
    auto in = std::istringstream{src};
    in >> r.name;
    in >> r.positive;
    in >> r.negative;
    in >> r.farad;
    return r;
}

auto operator<<(std::ostream& out, SpiceCapacitor const& c) -> std::ostream&
{
    out << std::format(
        "SpiceCapacitor(name: {}, pos: {}, neg: {}, farad: {})",
        c.name,
        c.positive,
        c.negative,
        c.farad
    );
    return out;
}

}  // namespace mc