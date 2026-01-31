cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
set(PROJECT_NAME display)

include(pico_sdk_import.cmake)

project(display)

pico_sdk_init()

add_subdirectory(src)
