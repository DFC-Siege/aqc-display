#include <algorithm>
#include <cstdint>
#include <functional>
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <hardware/uart.h>
#include <pico/mutex.h>
#include <pico/platform/common.h>
#include <string>
#include <utility>

#include "serial_manager.hpp"

namespace Communication {
SerialManager::SerialManager() {
        mutex_init(&queue_mutex);
        uart_init(uart1, BAUDRATE);
        gpio_set_function(TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(RX_PIN, GPIO_FUNC_UART);
        uart_set_hw_flow(uart1, false, false);
        uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
        uart_set_fifo_enabled(uart1, true);
}

uint32_t
SerialManager::add_listener(std::function<void(std::string)> function) {
        listeners.emplace_back(Listener{++last_id, function});
        return last_id;
}

void SerialManager::remove_listener(uint32_t id) {
        auto it =
            std::remove_if(listeners.begin(), listeners.end(),
                           [id](const Listener &l) { return l.id == id; });
        listeners.erase(it, listeners.end());
}

void SerialManager::update() {
        mutex_enter_blocking(&queue_mutex);
        while (!to_be_invoked.empty()) {
                const auto &data = to_be_invoked.front();
                to_be_invoked.pop();

                mutex_exit(&queue_mutex);
                for (auto &listener : listeners) {
                        listener.function(data);
                }
                mutex_enter_blocking(&queue_mutex);
        }

        while (!to_be_sent.empty()) {
                const auto &data = to_be_sent.front();
                to_be_sent.pop();

                mutex_exit(&queue_mutex);
                send(data);
                mutex_enter_blocking(&queue_mutex);
        }
        mutex_exit(&queue_mutex);
}

void SerialManager::send(const std::string &value) {
        uart_write_blocking(uart1,
                            reinterpret_cast<const uint8_t *>(value.c_str()),
                            value.length());
}

void SerialManager::process() {
        static std::string local_buffer;

        while (uart_is_readable(uart1)) {
                char c = uart_getc(uart1);
                if (c == '\n') {
                        mutex_enter_blocking(&queue_mutex);
                        to_be_invoked.push(std::move(local_buffer));
                        mutex_exit(&queue_mutex);
                        local_buffer.clear();
                } else if (c != '\r') {
                        local_buffer += c;
                }
        }
        tight_loop_contents();
}
} // namespace Communication
