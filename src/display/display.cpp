#include "display.hpp"
#include "config.hpp"
#include "displaylib_16/st7789.hpp"
#include "hardware/spi.h"
#include <hardware/gpio.h>
#include <string>

namespace {
void initialize_gpio(const Display::Config &config) {
        gpio_init(config.pin_bl);
        gpio_set_dir(config.pin_bl, GPIO_OUT);
        gpio_put(config.pin_bl, 1);
}

void initialize_tft(ST7789_TFT &tft, const Display::Config &config) {
        tft.setupGPIO(config.pin_rst, config.pin_dc, config.pin_cs,
                      config.pin_sclk, config.pin_mosi);
        tft.TFTInitScreenSize(config.offset_x, config.offset_y, config.width,
                              config.height);
        tft.TFTInitSPIType(config.baudrate, spi0);
        tft.TFTST7789Initialize();
}
} // namespace

namespace Display {
void Display::initialize(const Config &config) {
        initialize_gpio(config);
        initialize_tft(tft, config);

        this->config = config;
        rotation = config.rotation;
        background = config.default_background;
        text_color = config.default_text_color;
        font = config.default_font;

        reset_display();

        initialized = true;
}

void Display::reset_display() {
        tft.setRotation(rotation);
        tft.fillScreen(background);
        tft.setTextColor(text_color, background);
        tft.setCursor(0, 0);
        tft.setFont(font);
}

void Display::clear() {
        tft.fillScreen(background);
}

void Display::print(const std::string &value) {
        tft.print(value);
}
} // namespace Display
