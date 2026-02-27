#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>
#include <concepts>
#include <print>
#include <ctime>

#include <sh4zam/shz_sh4zam.hpp>

#include <gimbal/algorithms/gimbal_random.h>

#ifndef SHZ_DISABLE_BENCHMARKS
#   define BENCHMARK_ITERATION_COUNT   10
#else
#   define BENCHMARK_ITERATION_COUNT    1
#endif

#define BENCHMARK_ITERATION_MATCHES 3
#define SHZ_MEMORY_BARRIER_HARD() __sync_synchronize()

#if SHZ_BACKEND == SHZ_SH4
#   include <kos.h>
#   define PMCR_PMENABLE   0x8000  /* Enable */
#   define PMCR_PMST       0x4000  /* Start */
#   define PMCR_RUN        0xc000  /* Run: Enable | Start */
#   define PMCR_CLR        0x2000  /* Clear */
#   define PMCR_PMCLK      0x0100  /* Clock Type */
#   define PMCR_PMMODE     0x003f  /* Event Mode */

/* PMCR count type field position */
#   define PMCR_PMCLK_SHIFT 8

/* 5ns per count in 1 cycle = 1 count mode(PMCR_COUNT_CPU_CYCLES) */
#   define NS_PER_CYCLE      5

#   define PMCR_CTRL(o)  ( *((volatile uint16_t *)(0xff000084) + (o << 1)) )
#   define PMCTR_HIGH(o) ( *((volatile uint32_t *)(0xff100004) + (o << 1)) )
#   define PMCTR_LOW(o)  ( *((volatile uint32_t *)(0xff100008) + (o << 1)) )

SHZ_FORCE_INLINE void PERF_CNTR_START() {
    PMCR_CTRL(0) &= ~PMCR_RUN;
    PMCR_CTRL(0) |= PMCR_CLR;
    PMCR_CTRL(0) |= PMCR_RUN;
}

SHZ_FORCE_INLINE uint64_t PERF_CNTR_STOP() {
    PMCR_CTRL(0) &= ~PMCR_RUN;
    return (((uint64_t)PMCTR_HIGH(PRFC0) << 32) | PMCTR_LOW(PRFC0));
}
#endif

namespace {
    uint64_t ns_gettime64(void) noexcept {
        timespec ts;

        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_nsec + ts.tv_sec * 1000000000;
    }
}

template<typename F, typename... Args>
SHZ_NO_INLINE
std::pair<uint64_t, uint64_t> benchmark(auto res, const char* name, F &&function, Args&&... args) noexcept {
#if SHZ_BACKEND == SHZ_SH4
    perf_cntr_timer_enable();
#endif
    auto inner = [&]<bool CacheFlush>() SHZ_NO_INLINE SHZ_ICACHE_ALIGNED {
        uint64_t tmu_sum      = 0;
        uint64_t sum          = 0;
        uint64_t prev         = 0;
        unsigned matches      = 0;
        int      iterations   = 0;

#if SHZ_BACKEND == SHZ_SH4
        SHZ_MEMORY_BARRIER_SOFT();
        auto state = irq_disable();
#endif
        SHZ_MEMORY_BARRIER_SOFT();

 #pragma GCC unroll 0
        for(iterations = 0; iterations < BENCHMARK_ITERATION_COUNT; ++iterations) {
        SHZ_MEMORY_BARRIER_SOFT();
#if !defined(SHZ_DISABLE_BENCHMARKS) && (SHZ_BACKEND == SHZ_SH4)
            if constexpr(CacheFlush) {
                alignas(32) uint8_t dcache_buffer[1024 * 16];
                icache_inval_range((uintptr_t)&_executable_start, (size_t)((uintptr_t)&_etext - (uintptr_t)&_executable_start));
                dcache_purge_all_with_buffer((uintptr_t)dcache_buffer, sizeof(dcache_buffer));
            }
#endif

            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t tmu_start = ns_gettime64();
            SHZ_MEMORY_BARRIER_SOFT();
#if SHZ_BACKEND == SHZ_SH4
            PERF_CNTR_START();
            SHZ_MEMORY_BARRIER_SOFT();
#endif
            if constexpr(!std::same_as<decltype(res), std::nullptr_t>)
                *res = function(std::forward<Args>(args)...);
            else
                function(std::forward<Args>(args)...);

            SHZ_MEMORY_BARRIER_SOFT();
#if SHZ_BACKEND == SHZ_SH4
            uint64_t perfctr_cnt = PERF_CNTR_STOP();
#endif
            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t tmu_stop = ns_gettime64();
            SHZ_MEMORY_BARRIER_SOFT();
            uint64_t tmu_cnt = tmu_stop - tmu_start;
            tmu_sum         += tmu_cnt;
#if SHZ_BACKEND == SHZ_SH4
            const auto& cnt = perfctr_cnt;
#else
            const auto& cnt = tmu_cnt;
#endif
            sum += cnt;
            if(cnt == prev) {
                if(++matches == BENCHMARK_ITERATION_MATCHES)
                    break;
            } else {
                prev = cnt;
                matches = 0;
            }
            SHZ_MEMORY_BARRIER_SOFT();
        }

#if SHZ_BACKEND == SHZ_SH4
        SHZ_MEMORY_BARRIER_SOFT();
        irq_restore(state);
#endif
        SHZ_MEMORY_BARRIER_SOFT();

#ifndef SHZ_DISABLE_BENCHMARKS
        std::println("\t{:>25} [{:8}] : {:4}/{:4} cc, {:4} ns, {:2} calls",
              name,
              (CacheFlush)? "UNCACHED" : "CACHED",
              prev,
              sum       / iterations,
              tmu_sum   / iterations,
              iterations);
#endif
        return (iterations < BENCHMARK_ITERATION_COUNT)? prev : (sum / iterations);
    };

    return std::make_pair(
#if SHZ_BACKEND == SHZ_SH4
        inner.template operator()<true>(),
#else
        0,
#endif
        inner.template operator()<false>()
    );
}

#define benchmark(res, f, ...) (benchmark)(res, #f, f __VA_OPT__(,) __VA_ARGS__)

template<typename R, typename ShzFn, typename RefFn, typename... Args>
bool benchmark_cmp(const char* shzName, ShzFn&& shzFn,
                   const char* refName, RefFn&& refFn,
                   Args&&... args) noexcept
{
    uint64_t shzUncacheCyc, shzCacheCyc, refUncacheCyc, refCacheCyc;

    if constexpr(std::same_as<R, void> || std::same_as<R, std::nullptr_t>) {
        std::tie(shzUncacheCyc, shzCacheCyc) = (benchmark)(nullptr, shzName, std::forward<ShzFn>(shzFn), std::forward<Args>(args)...);
        std::tie(refUncacheCyc, refCacheCyc) = (benchmark)(nullptr, refName, std::forward<RefFn>(refFn), std::forward<Args>(args)...);
    } else {
        volatile R result;

        std::tie(shzUncacheCyc, shzCacheCyc) = (benchmark)(&result, shzName, std::forward<ShzFn>(shzFn), std::forward<Args>(args)...);
        std::tie(refUncacheCyc, refCacheCyc) = (benchmark)(&result, refName, std::forward<RefFn>(refFn), std::forward<Args>(args)...);
    }

#ifndef SHZ_DISABLE_BENCHMARKS
    double cacheGainz   = ((double)refCacheCyc  ) / ((double)shzCacheCyc  );

#   if SHZ_BACKEND == SHZ_SH4
    double uncacheGainz = ((double)refUncacheCyc) / ((double)shzUncacheCyc);
#   else
    double uncacheGainz = 0.0;
#   endif

    bool gainz = (cacheGainz > 1.0f || uncacheGainz > 1.0f);

    std::println("* [   {:6}  ]:\t{:.4f}x / {:.4f}x [UNCACHED / CACHED]", gainz? "GAINZ" : "LOSSEZ", uncacheGainz, cacheGainz);

#   if SHZ_BACKEND == SHZ_SH4
    return gainz;
#   else
    return true;
#   endif
#else
    return true;
#endif
}

#define benchmark_cmp(retType, shzFn, refFn, ...) (benchmark_cmp<retType>)(#shzFn, shzFn, #refFn, refFn __VA_OPT__(,) __VA_ARGS__)

#endif
