add_subdirectory(display)
add_subdirectory(input)
add_subdirectory(ui)
add_subdirectory(sensors)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        pico_multicore
        display_lib
        input_lib
        ui_lib
        sensors_lib
        serial_hal
        rp2040_serial_hal
        transporter
        direct_transporter
        serial_transporter
        chunked_transporter
        dispatcher
        console_logger
        serializer
)

pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
