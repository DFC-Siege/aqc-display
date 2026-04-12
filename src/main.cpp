#include <chrono>
#include <cstdint>
#include <cstdio>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <memory>
#include <pico/multicore.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <utility>

#include "base_transporter.hpp"
#include "chunked_transporter.hpp"
#include "console_logger.hpp"
#include "direct_transporter.hpp"
#include "dispatcher.hpp"
#include "display/display.hpp"
#include "display/presets/default.hpp"
#include "i_logger.hpp"
#include "input_manager.hpp"
#include "logger.hpp"
#include "multiplexer.hpp"
#include "requester.hpp"
#include "sensors/scd40/scd40_sensor.hpp"
#include "sensors/sps30/sps30_sensor.hpp"
#include "serial_hal.hpp"
#include "serial_transporter.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

static constexpr auto TAG = "main";

enum Channel : transport::TransporterId {
        Chunked,
        Direct,
};

enum Command : transport::CommandId {
        SCD,
        SCDRequest,
};

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

        auto &display = display::Display::getInstance();
        display.initialize(Presets::Default);

        input::InputManager input_manager;
        sensors::SCD40Sensor scd_sensor;
        sensors::SPS30Sensor sps_sensor;

        ui::PageFactory page_factory{display, input_manager, scd_sensor,
                                     sps_sensor};
        ui::UIManager ui_manager{page_factory, display};

        static constexpr auto MTU = 17;
        static constexpr auto MAX_TRIES = 3;
        static constexpr auto TIMEOUT = std::chrono::milliseconds(1000);
        static constexpr auto BAUDRATE = 115200;
        static constexpr auto TX_PIN = 9;
        static constexpr auto RX_PIN = 8;
        serial::SerialHal serial_hal(uart1, TX_PIN, RX_PIN, BAUDRATE);
        auto serial_transporter =
            std::make_unique<transport::SerialTransporter>(serial_hal, MTU);
        transport::Multiplexer<transport::SerialTransporter> multiplexer(
            std::move(serial_transporter));

        using MuxChannel =
            transport::Multiplexer<transport::SerialTransporter>::InnerChannel;
        using ChunkedMuxChannel = transport::ChunkedTransporter<MuxChannel>;
        using DirectMuxChannel = transport::DirectTransporter<MuxChannel>;

        auto inner_chunked_channel =
            multiplexer.create_inner_channel(Channel::Chunked);
        auto chunked = std::make_unique<ChunkedMuxChannel>(
            std::move(inner_chunked_channel), MAX_TRIES, TIMEOUT);

        auto inner_direct_channel =
            multiplexer.create_inner_channel(Channel::Direct);
        auto direct =
            std::make_unique<DirectMuxChannel>(std::move(inner_direct_channel));

        transport::Dispatcher<transport::BaseTransporter> dispatcher;
        dispatcher.register_transporter(Channel::Chunked, std::move(chunked));
        dispatcher.register_transporter(Channel::Direct, std::move(direct));

        transport::Requester<transport::BaseTransporter> requester(dispatcher);

        scd_sensor.add_listener([&dispatcher](const auto &data) {
                logging::logger().println("sending scd40");
                const auto result = dispatcher.send(
                    Channel::Chunked, Command::SCD, data.serialize());
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
