FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.3.2)
FetchContent_MakeAvailable(Catch2)

add_executable(tests example.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)
add_executable(core_tests core/math.cpp core/collision.cpp core/game.cpp)
target_link_libraries(core_tests core Catch2::Catch2WithMain)

add_test(test_all tests core_tests)
