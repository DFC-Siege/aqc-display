#include <cstdint>
#include <cstdio>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/error.h>
#include <string>

#include "hardware/gpio.h"
#include "pico/time.h"
#include "scd40.hpp"

namespace Sensors {

SCD40::SCD40() {
        sleep_ms(1000);
        i2c_init(I2C_PORT, BAUDRATE);
        gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(SDA_PIN);
        gpio_pull_up(SCL_PIN);

        start_measurement();
}

void SCD40::start_measurement() {
        printf("starting measurement \n");
        uint8_t cmd[] = {0x21, 0xb1};
        int result = i2c_write_blocking(I2C_PORT, ADDR, cmd, 2, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40 intitial write error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }
        next_measurement_time = make_timeout_time_ms(1000);
}

void SCD40::process() {
        if (get_absolute_time() < next_measurement_time) {
                return;
        }

        uint8_t read_cmd[] = {0xec, 0x05};
        uint8_t data[9];

        int result = i2c_write_blocking(I2C_PORT, ADDR, read_cmd, 2, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40 write error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        sleep_ms(5);

        result = i2c_read_blocking(I2C_PORT, ADDR, data, 9, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40 read error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        last_measurement.error.clear();
        last_measurement.co2 = (data[0] << 8) | data[1];
        last_measurement.temperature =
            -45.0f + 175.0f * (float)((data[3] << 8) | data[4]) / 65536.0f;
        last_measurement.humidity =
            100.0f * (float)((data[6] << 8) | data[7]) / 65536.0f;

        printf("SCD40: CO2: %u ppm, Temp: %.2f C, Humidity: %.2f%%\n",
               last_measurement.co2, last_measurement.temperature,
               last_measurement.humidity);

        next_measurement_time = make_timeout_time_ms(5000);
        invoke_listeners(last_measurement);
}
} // namespace Sensors
