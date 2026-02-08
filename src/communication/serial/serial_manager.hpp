#pragma once

#include <cstdint>
#include <functional>
#include <pico/mutex.h>
#include <pico/types.h>
#include <queue>
#include <string>
#include <vector>

namespace Communication {
struct Listener {
        uint32_t id;
        std::function<void(std::string)> function;
};

class SerialManager {
      public:
        SerialManager();

        uint32_t add_listener(std::function<void(std::string)>);
        void remove_listener(uint32_t id);
        void update();
        void process();
        void send(const std::string &value);

      private:
        static constexpr uint BAUDRATE = 115200;
        static constexpr auto TX_PIN = 9;
        static constexpr auto RX_PIN = 8;
        std::vector<Listener> listeners;
        std::queue<std::string> to_be_invoked;
        std::queue<std::string> to_be_sent;
        mutex_t queue_mutex;
        uint32_t last_id = 0;
};
} // namespace Communication
