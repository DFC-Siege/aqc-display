#include "page.hpp"
#include "components/border.hpp"

namespace UI {
void Page::update() {
}

void Page::draw() {
        if (border.has_value()) {
                const auto &rect = border->rect;
                const auto &inner_rect = border->get_inner_rect();
                display.draw_rectangle(rect.x, rect.y, rect.width, rect.height,
                                       border->color);
                display.draw_rectangle(inner_rect.x, inner_rect.y,
                                       inner_rect.width, inner_rect.height,
                                       border->color);
        }
}
} // namespace UI
