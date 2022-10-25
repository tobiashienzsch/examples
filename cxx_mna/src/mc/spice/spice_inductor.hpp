#pragma once

#include <ostream>
#include <string>

namespace mc {

struct SpiceInductor
{
    std::string name;
    int positive{};
    int negative{};
    double farad{};
};

auto parseSpiceInductor(std::string const& src) -> SpiceInductor;
auto operator<<(std::ostream& out, SpiceInductor const& i) -> std::ostream&;

}  // namespace mc