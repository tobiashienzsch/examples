#pragma once

#include <ostream>
#include <string>

namespace mc {

struct SpiceInductor
{
    static constexpr auto token = 'L';

    std::string name;
    std::string positive{};
    std::string negative{};
    double henry{};
};

auto parseSpiceInductor(std::string const& src) -> SpiceInductor;
auto operator<<(std::ostream& out, SpiceInductor const& i) -> std::ostream&;

}  // namespace mc