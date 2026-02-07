#pragma once

#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"

namespace UI {
class HomePage : public Page {
      public:
        HomePage(Display::Display &display, Input::InputManager &input_manager)
            : Page(display, input_manager) {
        }

        void update();
        void draw();
};
} // namespace UI
