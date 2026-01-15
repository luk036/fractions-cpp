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
        CHECK(py_f1.numerator() == 3);
        CHECK(py_f1.denominator() == 4);

        fractions::Fraction<int64_t> py_f2(6, 8);  // Should normalize to 3/4
        CHECK(py_f2.numerator() == 3);
        CHECK(py_f2.denominator() == 4);

        fractions::Fraction<int64_t> py_f3(-5, 10);  // Should normalize to -1/2
        CHECK(py_f3.numerator() == -1);
        CHECK(py_f3.denominator() == 2);

        fractions::Fraction<int64_t> py_f4(5, -10);  // Should normalize to -1/2
        CHECK(py_f4.numerator() == -1);
        CHECK(py_f4.denominator() == 2);
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

        CHECK(py_sum.numerator() == 5);
        CHECK(py_sum.denominator() == 6);
        CHECK(fast_sum.numer() == 5);
        CHECK(fast_sum.denom() == 6);

        // Subtraction
        auto py_diff = py_f1 - py_f2;
        auto fast_diff = fast_f1 - fast_f2;

        CHECK(py_diff.numerator() == 1);
        CHECK(py_diff.denominator() == 6);
        CHECK(fast_diff.numer() == 1);
        CHECK(fast_diff.denom() == 6);

        // Multiplication
        auto py_prod = py_f1 * py_f2;
        auto fast_prod = fast_f1 * fast_f2;

        CHECK(py_prod.numerator() == 1);
        CHECK(py_prod.denominator() == 6);
        CHECK(fast_prod.numer() == 1);
        CHECK(fast_prod.denom() == 6);

        // Division
        auto py_div = py_f1 / py_f2;
        auto fast_div = fast_f1 / fast_f2;

        CHECK(py_div.numerator() == 3);
        CHECK(py_div.denominator() == 2);
        CHECK(fast_div.numer() == 3);
        CHECK(fast_div.denom() == 2);
    }

    TEST_CASE("Optimized Arithmetic - Knuth's Algorithm") {
        // Test the optimized GCD-based arithmetic from Knuth TAOCP

        fractions::Fraction<int64_t> a(123456789, 987654321);
        fractions::Fraction<int64_t> b(111111111, 999999999);

        // Addition should use GCD optimization
        auto sum = a + b;
        CHECK(sum.numerator() == 233196158);
        CHECK(sum.denominator() == 987654321);

        // Multiplication should use GCD optimization
        auto prod = a * b;
        CHECK(prod.numerator() == 13717421);
        CHECK(prod.denominator() == 987654321);
    }

    TEST_CASE("Comparison Operators") {
        fractions::Fraction<int64_t> f1(1, 2);
        fractions::Fraction<int64_t> f2(1, 3);
        fractions::Fraction<int64_t> f3(2, 4);  // Same as f1

        CHECK(f1 == f3);
        CHECK(f1 != f2);
        CHECK(f1 > f2);
        CHECK(f2 < f1);
        CHECK(f1 >= f3);
        CHECK(f1 >= f2);
        CHECK(f2 <= f1);
        CHECK(f3 <= f1);

        // Comparison with integers
        CHECK(f1 != 0);  // 1/2 != 0
        CHECK(f1 != 1);  // 1/2 != 1
        CHECK(f1 < 1);
        CHECK(f1 > 0);
        CHECK(f1 <= 1);
        CHECK(f1 >= 0);
    }

    TEST_CASE("Integer Operations") {
        fractions::Fraction<int64_t> f(3, 4);

        // Operations with integers
        auto sum = f + static_cast<int64_t>(1);
        CHECK(sum.numerator() == 7);
        CHECK(sum.denominator() == 4);

        auto diff = f - static_cast<int64_t>(1);
        CHECK(diff.numerator() == -1);
        CHECK(diff.denominator() == 4);

        auto prod = f * static_cast<int64_t>(2);
        CHECK(prod.numerator() == 3);
        CHECK(prod.denominator() == 2);

        auto div = f / static_cast<int64_t>(2);
        CHECK(div.numerator() == 3);
        CHECK(div.denominator() == 8);

        // Integer on left side
        auto sum2 = static_cast<int64_t>(1) + f;
        CHECK(sum2.numerator() == 7);
        CHECK(sum2.denominator() == 4);

        auto div2 = static_cast<int64_t>(1) / f;
        CHECK(div2.numerator() == 4);
        CHECK(div2.denominator() == 3);
    }

    TEST_CASE("Conversions") {
        fractions::Fraction<int64_t> f(7, 3);

        // to_int (truncation)
        CHECK(f.to_int() == 2);

        // floor
        CHECK(f.floor() == 2);

        // ceil
        CHECK(f.ceil() == 3);

        // round
        CHECK(f.round() == 2);

        fractions::Fraction<int64_t> f2(5, 2);
        CHECK(f2.round() == 2);  // 2.5 rounds to 2 (ties to even)

        fractions::Fraction<int64_t> f3(3, 2);
        CHECK(f3.round() == 2);  // 1.5 rounds to 2 (ties to even)

        // to_double
        CHECK(f.to_double() == doctest::Approx(2.3333333333333335));

        // to_float
        CHECK(f.to_float() == doctest::Approx(2.3333333f));
    }

    TEST_CASE("String Representation") {
        fractions::Fraction<int64_t> f1(3, 4);
        CHECK(f1.to_string() == "3/4");

        fractions::Fraction<int64_t> f2(5, 1);
        CHECK(f2.to_string() == "5");

        fractions::Fraction<int64_t> f3(-3, 4);
        CHECK(f3.to_string() == "-3/4");
    }

    TEST_CASE("Negation and Absolute Value") {
        fractions::Fraction<int64_t> f1(3, 4);
        fractions::Fraction<int64_t> f2(-3, 4);

        CHECK((-f1).numerator() == -3);
        CHECK((-f1).denominator() == 4);

        CHECK((-f2).numerator() == 3);
        CHECK((-f2).denominator() == 4);

        CHECK(f1.abs().numerator() == 3);
        CHECK(f2.abs().numerator() == 3);
    }

    TEST_CASE("Power Operation") {
        fractions::Fraction<int64_t> f(2, 3);

        auto p1 = f.pow(2);
        CHECK(p1.numerator() == 4);
        CHECK(p1.denominator() == 9);

        auto p2 = f.pow(3);
        CHECK(p2.numerator() == 8);
        CHECK(p2.denominator() == 27);

        auto p3 = f.pow(-1);
        CHECK(p3.numerator() == 3);
        CHECK(p3.denominator() == 2);

        auto p4 = f.pow(-2);
        CHECK(p4.numerator() == 9);
        CHECK(p4.denominator() == 4);
    }

    TEST_CASE("Floor Division and Modulo") {
        fractions::Fraction<int64_t> f1(7, 3);
        fractions::Fraction<int64_t> f2(2, 3);

        auto floor_div = f1.floor_div(f2);
        CHECK(floor_div == 3);

        auto mod = f1 % f2;
        CHECK(mod.numerator() == 1);
        CHECK(mod.denominator() == 3);
    }

    TEST_CASE("Is Integer") {
        fractions::Fraction<int64_t> f1(4, 2);
        fractions::Fraction<int64_t> f2(3, 2);

        CHECK(f1.is_integer() == true);
        CHECK(f2.is_integer() == false);
    }

    TEST_CASE("As Integer Ratio") {
        fractions::Fraction<int64_t> f(3, 4);
        auto ratio = f.as_integer_ratio();

        CHECK(ratio.first == 3);
        CHECK(ratio.second == 4);
    }

    TEST_CASE("Hash Function") {
        fractions::Fraction<int64_t> f1(1, 2);
        fractions::Fraction<int64_t> f2(2, 4);  // Same value
        fractions::Fraction<int64_t> f3(1, 3);  // Different value

        CHECK(f1.hash() == f2.hash());  // Same values should have same hash
        CHECK(f1.hash() != f3.hash());  // Different values should (likely) have different hashes
    }

    TEST_CASE("Edge Cases") {
        // Zero
        fractions::Fraction<int64_t> zero(0, 1);
        CHECK(zero.numerator() == 0);
        CHECK(zero.denominator() == 1);

        // One
        fractions::Fraction<int64_t> one(1, 1);
        CHECK(one.numerator() == 1);
        CHECK(one.denominator() == 1);

        // Negative
        fractions::Fraction<int64_t> neg(-5, 3);
        CHECK(neg.numerator() == -5);
        CHECK(neg.denominator() == 3);

        // Large numbers
        fractions::Fraction<int64_t> large(123456789, 987654321);
        CHECK(large.numerator() == 13717421);
        CHECK(large.denominator() == 109739369);
    }

    TEST_CASE("Complex Arithmetic Chain") {
        fractions::Fraction<int64_t> a(1, 2);
        fractions::Fraction<int64_t> b(1, 3);
        fractions::Fraction<int64_t> c(1, 4);

        // (1/2 + 1/3) * (1/4 - 1/6)
        auto result = (a + b) * (c - fractions::Fraction<int64_t>(1, 6));
        CHECK(result.numerator() == 5);
        CHECK(result.denominator() == 72);
    }

    TEST_CASE("Stream Output") {
        fractions::Fraction<int64_t> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "3/4");
    }
}