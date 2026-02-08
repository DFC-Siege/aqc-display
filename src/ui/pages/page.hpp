#pragma once

#include <optional>

#include "components/border.hpp"
#include "components/rect.hpp"
#include "display.hpp"
#include "input_manager.hpp"

namespace UI {
class Page {
      public:
        Page(Display::Display &display, Input::InputManager &input_manager)
            : display(display), input_manager(input_manager) {
        }
        virtual void update();
        virtual void draw();
        virtual void first_draw() {
                display.clear();
        }
        virtual void before_destroy() = 0;

      protected:
        Display::Display &display;
        Input::InputManager &input_manager;
        Rect rect;
        std::optional<Border> border;
};
} // namespace UI
