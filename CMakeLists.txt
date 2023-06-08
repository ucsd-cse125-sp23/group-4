cmake_minimum_required(VERSION 3.20)
project(tagguys DESCRIPTION "Real-time Multiplayer Tag")

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(FETCHCONTENT_QUIET FALSE)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_compile_definitions(
  BOOST_FUNCTIONAL_OVERLOADED_FUNCTION_CONFIG_OVERLOAD_MAX=20)

add_subdirectory(src/config)
add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/client)
add_subdirectory(src/server)

enable_testing()
add_subdirectory(tests)

# copy static assets to executable directory
add_custom_target(
  copy_static_files
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:client>/assets
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config.json
          $<TARGET_FILE_DIR:client>
  COMMAND
    ${CMAKE_COMMAND} -E copy
    ${FETCHCONTENT_BASE_DIR}/sfml-src/extlibs/bin/x64/openal32.dll
    $<TARGET_FILE_DIR:client>)
add_dependencies(client copy_static_files)
