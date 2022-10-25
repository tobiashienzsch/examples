#include "spice_inductor.hpp"

#include <format>
#include <sstream>

namespace mc {

auto parseSpiceInductor(std::string const& src) -> SpiceInductor
{
    auto r  = SpiceInductor{};
    auto in = std::istringstream{src};
    in >> r.name;
    in >> r.positive;
    in >> r.negative;
    in >> r.farad;
    return r;
}

auto operator<<(std::ostream& out, SpiceInductor const& i) -> std::ostream&
{
    out << std::format(
        "SpiceInductor(name: {}, pos: {}, neg: {}, farad: {})",
        i.name,
        i.positive,
        i.negative,
        i.farad
    );
    return out;
}

}  // namespace mc