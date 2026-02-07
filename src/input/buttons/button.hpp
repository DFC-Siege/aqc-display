#pragma once

#include <cstdint>
#include <functional>

namespace Input {
class Button {
      public:
        Button(uint8_t pin, std::function<void()> on_press);
        Button(uint8_t pin, std::function<void()> on_press,
               std::function<void()> on_long_press);
        void update();

      private:
        const uint8_t pin;
        const std::function<void()> on_press;
        const std::function<void()> on_long_press;
        bool last_is_pressed = false;
        bool should_reset = false;
        uint32_t first_press_time = 0;
        uint32_t debounce_start = 0;
        static constexpr auto DEBOUNCE = 50;
        static constexpr auto HOLD_THRESHOLD = 1000;
};
} // namespace Input
