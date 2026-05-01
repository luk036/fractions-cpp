/**
 * @file test_pyfractions.cpp
 * Test file comparing pyfractions.hpp with extfractions.hpp
 */

#include <doctest/doctest.h>

#include "fractions/extfractions.hpp"
#include "fractions/pyfractions.hpp"

using fractions::ExtFraction;
using fractions::Fraction;

TEST_SUITE("Python Fractions Implementation Comparison") {
    TEST_CASE("Basic Construction") {
        // Test Fraction implementation (pyfractions.hpp)
        fractions::Fraction<int64_t> py_f1(3, 4);
        CHECK_EQ(py_f1.numerator(), 3);
        CHECK_EQ(py_f1.denominator(), 4);

        fractions::Fraction<int64_t> py_f2(6, 8);  // Should normalize to 3/4
        CHECK_EQ(py_f2.numerator(), 3);
        CHECK_EQ(py_f2.denominator(), 4);

        fractions::Fraction<int64_t> py_f3(-5, 10);  // Should normalize to -1/2
        CHECK_EQ(py_f3.numerator(), -1);
        CHECK_EQ(py_f3.denominator(), 2);

        fractions::Fraction<int64_t> py_f4(5, -10);  // Should normalize to -1/2
        CHECK_EQ(py_f4.numerator(), -1);
        CHECK_EQ(py_f4.denominator(), 2);
    }

    TEST_CASE("Comparison with extfractions.hpp") {
        // Test that both implementations produce the same results

        fractions::Fraction<int64_t> py_f1(1, 2);
        fractions::Fraction<int64_t> py_f2(1, 3);

        fractions::ExtFraction<int> fast_f1(1, 2);
        fractions::ExtFraction<int> fast_f2(1, 3);

        // Addition
        auto py_sum = py_f1 + py_f2;
        auto fast_sum = fast_f1 + fast_f2;

        CHECK_EQ(py_sum.numerator(), 5);
        CHECK_EQ(py_sum.denominator(), 6);
        CHECK_EQ(fast_sum.numer(), 5);
        CHECK_EQ(fast_sum.denom(), 6);

        // Subtraction
        auto py_diff = py_f1 - py_f2;
        auto fast_diff = fast_f1 - fast_f2;

        CHECK_EQ(py_diff.numerator(), 1);
        CHECK_EQ(py_diff.denominator(), 6);
        CHECK_EQ(fast_diff.numer(), 1);
        CHECK_EQ(fast_diff.denom(), 6);

        // Multiplication
        auto py_prod = py_f1 * py_f2;
        auto fast_prod = fast_f1 * fast_f2;

        CHECK_EQ(py_prod.numerator(), 1);
        CHECK_EQ(py_prod.denominator(), 6);
        CHECK_EQ(fast_prod.numer(), 1);
        CHECK_EQ(fast_prod.denom(), 6);

        // Division
        auto py_div = py_f1 / py_f2;
        auto fast_div = fast_f1 / fast_f2;

        CHECK_EQ(py_div.numerator(), 3);
        CHECK_EQ(py_div.denominator(), 2);
        CHECK_EQ(fast_div.numer(), 3);
        CHECK_EQ(fast_div.denom(), 2);
    }

    TEST_CASE("Optimized Arithmetic - Knuth's Algorithm") {
        // Test the optimized GCD-based arithmetic from Knuth TAOCP

        fractions::Fraction<int64_t> a(123456789, 987654321);
        fractions::Fraction<int64_t> b(111111111, 999999999);

        // Addition should use GCD optimization
        auto sum = a + b;
        CHECK_EQ(sum.numerator(), 233196158);
        CHECK_EQ(sum.denominator(), 987654321);

        // Multiplication should use GCD optimization
        auto prod = a * b;
        CHECK_EQ(prod.numerator(), 13717421);
        CHECK_EQ(prod.denominator(), 987654321);
    }

    TEST_CASE("Comparison Operators") {
        fractions::Fraction<int64_t> f1(1, 2);
        fractions::Fraction<int64_t> f2(1, 3);
        fractions::Fraction<int64_t> f3(2, 4);  // Same as f1

        CHECK_EQ(f1, f3);
        CHECK_NE(f1, f2);
        CHECK_GT(f1, f2);
        CHECK_LT(f2, f1);
        CHECK_GE(f1, f3);
        CHECK_GE(f1, f2);
        CHECK_LE(f2, f1);
        CHECK_LE(f3, f1);

        // Comparison with integers
        CHECK_NE(f1, 0);  // 1/2 != 0
        CHECK_NE(f1, 1);  // 1/2 != 1
        CHECK_LT(f1, 1);
        CHECK_GT(f1, 0);
        CHECK_LE(f1, 1);
        CHECK_GE(f1, 0);
    }

    TEST_CASE("Integer Operations") {
        fractions::Fraction<int64_t> f(3, 4);

        // Operations with integers
        auto sum = f + static_cast<int64_t>(1);
        CHECK_EQ(sum.numerator(), 7);
        CHECK_EQ(sum.denominator(), 4);

        auto diff = f - static_cast<int64_t>(1);
        CHECK_EQ(diff.numerator(), -1);
        CHECK_EQ(diff.denominator(), 4);

        auto prod = f * static_cast<int64_t>(2);
        CHECK_EQ(prod.numerator(), 3);
        CHECK_EQ(prod.denominator(), 2);

        auto div = f / static_cast<int64_t>(2);
        CHECK_EQ(div.numerator(), 3);
        CHECK_EQ(div.denominator(), 8);

        // Integer on left side
        auto sum2 = static_cast<int64_t>(1) + f;
        CHECK_EQ(sum2.numerator(), 7);
        CHECK_EQ(sum2.denominator(), 4);

        auto div2 = static_cast<int64_t>(1) / f;
        CHECK_EQ(div2.numerator(), 4);
        CHECK_EQ(div2.denominator(), 3);
    }

    TEST_CASE("Conversions") {
        fractions::Fraction<int64_t> f(7, 3);

        // to_int (truncation)
        CHECK_EQ(f.to_int(), 2);

        // floor
        CHECK_EQ(f.floor(), 2);

        // ceil
        CHECK_EQ(f.ceil(), 3);

        // round
        CHECK_EQ(f.round(), 2);

        fractions::Fraction<int64_t> f2(5, 2);
        CHECK_EQ(f2.round(), 2);  // 2.5 rounds to 2 (ties to even)

        fractions::Fraction<int64_t> f3(3, 2);
        CHECK_EQ(f3.round(), 2);  // 1.5 rounds to 2 (ties to even)

        // to_double
        CHECK_EQ(f.to_double(), doctest::Approx(2.3333333333333335));

        // to_float
        CHECK_EQ(f.to_float(), doctest::Approx(2.3333333f));
    }

    TEST_CASE("String Representation") {
        fractions::Fraction<int64_t> f1(3, 4);
        CHECK_EQ(f1.to_string(), "3/4");

        fractions::Fraction<int64_t> f2(5, 1);
        CHECK_EQ(f2.to_string(), "5");

        fractions::Fraction<int64_t> f3(-3, 4);
        CHECK_EQ(f3.to_string(), "-3/4");
    }

    TEST_CASE("Negation and Absolute Value") {
        fractions::Fraction<int64_t> f1(3, 4);
        fractions::Fraction<int64_t> f2(-3, 4);

        CHECK_EQ((-f1).numerator(), -3);
        CHECK_EQ((-f1).denominator(), 4);

        CHECK_EQ((-f2).numerator(), 3);
        CHECK_EQ((-f2).denominator(), 4);

        CHECK_EQ(f1.abs().numerator(), 3);
        CHECK_EQ(f2.abs().numerator(), 3);
    }

    TEST_CASE("Power Operation") {
        fractions::Fraction<int64_t> f(2, 3);

        auto p1 = f.pow(2);
        CHECK_EQ(p1.numerator(), 4);
        CHECK_EQ(p1.denominator(), 9);

        auto p2 = f.pow(3);
        CHECK_EQ(p2.numerator(), 8);
        CHECK_EQ(p2.denominator(), 27);

        auto p3 = f.pow(-1);
        CHECK_EQ(p3.numerator(), 3);
        CHECK_EQ(p3.denominator(), 2);

        auto p4 = f.pow(-2);
        CHECK_EQ(p4.numerator(), 9);
        CHECK_EQ(p4.denominator(), 4);
    }

    TEST_CASE("Floor Division and Modulo") {
        fractions::Fraction<int64_t> f1(7, 3);
        fractions::Fraction<int64_t> f2(2, 3);

        auto floor_div = f1.floor_div(f2);
        CHECK_EQ(floor_div, 3);

        auto mod = f1 % f2;
        CHECK_EQ(mod.numerator(), 1);
        CHECK_EQ(mod.denominator(), 3);
    }

    TEST_CASE("Is Integer") {
        fractions::Fraction<int64_t> f1(4, 2);
        fractions::Fraction<int64_t> f2(3, 2);

        CHECK_EQ(f1.is_integer(), true);
        CHECK_EQ(f2.is_integer(), false);
    }

    TEST_CASE("As Integer Ratio") {
        fractions::Fraction<int64_t> f(3, 4);
        auto ratio = f.as_integer_ratio();

        CHECK_EQ(ratio.first, 3);
        CHECK_EQ(ratio.second, 4);
    }

    TEST_CASE("Hash Function") {
        fractions::Fraction<int64_t> f1(1, 2);
        fractions::Fraction<int64_t> f2(2, 4);  // Same value
        fractions::Fraction<int64_t> f3(1, 3);  // Different value

        CHECK_EQ(f1.hash(), f2.hash());  // Same values should have same hash
        CHECK_NE(f1.hash(), f3.hash());  // Different values should (likely) have different hashes
    }

    TEST_CASE("Edge Cases") {
        // Zero
        fractions::Fraction<int64_t> zero(0, 1);
        CHECK_EQ(zero.numerator(), 0);
        CHECK_EQ(zero.denominator(), 1);

        // One
        fractions::Fraction<int64_t> one(1, 1);
        CHECK_EQ(one.numerator(), 1);
        CHECK_EQ(one.denominator(), 1);

        // Negative
        fractions::Fraction<int64_t> neg(-5, 3);
        CHECK_EQ(neg.numerator(), -5);
        CHECK_EQ(neg.denominator(), 3);

        // Large numbers
        fractions::Fraction<int64_t> large(123456789, 987654321);
        CHECK_EQ(large.numerator(), 13717421);
        CHECK_EQ(large.denominator(), 109739369);
    }

    TEST_CASE("Complex Arithmetic Chain") {
        fractions::Fraction<int64_t> a(1, 2);
        fractions::Fraction<int64_t> b(1, 3);
        fractions::Fraction<int64_t> c(1, 4);

        // (1/2 + 1/3) * (1/4 - 1/6)
        auto result = (a + b) * (c - fractions::Fraction<int64_t>(1, 6));
        CHECK_EQ(result.numerator(), 5);
        CHECK_EQ(result.denominator(), 72);
    }

    TEST_CASE("Stream Output") {
        fractions::Fraction<int64_t> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "3/4");
    }

    TEST_CASE("Overflow Handling") {
        using T = int32_t;
        const T max_val = std::numeric_limits<T>::max();

        // Test comparison with large values
        fractions::Fraction<T> large_num(max_val, 1);
        fractions::Fraction<T> large_denom(1, max_val);
        fractions::Fraction<T> very_large(max_val, max_val - 1);

        // These should not crash
        CHECK_GT(large_num, large_denom);
        CHECK_LT(large_denom, large_num);

        // Test arithmetic with large values
        fractions::Fraction<T> f1(max_val / 2, 1);
        fractions::Fraction<T> f2(max_val / 2, 1);

        // Addition should handle overflow gracefully
        auto sum = f1 + f2;
        CHECK_LT(sum.numerator(), max_val);  // Should be normalized/approximated

        // Multiplication with large values
        fractions::Fraction<T> f3(max_val / 1000, max_val / 1000);
        auto product = f3 * f3;
        CHECK_LE(product.numerator(), max_val);
        CHECK_LE(product.denominator(), max_val);

        // Division by large values
        fractions::Fraction<T> f4(max_val, 1);
        fractions::Fraction<T> f5(1, max_val);
        auto division = f4 / f5;
        CHECK_LE(division.numerator(), max_val);
        CHECK_LE(division.denominator(), max_val);

        // Test floor_div with large values
        fractions::Fraction<T> f6(max_val, 1);
        fractions::Fraction<T> f7(2, 1);
        auto floor_result = f6.floor_div(f7);
        CHECK_GT(floor_result, 0);

        // Test modulo with large values
        fractions::Fraction<T> f8(max_val - 1, 1);
        fractions::Fraction<T> f9(max_val / 2, 1);
        auto mod_result = f8 % f9;
        CHECK_LT(mod_result.numerator(), f9.numerator());
    }
}