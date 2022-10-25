#pragma once

#include <ostream>
#include <string>

namespace mc {

struct SpiceCapacitor
{
    static constexpr auto token = 'C';

    std::string name;
    int positive{};
    int negative{};
    double farad{};
};

auto parseSpiceCapacitor(std::string const& src) -> SpiceCapacitor;
auto operator<<(std::ostream& out, SpiceCapacitor const& c) -> std::ostream&;

}  // namespace mc