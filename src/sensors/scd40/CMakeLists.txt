target_sources(sensors_lib PRIVATE
    scd40_sensor.cpp
)

target_include_directories(sensors_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
