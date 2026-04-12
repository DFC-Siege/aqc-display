#include "sps30_sensor.hpp"
#include "hardware/gpio.h"
#include "pico/time.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/types.h>

namespace sensors {

SPS30Sensor::SPS30Sensor(i2c_inst_t *port, uint sda, uint scl, uint baudrate,
                         uint address)
    : port(port), sda(sda), scl(scl), baudrate(baudrate), address(address) {
        sleep_ms(1000);
        i2c_init(port, baudrate);
        gpio_set_function(sda, GPIO_FUNC_I2C);
        gpio_set_function(scl, GPIO_FUNC_I2C);
        gpio_pull_up(sda);
        gpio_pull_up(scl);

        start_measurement();
}

void SPS30Sensor::start_measurement() {
        uint8_t cmd[] = {0x00, 0x10, 0x03, 0x00, 0xAC};
        i2c_write_blocking(port, address, cmd, 5, false);
        next_measurement_time = make_timeout_time_ms(1000);
}

bool SPS30Sensor::is_data_ready() {
        uint8_t cmd[] = {0x02, 0x02};
        uint8_t response[3];
        if (i2c_write_blocking(port, address, cmd, 2, false) < 0)
                return false;
        if (i2c_read_blocking(port, address, response, 3, false) < 0)
                return false;
        return response[1] == 0x01;
}

void SPS30Sensor::process() {
        if (get_absolute_time() < next_measurement_time)
                return;

        if (!is_data_ready()) {
                next_measurement_time = make_timeout_time_ms(100);
                return;
        }

        uint8_t read_cmd[] = {0x03, 0x00};
        uint8_t data[60];

        if (i2c_write_blocking(port, address, read_cmd, 2, false) < 0) {
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        if (i2c_read_blocking(port, address, data, 60, false) < 0) {
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        auto parse_float = [](uint8_t *b) {
                uint32_t val = ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) |
                               ((uint32_t)b[3] << 8) | (uint32_t)b[4];
                float f;
                std::memcpy(&f, &val, 4);
                return f;
        };

        last_measurement.pm1_0 = parse_float(&data[0]);
        last_measurement.pm2_5 = parse_float(&data[6]);
        last_measurement.pm4_0 = parse_float(&data[12]);
        last_measurement.pm10_0 = parse_float(&data[18]);
        last_measurement.typical_particle_size = parse_float(&data[54]);
        printf("SPS30: PM1: %.2f ppm, PM2.5: %.2f ppm, PM4: %.2f ppm, PM10: "
               "%.2f ppm\n",
               last_measurement.pm1_0, last_measurement.pm2_5,
               last_measurement.pm4_0, last_measurement.pm10_0);

        next_measurement_time = make_timeout_time_ms(1000);
        invoke_listeners(last_measurement);
}
} // namespace sensors
