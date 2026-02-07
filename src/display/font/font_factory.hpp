#pragma once

#include "font.hpp"
#include "font_types.hpp"

namespace Display {

class FontFactory {
      public:
        static Font create(FontType type);
};
} // namespace Display
