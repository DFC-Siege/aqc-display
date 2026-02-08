#pragma once

#include <cstdint>

namespace UI {
using Color = uint16_t;

#define THEME_GRUVBOX_MATERIAL

namespace Colors {
#if defined(THEME_DEFAULT)
static constexpr Color BACKGROUND = 0x0000;
static constexpr Color PRIMARY = 0xFFFF;
static constexpr Color SECONDARY = 0x7BEF;
static constexpr Color ERROR = 0xF800;
static constexpr Color WARNING = 0xFFE0;
static constexpr Color INFO = 0x07FF;
static constexpr Color SUCCESS = 0x07E0;
static constexpr Color DEBUG = 0xF81F;

#elif defined(THEME_GRUVBOX_MATERIAL)
static constexpr Color BACKGROUND = 0x2945; // #282828
static constexpr Color PRIMARY = 0xD5F3;    // #d4be98
static constexpr Color SECONDARY = 0x39C7;  // #3c3836 (color0)
static constexpr Color ERROR = 0xEB4C;      // #ea6962 (color1)
static constexpr Color WARNING = 0xDD4A;    // #d8a657 (color3)
static constexpr Color INFO = 0x7D74;       // #7daea3 (color4/accent)
static constexpr Color SUCCESS = 0xA5AC;    // #a9b665 (color2)
static constexpr Color DEBUG = 0xD433;      // #d3869b (color5)
#else
#error "No UI theme defined!"
#endif
} // namespace Colors
} // namespace UI
