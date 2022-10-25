#pragma once

#include <mc/spice/spice_capacitor.hpp>
#include <mc/spice/spice_inductor.hpp>
#include <mc/spice/spice_resistor.hpp>
#include <mc/spice/spice_voltage_source.hpp>

#include <ostream>
#include <variant>

namespace mc {

using SpiceElement = std::variant<
    SpiceResistor,      // R
    SpiceCapacitor,     // C
    SpiceInductor,      // L
    SpiceVoltageSource  // V
    >;

auto operator<<(std::ostream& out, SpiceElement const& e) -> std::ostream&;

}  // namespace mc