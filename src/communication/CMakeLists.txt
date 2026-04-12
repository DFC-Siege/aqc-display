add_library(communication INTERFACE)
target_include_directories(communication INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(communication INTERFACE 
        serial_hal
        dispatcher
        result
        transporter
        direct_transporter
        serial_transporter
        chunked_transporter
)
