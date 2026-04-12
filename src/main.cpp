#include <cstdint>
#include <cstdio>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <memory>
#include <pico/multicore.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <utility>

#include "communication/communication_manager.hpp"
#include "console_logger.hpp"
#include "display/display.hpp"
#include "display/presets/default.hpp"
#include "i_logger.hpp"
#include "input_manager.hpp"
#include "logger.hpp"
#include "models/commands.hpp"
#include "sensors/scd40/scd40_sensor.hpp"
#include "sensors/sps30/sps30_sensor.hpp"
#include "serial_hal.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

static constexpr auto TAG = "main";

void core1_entry() {
        auto *scd_sensor =
            (sensors::SCD40Sensor *)multicore_fifo_pop_blocking();
        auto *sps_sensor =
            (sensors::SPS30Sensor *)multicore_fifo_pop_blocking();
        auto *serial_hal = (serial::SerialHal *)multicore_fifo_pop_blocking();
        while (true) {
                scd_sensor->process();
                sps_sensor->process();
                serial_hal->loop();
        }
}

int main() {
        stdio_init_all();

        auto logger = std::make_unique<logging::ConsoleLogger>();
        logger->set_level(logging::LogLevel::Info);
        logging::set_logger(std::move(logger));

        static constexpr auto BAUDRATE = 115200;
        static constexpr auto TX_PIN = 9;
        static constexpr auto RX_PIN = 8;
        serial::SerialHal serial_hal(uart1, TX_PIN, RX_PIN, BAUDRATE);

        auto &display = display::Display::getInstance();
        display.initialize(Presets::Default);

        input::InputManager input_manager;

        static constexpr auto SCD_I2C_PORT = i2c1;
        static constexpr auto SCD_SDA_PIN = 14;
        static constexpr auto SCD_SCL_PIN = 15;
        static constexpr auto SCD_BAUDRATE = 100000;
        static constexpr auto SCD_ADDRESS = 0x62;
        sensors::SCD40Sensor scd_sensor{SCD_I2C_PORT, SCD_SDA_PIN, SCD_SCL_PIN,
                                        SCD_BAUDRATE, SCD_ADDRESS};

        static constexpr auto SPS_I2C_PORT = i2c0;
        static constexpr auto SPS_SDA_PIN = 24;
        static constexpr auto SPS_SCL_PIN = 21;
        static constexpr auto SPS_BAUDRATE = 100000;
        static constexpr auto SPS_ADDRESS = 0x69;
        sensors::SPS30Sensor sps_sensor{SPS_I2C_PORT, SPS_SDA_PIN, SPS_SCL_PIN,
                                        SPS_BAUDRATE, SPS_ADDRESS};

        ui::PageFactory page_factory{display, input_manager, scd_sensor,
                                     sps_sensor};
        ui::UIManager ui_manager{page_factory, display};
        communication::CommunicationManager communication_manager{serial_hal};
        auto &dispatcher = communication_manager.get_dispatcher();

        scd_sensor.add_listener([&dispatcher](auto data) {
                logging::logger().println("sending scd40");
                const auto result =
                    dispatcher.send(communication::Channel::Chunked,
                                    models::Command::SCD, std::move(data));
                if (result.failed()) {
                        logging::logger().println(logging::LogLevel::Error, TAG,
                                                  result.error());
                }
        });

        sps_sensor.add_listener([&dispatcher](auto data) {
                logging::logger().println("sending sps30");
                const auto result =
                    dispatcher.send(communication::Channel::Chunked,
                                    models::Command::SPS, std::move(data));
                if (result.failed()) {
                        logging::logger().println(logging::LogLevel::Error, TAG,
                                                  result.error());
                }
        });

        multicore_launch_core1(core1_entry);
        multicore_fifo_push_blocking((uint32_t)&scd_sensor);
        multicore_fifo_push_blocking((uint32_t)&sps_sensor);
        multicore_fifo_push_blocking((uint32_t)&serial_hal);

        while (true) {
                scd_sensor.update();
                sps_sensor.update();
                input_manager.update();
                ui_manager.update();
                tight_loop_contents();
        }
}
