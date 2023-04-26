project(server DESCRIPTION "tagguys server executable")

add_executable(server ${PROJECT_SOURCE_DIR}/main.cpp)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                        ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(server PRIVATE core network)
