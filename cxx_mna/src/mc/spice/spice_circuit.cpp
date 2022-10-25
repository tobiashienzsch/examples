#include "spice_circuit.hpp"

#include <mc/strings.hpp>

#include <fstream>

namespace mc {
auto loadSpiceCircuit(std::filesystem::path const& path) -> SpiceCircuit
{
    auto in = std::ifstream{path};

    auto circuit = SpiceCircuit{};
    std::getline(in, circuit.title);

    auto line = std::string{};
    while (std::getline(in, line)) {
        strings::trim(line);
        if (line == ".END" || line == ".end") { break; }

        if (line.empty()) { continue; }
        if (line[0] == '*') { continue; }

        switch (line[0]) {
            case SpiceResistor::token: {
                circuit.elements.push_back(parseSpiceResistor(line));
                break;
            }
            case SpiceCapacitor::token: {
                circuit.elements.push_back(parseSpiceCapacitor(line));
                break;
            }
            case SpiceInductor::token: {
                circuit.elements.push_back(parseSpiceInductor(line));
                break;
            }
            case SpiceVoltageSource::token: {
                circuit.elements.push_back(parseSpiceVoltageSource(line));
                break;
            }
            default: {
                break;
                // throw std::runtime_error{"unhandled spice directive"};
            }
        }
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
