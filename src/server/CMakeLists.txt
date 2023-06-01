project(server DESCRIPTION "tagguys server executable")

set(_SERVER_SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/manager.cpp
    ${PROJECT_SOURCE_DIR}/game.cpp)

add_executable(server ${_SERVER_SOURCES})
target_link_libraries(server PRIVATE core network config Boost::asio)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)
