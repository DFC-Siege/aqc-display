#pragma once

#include <memory>

#include "display.hpp"
#include "pages/page.hpp"
#include "pages/page_factory.hpp"
#include "pages/page_type.hpp"

namespace UI {
class UIManager {
      public:
        UIManager(const PageFactory &page_factory, Display::Display &display)
            : page_factory(page_factory), display(display) {
                change_page(PageType::HOME);
        }

        void update();
        void change_page(PageType type);

      private:
        bool initialized = false;
        const PageFactory &page_factory;
        Display::Display &display;
        std::unique_ptr<Page> page;
};
} // namespace UI
