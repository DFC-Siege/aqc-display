#pragma once

#include <cstdint>

namespace UI {
struct Rect {
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;

        Rect from_padding(uint16_t padding) const {
                return Rect(x + padding, y + padding, width - padding * 2,
                            height - padding * 2);
        }
};
} // namespace UI
