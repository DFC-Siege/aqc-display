#pragma once

#include "font.hpp"
#include "font_types.hpp"

namespace display {

class FontFactory {
      public:
        static Font create(FontType type);
};
} // namespace display
