#include "spice_element.hpp"

#include <fmt/format.h>
#include <sstream>

namespace mc {

auto operator<<(std::ostream& out, SpiceElement const& e) -> std::ostream&
{
    std::visit([&out](auto const& v) { out << v; }, e);
    return out;
}
}  // namespace mc