project(client DESCRIPTION "tagguys client executable")

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
  GIT_TAG master)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master)
FetchContent_MakeAvailable(glew glfw glm)

add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(client PRIVATE core network libglew_static glfw glm)
