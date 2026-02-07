#include "display.h"
#include "displaylib_16/displaylib_16_Font.hpp"
#include "displaylib_16/st7789.hpp"
#include "hardware/spi.h"
#include <hardware/gpio.h>
#include <string>

namespace Display {
void Display::initialize() {
        gpio_init(TFT_BL);
        gpio_set_dir(TFT_BL, GPIO_OUT);
        gpio_put(TFT_BL, 1);

        tft.setupGPIO(TFT_RST, TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI);

        tft.TFTInitScreenSize(0, 0, 135, 240);
        tft.TFTInitSPIType(24000, spi0);
        tft.TFTST7789Initialize();
        tft.setRotation(ST7789_TFT::Degrees_270);
        tft.fillScreen(tft.C_BLACK);
        tft.setTextColor(text_color, tft.C_BLACK);
        tft.setCursor(0, 0);
        tft.setFont(font_default);
        initialized = true;
}

void Display::clear() {
        tft.fillScreen(background);
}

void Display::print(const std::string &value) {
        tft.print(value);
}
} // namespace Display
