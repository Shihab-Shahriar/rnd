find_package(Python COMPONENTS Interpreter REQUIRED)


enable_testing()

add_executable(
  uniform
  test_uniform.cpp
)
target_link_libraries(
  uniform
  GTest::gtest_main
)

add_executable(
  normal
  test_normal.cpp
)
target_link_libraries(
  normal
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(uniform)
gtest_discover_tests(normal)