#pragma once

#include <cstdint>

#include "colors.hpp"
#include "components/rect.hpp"

namespace UI {
struct Border {
        uint8_t size;
        Rect rect;
        uint8_t rounding;
        Color color;

        Rect get_inner_rect() {
                return Rect{static_cast<uint16_t>(rect.x + size),
                            static_cast<uint16_t>(rect.y + size),
                            static_cast<uint16_t>(rect.width - size),
                            static_cast<uint16_t>(rect.height - size)};
        }
};
} // namespace UI
