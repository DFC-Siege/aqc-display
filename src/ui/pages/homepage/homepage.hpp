#pragma once

#include <cstdint>

#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"
#include "scd40/scd40.hpp"

namespace UI {
class HomePage : public Page {
      public:
        HomePage(Display::Display &display, Input::InputManager &input_manager,
                 Sensors::SCD40 &scd_sensor);
        void update() override;
        void draw() override;
        void before_destroy() override;

      private:
        static constexpr auto CO2_THRESHOLD = 999;
        Text text;
        Sensors::SCD40 &scd_sensor;
        uint32_t scd_listener_id;
};
} // namespace UI
