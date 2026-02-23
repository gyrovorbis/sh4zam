#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_mem.hpp"

#include <fastmem/fastmem.h>

#define GBL_SELF_TYPE   shz_mem_test_suite

#define BUFFER_KB       64
#define PADDING         12

GBL_TEST_FIXTURE_NONE
GBL_TEST_INIT_NONE
GBL_TEST_FINAL_NONE

template<size_t Bytes, size_t Align, size_t Pad>
struct MemcpyBuffer {
                   std::array<uint8_t, Pad>   prepad;
    alignas(Align) std::array<uint8_t, Bytes> buffer;
                   std::array<uint8_t, Pad>   postpad;
};

template<size_t Bytes, size_t SrcAlign, size_t DstAlign, size_t Pad>
bool memcpy_bench(auto&& pFn) {
    static MemcpyBuffer<Bytes, SrcAlign, Pad> src;
    static MemcpyBuffer<Bytes, DstAlign, Pad> dst;

    return benchmark_cmp(void*, pFn, memcpy, dst.buffer.data(), src.buffer.data(), Bytes);
}

template<size_t Bytes, size_t SrcAlign, size_t DstAlign, size_t Pad>
bool memcpy_verify(auto&& pFn) {
    static MemcpyBuffer<Bytes, SrcAlign, Pad> src;
    static MemcpyBuffer<Bytes, DstAlign, Pad> dst;

    gblRandBuffer(src.prepad.data(),  Pad);
    gblRandBuffer(src.buffer.data(),  Bytes);
    gblRandBuffer(src.postpad.data(), Pad);

    for(auto& c : dst.prepad ) c = 0;
    gblRandBuffer(dst.buffer.data(),  Bytes);
    for(auto& c : dst.postpad) c = 0;

    pFn(dst.buffer.data(), src.buffer.data(), Bytes);

    for(auto c : dst.prepad)
        if(c) return false;
    for(auto c : dst.postpad)
        if(c) return false;
    for(unsigned b = 0; b < Bytes; ++b)
        if(dst.buffer[b] != src.buffer[b])
            return false;

    return true;
}

GBL_TEST_CASE(memcpy1)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 1, 1, PADDING>)(shz::memcpy1));
    memcpy_bench <BUFFER_KB * 1024, 1, 1, PADDING>(shz::memcpy1);
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy2)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 2, 2, PADDING>)(shz::memcpy2));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 2, 2, PADDING>)(shz::memcpy2));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy4)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 4, 4, PADDING>)(shz::memcpy4));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 4, 4, PADDING>)(shz::memcpy4));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy8)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 8, 8, PADDING>)(shz::memcpy8));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 8, 8, PADDING>)(shz::memcpy8));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy32)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy32));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy32));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy64)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy64));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy64));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy128)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy128));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy128));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 32, 32, PADDING>)(shz::memcpy));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_fast)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_KB * 1024, 32, 32, PADDING>)(memcpy_fast));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_KB * 1024, 32, 32, PADDING>)(memcpy_fast));
GBL_TEST_CASE_END

GBL_TEST_REGISTER(memcpy1,
                  memcpy2,
                  memcpy4,
                  memcpy8,
                  memcpy32,
                  memcpy64,
                  memcpy128,
                  memcpy,
                  memcpy_fast)