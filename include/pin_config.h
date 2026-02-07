#pragma once
#include <cstdint>

namespace Pins {
static constexpr int8_t TFT_MISO = -1;
static constexpr int8_t TFT_MOSI = 3;
static constexpr int8_t TFT_SCLK = 2;
static constexpr int8_t TFT_CS = 5;
static constexpr int8_t TFT_DC = 1;
static constexpr int8_t TFT_RST = 0;
static constexpr int8_t TFT_BL = 4;

static constexpr uint8_t PWR_ON = 22;
static constexpr uint8_t RED_LED = 25;
static constexpr uint8_t BAT_VOLT = 26;

static constexpr uint8_t BUTTON1 = 6;
static constexpr uint8_t BUTTON2 = 7;

static constexpr uint8_t ESP32_RX = 9;
static constexpr uint8_t ESP32_TX = 8;
} // namespace Pins
