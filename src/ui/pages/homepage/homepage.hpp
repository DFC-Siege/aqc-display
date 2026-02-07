#pragma once

#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"

namespace UI {
class HomePage : public Page {
      public:
        HomePage(Display::Display &display, Input::InputManager &input_manager);
        void update();
        void draw();

      private:
        Text text;
};
} // namespace UI
