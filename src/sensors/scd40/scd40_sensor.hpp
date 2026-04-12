#pragma once

#include <hardware/i2c.h>
#include <pico/types.h>

#include "scd40.hpp"
#include "sensor.hpp"

namespace sensors {
class SCD40Sensor : public Sensor<models::SCD40> {
      public:
        SCD40Sensor();
        void process() override;

      private:
        static constexpr auto I2C_PORT = i2c1;
        static constexpr auto SDA_PIN = 14;
        static constexpr auto SCL_PIN = 15;
        static constexpr auto BAUDRATE = 100000;
        static constexpr auto ADDR = 0x62;
        models::SCD40 last_measurement;
        absolute_time_t next_measurement_time;

        void start_measurement();
};
} // namespace sensors
