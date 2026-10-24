include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Werror=c++20-extensions SHZ_HAS_WERROR_CPP20_EXTENSIONS)

foreach(SHZ_CPP_STD 17 20)
    add_library(Sh4zamCpp${SHZ_CPP_STD}Check OBJECT shz_cpp_standard_check.cpp)
    target_link_libraries(Sh4zamCpp${SHZ_CPP_STD}Check PRIVATE sh4zam)
    set_target_properties(Sh4zamCpp${SHZ_CPP_STD}Check PROPERTIES
                          CXX_STANDARD          ${SHZ_CPP_STD}
                          CXX_STANDARD_REQUIRED ON)
endforeach()

if(SHZ_HAS_WERROR_CPP20_EXTENSIONS)
    target_compile_options(Sh4zamCpp17Check PRIVATE -Werror=c++20-extensions)
endif()

add_library(Sh4zamC11Check OBJECT shz_c_standard_check.c)
target_link_libraries(Sh4zamC11Check PRIVATE sh4zam)
set_target_properties(Sh4zamC11Check PROPERTIES
                      C_STANDARD          11
                      C_STANDARD_REQUIRED ON)
