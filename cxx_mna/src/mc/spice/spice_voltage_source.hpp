#pragma once

#include <ostream>
#include <string>

namespace mc {
struct SpiceVoltageSource
{
    static constexpr auto token = 'V';

    enum struct Type
    {
        dc,
        ac,
    };

    std::string name;
    int positive{};
    int negative{};
    Type type{};
    double voltage{};
};

auto parseSpiceVoltageSource(std::string const& src) -> SpiceVoltageSource;
auto operator<<(std::ostream& out, SpiceVoltageSource const& v) -> std::ostream&;

}  // namespace mc