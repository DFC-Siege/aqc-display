add_library(input_lib STATIC 
        input_manager.cpp
        buttons/button_factory.cpp
        buttons/button.cpp
)

target_include_directories(input_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(input_lib 
    pico_stdlib 
    hardware_gpio
)
