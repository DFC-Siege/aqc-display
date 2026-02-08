#include <cstdint>

#include "components/border.hpp"
#include "page.hpp"

namespace UI {
void Page::update() {
}

void Page::draw() {
        if (border.has_value()) {
                const auto &rect = border->rect;
                for (auto i = 0; i < border->size; i++) {
                        int16_t current_rounding = border->rounding - i;
                        if (current_rounding < 0)
                                current_rounding = 0;

                        display.draw_rectangle(rect.x + i, rect.y + i,
                                               rect.width - i * 2,
                                               rect.height - i * 2,
                                               current_rounding, border->color);
                }
        }
}
} // namespace UI
