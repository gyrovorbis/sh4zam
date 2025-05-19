cmake_minimum_required(VERSION 3.5)

set(SHZ_VERSION_MAJOR 0)
set(SHZ_VERSION_MINOR 1)
set(SHZ_VERSION_PATCH 0)
set(SHZ_VERSION $(SHZ_VERSION_MAJOR).$(SHZ_VERSION_MINOR).$(SHZ_VERSION_PATCH))

project(sh4zam
        VERSION      $(SHZ_VERSION)
        DESCRIPTION  "Fast math library for the Sega Dreamcast's SH4 CPU"
        HOMEPAGE_URL "sh4zam.elysianshadows.com"
        LANGUAGES    C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_library(sh4zam STATIC
    shz_cdefs.h
    shz_trig.h
    shz_trig.hpp
    shz_scalar.h
    shz_scalar.hpp
    shz_vector.h
    shz_vector.hpp
    shz_xmtrx.h
    shz_xmtrx.hpp
    shz_matrix.h
    shz_matrix.hpp
    shz_quat.h
    shz_mem.h
    shz_mem.s)

include(GNUInstallDirs)
install(TARGETS sh4zam
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

option(SHZ_ENABLE_TESTS "Enable SH4ZAM Unit Tests" OFF)

if(SHZ_ENABLE_TESTS)
    add_subdirectory(test)
endif(SHZ_ENABLE_TESTS)
