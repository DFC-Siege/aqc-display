#include <hardware/gpio.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>

#include "display/display.hpp"
#include "presets/default.hpp"

int main() {
        stdio_init_all();

        auto &display = Display::Display::getInstance();
        display.initialize(Presets::Default);
        display.print("yee");

        while (true) {
                tight_loop_contents();
        }
}
