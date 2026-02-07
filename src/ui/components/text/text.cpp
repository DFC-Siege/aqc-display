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
        const uint16_t width = font.width * text.size();
        return {width, font.height};
}
} // namespace UI
