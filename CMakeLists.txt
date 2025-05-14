cmake_minimum_required(VERSION 3.5)

project(sh4zam LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(sh4zam main.cpp
    shz_intrin.h
    shz_matrix.h
    shz_vector.h
    shz_xmtrx.h
    shz_quat.h
    shz_cdefs.h
    shz_vector.hpp)

include(GNUInstallDirs)
install(TARGETS sh4zam
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
