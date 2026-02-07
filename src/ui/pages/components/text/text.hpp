#pragma once

#include <string>

#include "display.hpp"
#include "font/font.hpp"
#include "font/font_types.hpp"
#include "pages/components/bounding_box.hpp"
#include "pages/components/drawable.hpp"
#include "types.hpp"

namespace UI {
class Text : public Drawable {
      public:
        Text(Display::Display &display);
        void draw();
        void set_text(const std::string &value);
        void set_background(Color value);
        void set_font(Display::FontType font);

      private:
        std::string text;
        Display::Font font;
        BoundingBox calculate_bounding_box() const;
};
} // namespace UI
