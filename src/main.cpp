#include <hardware/gpio.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>

#include "display/display.hpp"
#include "presets/default.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

int main() {
        stdio_init_all();

        auto &display = Display::Display::getInstance();
        display.initialize(Presets::Default);

        UI::PageFactory page_factory{display};
        UI::UIManager ui_manager{page_factory, display};

        while (true) {
                ui_manager.update();
                tight_loop_contents();
        }
}
