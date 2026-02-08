#pragma once

#include "scd40/scd40.hpp"
#include "serial/serial_manager.hpp"
#include "sps30/sps30.hpp"

namespace Communication {
class CommunicationManager {
      public:
        CommunicationManager(SerialManager &serial_manager,
                             Sensors::SCD40 &scd_sensor,
                             Sensors::SPS30 &sps_sensor);

      private:
        SerialManager &serial_manager;
        Sensors::SCD40 &scd_sensor;
        Sensors::SPS30 &sps_sensor;
};
} // namespace Communication
