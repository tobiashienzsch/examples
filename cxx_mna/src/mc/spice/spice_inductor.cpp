#include "spice_inductor.hpp"

#include <mc/spice/detail/parse_spice_number.hpp>
#include <mc/stream.hpp>

#include <format>
#include <sstream>

namespace mc {

auto parseSpiceInductor(std::string const& src) -> SpiceInductor
{
    auto in           = std::istringstream{src};
    auto inductor     = SpiceInductor{};
    inductor.name     = readFromStream<std::string>(in);
    inductor.positive = readFromStream<std::string>(in);
    inductor.negative = readFromStream<std::string>(in);
    inductor.henry    = detail::parseSpiceNumber(readFromStream<std::string>(in));
    return inductor;
}

auto operator<<(std::ostream& out, SpiceInductor const& i) -> std::ostream&
{
    out << std::format(
        "Inductor(name: {}, pos: {}, neg: {}, henry: {})",
        i.name,
        i.positive,
        i.negative,
        i.henry
    );
    return out;
}

}  // namespace mc