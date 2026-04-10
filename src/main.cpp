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
#include <string>
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
#include "platform_mutex.hpp"
#include "result.hpp"
#include "sensors/scd40/scd40.hpp"
#include "sensors/sps30/sps30.hpp"
#include "serial_hal.hpp"
#include "serial_transporter.hpp"
#include "serializer.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

static constexpr auto TAG = "main";

enum Channel : transport::TransporterId {
        Chunked,
        Direct,
};

enum Command : transport::CommandId {
        SCD,
};

struct SCDData {
        uint16_t co2 = 0;
        float temperature = 0.0f;
        float humidity = 0.0f;
        std::string error;

        serializer::Data serialize() const {
                serializer::Writer w;
                w.write(co2);
                w.write(temperature);
                w.write(humidity);
                w.write(error);
                return std::move(w.buf);
        }

        static result::Result<SCDData> deserialize(serializer::DataView buf) {
                serializer::Reader r{buf};
                return result::ok(SCDData{
                    .co2 = TRY(r.read<uint16_t>()),
                    .temperature = TRY(r.read<float>()),
                    .humidity = TRY(r.read<float>()),
                    .error = TRY(r.read_string()),
                });
        }
};

void core1_entry() {
        auto *scd_sensor = (Sensors::SCD40 *)multicore_fifo_pop_blocking();
        auto *sps_sensor = (Sensors::SPS30 *)multicore_fifo_pop_blocking();
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
        logging::Logger<PicoMutex>::set(std::move(logger));

        auto &display = Display::Display::getInstance();
        display.initialize(Presets::Default);

        Input::InputManager input_manager;
        Sensors::SCD40 scd_sensor;
        Sensors::SPS30 sps_sensor;

        UI::PageFactory page_factory{display, input_manager, scd_sensor,
                                     sps_sensor};
        UI::UIManager ui_manager{page_factory, display};

        static constexpr auto MTU = 17;
        static constexpr auto MAX_TRIES = 3;
        static constexpr auto TIMEOUT = std::chrono::milliseconds(1000);
        static constexpr auto BAUDRATE = 115200;
        static constexpr auto TX_PIN = 9;
        static constexpr auto RX_PIN = 8;
        serial::SerialHal serial_hal(uart1, TX_PIN, RX_PIN, BAUDRATE);
        transport::SerialTransporter serial_transporter(serial_hal, MTU);
        transport::Multiplexer<transport::SerialTransporter, PicoMutex>
            multiplexer(std::move(serial_transporter));

        using MuxChannel = transport::Multiplexer<transport::SerialTransporter,
                                                  PicoMutex>::InnerChannel;
        using ChunkedMuxChannel =
            transport::ChunkedTransporter<MuxChannel, PicoMutex>;
        using DirectMuxChannel = transport::DirectTransporter<MuxChannel>;

        auto &inner_chunked_channel =
            multiplexer.create_inner_channel(Channel::Chunked);
        auto chunked = std::make_unique<ChunkedMuxChannel>(
            std::move(inner_chunked_channel), MAX_TRIES, TIMEOUT);

        auto &inner_direct_channel =
            multiplexer.create_inner_channel(Channel::Direct);
        auto direct =
            std::make_unique<DirectMuxChannel>(std::move(inner_direct_channel));

        transport::Dispatcher<transport::BaseTransporter, PicoMutex> dispatcher;
        dispatcher.register_transporter(Channel::Chunked, std::move(chunked));
        dispatcher.register_transporter(Channel::Direct, std::move(direct));

        scd_sensor.add_listener([&dispatcher](const auto &value) {
                SCDData data;
                data.co2 = value.co2;
                data.humidity = value.humidity;
                data.temperature = value.temperature;
                data.error = value.error;
                logging::logger().println("sending");
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
