#pragma once

#include "scd40/scd40.hpp"
#include "serial_hal.hpp"
#include "sps30/sps30.hpp"

namespace Communication {
class CommunicationManager {
      public:
        CommunicationManager(serial::SerialHal &serial_hal,
                             Sensors::SCD40 &scd_sensor,
                             Sensors::SPS30 &sps_sensor);

      private:
        serial::SerialHal &serial_hal;
        Sensors::SCD40 &scd_sensor;
        Sensors::SPS30 &sps_sensor;
};
} // namespace Communication
