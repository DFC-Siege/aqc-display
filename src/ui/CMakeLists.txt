add_library(ui_lib STATIC 
        ui_manager.cpp
        pages/page_factory.cpp
        pages/page.cpp
        pages/homepage/homepage.cpp
        components/drawable.cpp
        components/text/text.cpp
)

target_link_libraries(ui_lib PUBLIC display_lib)
target_link_libraries(ui_lib PUBLIC input_lib)
target_link_libraries(ui_lib PUBLIC sensors_lib)
target_include_directories(ui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
