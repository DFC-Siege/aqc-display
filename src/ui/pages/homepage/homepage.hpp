#pragma once

#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40/scd40.hpp"
#include "sps30/sps30.hpp"
#include <cstdint>

namespace UI {
class HomePage : public Page {
      public:
        HomePage(Display::Display &display, Input::InputManager &input_manager,
                 Sensors::SCD40 &scd_sensor, Sensors::SPS30 &sps_sensor);
        void draw() override;
        void before_destroy() override;

      private:
        void setup_positions();
        void setup_listeners();
        void update_scd_metrics(const Sensors::SCD40Measurement &data);
        void update_sps_metrics(const Sensors::SPS30Measurement &data);

        Text temperature_text;
        Text co2_text;
        Text humidity_text;
        Text pm1_text;
        Text pm2_text;
        Text pm4_text;
        Text pm10_text;

        Sensors::SCD40 &scd_sensor;
        Sensors::SPS30 &sps_sensor;
        uint32_t scd_listener_id;
        uint32_t sps_listener_id;
};
} // namespace UI
