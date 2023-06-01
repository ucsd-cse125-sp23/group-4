cmake_minimum_required(VERSION 3.20)
project(tagguys DESCRIPTION "Real-time Multiplayer Tag")

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(FETCHCONTENT_QUIET FALSE)

add_compile_definitions(
  BOOST_FUNCTIONAL_OVERLOADED_FUNCTION_CONFIG_OVERLOAD_MAX=20)

add_subdirectory(src/config)
add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/client)
add_subdirectory(src/server)

enable_testing()
add_subdirectory(tests)
