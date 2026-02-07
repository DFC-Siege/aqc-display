#pragma once

#include <cstdint>

namespace UI {
using Color = uint16_t;

namespace Colors {
static constexpr Color BACKGROUND = 0x0000;
static constexpr Color PRIMARY = 0xFFFF;
static constexpr Color SECONDARY = 0xF800;
static constexpr Color ERROR = 0x07E0;
static constexpr Color WARNING = 0x001F;
static constexpr Color INFO = 0x07FF;
static constexpr Color DEBUG = 0xF81F;
} // namespace Colors
} // namespace UI
