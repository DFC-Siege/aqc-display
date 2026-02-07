#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

#include "colors.hpp"
#include "components/bounding_box.hpp"
#include "components/drawable.hpp"
#include "components/postition.hpp"
#include "components/rect.hpp"
#include "display.hpp"
#include "font/font.hpp"
#include "font/font_factory.hpp"
#include "font/font_types.hpp"
#include "text.hpp"

namespace UI {
Text::Text(Display::Display &display, const Rect &rect)
    : text(""), font(Display::FontFactory::create(Display::FontType::DEFAULT)),
      Drawable(display, rect, Position{}, BoundingBox{}, Colors::BACKGROUND,
               Colors::PRIMARY) {
}

Text::Text(Display::Display &display, const Rect &rect, std::string text)
    : text(text),
      font(Display::FontFactory::create(Display::FontType::DEFAULT)),
      Drawable(display, rect, Position{}, BoundingBox{}, Colors::BACKGROUND,
               Colors::PRIMARY) {
}

void Text::draw() {
        clear();
        bounding_box = calculate_bounding_box();
        display.set_cursor(rect.x + position.x, rect.y + position.y);
        display.set_background(background);
        display.set_foreground(foreground);
        const auto &text_to_draw = wrap_text();
        display.print(text_to_draw);
}

void Text::set_text(const std::string &value) {
        text = value;
        draw();
}

Display::Font Text::get_font() const {
        return font;
}

BoundingBox Text::calculate_bounding_box() const {
        const auto &config = display.get_config();
        const uint32_t screen_limit = config.get_width();
        const auto max_width = rect.width - rect.x;

        const uint32_t effective_limit =
            (max_width > 0) ? max_width : screen_limit;

        uint32_t max_width_observed = 0;
        uint32_t current_line_width = 0;
        uint32_t lines = text.empty() ? 0 : 1;

        for (char c : text) {
                if (c == '\n') {
                        lines++;
                        current_line_width = 0;
                        continue;
                }

                current_line_width += font.width;

                if (current_line_width > effective_limit) {
                        lines++;
                        current_line_width = font.width;
                }

                if (current_line_width > max_width_observed) {
                        max_width_observed = current_line_width;
                }
        }

        uint16_t box_width = (max_width_observed > effective_limit)
                                 ? effective_limit
                                 : max_width_observed;
        uint16_t box_height = lines * font.height;

        return {(uint8_t)box_width, (uint8_t)box_height};
}

std::string Text::wrap_text() const {
        const auto max_width = rect.width - rect.x;
        if (max_width <= 0)
                return text;

        std::string result;
        uint32_t current_line_width = 0;

        for (char c : text) {
                if (c == '\n') {
                        current_line_width = 0;
                        result += c;
                        continue;
                }

                if (current_line_width + font.width > max_width) {
                        result += '\n';
                        current_line_width = 0;
                }

                result += c;
                current_line_width += font.width;
        }
        return result;
}
} // namespace UI
