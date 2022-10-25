#include "spice_voltage_source.hpp"

#include <mc/spice/detail/parse_spice_number.hpp>

#include <format>
#include <sstream>

namespace mc {
auto parseSpiceVoltageSource(std::string const& src) -> SpiceVoltageSource
{
    auto v     = SpiceVoltageSource{};
    auto in    = std::istringstream{src};
    v.name     = readFromStream<std::string>(in);
    v.positive = readFromStream<std::string>(in);
    v.negative = readFromStream<std::string>(in);
    v.voltage  = detail::parseSpiceNumber(readFromStream<std::string>(in));
    return v;
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