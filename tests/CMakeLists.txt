find_package(Python COMPONENTS Interpreter REQUIRED)


enable_testing()


add_executable(
  run_normal
  test_normal.cpp
)

add_test(NAME testPython
  COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/check.py
)
SET_TESTS_PROPERTIES(testPython PROPERTIES DEPENDS run_normal)

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