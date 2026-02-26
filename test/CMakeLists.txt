cmake_minimum_required(VERSION 3.10)

project(Sh4zamTest
        VERSION     ${SHZ_VERSION}
        DESCRIPTION "Unit tests for SH4ZAM"
        LANGUAGES   C CXX ASM)

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
    shz_test.hpp
    shz_scalar_test_suite.cpp
    shz_trig_test_suite.cpp
    shz_vector_test_suite.cpp
    shz_quat_test_suite.cpp
    shz_xmtrx_test_suite.cpp
    shz_matrix_test_suite.cpp
    shz_mem_test_suite.cpp)

# Breaks with older CMake versions, fml.
#set_property(TARGET Sh4zamTests PROPERTY CXX_STANDARD 23)

target_include_directories(Sh4zamTests
    PRIVATE ..)

target_link_libraries(Sh4zamTests
                      PRIVATE libGimbal sh4zam cglm fastmem)

if(PLATFORM_DREAMCAST)
    include(kallistios REQUIRED)
    kos_run_target(Sh4zamTests run-sh4zam-tests)
endif()

option(SHZ_DISABLE_BENCHMARKS "Disable SH4ZAM test benchmarks" OFF)

if(SHZ_DISABLE_BENCHMARKS)
    target_compile_definitions(Sh4zamTests PRIVATE SHZ_DISABLE_BENCHMARKS)
endif(SHZ_DISABLE_BENCHMARKS)

