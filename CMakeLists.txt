cmake_minimum_required(VERSION 3.20)
project(tagguys DESCRIPTION "Real-time Multiplayer Tag")
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(3rd_party/imgui)

enable_testing()
add_subdirectory(tests)
