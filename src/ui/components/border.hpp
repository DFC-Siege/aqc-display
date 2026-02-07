#pragma once

#include <cstdint>

#include "colors.hpp"
#include "components/rect.hpp"

namespace UI {
struct Border {
        uint16_t size;
        Rect rect;
        uint16_t rounding;
        Color color;

        Rect get_inner_rect() const {
                return Rect{static_cast<uint16_t>(rect.x + size),
                            static_cast<uint16_t>(rect.y + size),
                            static_cast<uint16_t>(rect.width - (size * 2)),
                            static_cast<uint16_t>(rect.height - (size * 2))};
        }
};
} // namespace UI
