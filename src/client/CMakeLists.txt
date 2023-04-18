project(client DESCRIPTION "tagguys client executable")

add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(client PRIVATE core network)
