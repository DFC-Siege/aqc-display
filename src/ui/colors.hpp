#pragma once

#include <cstdint>

namespace UI {
using Color = uint16_t;

namespace Colors {
static constexpr Color BACKGROUND = 0x0000; // Black
static constexpr Color PRIMARY = 0xFFFF;    // White
static constexpr Color SECONDARY = 0x7BEF;  // Grey
static constexpr Color ERROR = 0xF800;      // Red
static constexpr Color WARNING = 0xFFE0;    // Yellow (Red + Green)
static constexpr Color INFO = 0x07FF;       // Cyan (Green + Blue)
static constexpr Color SUCCESS = 0x07E0;    // Green
static constexpr Color DEBUG = 0xF81F;      // Magenta
} // namespace Colors
} // namespace UI
