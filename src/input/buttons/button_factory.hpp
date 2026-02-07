#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>

#include "button.hpp"

namespace Input {
enum ButtonType {
        BUTTON1,
        BUTTON2,
};

class ButtonFactory {
      public:
        static std::unique_ptr<Button> create(ButtonType type,
                                              std::function<void()> function);
        static std::unique_ptr<Button> create(ButtonType type,
                                              std::function<void()> on_press,
                                              std::function<void()> on_hold);

      private:
        static std::map<ButtonType, uint8_t> pins;
};
} // namespace Input
