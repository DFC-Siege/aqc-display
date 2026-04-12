#pragma once

#include <optional>

#include "components/border.hpp"
#include "components/rect.hpp"
#include "display.hpp"
#include "input_manager.hpp"

namespace ui {
class Page {
      public:
        Page(display::Display &display, input::InputManager &input_manager)
            : display(display), input_manager(input_manager) {
        }
        virtual ~Page() = default;
        virtual void update();
        virtual void draw();
        virtual void first_draw() {
                display.clear();
        }
        virtual void before_destroy() = 0;

      protected:
        display::Display &display;
        input::InputManager &input_manager;
        Rect rect;
        std::optional<Border> border;
};
} // namespace ui
