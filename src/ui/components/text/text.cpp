#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

#include "colors.hpp"
#include "components/bounding_box.hpp"
#include "components/drawable.hpp"
#include "components/postition.hpp"
#include "display.hpp"
#include "font/font_factory.hpp"
#include "font/font_types.hpp"
#include "text.hpp"

namespace UI {
Text::Text(Display::Display &display)
    : text(""), font(Display::FontFactory::create(Display::FontType::DEFAULT)),
      Drawable(display, Position{}, BoundingBox{}, Colors::BACKGROUND,
               Colors::PRIMARY) {
}

Text::Text(Display::Display &display, std::string text)
    : text(text),
      font(Display::FontFactory::create(Display::FontType::DEFAULT)),
      Drawable(display, Position{}, BoundingBox{}, Colors::BACKGROUND,
               Colors::PRIMARY) {
}

void Text::draw() {
        clear();
        bounding_box = calculate_bounding_box();
        display.set_cursor(position.x, position.y);
        display.set_background(background);
        display.set_foreground(foreground);
        display.print(text);
}

void Text::set_text(const std::string &value) {
        text = value;
        draw();
}

BoundingBox Text::calculate_bounding_box() const {
        const auto &config = display.get_config();
        const uint32_t screen_width =
            config.is_rotated() ? config.height : config.width;

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

                if (current_line_width > screen_width) {
                        lines++;
                        current_line_width = font.width;
                }

                if (current_line_width > max_width_observed) {
                        max_width_observed = current_line_width;
                }
        }

        uint16_t box_width = (max_width_observed > screen_width)
                                 ? screen_width
                                 : max_width_observed;
        uint16_t box_height = lines * font.height;

        return {(uint8_t)box_width, (uint8_t)box_height};
}
} // namespace UI
