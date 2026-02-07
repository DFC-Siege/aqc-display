#include <cstdio>
#include <format>

#include "buttons/button_factory.hpp"
#include "components/text/text.hpp"
#include "display.hpp"
#include "homepage.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40/scd40.hpp"

namespace UI {
HomePage::HomePage(Display::Display &display,
                   Input::InputManager &input_manager,
                   Sensors::SCD40 &scd_sensor)
    : Page(display, input_manager), text(Text{display, "yee"}),
      scd_sensor(scd_sensor) {
        scd_listener_id =
            scd_sensor.add_listener([this](Sensors::SCD40Measurement data) {
                    if (!data.error.empty()) {
                            const auto text =
                                std::format("error: {}", data.error);
                            this->text.set_text(text);
                            return;
                    }

                    const auto text =
                        std::format("Temp: {:.1f} C\nCO2: {}\nHum: {:.1f}",
                                    data.temperature, data.co2, data.humidity);
                    this->text.set_text(text);
            });
        input_manager.set_action(Input::ButtonType::BUTTON1, [this]() {
                this->text.set_text("yeet");
                this->draw();
        });
        input_manager.set_action(Input::ButtonType::BUTTON2, [this]() {
                this->text.set_text("yeehaw");
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
        text.draw();
}
} // namespace UI
