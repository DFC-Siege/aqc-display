#include "communication_manager.hpp"
#include "scd40/scd40.hpp"
#include "serial/serial_manager.hpp"
#include "sps30/sps30.hpp"

namespace Communication {
CommunicationManager::CommunicationManager(SerialManager &serial_manager,
                                           Sensors::SCD40 &scd_sensor,
                                           Sensors::SPS30 &sps_sensor)
    : serial_manager(serial_manager), scd_sensor(scd_sensor),
      sps_sensor(sps_sensor) {
        scd_sensor.add_listener([this](Sensors::SCD40Measurement value) {
                this->serial_manager.send(value.to_json());
        });

        sps_sensor.add_listener([this](Sensors::SPS30Measurement value) {
                this->serial_manager.send(value.to_json());
        });
}
} // namespace Communication
