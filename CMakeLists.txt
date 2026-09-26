cmake_minimum_required(VERSION 3.10...3.31)

set(SHZ_VERSION_MAJOR 0)
set(SHZ_VERSION_MINOR 9)
set(SHZ_VERSION_PATCH 1)
set(SHZ_VERSION ${SHZ_VERSION_MAJOR}.${SHZ_VERSION_MINOR}.${SHZ_VERSION_PATCH})

project(sh4zam
        VERSION      ${SHZ_VERSION}
        DESCRIPTION  "Fast math library for the Sega Dreamcast's SH4 CPU"
        HOMEPAGE_URL "sh4zam.com"
        LANGUAGES    C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)

include(GNUInstallDirs)

if(PLATFORM_DREAMCAST)
    set(SHZ_BACKEND_DEFAULT "SH4")
else()
    set(SHZ_BACKEND_DEFAULT "SW")
endif()
set(SHZ_BACKEND_OPTIONS "SH4" "SPU" "SW")
set(SHZ_BACKEND ${SHZ_BACKEND_DEFAULT} CACHE STRING "Select SH4ZAM back-end implementation.")

set_property(CACHE SHZ_BACKEND PROPERTY STRINGS ${SHZ_BACKEND_OPTIONS})

set(SHZ_SOURCES
    source/shz_matrix.c
    source/shz_quat.c
    source/shz_version.c
    source/shz_xmtrx.c)

if(PLATFORM_DREAMCAST)
    list(APPEND SHZ_SOURCES
         source/sh4/shz_complex_sh4.c
         source/sh4/shz_xmtrx_sh4.s
         source/sh4/shz_mem_sh4.s)
endif()

list(APPEND SHZ_SOURCES
     source/sw/shz_complex_sw.c
     source/sw/shz_xmtrx_sw.c)

set(SHZ_INCLUDES
    include/sh4zam/shz_cdefs.h
    include/sh4zam/shz_cdefs.hpp
    include/sh4zam/shz_complex.h
    include/sh4zam/shz_complex.hpp
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
    include/sh4zam/shz_quat.hpp
    include/sh4zam/shz_mem.h
    include/sh4zam/shz_mem.hpp
    include/sh4zam/shz_sh4zam.h
    include/sh4zam/shz_sh4zam.hpp
    include/sh4zam/inline/shz_complex.inl.h
    include/sh4zam/inline/shz_trig.inl.h
    include/sh4zam/inline/shz_mem.inl.h
    include/sh4zam/inline/shz_quat.inl.h
    include/sh4zam/inline/shz_matrix.inl.h
    include/sh4zam/inline/shz_vector.inl.h
    include/sh4zam/inline/shz_scalar.inl.h
    include/sh4zam/inline/shz_xmtrx.inl.h)

if(PLATFORM_DREAMCAST)
    list(APPEND SHZ_INCLUDES
         include/sh4zam/inline/sh4/shz_mem_sh4.inl.h
         include/sh4zam/inline/sh4/shz_quat_sh4.inl.h
         include/sh4zam/inline/sh4/shz_scalar_sh4.inl.h
         include/sh4zam/inline/sh4/shz_trig_sh4.inl.h
         include/sh4zam/inline/sh4/shz_vector_sh4.inl.h
         include/sh4zam/inline/sh4/shz_xmtrx_sh4.inl.h)
endif()

if(SHZ_BACKEND == "SPU")
    list(APPEND SHZ_INCLUDES
         include/sh4zam/inline/sh4/shz_vector_spu.inl.h)
endif()

list(APPEND SHZ_INCLUDES
     include/sh4zam/inline/sw/shz_mem_sw.inl.h
     include/sh4zam/inline/sw/shz_quat_sw.inl.h
     include/sh4zam/inline/sw/shz_scalar_sw.inl.h
     include/sh4zam/inline/sw/shz_trig_sw.inl.h
     include/sh4zam/inline/sw/shz_vector_sw.inl.h
     include/sh4zam/inline/sw/shz_xmtrx_sw.inl.h)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    option(SHZ_SAVE_TEMPS "Save intermediate compilation files." OFF)

    if(SHZ_SAVE_TEMPS)
        message("Saving temporaries enabled: disabling LTO!")
        add_compile_options(-save-temps=obj)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
    endif()
endif()

# Allow a parent project to control how sh4zam is built. simulant builds it as
# an OBJECT library so the sh4zam object files are embedded directly into
# libsimulant.a, meaning downstream users don't have to link sh4zam themselves.
if(NOT DEFINED SHZ_LIBRARY_TYPE)
    set(SHZ_LIBRARY_TYPE STATIC)
endif()

add_library(sh4zam ${SHZ_LIBRARY_TYPE} ${SHZ_SOURCES})

target_include_directories(sh4zam
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# When embedded as an OBJECT library inside another target (e.g. Simulant) there
# is nothing to install on its own, and its headers are kept private.
if(NOT SHZ_LIBRARY_TYPE STREQUAL "OBJECT")
    install(TARGETS sh4zam)
    install(DIRECTORY include/sh4zam DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

option(SHZ_ENABLE_PIC "Enable position-independent code." OFF)

if(SHZ_ENABLE_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE SHZ_ENABLE_PIC)
endif()

option(SHZ_ENABLE_TESTS "Enable SH4ZAM Unit Tests" OFF)

if(SHZ_ENABLE_TESTS)
    add_subdirectory(test)
endif()

if(PLATFORM_DREAMCAST OR MSVC)
    set(SHZ_TLS_MODEL_DEFAULT "IMPLICIT")
elseif(SHZ_BACKEND == "SPU")
    set(SHZ_TLS_MODEL_DEFAULT "DISABLED")
else()
    set(SHZ_TLS_MODEL_DEFAULT "PTHREAD")
endif()
set(SHZ_TLS_MODEL_OPTIONS "DISABLED" "IMPLICIT" "PTHREAD" "CTHREAD")
set(SHZ_TLS_MODEL ${SHZ_TLS_MODEL_DEFAULT} CACHE STRING "Select thread-local storage implementation.")

set_property(CACHE SHZ_TLS_MODEL PROPERTY STRINGS ${SHZ_TLS_MODEL_OPTIONS})
target_compile_definitions(sh4zam PUBLIC SHZ_BACKEND=SHZ_${SHZ_BACKEND} SHZ_TLS_MODEL=SHZ_TLS_${SHZ_TLS_MODEL})

if(SHZ_TLS_MODEL STREQUAL "PTHREAD" OR SHZ_TLS_MODEL STREQUAL "CTHREAD")
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(sh4zam PUBLIC Threads::Threads)
endif()
