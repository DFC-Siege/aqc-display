#include <memory>

#include "homepage/homepage.hpp"
#include "page_factory.hpp"
#include "page_type.hpp"

namespace UI {
std::unique_ptr<Page> PageFactory::create(PageType type) const {
        switch (type) {
        case PageType::HOME:
                return std::make_unique<HomePage>(display, input_manager);
        default:
                return nullptr;
        }
}
} // namespace UI
