project(client DESCRIPTION "tagguys client executable")

FetchContent_Declare(glew GIT_REPOSITORY https://github.com/nigels-com/glew.git)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git)
FetchContent_MakeAvailable(glew glfw glm imgui)

add_executable(client ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(client PRIVATE core network)
