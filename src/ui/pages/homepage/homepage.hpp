#pragma once

#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40.hpp"
#include "scd40/scd40_sensor.hpp"
#include "sps30.hpp"
#include "sps30/sps30_sensor.hpp"
#include <cstdint>

namespace ui {
class HomePage : public Page {
      public:
        HomePage(display::Display &display, input::InputManager &input_manager,
                 sensors::SCD40Sensor &scd_sensor,
                 sensors::SPS30Sensor &sps_sensor);
        void draw() override;
        void before_destroy() override;

      private:
        static constexpr auto PADDING = 8;
        Text temperature_text;
        Text apparent_temperature_text;
        Text co2_text;
        Text humidity_text;
        Text pm1_text;
        Text pm2_text;
        Text pm4_text;
        Text pm10_text;
        sensors::SCD40Sensor &scd_sensor;
        sensors::SPS30Sensor &sps_sensor;
        uint32_t scd_listener_id;
        uint32_t sps_listener_id;

        void setup_positions();
        void setup_listeners();
        void update_scd_metrics(const models::SCD40 &data);
        void update_sps_metrics(const models::SPS30 &data);
};
} // namespace ui
