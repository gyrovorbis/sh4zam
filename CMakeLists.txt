cmake_minimum_required(VERSION 3.10)

set(SHZ_VERSION_MAJOR 0)
set(SHZ_VERSION_MINOR 0)
set(SHZ_VERSION_PATCH 0)
set(SHZ_VERSION ${SHZ_VERSION_MAJOR}.${SHZ_VERSION_MINOR}.${SHZ_VERSION_PATCH})

project(sh4zam
        VERSION      ${SHZ_VERSION}
        DESCRIPTION  "Fast math library for the Sega Dreamcast's SH4 CPU"
        HOMEPAGE_URL "sh4zam.falcogirgis.com"
        LANGUAGES    ASM C CXX)

# Compiler min version check broken with DreamSDK!
#set(GCC_VERSION_MIN "12.0")
#if(CMAKE_C_COMPILER_VERSION VERSION_LESS ${GCC_VERSION_MIN})
#    message(FATAL_ERROR "GCC version is too old! Minimum required version: ${GCC_VERSION_MIN}.")
#endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(SHZ_SOURCES
    source/shz_circle.c
    source/shz_matrix.c
    source/shz_mem.s
    source/shz_xmtrx.c
    source/shz_xmtrx.s
    source/shz_quat.c)

set(SHZ_INCLUDES
    include/sh4zam/shz_cdefs.h
    include/sh4zam/shz_cdefs.hpp
    include/sh4zam/shz_circle.h
    include/sh4zam/shz_circle.hpp
    include/sh4zam/shz_trig.h
    include/sh4zam/shz_trig.hpp
    include/sh4zam/shz_trig.inl.h
    include/sh4zam/shz_scalar.h
    include/sh4zam/shz_scalar.hpp
    include/sh4zam/shz_scalar.inl.h
    include/sh4zam/shz_vector.h
    include/sh4zam/shz_vector.hpp
    include/sh4zam/shz_vector.inl.h
    include/sh4zam/shz_xmtrx.h
    include/sh4zam/shz_xmtrx.hpp
    include/sh4zam/shz_xmtrx.inl.h
    include/sh4zam/shz_matrix.h
    include/sh4zam/shz_matrix.hpp
    include/sh4zam/shz_matrix.inl.h
    include/sh4zam/shz_quat.h
    include/sh4zam/shz_quat.hpp
    include/sh4zam/shz_quat.inl.h
    include/sh4zam/shz_mem.h
    include/sh4zam/shz_mem.hpp
    include/sh4zam/shz_mem.inl.h
    include/sh4zam/shz_fpscr.h
    include/sh4zam/shz_sh4zam.h
    include/sh4zam/shz_sh4zam.hpp)

add_library(sh4zam STATIC ${SHZ_SOURCES})

set_target_properties(sh4zam PROPERTIES
                      PUBLIC_HEADER "${SHZ_INCLUDES}")

target_compile_definitions(sh4zam 
    PUBLIC
        -DSHZ_VERSION_MAJOR=${SHZ_VERSION_MAJOR}
        -DSHZ_VERSION_MINOR=${SHZ_VERSION_MINOR}
        -DSHZ_VERSION_PATCH=${SHZ_VERSION_PATCH}
        -DSHZ_VERSION="${SHZ_VERSION}")

target_include_directories(sh4zam
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS sh4zam
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sh4zam)

option(SHZ_ENABLE_TESTS "Enable SH4ZAM Unit Tests" OFF)

if(SHZ_ENABLE_TESTS)
    add_subdirectory(test)
endif(SHZ_ENABLE_TESTS)
