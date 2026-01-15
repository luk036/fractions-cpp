/**
 * @file bench_comparison.cpp
 * Benchmark comparison between Fraction and ExtFraction implementations
 */

#include <benchmark/benchmark.h>
#include "fractions/extfractions.hpp"
#include "fractions/pyfractions.hpp"

using fractions::ExtFraction;
using fractions::Fraction;

// ============================================================================
// Construction Benchmarks
// ============================================================================

static void BM_ExtFraction_Construction(benchmark::State& state) {
    for (auto _ : state) {
        ExtFraction<int64_t> f(123456789, 987654321);
        benchmark::DoNotOptimize(f);
    }
}

static void BM_Fraction_Construction(benchmark::State& state) {
    for (auto _ : state) {
        Fraction<int64_t> f(123456789, 987654321);
        benchmark::DoNotOptimize(f);
    }
}

// ============================================================================
// Addition Benchmarks
// ============================================================================

static void BM_ExtFraction_Addition(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 + f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Addition(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 + f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_ExtFraction_Addition_Large(benchmark::State& state) {
    ExtFraction<int64_t> f1(123456789, 987654321);
    ExtFraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        auto result = f1 + f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Addition_Large(benchmark::State& state) {
    Fraction<int64_t> f1(123456789, 987654321);
    Fraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        auto result = f1 + f2;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Subtraction Benchmarks
// ============================================================================

static void BM_ExtFraction_Subtraction(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 - f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Subtraction(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 - f2;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Multiplication Benchmarks
// ============================================================================

static void BM_ExtFraction_Multiplication(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 * f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Multiplication(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 * f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_ExtFraction_Multiplication_Large(benchmark::State& state) {
    ExtFraction<int64_t> f1(123456789, 987654321);
    ExtFraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        auto result = f1 * f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Multiplication_Large(benchmark::State& state) {
    Fraction<int64_t> f1(123456789, 987654321);
    Fraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        auto result = f1 * f2;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Division Benchmarks
// ============================================================================

static void BM_ExtFraction_Division(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 / f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Division(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        auto result = f1 / f2;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Comparison Benchmarks
// ============================================================================

static void BM_ExtFraction_LessThan(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        bool result = f1 < f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_LessThan(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(1, 3);
    for (auto _ : state) {
        bool result = f1 < f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_ExtFraction_LessThan_Large(benchmark::State& state) {
    ExtFraction<int64_t> f1(123456789, 987654321);
    ExtFraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        bool result = f1 < f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_LessThan_Large(benchmark::State& state) {
    Fraction<int64_t> f1(123456789, 987654321);
    Fraction<int64_t> f2(111111111, 999999999);
    for (auto _ : state) {
        bool result = f1 < f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_ExtFraction_Equality(benchmark::State& state) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(2, 4);
    for (auto _ : state) {
        bool result = f1 == f2;
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Equality(benchmark::State& state) {
    Fraction<int64_t> f1(1, 2);
    Fraction<int64_t> f2(2, 4);
    for (auto _ : state) {
        bool result = f1 == f2;
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Complex Operation Benchmarks
// ============================================================================

static void BM_ExtFraction_Complex_Operation(benchmark::State& state) {
    ExtFraction<int64_t> a(1, 2);
    ExtFraction<int64_t> b(1, 3);
    ExtFraction<int64_t> c(1, 4);
    for (auto _ : state) {
        auto result = (a + b) * (c - ExtFraction<int64_t>(1, 6));
        benchmark::DoNotOptimize(result);
    }
}

static void BM_Fraction_Complex_Operation(benchmark::State& state) {
    Fraction<int64_t> a(1, 2);
    Fraction<int64_t> b(1, 3);
    Fraction<int64_t> c(1, 4);
    for (auto _ : state) {
        auto result = (a + b) * (c - Fraction<int64_t>(1, 6));
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================================
// Register benchmarks
// ============================================================================

BENCHMARK(BM_ExtFraction_Construction);
BENCHMARK(BM_Fraction_Construction);

BENCHMARK(BM_ExtFraction_Addition);
BENCHMARK(BM_Fraction_Addition);
BENCHMARK(BM_ExtFraction_Addition_Large);
BENCHMARK(BM_Fraction_Addition_Large);

BENCHMARK(BM_ExtFraction_Subtraction);
BENCHMARK(BM_Fraction_Subtraction);

BENCHMARK(BM_ExtFraction_Multiplication);
BENCHMARK(BM_Fraction_Multiplication);
BENCHMARK(BM_ExtFraction_Multiplication_Large);
BENCHMARK(BM_Fraction_Multiplication_Large);

BENCHMARK(BM_ExtFraction_Division);
BENCHMARK(BM_Fraction_Division);

BENCHMARK(BM_ExtFraction_LessThan);
BENCHMARK(BM_Fraction_LessThan);
BENCHMARK(BM_ExtFraction_LessThan_Large);
BENCHMARK(BM_Fraction_LessThan_Large);

BENCHMARK(BM_ExtFraction_Equality);
BENCHMARK(BM_Fraction_Equality);

BENCHMARK(BM_ExtFraction_Complex_Operation);
BENCHMARK(BM_Fraction_Complex_Operation);

BENCHMARK_MAIN();