#pragma once

#include <cstdint>
#include <hardware/i2c.h>
#include <pico/types.h>

#include "sensor.hpp"
#include "sps30.hpp"

namespace sensors {
class SPS30Sensor : public Sensor<models::SPS30> {
      public:
        SPS30Sensor(i2c_inst_t *port, uint sda, uint scl, uint baudrate,
                    uint address);
        void process() override;

      private:
        models::SPS30 last_measurement;
        absolute_time_t next_measurement_time;
        uint32_t error_count = 0;
        i2c_inst_t *port;
        uint sda;
        uint scl;
        uint baudrate;
        uint address;

        void start_measurement();
        void stop_measurement();
        void reset();
        bool is_data_ready();
};
} // namespace sensors
