#pragma once

#include <cstdint>
#include <cstdio>
#include <pico/error.h>
#include <pico/types.h>
#include <string>

#include "hardware/i2c.h"
#include "sensor.hpp"

namespace Sensors {
struct SCD40Measurement {
        uint16_t co2 = 0;
        float temperature = 0.0f;
        float humidity = 0.0f;
        std::string error;

        std::string to_json() const {
                char buffer[256];
                int len =
                    std::snprintf(buffer, sizeof(buffer),
                                  "{\"type\":\"scd40\",\"co2\":%u,\"temp\":%."
                                  "2f,\"hum\":%.2f,\"error\":\"%s\"}",
                                  co2, temperature, humidity, error.c_str());

                return std::string(buffer, len);
        }
};

class SCD40 : public Sensor<SCD40Measurement> {
      public:
        SCD40();
        void process() override;

      private:
        static constexpr auto I2C_PORT = i2c1;
        static constexpr auto SDA_PIN = 14;
        static constexpr auto SCL_PIN = 15;
        static constexpr auto BAUDRATE = 100000;
        static constexpr auto ADDR = 0x62;
        SCD40Measurement last_measurement;
        absolute_time_t next_measurement_time;

        void start_measurement();
};
} // namespace Sensors
