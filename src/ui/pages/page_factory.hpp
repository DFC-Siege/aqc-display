#pragma once

#include <memory>

#include "display.hpp"
#include "input_manager.hpp"
#include "page_type.hpp"
#include "scd40/scd40.hpp"
#include "sps30/sps30.hpp"

namespace UI {
class Page;
class PageFactory {
      public:
        PageFactory(Display::Display &display,
                    Input::InputManager &input_manager,
                    Sensors::SCD40 &scd_sensor, Sensors::SPS30 &sps_sensor);
        std::unique_ptr<Page> create(PageType type) const;

      private:
        Display::Display &display;
        Input::InputManager &input_manager;
        Sensors::SCD40 &scd_sensor;
        Sensors::SPS30 &sps_sensor;
};
} // namespace UI
