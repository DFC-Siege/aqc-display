#include <cstdint>
#include <hardware/gpio.h>
#include <string>

#include "config.hpp"
#include "display.hpp"
#include "displaylib_16/st7789.hpp"
#include "font/font_factory.hpp"
#include "font/font_types.hpp"
#include "hardware/spi.h"

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
        foreground = config.default_foreground;
        set_font(config.default_font);

        reset_display();

        initialized = true;
}

void Display::reset_display() {
        tft.setRotation(rotation);
        tft.fillScreen(background);
        tft.setTextColor(foreground, background);
        tft.setCursor(0, 0);
        tft.setFont(font.font_name);
}

void Display::clear() {
        tft.fillScreen(background);
}

void Display::clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
        tft.fillRect(x, y, width, height, background);
}

void Display::print(const std::string &value) {
        tft.print(value);
}

void Display::set_cursor(uint16_t x, uint16_t y) {
        tft.setCursor(x, y);
}

void Display::set_font(FontType type) {
        font = FontFactory::create(type);
        tft.setFont(font.font_name);
}

void Display::set_foreground(Color color) {
        foreground = color;
        tft.setTextColor(foreground, background);
}

void Display::set_background(Color color) {
        background = color;
        tft.setTextColor(foreground, background);
}

Config Display::get_config() const {
        return config;
}

void Display::draw_rectangle(uint16_t x, uint16_t y, uint16_t width,
                             uint16_t height, uint16_t color) {
        tft.drawRectWH(x, y, width, height, color);
}
} // namespace Display
