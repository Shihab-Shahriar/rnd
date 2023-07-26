enable_testing()

add_executable(
  hello_test
  test_uniform.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)