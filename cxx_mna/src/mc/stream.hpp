#pragma once

#include <istream>

namespace mc {
template<typename T>
auto readFromStream(std::istream& in) -> T
{
    auto val = T{};
    in >> val;
    return val;
}
}  // namespace mc