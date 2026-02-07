#pragma once

#include "hardware/i2c.h"
#include "sensor.hpp"
#include <cstdint>
#include <pico/error.h>
#include <pico/types.h>
#include <string>

namespace Sensors {
struct SCD40Measurement {
        uint16_t co2 = 0;
        float temperature = 0.0f;
        float humidity = 0.0f;
        std::string error;
};

class SCD40 : public Sensor<SCD40Measurement> {
      public:
        SCD40();
        void process() override;

      private:
        static constexpr i2c_inst_t *I2C_PORT = i2c0;
        static constexpr uint SDA_PIN = 8;
        static constexpr uint SCL_PIN = 9;
        static constexpr uint BAUDRATE = 100000;
        static constexpr uint8_t ADDR = 0x62;
        SCD40Measurement last_measurement;

        void start_measurement();

        std::string get_error_reason(int error_code);
};
} // namespace Sensors
