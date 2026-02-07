#pragma once

#include <string>

#include "components/bounding_box.hpp"
#include "components/drawable.hpp"
#include "display.hpp"
#include "font/font.hpp"
#include "font/font_types.hpp"
#include "types.hpp"

namespace UI {
class Text : public Drawable {
      public:
        Text(Display::Display &display);
        void draw() override;
        void set_text(const std::string &value);
        void set_background(Color value);
        void set_font(Display::FontType font);

      private:
        std::string text;
        Display::Font font;
        BoundingBox calculate_bounding_box() const;
};
} // namespace UI
