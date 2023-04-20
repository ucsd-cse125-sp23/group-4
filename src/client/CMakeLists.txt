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
  GIT_TAG v1.1)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp
  GIT_TAG master)

#add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)


# FetchContent_Declare(
#   assimp
#   GIT_REPOSITORY https://github.com/assimp/assimp.git
# )
# FetchContent_GetProperties(assimp)
# if(NOT assimp_POPULATED)
#   FetchContent_Populate(assimp)
#   add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
# endif()
FetchContent_MakeAvailable(glew glfw glm freetype imgui assimp)

set(SOURCES
  graphics/AnimationClip.cpp
  graphics/Camera.cpp
  graphics/Cube.cpp
  graphics/Joint.cpp
  graphics/Scene.cpp
  graphics/shader.cpp
  graphics/Skeleton.cpp
  graphics/Skin.cpp
  graphics/SkinnedMesh.cpp
  graphics/Tokenizer.cpp
  graphics/Window.cpp
)

add_executable(client main.cpp ${SOURCES})
target_include_directories(client
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include/client/graphics
)
target_link_libraries(
  client
  PRIVATE core
          network
          libglew_static
          glfw
          glm
          imgui
          assimp)
target_compile_features(client PUBLIC cxx_std_11)
target_compile_options(client
  PUBLIC -Wno-deprecated-declarations
)

# add_custom_command(
#   TARGET client POST_BUILD
#   COMMAND cp ${PROJECT_BINARY_DIR}/client ${CMAKE_SOURCE_DIR}/client
# )

