#pragma once

#include "components/bounding_box.hpp"
#include "components/postition.hpp"
#include "components/rect.hpp"
#include "display.hpp"
#include "types.hpp"

namespace UI {
class Drawable {
      public:
        Drawable(Display::Display &display, const Rect &rect, Position position,
                 BoundingBox bounding_box, Color background, Color foreground);
        virtual void draw() = 0;

        void clear();
        void set_position(const Position &position);
        void set_background(Color color);
        void set_foreground(Color color);

      protected:
        Display::Display &display;
        const Rect &rect;
        Position position;
        BoundingBox bounding_box;
        Color background;
        Color foreground;
        void set_bounding_box(const BoundingBox &bounding_box);
};
} // namespace UI
