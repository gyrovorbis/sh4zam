cmake_minimum_required(VERSION 3.10)

set(SHZ_VERSION_MAJOR 0)
set(SHZ_VERSION_MINOR 0)
set(SHZ_VERSION_PATCH 0)
set(SHZ_VERSION ${SHZ_VERSION_MAJOR}.${SHZ_VERSION_MINOR}.${SHZ_VERSION_PATCH})

project(sh4zam
        VERSION      ${SHZ_VERSION}
        DESCRIPTION  "Fast math library for the Sega Dreamcast's SH4 CPU"
        HOMEPAGE_URL "sh4zam.elysianshadows.com"
        LANGUAGES    ASM C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(SHZ_SOURCES
    source/shz_mem.s)

set(SHZ_INCLUDES
    include/sh4zam/shz_cdefs.h
    include/sh4zam/shz_trig.h
    include/sh4zam/shz_trig.hpp
    include/sh4zam/shz_scalar.h
    include/sh4zam/shz_scalar.hpp
    include/sh4zam/shz_vector.h
    include/sh4zam/shz_vector.hpp
    include/sh4zam/shz_xmtrx.h
    include/sh4zam/shz_xmtrx.hpp
    include/sh4zam/shz_matrix.h
    include/sh4zam/shz_matrix.hpp
    include/sh4zam/shz_quat.h
    include/sh4zam/shz_mem.h)

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
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

option(SHZ_ENABLE_TESTS "Enable SH4ZAM Unit Tests" OFF)

if(SHZ_ENABLE_TESTS)
    add_subdirectory(test)
endif(SHZ_ENABLE_TESTS)
