#pragma once

#include <functional>
#include <map>
#include <memory>

#include "buttons/button.hpp"
#include "buttons/button_factory.hpp"

namespace Input {
class InputManager {
      public:
        void clear_all();
        void clear_button(ButtonType type);
        void set_action(ButtonType type, std::function<void()> function);
        void update();

      private:
        std::map<ButtonType, std::unique_ptr<Button>> buttons;
};
} // namespace Input
