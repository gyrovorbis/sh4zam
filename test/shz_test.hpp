#ifndef SHZ_TEST_HPP
#define SHZ_TEST_HPP

#include <type_traits>
#include <concepts>
#include <print>
#include <chrono>
#include <sh4zam/shz_sh4zam.hpp>
#include <cglm/cglm.h>

#include <gimbal/algorithms/gimbal_random.h>

#ifndef SHZ_DISABLE_BENCHMARKS
#   define BENCHMARK_ITERATION_COUNT   10
#else
#   define BENCHMARK_ITERATION_COUNT    1
#endif

#define BENCHMARK_ITERATION_MATCHES     3

#if SHZ_BACKEND == SHZ_SH4
#   define PERF_CNTR       PRFC1
#   define PERF_CNTR2      PRFC0

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
    PMCR_CTRL(PERF_CNTR) = PMCR_CLR | PMCR_RUN
                         | (PMCR_COUNT_CPU_CYCLES << PMCR_PMCLK_SHIFT)
                         | PMCR_ELAPSED_TIME_MODE;
}

SHZ_FORCE_INLINE uint64_t PERF_CNTR_STOP() {
    PMCR_CTRL(PERF_CNTR) &= ~PMCR_RUN;
    return (((uint64_t)(PMCTR_HIGH(PERF_CNTR) & 0xfff) << 32) | PMCTR_LOW(PERF_CNTR));
}

/* PRFC0: dual-purpose second counter.
   NOTE: this steals PRFC0 away from KOS's perf_cntr_timer_ns() for the
   duration -- harmless here since ns_gettime64() below goes through
   chrono -> newlib gettimeofday -> TMU, never through the perf counters.

   On the UNCACHED pass, caches are deliberately cold every iteration, so
   dcache-miss pipeline-freeze cycles are meaningful there. On the CACHED
   pass, caches are already known to be hot -- dcache misses would just
   read ~0 -- so instead we measure PARALLEL_INSTRUCTION_ISSUED_MODE there,
   which gives a direct hardware count of dual-issued instructions, rather
   than inferring it indirectly from disassembly and address arithmetic. */
SHZ_FORCE_INLINE void PERF_CNTR2_START(perf_cntr_event_t event) {
    PMCR_CTRL(PERF_CNTR2) = PMCR_CLR | PMCR_RUN
                          | (PMCR_COUNT_CPU_CYCLES << PMCR_PMCLK_SHIFT)
                          | event;
}

SHZ_FORCE_INLINE uint64_t PERF_CNTR2_STOP() {
    PMCR_CTRL(PERF_CNTR2) &= ~PMCR_RUN;
    return (((uint64_t)(PMCTR_HIGH(PERF_CNTR2) & 0xfff) << 32) | PMCTR_LOW(PERF_CNTR2));
}

#endif

namespace {
    inline uint64_t ns_gettime64(void) noexcept {
#if SHZ_BACKEND == SHZ_SH4
        return timer_ns_gettime64();
#else
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
#endif
    }
}

template<typename... Args>
SHZ_NO_INLINE
std::pair<uint64_t, uint64_t> benchmark(auto res, const char* name, auto&& function, Args&&... args) noexcept {
    auto inner = [&]<bool CacheFlush>() SHZ_NO_INLINE SHZ_FUNC_ALIGNAS(32) SHZ_NO_UNROLL_LOOPS {
        uint64_t tmu_sum    = 0;
        uint64_t flush_sum  = 0;
        uint64_t sum        = 0;
        uint64_t prev       = 0;
        uint64_t pc2_sum    = 0;
        uint64_t pc2_prev   = 0;
        unsigned matches    = 0;
        int      iterations = -1;
        bool     converged  = false;

#if SHZ_BACKEND == SHZ_SH4
        SHZ_INSTR_BARRIER();
        auto state = irq_disable();
#endif
        SHZ_INSTR_BARRIER();
        /* On SH4, PMCR already gives us a real per-iteration cycle count, so
           the TMU read (a genuine function call down to hardware registers)
           is just supplementary "ns" reporting -- no reason to pay its call
           overhead (and register pressure) twice per iteration. Hoisted out
           uniformly across backends: other backends don't have a perf-counter
           equivalent yet, but will, and should follow the same shape when
           they do. */
        uint64_t tmu_start = ns_gettime64();
        SHZ_INSTR_BARRIER();

        for(; iterations < BENCHMARK_ITERATION_COUNT; ++iterations) {
            SHZ_INSTR_BARRIER();

#if !defined(SHZ_DISABLE_BENCHMARKS) && (SHZ_BACKEND == SHZ_SH4)
            if constexpr(CacheFlush) {
                flush_sum += [] SHZ_NO_INLINE {
                    SHZ_INSTR_BARRIER();
                    uint64_t flush_start = ns_gettime64();
                    SHZ_INSTR_BARRIER();
                    /* The icache is direct-mapped (arch/cache.h), so any
                       contiguous range >= its size hits every index exactly
                       once -- invalidating the whole cache. Passing the full
                       .text range here instead re-invalidates those same
                       256 entries hundreds of times over (2.5MB / 32B is
                       ~81000 loop iterations vs. the 256 actually needed),
                       and dominates UNCACHED benchmark wall time for no
                       benefit: the end state is identical either way. */
#   if KOS_VERSION_BELOW(2, 3, 0)
                    icache_flush_range((uintptr_t)&_executable_start,
                                       ARCH_CACHE_L1_ICACHE_SIZE);
#   else
                    icache_inval_range((uintptr_t)&_executable_start,
                                       ARCH_CACHE_L1_ICACHE_SIZE);
#   endif
                    dcache_purge_all();
                    return ns_gettime64() - flush_start;
                }();
                SHZ_INSTR_BARRIER();
            }
#endif

            SHZ_INSTR_BARRIER();
#if SHZ_BACKEND == SHZ_SH4
            PERF_CNTR_START();
            if constexpr(CacheFlush)
                PERF_CNTR2_START(PMCR_PIPELINE_FREEZE_BY_DCACHE_MISS_MODE);
            else
                PERF_CNTR2_START(PMCR_PARALLEL_INSTRUCTION_ISSUED_MODE);
            SHZ_INSTR_BARRIER();
#endif
            [](auto r, auto&& fn, auto&&... fargs) {
                if constexpr(!std::same_as<decltype(r), std::nullptr_t>)
                    [[maybe_unused]] auto tmp =
                        *r = fn(std::forward<decltype(fargs)>(fargs)...);
                else
                    fn(std::forward<decltype(fargs)>(fargs)...);
            }(std::forward<decltype(res)>(res), std::forward<decltype(function)>(function), std::forward<decltype(args)>(args)...);
            SHZ_INSTR_BARRIER();
#if SHZ_BACKEND == SHZ_SH4
            uint64_t perfctr_cnt = PERF_CNTR_STOP();
            uint64_t pc2_cnt     = PERF_CNTR2_STOP();
#endif
            SHZ_INSTR_BARRIER();

            // If we're warming up the cache for the first iteration, don't add metrics.
            if(iterations == -1)
                continue;

#if SHZ_BACKEND == SHZ_SH4
            const auto& cnt = perfctr_cnt;
            pc2_sum  += pc2_cnt;
            pc2_prev  = pc2_cnt;
#else
            /* TODO: swap in a real perf-counter read once this backend has one. */
            uint64_t cnt = 0;
#endif
            sum += cnt;
            if(cnt == prev) {
                if(++matches == BENCHMARK_ITERATION_MATCHES) {
                    // Have to increment ourselves upon early exit.
                    ++iterations;
                    converged = true;
                    break;
                }
            } else {
                prev = cnt;
                matches = 0;
            }
            SHZ_INSTR_BARRIER();
        }

        SHZ_INSTR_BARRIER();
        uint64_t tmu_stop = ns_gettime64();
        tmu_sum          = (tmu_stop - tmu_start) - flush_sum;
#if SHZ_BACKEND == SHZ_SH4
        SHZ_INSTR_BARRIER();
        irq_restore(state);
#endif
        SHZ_INSTR_BARRIER();

#ifndef SHZ_DISABLE_BENCHMARKS
#   if SHZ_BACKEND == SHZ_SH4
        std::println("\t{:>30} [{:>8}] : {:6}/{:6} cc, {:6} ns, {:3}/{:3} {}, {:2} calls",
              name,
              (CacheFlush)? "UNCACHED" : "CACHED",
              prev,
              sum     / iterations,
              tmu_sum / iterations,
              pc2_prev,
              pc2_sum / iterations,
              (CacheFlush)? "dcf" : "iss",
              iterations);
#   else
        std::println("\t{:>30} [{:>8}] : {:6}/{:6} cc, {:4} ns, {:2} calls",
              name,
              (CacheFlush)? "UNCACHED" : "CACHED",
              prev,
              sum     / iterations,
              tmu_sum / iterations,
              iterations);
#   endif
#endif
        return converged ? prev : (sum / iterations);
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
SHZ_NO_INLINE
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
    float cacheGainz   = ((float)refCacheCyc  ) / ((float)shzCacheCyc  );

#   if SHZ_BACKEND == SHZ_SH4
    float uncacheGainz = ((float)refUncacheCyc) / ((float)shzUncacheCyc);
#   else
    float uncacheGainz = 0.0f;
#   endif

    // Ratios within this band of 1.0 are indistinguishable from the kind of
    // incidental code-layout/scheduling noise we've repeatedly measured
    // (a few cycles out of a few dozen), not a genuine win or regression.
    //
    // A pure percentage tolerance breaks down at small absolute magnitudes:
    // a 1-cycle swing on a ~20-cycle function is "5%", identical noise on a
    // bigger function is invisible. So we also allow a small fixed cycle
    // tolerance -- noise if EITHER condition is met. Values are set from
    // real measured cases: set_translation's cached 20-vs-19 (1 cycle) and
    // uncached 114-vs-105 (9 cycles) are noise; scale_reverse's cached
    // 63-vs-45 (18 cycles, 28%) is a genuine regression that must still
    // trip a LOSSEZ, so the CACHED absolute tolerance stays well under that.
    //
    // CACHED and UNCACHED get separate budgets, because they have different
    // noise floors. CACHED is deterministic: set_scale's shz and glm closures
    // were verified byte-for-byte identical in compiled output, and CACHED
    // measured them dead equal (36/36cc) on real hardware -- so its tolerance
    // stays tight. UNCACHED is not deterministic in the same way: those same
    // two byte-identical closures still measured a stable ~20-cycle gap
    // (152-vs-132cc), reproducible across separate runs, because UNCACHED's
    // full icache/dcache flush before every call leaves the SDRAM controller's
    // open row/bus-arbitration state dependent on whichever closure happened
    // to run first -- real hardware jitter inherent to the cold-cache
    // methodology, not a code regression. UNCACHED's absolute budget is set
    // wide enough to absorb that.
    constexpr float    BENCHMARK_TOLERANCE_PCT_CACHED   = 0.05f;
    constexpr uint64_t BENCHMARK_TOLERANCE_ABS_CACHED   = 10;
    constexpr float    BENCHMARK_TOLERANCE_PCT_UNCACHED = 0.05f;
    constexpr uint64_t BENCHMARK_TOLERANCE_ABS_UNCACHED = 24;

    enum {
        EQUAL,
        GAINZ,
        APPROX,
        SKETCH,
        LOSSEZ
    } gainz;
    const char* gainz_str;

    enum axis { WORSE, NOISE, BETTER };
    auto classify = [](float g, uint64_t shzCyc, uint64_t refCyc, float pctTol, uint64_t absTol) {
        uint64_t absDiff = (shzCyc > refCyc) ? (shzCyc - refCyc) : (refCyc - shzCyc);
        if(absDiff <= absTol) return NOISE;
        if(g > 1.0f + pctTol) return BETTER;
        if(g < 1.0f - pctTol) return WORSE;
        return NOISE;
    };

    axis cacheAxis   = classify(cacheGainz,   shzCacheCyc,   refCacheCyc,
                                BENCHMARK_TOLERANCE_PCT_CACHED, BENCHMARK_TOLERANCE_ABS_CACHED);
    axis uncacheAxis = classify(uncacheGainz, shzUncacheCyc, refUncacheCyc,
                                BENCHMARK_TOLERANCE_PCT_UNCACHED, BENCHMARK_TOLERANCE_ABS_UNCACHED);

    if(shz_equalf(cacheGainz, 1.0f) && shz_equalf(uncacheGainz, 1.0f)) {
        gainz = EQUAL;
        gainz_str = "EQUAL";
    } else if((cacheAxis == BETTER && uncacheAxis != WORSE) || (cacheAxis != WORSE && uncacheAxis == BETTER)) {
        gainz = GAINZ;
        gainz_str = "GAINZ";
    } else if(cacheAxis == NOISE && uncacheAxis == NOISE) {
        gainz = APPROX;
        gainz_str = "APPROX";
    } else if((cacheAxis == BETTER && uncacheAxis == WORSE) || (cacheAxis == WORSE && uncacheAxis == BETTER)) {
        gainz = SKETCH;
        gainz_str = "SKETCH";
    } else {
        gainz = LOSSEZ;
        gainz_str = "LOSSEZ";
    }

    std::println("* [   {:6}  ]:\t{:.4f}x / {:.4f}x [UNCACHED / CACHED]", gainz_str, uncacheGainz, cacheGainz);

#   if SHZ_BACKEND == SHZ_SH4
    return gainz != LOSSEZ;
#   else
    return true;
#   endif
#else
    return true;
#endif
}

#define benchmark_cmp(retType, shzFn, refFn, ...) (benchmark_cmp<retType>)(#shzFn, shzFn, #refFn, refFn __VA_OPT__(,) __VA_ARGS__)

union shz_glm_mat4 {
    shz::mat4x4 shz;
    mat4        glm;
};

union shz_glm_vec3 {
    shz::vec3 shz;
    vec3      glm;
};

union shz_glm_vec4 {
    shz::vec4 shz;
    vec4      glm;
};

#endif
