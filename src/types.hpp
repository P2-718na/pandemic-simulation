#ifndef PANDEMIC_TYPES_HPP
#define PANDEMIC_TYPES_HPP

#include <utility>

namespace pandemic {

// This type is used across the entire code, so I declared it in its separate
// header. This space can be used to define other "universal" types, if needed
// in the future.
using Coords = std::pair<int, int>;

}

#endif  // define PANDEMIC_TYPES_HPP
