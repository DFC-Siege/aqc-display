#pragma once

#include <memory>

#include "display.hpp"
#include "page_type.hpp"

namespace UI {
class Page;
class PageFactory {
      public:
        PageFactory(Display::Display &display) : display(display) {
        }

        std::unique_ptr<Page> create(PageType type) const;

      private:
        Display::Display &display;
};
} // namespace UI
