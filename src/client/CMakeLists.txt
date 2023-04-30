project(client DESCRIPTION "tagguys client executable")

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
  GIT_TAG glew-cmake-2.2.0)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8)
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype
  GIT_TAG VER-2-13-0)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/yesean/imgui
  GIT_TAG v1.0)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp
  GIT_TAG v5.2.5)
FetchContent_MakeAvailable(glew glfw glm freetype imgui assimp)

add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(
  client
  PRIVATE core
          network
          libglew_static
          glfw
          glm
          imgui
          assimp)
