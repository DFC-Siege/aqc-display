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
        i2c_write_blocking(I2C_PORT, ADDR, cmd, 2, false);
        sleep_ms(1000);
}

void SCD40::process() {
        uint8_t read_cmd[] = {0xec, 0x05};
        uint8_t data[9];

        int result = i2c_write_blocking(I2C_PORT, ADDR, read_cmd, 2, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40 write error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
                return;
        }

        sleep_ms(5);

        result = i2c_read_blocking(I2C_PORT, ADDR, data, 9, false);
        if (result < 0) {
                last_measurement.error = get_error_reason(result);
                printf("SCD40 read error: %s\n",
                       last_measurement.error.c_str());
                invoke_listeners(last_measurement);
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

        invoke_listeners(last_measurement);
}

std::string SCD40::get_error_reason(int error_code) {
        switch (error_code) {
        case PICO_OK:
                return "Success";
        case PICO_ERROR_GENERIC:
                return "Unspecified error";
        case PICO_ERROR_TIMEOUT:
                return "Timeout";
        case PICO_ERROR_NO_DATA:
                return "No data/FIFO empty";
        case PICO_ERROR_NOT_PERMITTED:
                return "Permission violation";
        case PICO_ERROR_INVALID_ARG:
                return "Invalid argument";
        case PICO_ERROR_IO:
                return "I/O error";
        case PICO_ERROR_BADAUTH:
                return "Bad credentials";
        case PICO_ERROR_CONNECT_FAILED:
                return "Connection failed";
        case PICO_ERROR_INSUFFICIENT_RESOURCES:
                return "Resource allocation failed";
        case PICO_ERROR_INVALID_ADDRESS:
                return "Address out-of-bounds";
        case PICO_ERROR_BAD_ALIGNMENT:
                return "Misaligned address";
        case PICO_ERROR_INVALID_STATE:
                return "Invalid state";
        case PICO_ERROR_BUFFER_TOO_SMALL:
                return "Buffer too small";
        case PICO_ERROR_PRECONDITION_NOT_MET:
                return "Precondition not met";
        case PICO_ERROR_MODIFIED_DATA:
                return "Inconsistent data";
        case PICO_ERROR_INVALID_DATA:
                return "Validation failed";
        case PICO_ERROR_NOT_FOUND:
                return "Not found";
        case PICO_ERROR_UNSUPPORTED_MODIFICATION:
                return "Clear OTP bit attempt";
        case PICO_ERROR_LOCK_REQUIRED:
                return "Lock not owned";
        case PICO_ERROR_VERSION_MISMATCH:
                return "Version mismatch";
        case PICO_ERROR_RESOURCE_IN_USE:
                return "Resource unavailable";
        default:
                return "Unknown error (" + std::to_string(error_code) + ")";
        }
}

} // namespace Sensors
