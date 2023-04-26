add_executable(tests example.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)
add_test(test_all tests)
