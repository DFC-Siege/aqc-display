#include <string>

#include "colors.hpp"
#include "display.hpp"
#include "font/font_factory.hpp"
#include "font/font_types.hpp"
#include "pages/components/bounding_box.hpp"
#include "pages/components/drawable.hpp"
#include "pages/components/postition.hpp"
#include "text.hpp"

namespace UI {
Text::Text(Display::Display &display)
    : text(""), font(Display::FontFactory::create(Display::FontType::DEFAULT)),
      Drawable(display, Position{}, BoundingBox{}, Colors::BACKGROUND,
               Colors::PRIMARY) {
}

void Text::draw() {
        display.set_cursor(position.x, position.y);
        display.set_background(background);
        display.set_foreground(foreground);
        display.print(text);
}

void Text::set_text(const std::string &value) {
        text = value;
}

BoundingBox Text::calculate_bounding_box() const {
        return {font.width, font.height};
}
} // namespace UI
