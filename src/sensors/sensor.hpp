#pragma once

#include "pico/mutex.h"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

namespace Sensors {
template <typename T> struct Listener {
        uint32_t id;
        std::function<void(T)> function;
};

template <typename T> class Sensor {
      public:
        Sensor() {
                mutex_init(&queue_mutex);
        }

        uint32_t add_listener(std::function<void(T)> function) {
                listeners.emplace_back(Listener<T>{++last_id, function});
                return last_id;
        }

        void remove_listener(uint32_t id) {
                auto it = std::remove_if(
                    listeners.begin(), listeners.end(),
                    [id](const Listener<T> &l) { return l.id == id; });
                listeners.erase(it, listeners.end());
        }

        virtual void process() = 0;

        void update() {
                mutex_enter_blocking(&queue_mutex);
                while (!to_be_invoked.empty()) {
                        T data = to_be_invoked.front();
                        to_be_invoked.pop();

                        mutex_exit(&queue_mutex);
                        for (auto &listener : listeners) {
                                listener.function(data);
                        }
                        mutex_enter_blocking(&queue_mutex);
                }
                mutex_exit(&queue_mutex);
        }

      protected:
        void invoke_listeners(T data) {
                mutex_enter_blocking(&queue_mutex);
                to_be_invoked.push(data);
                mutex_exit(&queue_mutex);
        }

      private:
        std::vector<Listener<T>> listeners;
        std::queue<T> to_be_invoked;
        mutex_t queue_mutex;
        uint32_t last_id = 0;
};
} // namespace Sensors
