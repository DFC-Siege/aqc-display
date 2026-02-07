#include "display/display.h"
#include "pico/stdio.h"
#include <hardware/gpio.h>
#include <pico/platform/common.h>

int main() {
        stdio_init_all();

        auto &display = Display::Display::getInstance();
        display.initialize();
        display.print("yee");

        while (true) {
                tight_loop_contents();
        }
}
