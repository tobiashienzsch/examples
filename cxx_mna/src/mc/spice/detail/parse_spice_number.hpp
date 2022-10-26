#pragma once

#include <string>

namespace mc::detail {

[[nodiscard]] auto parseSpiceNumber(std::string const& str) -> double;

}  // namespace mc::detail
