#include <hardware/gpio.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>

#include "display/display.hpp"
#include "display/presets/default.hpp"
#include "input_manager.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

int main() {
        stdio_init_all();

        auto &display = Display::Display::getInstance();
        display.initialize(Presets::Default);

        Input::InputManager input_manager;
        UI::PageFactory page_factory{display, input_manager};
        UI::UIManager ui_manager{page_factory, display};

        while (true) {
                input_manager.update();
                ui_manager.update();
                tight_loop_contents();
        }
}
