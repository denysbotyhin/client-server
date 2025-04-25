
cmake_minimum_required(VERSION 3.31.3)

project(Client_Server VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_LTO "Enable to add Link Time Optimization." ON)
option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(ConfigSafeGuards)
include(LTO)

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

set(UDP_SERVER_EXECUTABLE "udp_server")
set(TCP_SERVER_EXECUTABLE "tcp_server")
set(UDP_CLIENT_EXECUTABLE "udp_client")
set(TCP_CLIENT_EXECUTABLE "tcp_client")

add_subdirectory(app)

set_target_properties(
    ${UDP_CLIENT_EXECUTABLE}
    ${TCP_CLIENT_EXECUTABLE}
    ${UDP_SERVER_EXECUTABLE}
    ${TCP_SERVER_EXECUTABLE}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/archive/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)
