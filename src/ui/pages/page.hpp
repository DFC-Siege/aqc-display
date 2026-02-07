#pragma once

#include "display.hpp"

namespace UI {
class Page {
      public:
        Page(Display::Display &display) : display(display) {
        }

        virtual void update() = 0;
        virtual void draw() = 0;

      protected:
        Display::Display &display;
};
} // namespace UI
