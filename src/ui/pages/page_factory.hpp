#pragma once

#include <memory>

#include "display.hpp"
#include "input_manager.hpp"
#include "page_type.hpp"
#include "scd40/scd40_sensor.hpp"
#include "sps30/sps30_sensor.hpp"

namespace ui {
class Page;
class PageFactory {
      public:
        PageFactory(display::Display &display,
                    input::InputManager &input_manager,
                    sensors::SCD40Sensor &scd_sensor,
                    sensors::SPS30Sensor &sps_sensor);
        std::unique_ptr<Page> create(PageType type) const;

      private:
        display::Display &display;
        input::InputManager &input_manager;
        sensors::SCD40Sensor &scd_sensor;
        sensors::SPS30Sensor &sps_sensor;
};
} // namespace ui
