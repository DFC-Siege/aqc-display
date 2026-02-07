#include <cstdint>
#include <hardware/gpio.h>
#include <pico/multicore.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>
#include <pico/time.h>

#include "display/display.hpp"
#include "display/presets/default.hpp"
#include "input_manager.hpp"
#include "sensors/scd40/scd40.hpp"
#include "sensors/sps30/sps30.hpp"
#include "ui/pages/page_factory.hpp"
#include "ui/ui_manager.hpp"

void core1_entry() {
        auto *scd_sensor = (Sensors::SCD40 *)multicore_fifo_pop_blocking();
        auto *sps_sensor = (Sensors::SPS30 *)multicore_fifo_pop_blocking();

        while (true) {
                scd_sensor->process();
                sps_sensor->process();
        }
}

int main() {
        stdio_init_all();

        auto &display = Display::Display::getInstance();
        display.initialize(Presets::Default);

        Input::InputManager input_manager;
        Sensors::SCD40 scd_sensor;
        Sensors::SPS30 sps_sensor;

        UI::PageFactory page_factory{display, input_manager, scd_sensor,
                                     sps_sensor};
        UI::UIManager ui_manager{page_factory, display};

        multicore_launch_core1(core1_entry);
        multicore_fifo_push_blocking((uint32_t)&scd_sensor);
        multicore_fifo_push_blocking((uint32_t)&sps_sensor);

        while (true) {
                scd_sensor.update();
                sps_sensor.update();
                input_manager.update();
                ui_manager.update();
                tight_loop_contents();
        }
}
