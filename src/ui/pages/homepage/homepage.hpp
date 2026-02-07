#pragma once

#include "display.hpp"
#include "pages/page.hpp"

namespace UI {
class HomePage : public Page {
      public:
        HomePage(Display::Display &display) : Page(display) {
        }

        void update();
        void draw();
};
} // namespace UI
