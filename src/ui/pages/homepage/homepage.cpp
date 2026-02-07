#include "homepage.hpp"
#include "buttons/button_factory.hpp"
#include "components/text/text.hpp"
#include "display.hpp"
#include "input_manager.hpp"
#include "pages/page.hpp"

namespace UI {
HomePage::HomePage(Display::Display &display,
                   Input::InputManager &input_manager)
    : Page(display, input_manager), text(Text{display, "yee"}) {
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

void HomePage::draw() {
        display.set_cursor(0, 0);
        text.draw();
}
} // namespace UI
