# Add the executable
add_executable(basic_usage basic_usage.cpp)
add_executable(pi_openmp pi_openmp.cpp)
add_executable(pract_rand pract_rand.cpp)
add_executable(tmp tmp.cpp)


if(DEFINED TESTU01)
    message(STATUS "Building TESTU01 statistical test suite")
    link_directories(${TESTU01}/lib)
    add_executable(testu01_exec testu01.cpp)
    target_link_libraries(testu01_exec testu01 probdist mylib)
    target_include_directories(testu01_exec PUBLIC ${TESTU01}/include)

    add_executable(testu01_multi testu01-multistream.cpp)
    target_link_libraries(testu01_multi testu01 probdist mylib)
    target_include_directories(testu01_multi PUBLIC ${TESTU01}/include)
else()
    message(STATUS "TESTU01 not found, not building statistical test suite")
endif()