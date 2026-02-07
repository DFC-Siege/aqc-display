#include <cmath>
#include <cstdint>
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
        const uint32_t screen_width = display.get_config().width;
        const uint32_t total_width = (uint32_t)font.width * text.size();
        uint16_t box_width =
            (total_width > screen_width) ? screen_width : total_width;
        uint32_t lines = (total_width + screen_width - 1) / screen_width;
        if (lines == 0 && !text.empty())
                lines = 1;
        uint16_t box_height = lines * font.height;

        return {(uint8_t)box_width, (uint8_t)box_height};
}
} // namespace UI
