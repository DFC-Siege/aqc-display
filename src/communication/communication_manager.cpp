#include <cstdint>
#include <span>

#include "communication_manager.hpp"
#include "scd40/scd40.hpp"
#include "serial_hal.hpp"
#include "sps30/sps30.hpp"

namespace Communication {

CommunicationManager::CommunicationManager(serial::SerialHal &serial_hal,
                                           Sensors::SCD40 &scd_sensor,
                                           Sensors::SPS30 &sps_sensor)
    : serial_hal(serial_hal), scd_sensor(scd_sensor), sps_sensor(sps_sensor) {
        scd_sensor.add_listener(
            [this, &serial_hal](Sensors::SCD40Measurement value) {
                    const auto json = value.to_json();
                    serial_hal.send(std::span<const uint8_t>(
                        reinterpret_cast<const uint8_t *>(json.data()),
                        json.size()));
            });
        sps_sensor.add_listener(
            [this, &serial_hal](Sensors::SPS30Measurement value) {
                    const auto json = value.to_json();
                    serial_hal.send(std::span<const uint8_t>(
                        reinterpret_cast<const uint8_t *>(json.data()),
                        json.size()));
            });
}

} // namespace Communication
