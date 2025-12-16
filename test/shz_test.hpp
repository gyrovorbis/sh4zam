#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>

#include <kos.h>
#include <sh4zam/shz_sh4zam.hpp>

#include <gimbal/algorithms/gimbal_random.h>

#define BENCHMARK_ITERATION_COUNT   50
#define SHZ_MEMORY_BARRIER_HARD() __sync_synchronize()

template<typename F, typename... Args>
SHZ_NO_INLINE void benchmark(auto* res, const char* name, F &&function, Args&&... args) {
    uint64_t sum = 0;

    SHZ_MEMORY_BARRIER_HARD();
    auto state = irq_disable();

    for(unsigned i = 0; i < BENCHMARK_ITERATION_COUNT; ++i) {
        dcache_flush_all();
        icache_flush_range(_executable_start, \
                          (_etext  -_executable_start));

        //uint64_t tstart = timer_ns_gettime64();
        uint64_t start = perf_cntr_timer_ns();

        SHZ_MEMORY_BARRIER_HARD();
        *res = function(std::forward<Args>(args)...);
        SHZ_MEMORY_BARRIER_HARD();

        uint64_t stop = perf_cntr_timer_ns();
        sum += stop - start;
    }
    irq_restore(state);
    
    printf("\t%20s : %llu ns\n", name, sum / BENCHMARK_ITERATION_COUNT);
}

#define benchmark(res, f, ...) (benchmark)(res, #f, f __VA_OPT__(,) __VA_ARGS__)

#endif
