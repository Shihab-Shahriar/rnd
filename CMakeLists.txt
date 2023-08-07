# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.23)

# Project's name
project(RND)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -fopenmp")

include_directories(${PROJECT_SOURCE_DIR}/include)
# add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Statistical Tests
# To test using TESTU01, set the TESTU01 variable to the path it's installed at
set(TESTU01 /home/shihab/codes/TestU01)

# To test using PractRand, pipe the output of build/examples/pract_rand to 
# `RNG_test` with stdin32 option. For example-
# `./examples/pract_rand | ./RNG_test stdin32`