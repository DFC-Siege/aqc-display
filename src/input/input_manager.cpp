#include <functional>
#include <utility>

#include "buttons/button_factory.hpp"
#include "input_manager.hpp"

namespace Input {
void InputManager::set_action(ButtonType type, std::function<void()> function) {
        buttons[type] = std::move(ButtonFactory::create(type, function));
}

void InputManager::clear_all() {
        buttons.clear();
}

void InputManager::clear_button(ButtonType type) {
        buttons.erase(type);
}

void InputManager::update() {
        for (auto &[type, button] : buttons) {
                button->update();
        }
}
} // namespace Input
