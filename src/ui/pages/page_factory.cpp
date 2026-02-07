#include <memory>

#include "display.hpp"
#include "homepage/homepage.hpp"
#include "input_manager.hpp"
#include "page_factory.hpp"
#include "page_type.hpp"
#include "scd40/scd40.hpp"
#include "sps30/sps30.hpp"

namespace UI {
PageFactory::PageFactory(Display::Display &display,
                         Input::InputManager &input_manager,
                         Sensors::SCD40 &scd_sensor, Sensors::SPS30 &sps_sensor)
    : display(display), input_manager(input_manager), scd_sensor(scd_sensor),
      sps_sensor(sps_sensor) {
}

std::unique_ptr<Page> PageFactory::create(PageType type) const {
        switch (type) {
        case PageType::HOME:
                return std::make_unique<HomePage>(display, input_manager,
                                                  scd_sensor, sps_sensor);
        }

        return nullptr;
}
} // namespace UI
