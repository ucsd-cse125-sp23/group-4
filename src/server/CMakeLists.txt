project(server DESCRIPTION "tagguys server executable")

add_executable(server ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(server PRIVATE core network)
