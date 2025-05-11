cmake_minimum_required(VERSION 3.5)

project(libsh4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libsh4 main.cpp
    shz_intrin.h
    shz_matrix.h
    shz_vector.h
    shz_xmtrx.h
    shz_quat.h
    shz_cdefs.h
    shz_vector.hpp)

include(GNUInstallDirs)
install(TARGETS libsh4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
