#include "drawable.hpp"
#include "display.hpp"
#include "pages/components/bounding_box.hpp"
#include "pages/components/postition.hpp"
#include "types.hpp"

namespace UI {
Drawable::Drawable(Display::Display &display, Position position,
                   BoundingBox bounding_box, Color background, Color foreground)
    : display(display), position(position), bounding_box(bounding_box),
      background(background), foreground(foreground) {
}

void Drawable::clear() {
        display.clear(position.x, position.y, bounding_box.width,
                      bounding_box.height);
}

void Drawable::set_position(const Position &position) {
        this->position = position;
}

void Drawable::set_bounding_box(const BoundingBox &bounding_box) {
        this->bounding_box = bounding_box;
}

void Drawable::set_background(Color color) {
        background = color;
}

void Drawable::set_foreground(Color color) {
        foreground = color;
}
} // namespace UI
