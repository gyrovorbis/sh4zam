#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>
#include <concepts>

#include <kos.h>
#include <sh4zam/shz_sh4zam.hpp>

#include <gimbal/algorithms/gimbal_random.h>

#define BENCHMARK_ITERATION_COUNT   20
#define BENCHMARK_ITERATION_MATCHES 2
#define SHZ_MEMORY_BARRIER_HARD() __sync_synchronize()

#define PMCR_PMENABLE   0x8000  /* Enable */
#define PMCR_PMST       0x4000  /* Start */
#define PMCR_RUN        0xc000  /* Run: Enable | Start */
#define PMCR_CLR        0x2000  /* Clear */
#define PMCR_PMCLK      0x0100  /* Clock Type */
#define PMCR_PMMODE     0x003f  /* Event Mode */

/* PMCR count type field position */
#define PMCR_PMCLK_SHIFT 8

/* 5ns per count in 1 cycle = 1 count mode(PMCR_COUNT_CPU_CYCLES) */
#define NS_PER_CYCLE      5

#define PMCR_CTRL(o)  ( *((volatile uint16_t *)(0xff000084) + (o << 1)) )
#define PMCTR_HIGH(o) ( *((volatile uint32_t *)(0xff100004) + (o << 1)) )
#define PMCTR_LOW(o)  ( *((volatile uint32_t *)(0xff100008) + (o << 1)) )

SHZ_FORCE_INLINE void PERF_CNTR_START() {
    PMCR_CTRL(0) &= ~PMCR_RUN;
    PMCR_CTRL(0) |= PMCR_CLR;
    PMCR_CTRL(0) |= PMCR_RUN;
}

SHZ_FORCE_INLINE uint64_t PERF_CNTR_STOP() {
    PMCR_CTRL(0) &= ~PMCR_RUN;
    return (((uint64_t)PMCTR_HIGH(PRFC0) << 32) | PMCTR_LOW(PRFC0));
}

template<typename F, typename... Args>
SHZ_NO_INLINE
void benchmark(auto res, const char* name, F &&function, Args&&... args) {
    alignas(32) uint8_t dcache_buffer[1024 * 16];

    perf_cntr_timer_enable();

    auto inner = [&]<bool CacheFlush>() SHZ_NO_INLINE SHZ_ICACHE_ALIGNED {
        uint64_t perfctr_prev = 0;
        uint64_t perfctr_sum  = 0;
        uint64_t tmu_sum      = 0;
        int      iterations   = 0;
        unsigned matches      = 0;

        SHZ_MEMORY_BARRIER_HARD();
        auto state = irq_disable();

 #pragma GCC unroll 0
        for(iterations = 0; iterations < BENCHMARK_ITERATION_COUNT; ++iterations) {
            if constexpr(CacheFlush) {
                icache_flush_range((uintptr_t)&_executable_start, (size_t)((uintptr_t)&_etext - (uintptr_t)&_executable_start));
                dcache_purge_all_with_buffer((uintptr_t)dcache_buffer, sizeof(dcache_buffer));
            }

            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t tmu_start = timer_ns_gettime64();
            SHZ_MEMORY_BARRIER_SOFT();
            PERF_CNTR_START();
            SHZ_MEMORY_BARRIER_SOFT();

            if constexpr(!std::same_as<decltype(res), std::nullptr_t>)
                *res = function(std::forward<Args>(args)...);
            else
                function(std::forward<Args>(args)...);

            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t perfctr_cnt = PERF_CNTR_STOP();
            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t tmu_stop = timer_ns_gettime64();
            SHZ_MEMORY_BARRIER_SOFT();
            perfctr_sum += perfctr_cnt;
            tmu_sum     += tmu_stop - tmu_start;
            SHZ_MEMORY_BARRIER_SOFT();
            if(perfctr_cnt == perfctr_prev) {
                if(++matches == BENCHMARK_ITERATION_MATCHES)
                    break;
            } else {
                perfctr_prev = perfctr_cnt;
                matches = 0;
            }
        }

        irq_restore(state);

        printf("\t%20s[%s] : %llu/%llu cc, %llu ns, %d calls\n", name,
              (CacheFlush)? "UNCACHED" : "CACHED",
              perfctr_prev,
              perfctr_sum / iterations,
              tmu_sum     / iterations,
              iterations);
    };

    inner.template operator()<true>();
    inner.template operator()<false>();

}

#define benchmark(res, f, ...) (benchmark)(res, #f, f __VA_OPT__(,) __VA_ARGS__)

#endif
