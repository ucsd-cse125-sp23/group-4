project(
  core
  DESCRIPTION
    "tagguys core library, includes: game logic, internal data structures, etc")

add_library(core ${PROJECT_SOURCE_DIR}/lib.cpp)
target_include_directories(core PUBLIC ${tagguys_SOURCE_DIR}/include)
