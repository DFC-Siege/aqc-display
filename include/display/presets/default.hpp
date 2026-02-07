#pragma once

#include <displaylib_16/displaylib_16_Font.hpp>
#include <displaylib_16/st7789.hpp>

#include "config.hpp"
#include "font/font_types.hpp"

namespace Presets {
inline constexpr Display::Config Default = {.pin_mosi = 3,
                                            .pin_sclk = 2,
                                            .pin_cs = 5,
                                            .pin_dc = 1,
                                            .pin_rst = 0,
                                            .pin_bl = 4,
                                            .baudrate = 62'500'000,
                                            .offset_x = 0,
                                            .offset_y = 0,
                                            .width = 135,
                                            .height = 240,
                                            .rotation = ST7789_TFT::Degrees_270,
                                            .default_background = 0x0000,
                                            .default_foreground = 0xFFFF,
                                            .default_font =
                                                Display::FontType::DEFAULT};
}
