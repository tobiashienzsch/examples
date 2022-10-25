#pragma once

#include <mc/spice/spice_element.hpp>

#include <filesystem>
#include <string>
#include <vector>

namespace mc {
struct SpiceCircuit
{
    std::string title;
    std::vector<SpiceElement> elements;
};

[[nodiscard]] auto loadSpiceCircuit(std::filesystem::path const& path) -> SpiceCircuit;
auto operator<<(std::ostream& out, SpiceCircuit const& c) -> std::ostream&;
}  // namespace mc
