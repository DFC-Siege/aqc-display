#pragma once

#include <hardware/i2c.h>
#include <pico/types.h>

#include "sensor.hpp"
#include "sps30.hpp"

namespace sensors {
class SPS30Sensor : public Sensor<models::SPS30> {
      public:
        SPS30Sensor();
        void process() override;

      private:
        static constexpr auto I2C_PORT = i2c0;
        static constexpr auto SDA_PIN = 24;
        static constexpr auto SCL_PIN = 21;
        static constexpr auto BAUDRATE = 100000;
        static constexpr auto ADDR = 0x69;

        models::SPS30 last_measurement;
        absolute_time_t next_measurement_time;
        uint32_t error_count = 0;

        void start_measurement();
        void stop_measurement();
        void reset();
        bool is_data_ready();
};
} // namespace sensors
