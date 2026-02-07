#include "sps30.hpp"
#include "hardware/gpio.h"
#include "pico/time.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>

namespace Sensors {

SPS30::SPS30() {
        sleep_ms(1000);
        i2c_init(I2C_PORT, BAUDRATE);
        gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(SDA_PIN);
        gpio_pull_up(SCL_PIN);

        start_measurement();
}

void SPS30::start_measurement() {
        uint8_t cmd[] = {0x00, 0x10, 0x03, 0x00, 0xAC};
        i2c_write_blocking(I2C_PORT, ADDR, cmd, 5, false);
        next_measurement_time = make_timeout_time_ms(1000);
}

bool SPS30::is_data_ready() {
        uint8_t cmd[] = {0x02, 0x02};
        uint8_t response[3];
        if (i2c_write_blocking(I2C_PORT, ADDR, cmd, 2, false) < 0)
                return false;
        if (i2c_read_blocking(I2C_PORT, ADDR, response, 3, false) < 0)
                return false;
        return response[1] == 0x01;
}

void SPS30::process() {
        if (get_absolute_time() < next_measurement_time)
                return;

        if (!is_data_ready()) {
                next_measurement_time = make_timeout_time_ms(100);
                return;
        }

        uint8_t read_cmd[] = {0x03, 0x00};
        uint8_t data[60];

        if (i2c_write_blocking(I2C_PORT, ADDR, read_cmd, 2, false) < 0) {
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        if (i2c_read_blocking(I2C_PORT, ADDR, data, 60, false) < 0) {
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
} // namespace Sensors
