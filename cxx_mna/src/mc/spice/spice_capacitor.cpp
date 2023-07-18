#include "spice_capacitor.hpp"

#include <mc/spice/detail/parse_spice_number.hpp>
#include <mc/stream.hpp>

#include <fmt/format.h>
#include <sstream>

namespace mc {
auto parseSpiceCapacitor(std::string const& src) -> SpiceCapacitor
{
    auto in            = std::istringstream{src};
    auto capacitor     = SpiceCapacitor{};
    capacitor.name     = readFromStream<std::string>(in);
    capacitor.positive = readFromStream<std::string>(in);
    capacitor.negative = readFromStream<std::string>(in);
    capacitor.farad    = detail::parseSpiceNumber(readFromStream<std::string>(in));
    return capacitor;
}

auto operator<<(std::ostream& out, SpiceCapacitor const& c) -> std::ostream&
{
    out << fmt::format(
        "Capacitor(name: {}, pos: {}, neg: {}, farad: {})",
        c.name,
        c.positive,
        c.negative,
        c.farad
    );
    return out;
}

}  // namespace mc