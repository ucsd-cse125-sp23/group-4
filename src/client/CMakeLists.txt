project(client DESCRIPTION "tagguys client executable")

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(ASSIMP_BUILD_ZLIB OFF)
set(IMGUI_ENABLE_STDLIB_SUPPORT ON)
set(IMGUI_EXAMPLES OFF)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)
set(SFML_USE_STATIC_STD_LIBS FALSE)
set(SFML_BUILD_AUDIO ON)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_WINDOW OFF)
set(SFML_BUILD_NETWORK OFF)

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
  zlib
  GIT_REPOSITORY https://github.com/madler/zlib
  GIT_TAG v1.2.13
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
  GIT_TAG 18b6cff
  GIT_PROGRESS TRUE)
# override find_package(zlib)
FetchContent_MakeAvailable(zlib)
file(RENAME ${zlib_SOURCE_DIR}/zconf.h.included ${zlib_SOURCE_DIR}/zconf.h)
set(ZLIB_ROOT ${zlib_SOURCE_DIR})
set(ZLIB_LIBRARY zlibstatic)

# turn off install for freetype and assimp
set(_disable_install TRUE)
function(install)
  if(NOT _disable_install)
    _install(${ARGV})
  endif()
endfunction(install)

FetchContent_MakeAvailable(freetype assimp)
set(_disable_install FALSE)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML
  GIT_TAG 2.5.1
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(glew glfw glm freetype imgui SFML)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Camera.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Cube.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Scene.cpp
    ${PROJECT_SOURCE_DIR}/graphics/shader.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Input.cpp
    ${PROJECT_SOURCE_DIR}/graphics/SkinnedMesh.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Particle.cpp
    ${PROJECT_SOURCE_DIR}/graphics/ParticleSystem.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Texture.cpp
    ${PROJECT_SOURCE_DIR}/graphics/TextureCube.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Player.cpp
    ${PROJECT_SOURCE_DIR}/graphics/ItemBox.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Window.cpp
    ${PROJECT_SOURCE_DIR}/graphics/imported/parallel.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpAnimation.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpMesh.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpModel.cpp
    ${PROJECT_SOURCE_DIR}/graphics/AssimpNode.cpp
    ${PROJECT_SOURCE_DIR}/graphics/HUD.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Lobby.cpp
    ${PROJECT_SOURCE_DIR}/graphics/Start.cpp)

add_executable(client ${SOURCES})
target_include_directories(client
                           PRIVATE ${CMAKE_SOURCE_DIR}/include/client/graphics)
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
          freetype
          sfml-audio)

if(APPLE)
  target_compile_options(client PRIVATE -Wno-deprecated-declarations)
  add_compile_definitions(FREEGLUT_BUILD_DEMOS=OFF
                          OPENGL_gl_LIBRARY=/usr/local/lib/libGL.dylib)
endif()

set_target_properties(
  sfml-audio
  PROPERTIES CXX_STANDARD 98
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS NO)
