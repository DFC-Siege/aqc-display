#include "homepage.hpp"
#include "buttons/button_factory.hpp"
#include "colors.hpp"
#include "components/border.hpp"
#include "components/rect.hpp"
#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40/scd40.hpp"
#include "sps30/sps30.hpp"
#include <cstdint>
#include <format>

namespace UI {

HomePage::HomePage(Display::Display &display,
                   Input::InputManager &input_manager,
                   Sensors::SCD40 &scd_sensor, Sensors::SPS30 &sps_sensor)
    : Page(display, input_manager),
      temperature_text(Text{display, rect, "Temperature:"}),
      co2_text(Text{display, rect, "CO2:"}),
      humidity_text(Text{display, rect, "Humidity:"}),
      pm1_text(Text{display, rect, "PM1:"}),
      pm2_text(Text{display, rect, "PM2.5:"}),
      pm4_text(Text{display, rect, "PM4:"}),
      pm10_text(Text{display, rect, "PM10:"}), scd_sensor(scd_sensor),
      sps_sensor(sps_sensor) {
        const auto &config = display.get_config();
        rect = Rect{PADDING, PADDING,
                    static_cast<uint16_t>(config.get_width() - PADDING),
                    static_cast<uint16_t>(config.get_height() - PADDING)};
        const auto border_rect =
            Rect{PADDING - 4, PADDING - 4,
                 static_cast<uint16_t>(config.get_width() - PADDING + 4),
                 static_cast<uint16_t>(config.get_height() - PADDING + 4)};
        border = Border{2, border_rect, 4, Colors::ERROR};
        setup_positions();
        setup_listeners();

        input_manager.set_action(Input::ButtonType::BUTTON1, [this]() {
                border->color = border->color == Colors::ERROR ? Colors::SUCCESS
                                                               : Colors::ERROR;
                draw();
        });
        input_manager.set_action(Input::ButtonType::BUTTON2, [this]() {
                border->color = border->color == Colors::ERROR ? Colors::SUCCESS
                                                               : Colors::ERROR;
                draw();
        });
        draw();
}

void HomePage::setup_positions() {
        const uint16_t h = temperature_text.get_font().height;

        temperature_text.set_position({0, 0});
        co2_text.set_position({0, h});
        humidity_text.set_position({0, static_cast<uint16_t>(h * 2)});

        pm1_text.set_position({0, static_cast<uint16_t>(h * 4)});
        pm2_text.set_position({0, static_cast<uint16_t>(h * 5)});
        pm4_text.set_position({0, static_cast<uint16_t>(h * 6)});
        pm10_text.set_position({0, static_cast<uint16_t>(h * 7)});
}

void HomePage::setup_listeners() {
        scd_listener_id = scd_sensor.add_listener([this](const auto &data) {
                update_scd_metrics(data);
                draw();
        });

        sps_listener_id = sps_sensor.add_listener([this](const auto &data) {
                update_sps_metrics(data);
                draw();
        });
}

void HomePage::update_scd_metrics(const Sensors::SCD40Measurement &data) {
        temperature_text.set_text(
            std::format("Temperature: {:.1f} C", data.temperature));
        if (data.temperature > 25.0f)
                temperature_text.set_foreground(Colors::ERROR);
        else if (data.temperature > 23.0f)
                temperature_text.set_foreground(Colors::WARNING);
        else
                temperature_text.set_foreground(Colors::PRIMARY);

        humidity_text.set_text(
            std::format("Humidity: {:.1f} %", data.humidity));
        if (data.humidity > 60.0f || data.humidity < 30.0f)
                humidity_text.set_foreground(Colors::WARNING);
        else
                humidity_text.set_foreground(Colors::PRIMARY);

        co2_text.set_text(std::format("CO2: {} ppm", data.co2));
        if (data.co2 > 1500)
                co2_text.set_foreground(Colors::ERROR);
        else if (data.co2 > 1000)
                co2_text.set_foreground(Colors::WARNING);
        else
                co2_text.set_foreground(Colors::PRIMARY);
}

void HomePage::update_sps_metrics(const Sensors::SPS30Measurement &data) {
        auto set_pm_color = [](Text &t, float val, float warn, float err) {
                if (val > err)
                        t.set_foreground(Colors::ERROR);
                else if (val > warn)
                        t.set_foreground(Colors::WARNING);
                else
                        t.set_foreground(Colors::PRIMARY);
        };

        pm1_text.set_text(std::format("PM1.0: {:.1f}", data.pm1_0));
        set_pm_color(pm1_text, data.pm1_0, 10.0f, 30.0f);

        pm2_text.set_text(std::format("PM2.5: {:.1f}", data.pm2_5));
        set_pm_color(pm2_text, data.pm2_5, 12.0f, 35.5f);

        pm4_text.set_text(std::format("PM4.0: {:.1f}", data.pm4_0));
        set_pm_color(pm4_text, data.pm4_0, 25.0f, 50.0f);

        pm10_text.set_text(std::format("PM10: {:.1f}", data.pm10_0));
        set_pm_color(pm10_text, data.pm10_0, 54.0f, 154.0f);
}

void HomePage::draw() {
        Page::draw();

        temperature_text.draw();
        co2_text.draw();
        humidity_text.draw();
        pm1_text.draw();
        pm2_text.draw();
        pm4_text.draw();
        pm10_text.draw();
}

void HomePage::before_destroy() {
        scd_sensor.remove_listener(scd_listener_id);
        sps_sensor.remove_listener(sps_listener_id);
}

} // namespace UI
