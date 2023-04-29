project(
  network
  DESCRIPTION
    "tagguys network library, includes: socket handling, message parsing, etc")

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost
  GIT_TAG boost-1.82.0)
FetchContent_MakeAvailable(boost)

add_library(network ${PROJECT_SOURCE_DIR}/lib.cpp)
target_include_directories(network PUBLIC ${tagguys_SOURCE_DIR}/include)
target_link_libraries(network PRIVATE Boost::asio)
