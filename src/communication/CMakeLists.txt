add_library(communication_lib STATIC 
        serial/serial_manager.cpp
        communication_manager.cpp
)

target_include_directories(communication_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(communication_lib 
    PUBLIC 
        pico_stdlib 
        hardware_gpio
        hardware_uart
)
