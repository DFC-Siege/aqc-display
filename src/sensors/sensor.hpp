#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <pico/error.h>
#include <pico/mutex.h>
#include <queue>
#include <string>
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

        static std::string get_error_reason(int error_code) {
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
                        return "Unknown error (" + std::to_string(error_code) +
                               ")";
                }
        }

      private:
        std::vector<Listener<T>> listeners;
        std::queue<T> to_be_invoked;
        mutex_t queue_mutex;
        uint32_t last_id = 0;
};
} // namespace Sensors
