#include "displaylib_16/displaylib_16_Font.hpp"
#include "displaylib_16/st7789.hpp"
#include "hardware/spi.h"
#include "pico/stdio.h"
#include "pin_config.h"
#include <hardware/gpio.h>
#include <pico/platform/common.h>

int main() {
        stdio_init_all();

        gpio_init(Pins::TFT_BL);
        gpio_set_dir(Pins::TFT_BL, GPIO_OUT);
        gpio_put(Pins::TFT_BL, 1);

        ST7789_TFT tft;

        tft.setupGPIO(Pins::TFT_RST, Pins::TFT_DC, Pins::TFT_CS, Pins::TFT_SCLK,
                      Pins::TFT_MOSI);

        tft.TFTInitScreenSize(52, 40, 135, 240);
        tft.TFTInitSPIType(24000, spi0);
        tft.TFTST7789Initialize();
        tft.setRotation(ST7789_TFT::Degrees_90);

        tft.fillScreen(ST7789_TFT::C_BLACK);
        tft.setTextColor(ST7789_TFT::C_WHITE);

        tft.setFont(font_default);

        tft.writeCharString(10, 50, (char *)"Hello, world!");

        while (true) {
                tight_loop_contents();
        }
}
