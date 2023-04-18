cmake_minimum_required(VERSION 3.26)
include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tagguys DESCRIPTION "Real-time Multiplayer Tag")

FetchContent_Declare(glew GIT_REPOSITORY https://github.com/nigels-com/glew.git)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.3.2)

FetchContent_MakeAvailable(glew glfw glm imgui Catch2)
find_package(Boost REQUIRED)

add_library(core src/core/lib.cpp include/core/lib.hpp)
target_include_directories(core PUBLIC include)

add_library(network src/network/lib.cpp include/network/lib.hpp)
target_include_directories(network PUBLIC include)

add_executable(client src/client/main.cpp)
target_link_libraries(client PRIVATE core network)

add_executable(server src/server/main.cpp)
target_link_libraries(server PRIVATE core network)
