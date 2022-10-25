#pragma once

#include <ostream>
#include <string>

namespace mc {
struct SpiceResistor
{
    static constexpr auto token = 'R';

    std::string name;
    std::string positive{};
    std::string negative{};
    double ohm{};
};

auto parseSpiceResistor(std::string const& src) -> SpiceResistor;
auto operator<<(std::ostream& out, SpiceResistor const& r) -> std::ostream&;

}  // namespace mc