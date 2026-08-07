/**
 * @file bench_comparison.cpp
 * Benchmark comparison between Fraction and ExtFraction implementations
 */

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include "fractions/extfractions.hpp"
#include "fractions/pyfractions.hpp"

using fractions::ExtFraction;
using fractions::Fraction;

// ============================================================================
// Construction Benchmarks
// ============================================================================

static void bench_construction(ankerl::nanobench::Bench& b) {
    b.run("ExtFraction<int64_t> construction", [&] {
        ExtFraction<int64_t> f(123456789, 987654321);
        ankerl::nanobench::doNotOptimizeAway(f);
    });

    b.run("Fraction<int64_t> construction", [&] {
        Fraction<int64_t> f(123456789, 987654321);
        ankerl::nanobench::doNotOptimizeAway(f);
    });
}

// ============================================================================
// Addition Benchmarks
// ============================================================================

static void bench_addition(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);

    b.run("ExtFraction<int64_t> addition 1/2 + 1/3", [&] {
        auto result = f1 + f2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> g1(1, 2);
    Fraction<int64_t> g2(1, 3);

    b.run("Fraction<int64_t> addition 1/2 + 1/3", [&] {
        auto result = g1 + g2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    ExtFraction<int64_t> h1(123456789, 987654321);
    ExtFraction<int64_t> h2(111111111, 999999999);

    b.run("ExtFraction<int64_t> addition large", [&] {
        auto result = h1 + h2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> i1(123456789, 987654321);
    Fraction<int64_t> i2(111111111, 999999999);

    b.run("Fraction<int64_t> addition large", [&] {
        auto result = i1 + i2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Subtraction Benchmarks
// ============================================================================

static void bench_subtraction(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);

    b.run("ExtFraction<int64_t> subtraction 1/2 - 1/3", [&] {
        auto result = f1 - f2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> g1(1, 2);
    Fraction<int64_t> g2(1, 3);

    b.run("Fraction<int64_t> subtraction 1/2 - 1/3", [&] {
        auto result = g1 - g2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Multiplication Benchmarks
// ============================================================================

static void bench_multiplication(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);

    b.run("ExtFraction<int64_t> multiplication 1/2 * 1/3", [&] {
        auto result = f1 * f2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> g1(1, 2);
    Fraction<int64_t> g2(1, 3);

    b.run("Fraction<int64_t> multiplication 1/2 * 1/3", [&] {
        auto result = g1 * g2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    ExtFraction<int64_t> h1(123456789, 987654321);
    ExtFraction<int64_t> h2(111111111, 999999999);

    b.run("ExtFraction<int64_t> multiplication large", [&] {
        auto result = h1 * h2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> i1(123456789, 987654321);
    Fraction<int64_t> i2(111111111, 999999999);

    b.run("Fraction<int64_t> multiplication large", [&] {
        auto result = i1 * i2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Division Benchmarks
// ============================================================================

static void bench_division(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);

    b.run("ExtFraction<int64_t> division 1/2 / 1/3", [&] {
        auto result = f1 / f2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> g1(1, 2);
    Fraction<int64_t> g2(1, 3);

    b.run("Fraction<int64_t> division 1/2 / 1/3", [&] {
        auto result = g1 / g2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Comparison Benchmarks
// ============================================================================

static void bench_comparison(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> f1(1, 2);
    ExtFraction<int64_t> f2(1, 3);

    b.run("ExtFraction<int64_t> less-than 1/2 < 1/3", [&] {
        bool result = f1 < f2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> g1(1, 2);
    Fraction<int64_t> g2(1, 3);

    b.run("Fraction<int64_t> less-than 1/2 < 1/3", [&] {
        bool result = g1 < g2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    ExtFraction<int64_t> h1(123456789, 987654321);
    ExtFraction<int64_t> h2(111111111, 999999999);

    b.run("ExtFraction<int64_t> less-than large", [&] {
        bool result = h1 < h2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> i1(123456789, 987654321);
    Fraction<int64_t> i2(111111111, 999999999);

    b.run("Fraction<int64_t> less-than large", [&] {
        bool result = i1 < i2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    ExtFraction<int64_t> j1(1, 2);
    ExtFraction<int64_t> j2(2, 4);

    b.run("ExtFraction<int64_t> equality 1/2 == 2/4", [&] {
        bool result = j1 == j2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> k1(1, 2);
    Fraction<int64_t> k2(2, 4);

    b.run("Fraction<int64_t> equality 1/2 == 2/4", [&] {
        bool result = k1 == k2;
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Complex Operation Benchmarks
// ============================================================================

static void bench_complex(ankerl::nanobench::Bench& b) {
    ExtFraction<int64_t> a(1, 2);
    ExtFraction<int64_t> b_val(1, 3);
    ExtFraction<int64_t> c(1, 4);

    b.run("ExtFraction<int64_t> complex (1/2 + 1/3) * (1/4 - 1/6)", [&] {
        auto result = (a + b_val) * (c - ExtFraction<int64_t>(1, 6));
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    Fraction<int64_t> d(1, 2);
    Fraction<int64_t> e(1, 3);
    Fraction<int64_t> f(1, 4);

    b.run("Fraction<int64_t> complex (1/2 + 1/3) * (1/4 - 1/6)", [&] {
        auto result = (d + e) * (f - Fraction<int64_t>(1, 6));
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

// ============================================================================
// Main
// ============================================================================

int main() {
    ankerl::nanobench::Bench bench;
    bench.title("fractions-cpp benchmarks")
        .unit("iteration")
        .warmup(100)
        .minEpochIterations(5000);

    bench_construction(bench);
    bench_addition(bench);
    bench_subtraction(bench);
    bench_multiplication(bench);
    bench_division(bench);
    bench_comparison(bench);
    bench_complex(bench);
}
