project(client DESCRIPTION "tagguys client executable")

add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                        ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(
  client
  PRIVATE core
          network
          libglew_static
          glfw
          glm
          assimp
          imgui)
