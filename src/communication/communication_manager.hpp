#pragma once

#include <chrono>
#include <memory>
#include <utility>

#include "base_transporter.hpp"
#include "chunked_transporter.hpp"
#include "commands.hpp"
#include "direct_transporter.hpp"
#include "dispatcher.hpp"
#include "i_logger.hpp"
#include "logger.hpp"
#include "multiplexer.hpp"
#include "scd40_sensor.hpp"
#include "serial_hal.hpp"
#include "serial_transporter.hpp"
#include "serialized_dispatcher.hpp"
#include "sps30_sensor.hpp"

namespace communication {
enum Channel : transport::TransporterId {
        Chunked,
        Direct,
};

class CommunicationManager {
      public:
        CommunicationManager(serial::SerialHal &serial_hal,
                             sensors::SCD40Sensor &scd_sensor,
                             sensors::SPS30Sensor &sps_sensor)
            : multiplexer(std::make_unique<transport::SerialTransporter>(
                  serial_hal, MTU)) {
                auto inner_chunked_channel =
                    multiplexer.create_inner_channel(Channel::Chunked);
                auto chunked = std::make_unique<ChunkedMuxChannel>(
                    std::move(inner_chunked_channel), MAX_TRIES, TIMEOUT);
                auto inner_direct_channel =
                    multiplexer.create_inner_channel(Channel::Direct);
                auto direct = std::make_unique<DirectMuxChannel>(
                    std::move(inner_direct_channel));
                auto dispatcher = std::make_unique<
                    transport::Dispatcher<transport::BaseTransporter>>();
                dispatcher->register_transporter(Channel::Chunked,
                                                 std::move(chunked));
                dispatcher->register_transporter(Channel::Direct,
                                                 std::move(direct));
                serialized_dispatcher =
                    std::make_unique<transport::SerializedDispatcher<
                        transport::BaseTransporter>>(std::move(dispatcher));

                scd_sensor.add_listener([this](auto data) {
                        const auto result = serialized_dispatcher->send(
                            Channel::Chunked, models::Command::SCD,
                            std::move(data));
                        if (result.failed()) {
                                logging::logger().println(
                                    logging::LogLevel::Error, TAG,
                                    result.error());
                        }
                });

                sps_sensor.add_listener([this](auto data) {
                        const auto result = serialized_dispatcher->send(
                            Channel::Chunked, models::Command::SPS,
                            std::move(data));
                        if (result.failed()) {
                                logging::logger().println(
                                    logging::LogLevel::Error, TAG,
                                    result.error());
                        }
                });
        }

        transport::SerializedDispatcher<transport::BaseTransporter> &
        get_dispatcher() {
                return *serialized_dispatcher;
        }

      private:
        static constexpr auto TAG = "CommunicationManager";
        static constexpr auto MTU = 17;
        static constexpr auto MAX_TRIES = 3;
        static constexpr auto TIMEOUT = std::chrono::milliseconds(1000);

        using MuxChannel =
            transport::Multiplexer<transport::SerialTransporter>::InnerChannel;
        using ChunkedMuxChannel = transport::ChunkedTransporter<MuxChannel>;
        using DirectMuxChannel = transport::DirectTransporter<MuxChannel>;

        transport::Multiplexer<transport::SerialTransporter> multiplexer;
        std::unique_ptr<
            transport::SerializedDispatcher<transport::BaseTransporter>>
            serialized_dispatcher;
};
} // namespace communication
