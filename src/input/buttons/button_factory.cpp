#include <cstdint>
#include <functional>
#include <map>
#include <memory>

#include "button.hpp"
#include "button_factory.hpp"

namespace Input {
std::map<ButtonType, uint8_t> ButtonFactory::pins = {{ButtonType::BUTTON1, 6},
                                                     {ButtonType::BUTTON2, 7}};

std::unique_ptr<Button> ButtonFactory::create(ButtonType type,
                                              std::function<void()> function) {
        return std::make_unique<Button>(pins[type], function);
}

} // namespace Input
