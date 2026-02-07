add_library(display_lib STATIC display.cpp)

target_include_directories(display_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(display_lib 
    pico_stdlib 
    hardware_spi 
    pico_displaylib_16
)
