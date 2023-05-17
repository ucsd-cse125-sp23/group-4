project(client DESCRIPTION "tagguys client executable")

set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(IMGUI_ENABLE_STDLIB_SUPPORT ON)
set(IMGUI_EXAMPLES OFF)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
  GIT_TAG glew-cmake-2.2.0
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.8.5
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype
  GIT_TAG VER-2-13-0
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/yesean/imgui
  GIT_TAG v1.1
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp
  GIT_TAG v5.2.5
  GIT_PROGRESS TRUE)
FetchContent_Declare(
  freeglut
  GIT_REPOSITORY https://github.com/FreeGLUTProject/freeglut.git
  GIT_TAG v3.4.0
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(
  glew
  glfw
  glm
  freeglut
  freetype
  imgui
  assimp)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AnimationClip.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Camera.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Cube.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Joint.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Scene.cpp
    ${PROJECT_SOURCE_DIR}/graphics/shader.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Input.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Skeleton.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Skin.cpp
    ${PROJECT_SOURCE_DIR}/graphics/SkinnedMesh.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Texture.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Player.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Window.cpp
    ${PROJECT_SOURCE_DIR}/graphics/imported/Tokenizer.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpAnimation.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpMesh.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpModel.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpNode.cpp)

add_executable(client ${SOURCES})
target_include_directories(
  client PRIVATE ${CMAKE_SOURCE_DIR}/include/client/graphics
                 ${freeglut_SOURCE_DIR}/include)
target_link_libraries(
  client
  PRIVATE core
          network
          config
          Boost::asio
          libglew_static
          glfw
          glm
          imgui
          imgui-glfw
          imgui-opengl3
          assimp
          freeglut_static
          freetype)

if(APPLE)
  target_compile_options(client PRIVATE -Wno-deprecated-declarations)
  add_compile_definitions(FREEGLUT_BUILD_DEMOS=OFF
                          OPENGL_gl_LIBRARY=/usr/local/lib/libGL.dylib)
endif()

# Copy assets folder to executable root directory
add_custom_command(
  TARGET client
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets"
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")
