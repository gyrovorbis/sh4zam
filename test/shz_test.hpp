#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>
#include <concepts>

#include <kos.h>
#include <sh4zam/shz_sh4zam.hpp>

#include <gimbal/algorithms/gimbal_random.h>

#define BENCHMARK_ITERATION_COUNT   100
#define SHZ_MEMORY_BARRIER_HARD() __sync_synchronize()

#define PMCR_CTRL(o)  ( *((volatile uint16_t *)(0xff000084) + (o << 1)) )
#define PMCTR_HIGH(o) ( *((volatile uint32_t *)(0xff100004) + (o << 1)) )
#define PMCTR_LOW(o)  ( *((volatile uint32_t *)(0xff100008) + (o << 1)) )

SHZ_FORCE_INLINE uint64_t PERF_CNTR(bool* valid) {
        uint32_t lo, hi, hi2;
        /* Read the high part twice, before and after reading the high part,
         * to make sure that the low counter didn't overflow. We can detect an
         * overflow by the two reads of the high part returning different
         * values. */
        hi = PMCTR_HIGH(PRFC0);
        lo = PMCTR_LOW(PRFC0);
        hi2 = PMCTR_HIGH(PRFC0);
        *valid = (hi == hi2);
        return (((uint64_t)hi << 32) | lo) * 5;
}

template<typename F, typename... Args>
SHZ_NO_INLINE
void benchmark(auto res, const char* name, F &&function, Args&&... args) {
    perf_cntr_timer_enable();

    auto inner = [&]<bool CacheFlush>() {
        uint64_t iterations = 0;
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
                    bool valid_start;
                    uint64_t tmu_start = timer_ns_gettime64();
                    uint64_t start = PERF_CNTR(&valid_start);
                    SHZ_MEMORY_BARRIER_SOFT();

                    [&] SHZ_HOT {
                        if constexpr(!std::same_as<decltype(res), std::nullptr_t>)
                            *res = function(std::forward<Args>(args)...);
                        else
                            function(std::forward<Args>(args)...);
                    }();
                    SHZ_MEMORY_BARRIER_SOFT();
                    bool valid_stop;
                    uint64_t stop = PERF_CNTR(&valid_stop);
                    uint64_t tmu_stop = timer_ns_gettime64();
                    SHZ_MEMORY_BARRIER_SOFT();

                    if(valid_stop && valid_start) {
                        sum += stop - start;
                        ++iterations;
                    }

                    tmu_sum += tmu_stop - tmu_start;
            }();
            SHZ_MEMORY_BARRIER_SOFT();
        }

        irq_restore(state);

        printf("\t%20s[%s] : %llu ns, %llu ns\n", name, (CacheFlush)? "UNCACHED" : "CACHED", sum / iterations, tmu_sum / BENCHMARK_ITERATION_COUNT);
        if(iterations != BENCHMARK_ITERATION_COUNT)
            printf("\tReested: %llu\n", BENCHMARK_ITERATION_COUNT - iterations);
    };

    inner.template operator()<true>();
    inner.template operator()<false>();

}

#define benchmark(res, f, ...) (benchmark)(res, #f, f __VA_OPT__(,) __VA_ARGS__)

#endif
