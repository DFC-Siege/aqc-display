#pragma once

#include <hardware/i2c.h>
#include <pico/types.h>

#include "scd40.hpp"
#include "sensor.hpp"

namespace sensors {
class SCD40Sensor : public Sensor<models::SCD40> {
      public:
        SCD40Sensor(i2c_inst_t *port, uint sda, uint scl, uint baudrate,
                    uint address);
        void process() override;

      private:
        models::SCD40 last_measurement;
        absolute_time_t next_measurement_time;
        i2c_inst_t *port;
        uint sda;
        uint scl;
        uint baudrate;
        uint address;

        void start_measurement();
};
} // namespace sensors
