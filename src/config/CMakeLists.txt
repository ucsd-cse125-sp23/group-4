project(client DESCRIPTION "tagguys config file manager")

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.2
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(json)

add_library(config ${PROJECT_SOURCE_DIR}/lib.cpp)
target_include_directories(config PUBLIC ${tagguys_SOURCE_DIR}/include)
target_link_libraries(config PUBLIC nlohmann_json::nlohmann_json)

configure_file(${CMAKE_SOURCE_DIR}/config.json
               ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json COPYONLY)
