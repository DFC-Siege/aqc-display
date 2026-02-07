#pragma once

#include "display.hpp"
#include "pages/components/bounding_box.hpp"
#include "pages/components/postition.hpp"
#include "types.hpp"

namespace UI {
class Drawable {
      public:
        Drawable(Display::Display &display, Position position,
                 BoundingBox bounding_box, Color background, Color foreground);
        void clear();
        void set_position(const Position &position);
        void set_background(Color color);
        void set_foreground(Color color);

      protected:
        Display::Display &display;
        Position position;
        BoundingBox bounding_box;
        Color background;
        Color foreground;

        void set_bounding_box(const BoundingBox &bounding_box);
};
} // namespace UI
