#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

#include "colors.hpp"
#include "components/bounding_box.hpp"
#include "components/drawable.hpp"
#include "components/postition.hpp"
#include "display.hpp"
#include "displaylib_16/st7789.hpp"
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
        bool rotated = config.rotation == ST7789_TFT::Degrees_270 ||
                       ST7789_TFT::Degrees_90;
        const uint32_t screen_width = rotated ? config.height : config.width;
        const uint32_t total_width = (uint32_t)font.width * text.size();

        uint16_t box_width =
            (total_width > screen_width) ? screen_width : total_width;
        uint32_t lines = (total_width + screen_width - 1) / screen_width;

        if (lines == 0 && !text.empty())
                lines = 1;

        uint16_t box_height = lines * font.height;

        printf("[UI LOG] Text: \"%s\" | Box: %ux%u | Lines: %u\n", text.c_str(),
               box_width, box_height, lines);

        return {(uint8_t)box_width, (uint8_t)box_height};
}
} // namespace UI
