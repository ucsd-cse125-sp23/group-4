cmake_minimum_required(VERSION 3.26)
project(tagguys DESCRIPTION "Real-time Multiplayer Tag")
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/client)
add_subdirectory(src/server)

enable_testing()
add_subdirectory(tests)
