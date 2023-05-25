FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.3.2)
FetchContent_MakeAvailable(Catch2)

set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/math.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/game.cpp)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch2::Catch2WithMain core)

add_test(NAME test_all COMMAND tests)
