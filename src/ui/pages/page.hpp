#pragma once

#include "display.hpp"
#include "input_manager.hpp"

namespace UI {
class Page {
      public:
        Page(Display::Display &display, Input::InputManager &input_manager)
            : display(display), input_manager(input_manager) {
        }

        virtual void update() = 0;
        virtual void draw() = 0;

      protected:
        Display::Display &display;
        Input::InputManager &input_manager;
};
} // namespace UI
