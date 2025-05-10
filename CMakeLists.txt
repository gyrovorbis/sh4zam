cmake_minimum_required(VERSION 3.5)

project(libsh4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libsh4 main.cpp
    sh4_intrin.h
    sh4_matrix.h
    sh4_vector.h
    sh4_xmtrx.h
    sh4_quat.h
    sh4_cdefs.h
    sh4_vector.hpp)

include(GNUInstallDirs)
install(TARGETS libsh4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
