add_executable(tests example.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                       ${CMAKE_BINARY_DIR}/bin)
add_test(test_all tests)
