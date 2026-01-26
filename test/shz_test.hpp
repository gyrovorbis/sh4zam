#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>
#include <concepts>

#include <kos.h>
#include <sh4zam/shz_sh4zam.hpp>

#include <gimbal/algorithms/gimbal_random.h>

#define BENCHMARK_ITERATION_COUNT   100
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
    perf_cntr_timer_enable();

    auto inner = [&]<bool CacheFlush>() {
        uint64_t sum = 0;
        uint64_t tmu_sum = 0;

        SHZ_MEMORY_BARRIER_HARD();
        auto state = irq_disable();

        for(unsigned i = 0; i < BENCHMARK_ITERATION_COUNT; ++i) {
            SHZ_MEMORY_BARRIER_SOFT();

            [&] SHZ_NO_INLINE SHZ_COLD {
                    if constexpr(CacheFlush) {
                        icache_flush_range(_executable_start, (_etext  -_executable_start));
                        dcache_purge_all();
                    }

                    SHZ_MEMORY_BARRIER_SOFT();
                    uint64_t tmu_start = timer_ns_gettime64();
                    SHZ_MEMORY_BARRIER_SOFT();
                    PERF_CNTR_START();
                    SHZ_MEMORY_BARRIER_SOFT();
                    [&] SHZ_HOT {
                        if constexpr(!std::same_as<decltype(res), std::nullptr_t>)
                            *res = function(std::forward<Args>(args)...);
                        else
                            function(std::forward<Args>(args)...);
                    }();
                    SHZ_MEMORY_BARRIER_SOFT();
                    uint64_t pfctr_time = PERF_CNTR_STOP();
                    SHZ_MEMORY_BARRIER_SOFT();
                    uint64_t tmu_stop = timer_ns_gettime64();
                    SHZ_MEMORY_BARRIER_SOFT();
                    sum += pfctr_time;
                    tmu_sum += tmu_stop - tmu_start;
            }();
            SHZ_MEMORY_BARRIER_SOFT();
        }

        irq_restore(state);

        printf("\t%20s[%s] : %llu cc, %llu ns\n", name,
              (CacheFlush)? "UNCACHED" : "CACHED",
              (uint64_t)((double)sum / (double)BENCHMARK_ITERATION_COUNT),
              (uint64_t)((double)tmu_sum / (double)BENCHMARK_ITERATION_COUNT));
    };

    inner.template operator()<true>();
    inner.template operator()<false>();

}

#define benchmark(res, f, ...) (benchmark)(res, #f, f __VA_OPT__(,) __VA_ARGS__)

#endif
