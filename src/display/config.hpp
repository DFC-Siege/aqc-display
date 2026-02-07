#pragma once

#include "font/font_types.hpp"
#include <cstdint>
#include <displaylib_16/displaylib_16_Font.hpp>
#include <displaylib_16/st7789.hpp>
#include <hardware/spi.h>

namespace Display {
struct Config {
        uint8_t pin_mosi;
        uint8_t pin_sclk;
        uint8_t pin_cs;
        uint8_t pin_dc;
        uint8_t pin_rst;
        uint8_t pin_bl;
        uint32_t baudrate;

        uint16_t offset_x;
        uint16_t offset_y;
        uint16_t width;
        uint16_t height;
        ST7789_TFT::display_rotate_e rotation;
        uint16_t default_background;
        uint16_t default_foreground;
        FontType default_font;

        bool is_rotated() const {
                return rotation == ST7789_TFT::Degrees_270 ||
                       rotation == ST7789_TFT::Degrees_90;
        }
};
} // namespace Display
