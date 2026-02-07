#include "font_factory.hpp"
#include "displaylib_16/displaylib_16_Font.hpp"
#include "font/font.hpp"
#include "font/font_types.hpp"

namespace Display {
Font FontFactory::create(FontType type) {
        switch (type) {
        case FontType::DEFAULT:
                return Font{type, display_Font_name_e::font_default, 8, 8};
        case FontType::GLL:
                return Font{type, display_Font_name_e::font_gll, 8, 8};
        case FontType::GROTESK:
                return Font{type, display_Font_name_e::font_groTesk, 16, 32};
        case FontType::SINCLAIRS:
                return Font{type, display_Font_name_e::font_sinclairS, 8, 8};
        case FontType::ORLA:
                return Font{type, display_Font_name_e::font_orla, 16, 24};
        case FontType::HALLFETICA:
                return Font{type, display_Font_name_e::font_hallfetica, 16, 16};
        case FontType::ARIALROUND:
                return Font{type, display_Font_name_e::font_arialRound, 16, 24};
        case FontType::ARIALBOLD:
                return Font{type, display_Font_name_e::font_arialBold, 16, 16};
        case FontType::MEGA:
                return Font{type, display_Font_name_e::font_mega, 16, 16};
        case FontType::PICO:
                return Font{type, display_Font_name_e::font_pico, 8, 12};
        case FontType::RETRO:
                return Font{type, display_Font_name_e::font_retro, 8, 16};
        }

        return Font{type, display_Font_name_e::font_default};
}
} // namespace Display
