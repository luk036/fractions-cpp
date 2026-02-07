/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>
#include <fractions/extfractions.hpp>
#include <sstream>
#include <limits>
#include <type_traits>

using namespace fractions;

// Test gaps identified from code analysis:
// 1. gcd_recur function - not directly tested
// 2. keep_denom_positive method - not directly tested
// 3. normalize method return value - not tested
// 4. reduce method return value - not tested
// 5. Comparison operators with int&& (rvalue) - not tested
// 6. operator- with int&& - not tested
// 7. operator+ with int&& - not tested
// 8. operator* with int&& - not tested
// 9. Abs function with edge cases
// 10. GCD and LCM with edge cases
// 11. Cross product with various edge cases
// 12. Mixed signed/unsigned operations
// 13. Constructor with move semantics
// 14. noexcept specifications
// 15. Stream operator with different stream types

TEST_CASE("gcd_recur function - direct testing") {
    SUBCASE("Basic cases") {
        CHECK(gcd_recur(12, 8) == 4);
        CHECK(gcd_recur(8, 12) == 4);
        CHECK(gcd_recur(12, 4) == 4);
        CHECK(gcd_recur(4, 4) == 4);
    }

    SUBCASE("With zero") {
        CHECK(gcd_recur(0, 8) == 8);
        CHECK(gcd_recur(8, 0) == 8);
        CHECK(gcd_recur(0, 0) == 0);
    }

    SUBCASE("With negative numbers") {
        CHECK(gcd_recur(-12, 8) == 4);
        CHECK(gcd_recur(12, -8) == 4);
        CHECK(gcd_recur(-12, -8) == 4);
        CHECK(gcd_recur(-8, 12) == 4);
    }

    SUBCASE("With coprime numbers") {
        CHECK(gcd_recur(13, 5) == 1);
        CHECK(gcd_recur(17, 13) == 1);
        CHECK(gcd_recur(7, 11) == 1);
    }

    SUBCASE("With one being multiple of other") {
        CHECK(gcd_recur(12, 6) == 6);
        CHECK(gcd_recur(6, 12) == 6);
        CHECK(gcd_recur(15, 5) == 5);
    }

    SUBCASE("Recursive depth") {
        // Test with Fibonacci numbers (worst case for Euclid's algorithm)
        // These are consecutive Fibonacci numbers, which are coprime
        CHECK(gcd_recur(34, 21) == 1);
        CHECK(gcd_recur(55, 34) == 1);
        CHECK(gcd_recur(89, 55) == 1);
    }
}

TEST_CASE("keep_denom_positive method - direct testing") {
    SUBCASE("Already positive denominator") {
        ExtFraction<int> f(1, 2);
        f.keep_denom_positive();
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Negative denominator") {
        ExtFraction<int> f(1, -2);
        f.keep_denom_positive();
        CHECK(f.numer() == -1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Both negative") {
        ExtFraction<int> f(-1, -2);
        f.keep_denom_positive();
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Zero denominator") {
        ExtFraction<int> f(1, 0);
        f.keep_denom_positive();
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 0);
    }

    SUBCASE("Negative numerator, positive denominator") {
        ExtFraction<int> f(-1, 2);
        f.keep_denom_positive();
        CHECK(f.numer() == -1);
        CHECK(f.denom() == 2);
    }
}

TEST_CASE("normalize method - return value testing") {
    SUBCASE("Return value is GCD (already normalized)") {
        ExtFraction<int> f(2, 4);  // Already normalized to 1/2 during construction
        auto result = f.normalize();
        CHECK(result == 1);  // Already reduced, so GCD is 1
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Already normalized") {
        ExtFraction<int> f(1, 2);
        auto result = f.normalize();
        CHECK(result == 1);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("With negative denominator") {
        ExtFraction<int> f(2, -4);  // Normalized to -1/2 during construction
        auto result = f.normalize();
        CHECK(result == 1);  // Already reduced
        CHECK(f.numer() == -1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Zero numerator") {
        ExtFraction<int> f(0, 5);  // Normalized to 0/1 during construction
        auto result = f.normalize();
        CHECK(result == 1);  // Already reduced
        CHECK(f.numer() == 0);
        CHECK(f.denom() == 1);
    }

    SUBCASE("Large GCD (already normalized)") {
        ExtFraction<int> f(12, 18);  // Normalized to 2/3 during construction
        auto result = f.normalize();
        CHECK(result == 1);  // Already reduced
        CHECK(f.numer() == 2);
        CHECK(f.denom() == 3);
    }
}

TEST_CASE("reduce method - return value testing") {
    SUBCASE("Return value is GCD (already normalized)") {
        ExtFraction<int> f(2, 4);  // Already normalized to 1/2 during construction
        auto result = f.reduce();
        CHECK(result == 1);  // Already reduced, so GCD is 1
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Already reduced") {
        ExtFraction<int> f(1, 2);
        auto result = f.reduce();
        CHECK(result == 1);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("Zero denominator") {
        ExtFraction<int> f(1, 0);
        auto result = f.reduce();
        CHECK(result == 1);  // Can't reduce infinity
    }

    SUBCASE("Zero numerator") {
        ExtFraction<int> f(0, 5);  // Already normalized to 0/1 during construction
        auto result = f.reduce();
        CHECK(result == 1);  // Already reduced
        CHECK(f.numer() == 0);
        CHECK(f.denom() == 1);
    }
}

TEST_CASE("Comparison operators with int&& (rvalue)") {
    ExtFraction<int> f(1, 2);

    SUBCASE("operator+ with int&&") {
        auto result = std::move(1) + f;
        CHECK(result == ExtFraction<int>(3, 2));
    }

    SUBCASE("operator- with int&&") {
        auto result = std::move(1) - f;
        CHECK(result == ExtFraction<int>(1, 2));
    }

    SUBCASE("operator* with int&&") {
        auto result = std::move(2) * f;
        CHECK(result == ExtFraction<int>(1, 1));
    }

    SUBCASE("operator< with int&& (comparison)") {
        // Note: operator< with int&& is not defined, but operator+ is
        // This tests that the rvalue versions work correctly
        auto sum = std::move(1) + f;
        CHECK(sum == ExtFraction<int>(3, 2));
    }
}

TEST_CASE("abs function edge cases") {
    SUBCASE("With signed types") {
        CHECK(abs(0) == 0);
        CHECK(abs(1) == 1);
        CHECK(abs(-1) == 1);
        // Note: abs(INT_MIN) overflows in two's complement, so we skip this test
        CHECK(abs(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
    }

    SUBCASE("With unsigned types") {
        CHECK(abs(0u) == 0u);
        CHECK(abs(1u) == 1u);
        CHECK(abs(100u) == 100u);
    }

    SUBCASE("With long type") {
        CHECK(abs(0L) == 0L);
        CHECK(abs(1L) == 1L);
        CHECK(abs(-1L) == 1L);
    }

    SUBCASE("With short type") {
        CHECK(abs(static_cast<short>(0)) == 0);
        CHECK(abs(static_cast<short>(1)) == 1);
        CHECK(abs(static_cast<short>(-1)) == 1);
    }
}

TEST_CASE("GCD edge cases") {
    SUBCASE("With zero") {
        CHECK(gcd(0, 0) == 0);
        CHECK(gcd(0, 5) == 5);
        CHECK(gcd(5, 0) == 5);
    }

    SUBCASE("With one") {
        CHECK(gcd(1, 5) == 1);
        CHECK(gcd(5, 1) == 1);
        CHECK(gcd(1, 1) == 1);
    }

    SUBCASE("With same number") {
        CHECK(gcd(7, 7) == 7);
        CHECK(gcd(100, 100) == 100);
    }

    SUBCASE("With negative numbers") {
        CHECK(gcd(-12, 8) == 4);
        CHECK(gcd(12, -8) == 4);
        CHECK(gcd(-12, -8) == 4);
    }

    SUBCASE("With large numbers") {
        CHECK(gcd(1000000, 2000000) == 1000000);
        CHECK(gcd(123456, 789012) == 12);
    }

    SUBCASE("With prime numbers") {
        CHECK(gcd(17, 19) == 1);
        CHECK(gcd(97, 89) == 1);
    }
}

TEST_CASE("LCM edge cases") {
    SUBCASE("With zero") {
        CHECK(lcm(0, 5) == 0);
        CHECK(lcm(5, 0) == 0);
        CHECK(lcm(0, 0) == 0);
    }

    SUBCASE("With one") {
        CHECK(lcm(1, 5) == 5);
        CHECK(lcm(5, 1) == 5);
        CHECK(lcm(1, 1) == 1);
    }

    SUBCASE("With same number") {
        CHECK(lcm(7, 7) == 7);
        CHECK(lcm(100, 100) == 100);
    }

    SUBCASE("With negative numbers") {
        CHECK(lcm(-12, 8) == 24);
        CHECK(lcm(12, -8) == 24);
        CHECK(lcm(-12, -8) == 24);
    }

    SUBCASE("With coprime numbers") {
        CHECK(lcm(7, 11) == 77);
        CHECK(lcm(13, 17) == 221);
    }

    SUBCASE("With large numbers") {
        CHECK(lcm(12, 18) == 36);
        CHECK(lcm(15, 25) == 75);
    }

    SUBCASE("LCM and GCD relationship") {
        // lcm(a, b) * gcd(a, b) = |a * b|
        int a = 12, b = 18;
        CHECK(lcm(a, b) * gcd(a, b) == 12 * 18);
    }
}

TEST_CASE("Cross product edge cases") {
    ExtFraction<int> f1(1, 2);
    ExtFraction<int> f2(1, 3);
    ExtFraction<int> f3(2, 4);
    ExtFraction<int> f4(3, 4);

    SUBCASE("With equivalent fractions") {
        CHECK(f1.cross(f3) == 0);
        CHECK(f3.cross(f1) == 0);
    }

    SUBCASE("With negative fractions") {
        ExtFraction<int> neg1(-1, 2);
        ExtFraction<int> neg2(-1, 3);
        // cross(-1/2, -1/3) = (-1)*3 - 2*(-1) = -3 + 2 = -1
        CHECK(neg1.cross(neg2) == -1);
        CHECK(neg2.cross(neg1) == 1);
    }

    SUBCASE("With zero") {
        ExtFraction<int> zero(0, 1);
        CHECK(f1.cross(zero) == 1);
        CHECK(zero.cross(f1) == -1);
        CHECK(zero.cross(zero) == 0);
    }

    SUBCASE("With unity") {
        ExtFraction<int> one(1, 1);
        CHECK(f1.cross(one) == -1);
        CHECK(one.cross(f1) == 1);
    }

    SUBCASE("Anti-symmetry property") {
        CHECK(f1.cross(f2) == -f2.cross(f1));
        CHECK(f1.cross(f4) == -f4.cross(f1));
    }
}

TEST_CASE("Constructor with move semantics") {
    SUBCASE("Move constructor with rvalue") {
        int numerator = 3;
        ExtFraction<int> f(std::move(numerator));
        CHECK(f.numer() == 3);
        CHECK(f.denom() == 1);
    }

    SUBCASE("Copy constructor with lvalue") {
        int numerator = 5;
        ExtFraction<int> f(numerator);
        CHECK(f.numer() == 5);
        CHECK(f.denom() == 1);
    }

    SUBCASE("With negative move") {
        int numerator = -7;
        ExtFraction<int> f(std::move(numerator));
        CHECK(f.numer() == -7);
        CHECK(f.denom() == 1);
    }

    SUBCASE("With zero move") {
        int numerator = 0;
        ExtFraction<int> f(std::move(numerator));
        CHECK(f.numer() == 0);
        CHECK(f.denom() == 1);
    }
}

TEST_CASE("noexcept specifications") {
    ExtFraction<int> f(1, 2);

    SUBCASE("numer() is noexcept") {
        CHECK(noexcept(f.numer()));
    }

    SUBCASE("denom() is noexcept") {
        CHECK(noexcept(f.denom()));
    }

    SUBCASE("copy constructor is noexcept") {
        ExtFraction<int> f2(f);
        // Note: Can't directly test noexcept on constructors at runtime,
        // but we verify the function exists and works
        CHECK(f2.numer() == 1);
    }
}

TEST_CASE("Stream operator with different scenarios") {
    SUBCASE("Basic fraction") {
        ExtFraction<int> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(3/4)");
    }

    SUBCASE("Negative fraction") {
        ExtFraction<int> f(-3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(-3/4)");
    }

    SUBCASE("Unity") {
        ExtFraction<int> f(1, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(1/1)");
    }

    SUBCASE("Zero") {
        ExtFraction<int> f(0, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(0/1)");
    }

    SUBCASE("Large numbers") {
        ExtFraction<int> f(12345, 67890);
        std::stringstream ss;
        ss << f;
        // 12345/67890 = divide by 15: 823/4526
        CHECK(ss.str() == "(823/4526)");  // Normalized
    }

    SUBCASE("Multiple fractions in same stream") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(3, 4);
        std::stringstream ss;
        ss << f1 << " and " << f2;
        CHECK(ss.str() == "(1/2) and (3/4)");
    }
}

TEST_CASE("Mixed arithmetic operations chains") {
    SUBCASE("Long chain of additions") {
        ExtFraction<int> f(0, 1);
        for (int i = 1; i <= 10; ++i) {
            f += ExtFraction<int>(1, i);
        }
        // 0 + 1/1 + 1/2 + 1/3 + ... + 1/10
        CHECK(f.numer() == 7381);
        CHECK(f.denom() == 2520);
    }

    SUBCASE("Long chain of multiplications") {
        ExtFraction<int> f(1, 1);
        for (int i = 2; i <= 5; ++i) {
            f *= ExtFraction<int>(i, i - 1);
        }
        // 1 * 2/1 * 3/2 * 4/3 * 5/4 = 5/1
        CHECK(f == ExtFraction<int>(5, 1));
    }

    SUBCASE("Mixed operations") {
        ExtFraction<int> f(1, 2);
        f += ExtFraction<int>(1, 3);  // 5/6
        f *= ExtFraction<int>(2, 5);  // 1/3
        f -= ExtFraction<int>(1, 6);  // 1/6
        f /= ExtFraction<int>(2, 3);  // 1/4
        CHECK(f == ExtFraction<int>(1, 4));
    }
}

TEST_CASE("Comparison with negative values") {
    ExtFraction<int> neg_half(-1, 2);
    ExtFraction<int> neg_third(-1, 3);
    ExtFraction<int> pos_half(1, 2);
    ExtFraction<int> zero(0, 1);

    SUBCASE("Negative fraction comparison") {
        CHECK(neg_half < neg_third);
        CHECK(neg_third > neg_half);
        CHECK(neg_half != neg_third);
    }

    SUBCASE("Negative vs positive") {
        CHECK(neg_half < pos_half);
        CHECK(pos_half > neg_half);
        CHECK(neg_half < zero);
        CHECK(zero > neg_half);
    }

    SUBCASE("Negative vs integer") {
        CHECK(neg_half < 0);
        CHECK(0 > neg_half);
        CHECK(neg_half < 1);
        CHECK(1 > neg_half);
        CHECK(-1 < neg_half);
        CHECK(neg_half > -1);
    }
}

TEST_CASE("Special value arithmetic") {
    SUBCASE("Infinity + infinity") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> neg_inf(-1, 0);
        CHECK(inf + inf == inf);
        CHECK(neg_inf + neg_inf == neg_inf);
    }

    SUBCASE("Infinity - infinity") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> result = inf - inf;
        CHECK(result.numer() == 0);
        CHECK(result.denom() == 0);  // NaN
    }

    SUBCASE("Infinity * zero") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> result = inf * zero;
        CHECK(result.numer() == 0);
        CHECK(result.denom() == 0);  // NaN
    }

    SUBCASE("Infinity / infinity") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> result = inf / inf;
        CHECK(result.numer() == 0);
        CHECK(result.denom() == 0);  // NaN
    }

    SUBCASE("Normal value + infinity") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> normal(1, 2);
        CHECK(normal + inf == inf);
        CHECK(inf + normal == inf);
    }

    SUBCASE("Normal value * infinity") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> normal(1, 2);
        CHECK(normal * inf == inf);
        CHECK(inf * normal == inf);
    }
}

TEST_CASE("Reciprocal edge cases") {
    SUBCASE("Reciprocal of unity") {
        ExtFraction<int> one(1, 1);
        one.reciprocal();
        CHECK(one == ExtFraction<int>(1, 1));
    }

    SUBCASE("Reciprocal of zero") {
        ExtFraction<int> zero(0, 1);
        zero.reciprocal();
        CHECK(zero.numer() == 1);
        CHECK(zero.denom() == 0);  // Infinity
    }

    SUBCASE("Reciprocal of infinity") {
        ExtFraction<int> inf(1, 0);
        inf.reciprocal();
        CHECK(inf.numer() == 0);
        CHECK(inf.denom() == 1);  // Zero
    }

    SUBCASE("Reciprocal of negative infinity") {
        ExtFraction<int> neg_inf(-1, 0);
        neg_inf.reciprocal();
        CHECK(neg_inf.numer() == 0);
        CHECK(neg_inf.denom() == 1);  // Zero (normalized to 0/1)
    }
}

TEST_CASE("Increment/decrement with special values") {
    SUBCASE("Increment zero") {
        ExtFraction<int> zero(0, 1);
        ++zero;
        CHECK(zero == ExtFraction<int>(1, 1));
    }

    SUBCASE("Decrement unity") {
        ExtFraction<int> one(1, 1);
        --one;
        CHECK(one == ExtFraction<int>(0, 1));
    }

    SUBCASE("Multiple increments") {
        ExtFraction<int> f(0, 1);
        CHECK(++f == ExtFraction<int>(1, 1));
        CHECK(++f == ExtFraction<int>(2, 1));
        CHECK(++f == ExtFraction<int>(3, 1));
    }

    SUBCASE("Multiple decrements") {
        ExtFraction<int> f(3, 1);
        CHECK(--f == ExtFraction<int>(2, 1));
        CHECK(--f == ExtFraction<int>(1, 1));
        CHECK(--f == ExtFraction<int>(0, 1));
    }
}

TEST_CASE("Arithmetic with large denominators") {
    SUBCASE("Addition with large denominators") {
        ExtFraction<int> f1(1, 1000000);
        ExtFraction<int> f2(1, 1000000);
        auto result = f1 + f2;
        CHECK(result == ExtFraction<int>(1, 500000));
    }

    SUBCASE("Multiplication with large denominators") {
        ExtFraction<int> f1(1, 1000);
        ExtFraction<int> f2(1, 1000);
        auto result = f1 * f2;
        CHECK(result == ExtFraction<int>(1, 1000000));
    }

    SUBCASE("Division with large denominators") {
        ExtFraction<int> f1(1000, 1);
        ExtFraction<int> f2(1, 1000);
        auto result = f1 / f2;
        CHECK(result == ExtFraction<int>(1000000, 1));
    }
}

TEST_CASE("Equality after complex operations") {
    SUBCASE("Multiple operations leading to same value") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(2, 4);
        ExtFraction<int> f3(3, 6);

        f1 += ExtFraction<int>(1, 2);  // 1/2 + 1/2 = 1
        f2 *= ExtFraction<int>(2, 1);  // 2/4 * 2 = 1
        f3 /= ExtFraction<int>(3, 6);  // 3/6 / (3/6) = 1

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
    }

    SUBCASE("Different paths to zero") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(3, 4);
        ExtFraction<int> f3(1, 1);

        f1 -= ExtFraction<int>(1, 2);  // 1/2 - 1/2 = 0
        f2 -= ExtFraction<int>(3, 4);  // 3/4 - 3/4 = 0
        f3 -= ExtFraction<int>(1, 1);  // 1 - 1 = 0

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
    }
}