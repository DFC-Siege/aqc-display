#pragma once

#include <chrono>
#include <memory>
#include <utility>

#include "base_transporter.hpp"
#include "chunked_transporter.hpp"
#include "direct_transporter.hpp"
#include "dispatcher.hpp"
#include "multiplexer.hpp"
#include "serial_hal.hpp"
#include "serial_transporter.hpp"

namespace communication {
enum Channel : transport::TransporterId {
        Chunked,
        Direct,
};

class CommunicationManager {
      public:
        CommunicationManager(serial::SerialHal &serial_hal) {
                static constexpr auto MTU = 17;
                static constexpr auto MAX_TRIES = 3;
                static constexpr auto TIMEOUT = std::chrono::milliseconds(1000);

                auto serial_transporter =
                    std::make_unique<transport::SerialTransporter>(serial_hal,
                                                                   MTU);
                transport::Multiplexer<transport::SerialTransporter>
                    multiplexer(std::move(serial_transporter));

                using MuxChannel = transport::Multiplexer<
                    transport::SerialTransporter>::InnerChannel;
                using ChunkedMuxChannel =
                    transport::ChunkedTransporter<MuxChannel>;
                using DirectMuxChannel =
                    transport::DirectTransporter<MuxChannel>;

                auto inner_chunked_channel =
                    multiplexer.create_inner_channel(Channel::Chunked);
                auto chunked = std::make_unique<ChunkedMuxChannel>(
                    std::move(inner_chunked_channel), MAX_TRIES, TIMEOUT);

                auto inner_direct_channel =
                    multiplexer.create_inner_channel(Channel::Direct);
                auto direct = std::make_unique<DirectMuxChannel>(
                    std::move(inner_direct_channel));

                dispatcher.register_transporter(Channel::Chunked,
                                                std::move(chunked));
                dispatcher.register_transporter(Channel::Direct,
                                                std::move(direct));
        }

        transport::Dispatcher<transport::BaseTransporter> &get_dispatcher() {
                return dispatcher;
        }

      private:
        transport::Dispatcher<transport::BaseTransporter> dispatcher;
};
} // namespace communication
