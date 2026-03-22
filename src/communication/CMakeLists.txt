add_library(communication_lib STATIC 
        communication_manager.cpp
)

target_include_directories(communication_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(communication_lib 
    PUBLIC 
        pico_stdlib 
        hardware_gpio
        hardware_uart
        sensors_lib
        rp2040_serial_hal
)
