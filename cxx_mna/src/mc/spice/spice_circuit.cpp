#include "spice_circuit.hpp"

#include <mc/strings.hpp>

#include <fstream>

namespace mc {
auto loadSpiceCircuit(std::filesystem::path const& path) -> SpiceCircuit
{
    auto in = std::ifstream{path};

    auto circuit = SpiceCircuit{};
    std::getline(in, circuit.title);

    auto& e   = circuit.elements;
    auto line = std::string{};
    while (std::getline(in, line)) {
        mc::trim(line);
        if (line == ".END" || line == ".end") { break; }

        if (line.empty()) { continue; }
        if (line[0] == '*') { continue; }

        if (line[0] == 'R') { e.push_back(parseSpiceResistor(line)); }
        if (line[0] == 'C') { e.push_back(parseSpiceCapacitor(line)); }
        if (line[0] == 'L') { e.push_back(parseSpiceInductor(line)); }
        if (line[0] == 'V') { e.push_back(parseSpiceVoltageSource(line)); }
    }

    return circuit;
}

auto operator<<(std::ostream& out, SpiceCircuit const& c) -> std::ostream&
{
    out << c.title << '\n';
    for (auto const& e : c.elements) { out << e << '\n'; }
    return out;
}
}  // namespace mc
