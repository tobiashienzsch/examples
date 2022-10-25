#include "spice_voltage_source.hpp"

#include <format>
#include <sstream>

namespace mc {
auto parseSpiceVoltageSource(std::string const& src) -> SpiceVoltageSource
{
    auto r  = SpiceVoltageSource{};
    auto in = std::istringstream{src};
    in >> r.name;
    in >> r.positive;
    in >> r.negative;
    in >> r.voltage;
    return r;
}

auto operator<<(std::ostream& out, SpiceVoltageSource const& r) -> std::ostream&
{
    out << std::format(
        "SpiceVoltageSource(name: {}, pos: {}, neg: {}, voltage: {})",
        r.name,
        r.positive,
        r.negative,
        r.voltage
    );
    return out;
}

}  // namespace mc