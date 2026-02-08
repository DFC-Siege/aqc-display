#pragma once

#include <cstdint>
#include <cstdio>
#include <pico/error.h>
#include <pico/types.h>
#include <string>

#include "hardware/i2c.h"
#include "sensor.hpp"

namespace Sensors {

struct SPS30Measurement {
        float pm1_0 = 0.0f;
        float pm2_5 = 0.0f;
        float pm4_0 = 0.0f;
        float pm10_0 = 0.0f;
        float typical_particle_size = 0.0f;
        std::string error;

        std::string to_json() const {
                char buffer[320];
                int len = std::snprintf(
                    buffer, sizeof(buffer),
                    "{\"type\":\"sps30\",\"pm1_0\":%.2f,\"pm2_5\":%.2f,\"pm4_"
                    "0\":%.2f,\"pm10_0\":%.2f,\"tps\":%.2f,\"error\":\"%s\"}",
                    pm1_0, pm2_5, pm4_0, pm10_0, typical_particle_size,
                    error.c_str());

                return std::string(buffer, len);
        }
};

class SPS30 : public Sensor<SPS30Measurement> {
      public:
        SPS30();
        void process() override;

      private:
        static constexpr auto I2C_PORT = i2c0;
        static constexpr auto SDA_PIN = 24;
        static constexpr auto SCL_PIN = 21;
        static constexpr auto BAUDRATE = 100000;
        static constexpr auto ADDR = 0x69;

        SPS30Measurement last_measurement;
        absolute_time_t next_measurement_time;
        uint32_t error_count = 0;

        void start_measurement();
        void stop_measurement();
        void reset();
        bool is_data_ready();
};

} // namespace Sensors
