#include "shz_test.h"
#include "shz_test.hpp"
#include "sh4zam/shz_mem.hpp"

#if SHZ_BACKEND == SHZ_SH4
#   include <fastmem/fastmem.h>
#endif

#define GBL_SELF_TYPE   shz_mem_test_suite

#define BUFFER_SIZE     (12 * 1024)
#define PADDING         1024

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
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 1, 1, PADDING>)(shz::memcpy1));
    memcpy_bench <BUFFER_SIZE, 1, 1, PADDING>(shz::memcpy1);
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy2)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 2, 2, PADDING>)(shz::memcpy2));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 2, 2, PADDING>)(shz::memcpy2));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy4)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 4, 4, PADDING>)(shz::memcpy4));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 4, 4, PADDING>)(shz::memcpy4));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy8)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 8, PADDING>)(shz::memcpy8));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 8, PADDING>)(shz::memcpy8));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy32)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy32));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy32));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy64)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy64));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy64));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy128)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy128));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy128));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy)
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(shz::memcpy));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_fast)
#if !(SHZ_BACKEND == SHZ_SH4)
    GBL_TEST_SKIP("Skipping libfastmem test, SH4 only!");
#else
    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(memcpy_fast));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(memcpy_fast));
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_sq_32)
    auto sq_memcpy = [](void* dst, const void* src, size_t bytes) {
#if SHZ_BACKEND == SHZ_SH4
        dcache_inval_range((uintptr_t)dst, bytes);
        auto* d = sq_lock(dst);
#endif
        auto* ret = shz_sq_memcpy32(d, src, bytes);
#if SHZ_BACKEND == SHZ_SH4
        sq_unlock();
#endif
        return ret;
    };

    GBL_TEST_VERIFY((memcpy_verify<BUFFER_SIZE, 8, 32, PADDING>)(sq_memcpy));
    GBL_TEST_VERIFY((memcpy_bench <BUFFER_SIZE, 8, 32, PADDING>)(sq_memcpy));
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_1)
    GBL_TEST_VERIFY((memcpy_verify<1, 1, 1, 1>)(shz::memcpy));
    (memcpy_bench <1, 1, 1, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <1, 1, 1, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_2)
    GBL_TEST_VERIFY((memcpy_verify<2, 2, 2, 1>)(shz::memcpy));
    (memcpy_bench <2, 2, 2, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <2, 2, 2, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_4)
    GBL_TEST_VERIFY((memcpy_verify<4, 4, 4, 1>)(shz::memcpy));
    (memcpy_bench <4, 4, 4, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <4, 4, 4, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_8)
    GBL_TEST_VERIFY((memcpy_verify<8, 8, 8, 1>)(shz::memcpy));
    (memcpy_bench <8, 8, 8, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <8, 8, 8, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_16)
    GBL_TEST_VERIFY((memcpy_verify<16, 16, 16, 1>)(shz::memcpy));
    (memcpy_bench <16, 16, 16, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <16, 16, 16, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_32)
    GBL_TEST_VERIFY((memcpy_verify<32, 32, 32, 1>)(shz::memcpy));
    (memcpy_bench <32, 32, 32, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <32, 32, 32, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_CASE(memcpy_primitive_64)
    GBL_TEST_VERIFY((memcpy_verify<64, 64, 64, 1>)(shz::memcpy));
    (memcpy_bench <64, 64, 64, 1>)(shz::memcpy);
#if SHZ_BACKEND != SHZ_SH4
    GBL_TEST_SKIP("SKipping listfastmem tests, SH4 only!");
#else
    (memcpy_bench <64, 64, 64, 1>)(memcpy_fast);
#endif
GBL_TEST_CASE_END

GBL_TEST_REGISTER(memcpy1,
                  memcpy2,
                  memcpy4,
                  memcpy8,
                  memcpy32,
                  memcpy64,
                  memcpy128,
                  memcpy,
                  memcpy_fast,
                  memcpy_sq_32,
                  memcpy_primitive_1,
                  memcpy_primitive_2,
                  memcpy_primitive_4,
                  memcpy_primitive_8,
                  memcpy_primitive_16,
                  memcpy_primitive_32,
                  memcpy_primitive_64)