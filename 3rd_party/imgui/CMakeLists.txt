project(imgui)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

add_library(ImGui STATIC
  imgui_draw.cpp
  imgui_tables.cpp
  imgui_widgets.cpp
  imgui.cpp
  backends/imgui_impl_glfw.cpp
  backends/imgui_impl_opengl3.cpp
  misc/cpp/imgui_stdlib.cpp
)

target_link_libraries(ImGui PRIVATE glfw)
target_include_directories(ImGui
  PUBLIC
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/backends
    ${PROJECT_SOURCE_DIR}/misc/cpp
    ${CMAKE_SOURCE_DIR}/3rd_party
)
target_compile_features(ImGui PUBLIC cxx_std_11)