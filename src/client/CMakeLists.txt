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
  GIT_TAG 0.9.8.5)
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


#FetchContent_Declare(
#    freetype
#    GIT_REPOSITORY https://github.com/freetype/freetype.git
#    GIT_TAG master
#)

FetchContent_Declare(
    freeglut
    GIT_REPOSITORY https://github.com/FreeGLUTProject/freeglut.git
    GIT_TAG master
)
FetchContent_GetProperties(freeglut)
if(NOT freeglut_POPULATED)
    FetchContent_Populate(freeglut)
    add_subdirectory(${freeglut_SOURCE_DIR} ${freeglut_BINARY_DIR})
endif()



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
    ${freeglut_SOURCE_DIR}/include
)
target_link_libraries(
  client
  PRIVATE core
          network
          libglew_static
          glfw
          glm
          imgui
          assimp
          freeglut_static)
target_compile_features(client PUBLIC cxx_std_11)
if(APPLE)
  target_compile_options(client
    PUBLIC -Wno-deprecated-declarations
  )
endif()
if(WIN32 OR MSVC OR MSYS OR MINGW)

endif()

# Copy assets folder to executable root directory
add_custom_command(
  TARGET client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" 
    "${PROJECT_BINARY_DIR}/assets"
)
