#pragma once

#include <cstdint>
#include <utility>

#include "displaylib_16/displaylib_16_Font.hpp"
#include "font_types.hpp"

namespace Display {
struct Font {
        FontType type;
        display_Font_name_e font_name;
        uint8_t width;
        uint8_t height;

        std::pair<uint8_t, uint8_t> get_font_size() const {
                return std::make_pair(width, height);
        }
};
} // namespace Display
