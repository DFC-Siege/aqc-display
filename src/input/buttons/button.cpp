#include <cstdint>
#include <functional>
#include <hardware/gpio.h>
#include <pico/time.h>

#include "button.hpp"

namespace Input {

Button::Button(uint8_t pin, std::function<void()> on_press)
    : pin(pin), on_press(on_press) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
}

Button::Button(uint8_t pin, std::function<void()> on_press,
               std::function<void()> on_long_press)
    : pin(pin), on_press(on_press), on_long_press(on_long_press) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
}

void Button::update() {
        bool is_pressed = !gpio_get(pin);
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (should_reset) {
                if (is_pressed)
                        return;
                last_is_pressed = is_pressed;
                should_reset = false;
        }

        const auto is_debounce = now - debounce_start < DEBOUNCE;
        if (is_debounce)
                return;

        if (!is_pressed && !last_is_pressed)
                return;

        if (is_pressed && !last_is_pressed) {
                first_press_time = now;
                last_is_pressed = is_pressed;
                return;
        }

        const auto is_long_press = now - first_press_time > HOLD_THRESHOLD;
        if (!is_pressed && last_is_pressed && !is_long_press) {
                last_is_pressed = is_pressed;
                debounce_start = now;
                if (on_press)
                        on_press();

                return;
        }

        if (last_is_pressed && is_long_press) {
                last_is_pressed = is_pressed;
                debounce_start = now;
                should_reset = true;
                if (on_long_press)
                        on_long_press();

                return;
        }
}
} // namespace Input
