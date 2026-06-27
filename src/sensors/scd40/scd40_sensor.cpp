#include <cmath>
#include <cstdint>
#include <cstdio>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <pico/error.h>
#include <pico/types.h>
#include <string>

#include "hardware/gpio.h"
#include "pico/time.h"
#include "scd40_sensor.hpp"

namespace sensors {
SCD40Sensor::SCD40Sensor(i2c_inst_t *port, uint sda, uint scl, uint baudrate,
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

void SCD40Sensor::start_measurement() {
        printf("starting measurement \n");
        uint8_t cmd[] = {0x21, 0xb1};
        int result = i2c_write_blocking(port, address, cmd, 2, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40Sensor intitial write error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }
        next_measurement_time = make_timeout_time_ms(1000);
}

void SCD40Sensor::process() {
        if (get_absolute_time() < next_measurement_time) {
                return;
        }

        uint8_t read_cmd[] = {0xec, 0x05};
        uint8_t data[9];

        int result = i2c_write_blocking(port, address, read_cmd, 2, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40Sensor write error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                next_measurement_time = make_timeout_time_ms(1000);
                return;
        }

        sleep_ms(5);

        result = i2c_read_blocking(port, address, data, 9, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40Sensor read error: %s\n",
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
        last_measurement.apparent_temperature = calculate_apparent_temperature(
            last_measurement.temperature, last_measurement.humidity);

        printf("SCD40Sensor: CO2: %u ppm, Temp: %.2f C, Apparent: %.2f C, "
               "Humidity: %.2f%%\n",
               last_measurement.co2, last_measurement.temperature,
               last_measurement.apparent_temperature,
               last_measurement.humidity);

        next_measurement_time = make_timeout_time_ms(5000);
        invoke_listeners(last_measurement);
}

float SCD40Sensor::calculate_apparent_temperature(float temperature,
                                                  float humidity) {
        float e = (humidity / 100.0f) * 6.105f *
                  expf((17.27f * temperature) / (237.7f + temperature));
        return temperature + 0.33f * e - 4.00f;
}
} // namespace sensors
