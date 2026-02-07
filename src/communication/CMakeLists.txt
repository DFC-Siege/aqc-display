add_library(communication_lib STATIC 
        serial/serial_manager.cpp
        communication_manager.cpp
)

target_include_directories(communication_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

