#pragma once

#include "displaylib_16/st7789.hpp"
#include <cstdint>
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
        static constexpr int8_t TFT_MISO = -1;
        static constexpr int8_t TFT_MOSI = 3;
        static constexpr int8_t TFT_SCLK = 2;
        static constexpr int8_t TFT_CS = 5;
        static constexpr int8_t TFT_DC = 1;
        static constexpr int8_t TFT_RST = 0;
        static constexpr int8_t TFT_BL = 4;
        ST7789_TFT tft;

        Display() {
        }

        bool _initialized = false;
};
} // namespace Display
