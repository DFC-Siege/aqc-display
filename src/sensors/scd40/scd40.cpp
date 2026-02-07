#include <cstdint>
#include <cstdio>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/error.h>

#include "hardware/gpio.h"
#include "pico/time.h"
#include "scd40.hpp"

namespace Sensors {

SCD40::SCD40() {
        i2c_init(I2C_PORT, BAUDRATE);
        gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(SDA_PIN);
        gpio_pull_up(SCL_PIN);

        start_measurement();
}

void SCD40::start_measurement() {
        uint8_t cmd[] = {0x21, 0xb1};
        i2c_write_blocking(I2C_PORT, ADDR, cmd, 2, false);
        sleep_ms(1000);
}

void SCD40::process() {
        uint8_t read_cmd[] = {0xec, 0x05};
        uint8_t data[9];

        i2c_write_blocking(I2C_PORT, ADDR, read_cmd, 2, false);
        sleep_ms(5);

        int result = i2c_read_blocking(I2C_PORT, ADDR, data, 9, false);

        if (result != PICO_ERROR_GENERIC) {
                last_measurement.co2 = (data[0] << 8) | data[1];
                last_measurement.temperature =
                    -45.0f +
                    175.0f * (float)((data[3] << 8) | data[4]) / 65536.0f;
                last_measurement.humidity =
                    100.0f * (float)((data[6] << 8) | data[7]) / 65536.0f;

                invoke_listeners(last_measurement);
        }
}

} // namespace Sensors
