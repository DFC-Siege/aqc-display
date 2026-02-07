#include <utility>

#include "pages/page_type.hpp"
#include "ui_manager.hpp"

namespace UI {
void UIManager::update() {
        if (!page)
                return;

        page->update();
}

void UIManager::change_page(PageType type) {
        page = std::move(page_factory.create(type));
        page->draw();
}
} // namespace UI
