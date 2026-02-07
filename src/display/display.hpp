#pragma once

#include <cstdint>
#include <displaylib_16/displaylib_16_Font.hpp>
#include <displaylib_16/st7789.hpp>
#include <hardware/spi.h>
#include <string>

#include "config.hpp"
#include "font/font.hpp"
#include "font/font_types.hpp"

namespace Display {
class Display {
      public:
        using Color = uint16_t;

        static Display &getInstance() {
                static Display instance;
                return instance;
        }

        Display(const Display &) = delete;
        Display &operator=(const Display &) = delete;

        void initialize(const Config &config);
        void clear();
        void clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        void print(const std::string &value);
        void set_background(Color value);
        void set_foreground(Color value);
        void set_font(FontType font);
        void reset_display();
        void set_cursor(uint16_t x, uint16_t y);
        Config get_config() const;
        void draw_rectangle(uint16_t x, uint16_t y, uint16_t width,
                            uint16_t height, uint16_t color);

      private:
        ST7789_TFT tft;
        Config config;
        ST7789_TFT::display_rotate_e rotation;
        Color background;
        Color foreground;
        Font font;

        Display() {
        }

        bool initialized = false;
};
} // namespace Display
