cmake_minimum_required(VERSION 3.10)

project(Sh4zamTest
        VERSION     ${SHZ_VERSION}
        DESCRIPTION "Unit tests for SH4ZAM"
        LANGUAGES   ASM C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_subdirectory(libgimbal)

add_executable(Sh4zamTests
    shz_test.c
    shz_test.h
    shz_scalar_test_suite.cpp
    shz_trig_test_suite.cpp
    shz_vector_test_suite.cpp
    shz_xmtrx_test_suite.cpp
    shz_quat_test_suite.cpp)

target_link_libraries(Sh4zamTests
                      PRIVATE libGimbal)
