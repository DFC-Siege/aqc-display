add_library(sensors_lib STATIC 
    scd40/scd40.cpp
    sps30/sps30.cpp
)

target_include_directories(sensors_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sensors_lib 
    PUBLIC 
        pico_stdlib 
        hardware_i2c
)
