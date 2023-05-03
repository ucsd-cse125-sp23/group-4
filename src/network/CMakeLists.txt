project(
  network
  DESCRIPTION
    "tagguys network library, includes: socket handling, message parsing, etc")

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost
  GIT_TAG boost-1.82.0)
FetchContent_MakeAvailable(boost)

set(_NETWORK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/message.cpp)

add_library(network ${_NETWORK_SOURCES})
target_include_directories(network PUBLIC ${tagguys_SOURCE_DIR}/include)
target_link_libraries(network PRIVATE Boost::asio Boost::serialization)
