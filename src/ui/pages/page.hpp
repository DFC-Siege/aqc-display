#pragma once

#include "components/rect.hpp"
#include "display.hpp"
#include "input_manager.hpp"

namespace UI {
class Page {
      public:
        Page(Display::Display &display, Input::InputManager &input_manager)
            : display(display), input_manager(input_manager) {
        }
        virtual void update() {
        }
        virtual void draw() = 0;
        virtual void before_destroy() = 0;

      protected:
        Display::Display &display;
        Input::InputManager &input_manager;
        Rect rect;
};
} // namespace UI
