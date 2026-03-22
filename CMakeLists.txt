cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
set(PROJECT_NAME display)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wno-psabi)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(${PROJECT_NAME} C CXX ASM)
pico_sdk_init()

set(PONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../pons)

add_subdirectory(${PONS_DIR}/core/result         ${CMAKE_BINARY_DIR}/pons/result)
add_subdirectory(${PONS_DIR}/core/core_async     ${CMAKE_BINARY_DIR}/pons/core_async)
add_subdirectory(${PONS_DIR}/core/logger         ${CMAKE_BINARY_DIR}/pons/logger)
add_subdirectory(${PONS_DIR}/hal/serial_hal      ${CMAKE_BINARY_DIR}/pons/serial_hal)
add_subdirectory(${PONS_DIR}/hal/store           ${CMAKE_BINARY_DIR}/pons/store)
add_subdirectory(${PONS_DIR}/hal/rp2040/rp2040_async       ${CMAKE_BINARY_DIR}/pons/rp2040_async)
add_subdirectory(${PONS_DIR}/hal/rp2040/rp2040_serial_hal  ${CMAKE_BINARY_DIR}/pons/rp2040_serial_hal)
add_subdirectory(${PONS_DIR}/platform/rp2040/platform_async ${CMAKE_BINARY_DIR}/pons/platform_async)
add_subdirectory(${PONS_DIR}/transport           ${CMAKE_BINARY_DIR}/pons/transport)
add_subdirectory(${PONS_DIR}/transport/chunked_transport    ${CMAKE_BINARY_DIR}/pons/chunked_transport)
add_subdirectory(${PONS_DIR}/transport/chunked_transport/serial_transport ${CMAKE_BINARY_DIR}/pons/serial_transport)

add_subdirectory(lib/displaylib_16bit_PICO)
add_subdirectory(src)
