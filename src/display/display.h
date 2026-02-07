#pragma once

#include "displaylib_16/displaylib_16_Font.hpp"
#include "displaylib_16/st7789.hpp"
#include <hardware/spi.h>
#include <string>
namespace Display {
class Display {
      public:
        static Display &getInstance() {
                static Display instance;
                return instance;
        }

        Display(const Display &) = delete;
        Display &operator=(const Display &) = delete;

        void initialize();
        void clear();
        void print(const std::string &value);

      private:
        static constexpr auto TFT_MISO = -1;
        static constexpr auto TFT_MOSI = 3;
        static constexpr auto TFT_SCLK = 2;
        static constexpr auto TFT_CS = 5;
        static constexpr auto TFT_DC = 1;
        static constexpr auto TFT_RST = 0;
        static constexpr auto TFT_BL = 4;
        static constexpr auto OFFSET_X = 0;
        static constexpr auto OFFSET_Y = 0;
        static constexpr auto WIDTH = 135;
        static constexpr auto HEIGHT = 240;
        static constexpr auto BAUDRATE = 24000;
        static constexpr auto ROTATION = ST7789_TFT::Degrees_270;
        static constexpr auto DEFAULT_FONT = font_default;
        ST7789_TFT tft;
        ST7789_TFT::pixel_color565_e background;
        ST7789_TFT::pixel_color565_e text_color;

        Display() {
        }

        bool initialized = false;
};
} // namespace Display
