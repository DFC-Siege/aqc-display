#pragma once

#include <cstdint>
#include <displaylib_16/displaylib_16_Font.hpp>
#include <displaylib_16/st7789.hpp>
#include <hardware/spi.h>
#include <string>

#include "config.hpp"

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
        void print(const std::string &value);
        void set_background(Color value);
        void reset_display();

      private:
        ST7789_TFT tft;
        Config config;
        ST7789_TFT::display_rotate_e rotation;
        Color background;
        Color text_color;
        display_Font_name_e font;

        Display() {
        }

        bool initialized = false;
};
} // namespace Display
