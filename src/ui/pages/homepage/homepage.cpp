#include <cstdint>
#include <cstdio>
#include <format>

#include "buttons/button_factory.hpp"
#include "colors.hpp"
#include "components/text/text.hpp"
#include "display.hpp"
#include "homepage.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40/scd40.hpp"
#include "sps30/sps30.hpp"

namespace UI {
HomePage::HomePage(Display::Display &display,
                   Input::InputManager &input_manager,
                   Sensors::SCD40 &scd_sensor, Sensors::SPS30 &sps_sensor)
    : Page(display, input_manager),
      scd_text(Text{display, "waiting for sensor"}),
      sps_text(Text{display, "waiting for sensor"}), scd_sensor(scd_sensor),
      sps_sensor(sps_sensor) {
        const auto &config = display.get_config();
        const uint16_t y =
            config.is_rotated() ? config.width / 2 : config.height / 2;
        sps_text.set_position({0, y});
        sps_text.draw();
        scd_listener_id =
            scd_sensor.add_listener([this](Sensors::SCD40Measurement data) {
                    if (!data.error.empty()) {
                            const auto text =
                                std::format("error: {}", data.error);
                            this->scd_text.set_text(text);
                            return;
                    }

                    const auto text =
                        std::format("Temp: {:.1f} C\nCO2: {}\nHum: {:.1f}",
                                    data.temperature, data.co2, data.humidity);
                    this->scd_text.set_foreground(data.co2 > CO2_THRESHOLD
                                                      ? Colors::ERROR
                                                      : Colors::PRIMARY);
                    this->scd_text.set_text(text);
            });
        sps_listener_id =
            sps_sensor.add_listener([this](Sensors::SPS30Measurement data) {
                    if (!data.error.empty()) {
                            const auto text =
                                std::format("error: {}", data.error);
                            this->sps_text.set_text(text);
                            return;
                    }

                    const auto text = std::format(
                        "PM1: {:.1f}\nPM2.5: {}\nPM4: {:.1f}\nPM10: {:.1f}",
                        data.pm1_0, data.pm2_5, data.pm4_0, data.pm10_0);
                    this->sps_text.set_text(text);
            });
        input_manager.set_action(Input::ButtonType::BUTTON1, [this]() {
                this->scd_text.set_text("yeet");
                this->draw();
        });
        input_manager.set_action(Input::ButtonType::BUTTON2, [this]() {
                this->scd_text.set_text("yeehaw");
                this->draw();
        });
}

void HomePage::update() {
}

void HomePage::before_destroy() {
        scd_sensor.remove_listener(scd_listener_id);
}

void HomePage::draw() {
        display.set_cursor(0, 0);
        scd_text.draw();
        sps_text.draw();
}
} // namespace UI
