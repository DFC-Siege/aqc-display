add_library(ui_lib STATIC 
        ui_manager.cpp
        pages/page_factory.cpp
        pages/homepage/homepage.cpp
)

target_link_libraries(ui_lib PUBLIC display_lib)
target_include_directories(ui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
