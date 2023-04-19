project(
  network
  DESCRIPTION
    "tagguys network library, includes: socket handling, message parsing, etc")

find_package(Boost REQUIRED)

add_library(network ${PROJECT_SOURCE_DIR}/lib.cpp)
target_include_directories(network PUBLIC ${tagguys_SOURCE_DIR}/include)
