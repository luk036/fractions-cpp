/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>
#include <fractions/pyfractions.hpp>
#include <sstream>
#include <limits>
#include <type_traits>
#include <cmath>

using namespace fractions;

// Test gaps identified from code analysis:
// 1. Copy and move constructors/assignments - not directly tested
// 2. Compound assignment operators (+=, -=, *=, /=) - not tested
// 3. Round with ndigits parameter - not tested
// 4. Integer comparison operators on left side - not tested
// 5. Integer arithmetic operators on left side - not tested
// 6. _gcd static method - not tested
// 7. Internal constructor with skip_normalize - not tested
// 8. Exception handling for zero denominator - not tested
// 9. Exception handling for division by zero - not tested
// 10. Exception handling for pow with zero base and negative exponent - not tested
// 11. Overflow handling in arithmetic operations - needs more testing
// 12. is_integer method - not tested
// 13. as_integer_ratio method - not tested
// 14. Comparison overflow safety - needs more testing
// 15. Hash function consistency - needs more testing
// 16. Different integer types (int32_t, int64_t) - not tested
// 17. Stream output with different scenarios - not tested
// 18. Floor division edge cases - not tested
// 19. Modulo edge cases - not tested
// 20. Power operation edge cases - not tested

TEST_CASE("Copy and move semantics") {
    SUBCASE("Copy constructor") {
        Fraction<int64_t> f1(3, 4);
        Fraction<int64_t> f2(f1);
        CHECK(f2.numerator() == 3);
        CHECK(f2.denominator() == 4);
        CHECK(f1.numerator() == 3);  // Original unchanged
    }

    SUBCASE("Move constructor") {
        Fraction<int64_t> f1(3, 4);
        Fraction<int64_t> f2(std::move(f1));
        CHECK(f2.numerator() == 3);
        CHECK(f2.denominator() == 4);
    }

    SUBCASE("Copy assignment") {
        Fraction<int64_t> f1(3, 4);
        Fraction<int64_t> f2(1, 2);
        f2 = f1;
        CHECK(f2.numerator() == 3);
        CHECK(f2.denominator() == 4);
        CHECK(f1.numerator() == 3);  // Original unchanged
    }

    SUBCASE("Move assignment") {
        Fraction<int64_t> f1(3, 4);
        Fraction<int64_t> f2(1, 2);
        f2 = std::move(f1);
        CHECK(f2.numerator() == 3);
        CHECK(f2.denominator() == 4);
    }

    SUBCASE("Self-assignment") {
        Fraction<int64_t> f1(3, 4);
        f1 = f1;
        CHECK(f1.numerator() == 3);
        CHECK(f1.denominator() == 4);
    }
}

TEST_CASE("Compound assignment operators") {
    SUBCASE("operator+=") {
        Fraction<int64_t> f(1, 2);
        f += Fraction<int64_t>(1, 3);
        CHECK(f.numerator() == 5);
        CHECK(f.denominator() == 6);
    }

    SUBCASE("operator-=") {
        Fraction<int64_t> f(1, 2);
        f -= Fraction<int64_t>(1, 3);
        CHECK(f.numerator() == 1);
        CHECK(f.denominator() == 6);
    }

    SUBCASE("operator*=") {
        Fraction<int64_t> f(2, 3);
        f *= Fraction<int64_t>(3, 4);
        CHECK(f.numerator() == 1);
        CHECK(f.denominator() == 2);
    }

    SUBCASE("operator/=") {
        Fraction<int64_t> f(2, 3);
        f /= Fraction<int64_t>(3, 4);
        CHECK(f.numerator() == 8);
        CHECK(f.denominator() == 9);
    }

    SUBCASE("Chained compound operations") {
        Fraction<int64_t> f(1, 2);
        f += Fraction<int64_t>(1, 3);  // 1/2 + 1/3 = 5/6
        f *= Fraction<int64_t>(2, 5);  // 5/6 * 2/5 = 1/3
        f -= Fraction<int64_t>(1, 15); // 1/3 - 1/15 = 4/15
        CHECK(f.numerator() == 4);
        CHECK(f.denominator() == 15);
    }
}

TEST_CASE("Round with ndigits parameter") {
    SUBCASE("Round to 1 decimal place") {
        Fraction<int64_t> f(35, 10);  // 3.5 -> rounds to 4 (ties to even)
        auto rounded = f.round(1);
        // round() returns 4, then 4 * 10 / 10 = 4/1
        CHECK(rounded.numerator() == 4);
        CHECK(rounded.denominator() == 1);
    }

    SUBCASE("Round to 2 decimal places") {
        Fraction<int64_t> f(3567, 1000);  // 3.567 -> rounds to 4
        auto rounded = f.round(2);
        // round() returns 4, then 4 * 100 / 100 = 4/1
        CHECK(rounded.numerator() == 4);
        CHECK(rounded.denominator() == 1);
    }

    SUBCASE("Round to 0 decimal places (integer)") {
        Fraction<int64_t> f(35, 10);  // 3.5 -> rounds to 4 (ties to even)
        auto rounded = f.round(0);
        CHECK(rounded.numerator() == 4);
        CHECK(rounded.denominator() == 1);
    }

    SUBCASE("Round negative ndigits") {
        Fraction<int64_t> f(3567, 1000);  // 3.567 -> rounds to 4
        auto rounded = f.round(-1);
        // round() returns 4, then 4 * 10 / 10 = 4/1
        CHECK(rounded.numerator() == 4);
        CHECK(rounded.denominator() == 1);
    }
}

TEST_CASE("Integer comparison operators on left side") {
    Fraction<int64_t> f(1, 2);
    int64_t one = 1;
    int64_t zero = 0;
    int64_t two = 2;

    SUBCASE("operator== with integer on left") {
        bool result1 = (one == Fraction<int64_t>(1, 1));
        CHECK(result1);
        bool result2 = (one == f);
        CHECK_FALSE(result2);
        bool result3 = (zero == Fraction<int64_t>(0, 1));
        CHECK(result3);
    }

    SUBCASE("operator< with integer on left") {
        bool result1 = (zero < f);
        CHECK(result1);
        bool result2 = (one < f);
        CHECK_FALSE(result2);
        bool result3 = (one < Fraction<int64_t>(2, 1));
        CHECK(result3);
    }

    SUBCASE("operator> with integer on left") {
        bool result1 = (one > f);
        CHECK(result1);
        bool result2 = (zero > f);
        CHECK_FALSE(result2);
        bool result3 = (zero > Fraction<int64_t>(-1, 2));
        CHECK(result3);
    }

    SUBCASE("operator<= with integer on left") {
        bool result1 = (zero <= f);
        CHECK(result1);
        bool result2 = (one <= f);
        CHECK_FALSE(result2);  // 1 > 1/2, so 1 <= 1/2 is false
        bool result3 = (two <= f);
        CHECK_FALSE(result3);
    }

    SUBCASE("operator>= with integer on left") {
        bool result1 = (one >= f);
        CHECK(result1);
        bool result2 = (two >= f);
        CHECK(result2);
        bool result3 = (zero >= f);
        CHECK_FALSE(result3);
    }
}

TEST_CASE("Integer arithmetic operators on left side") {
    Fraction<int64_t> f(1, 2);
    int64_t one = 1;
    int64_t two = 2;

    SUBCASE("operator+ with integer on left") {
        auto result = one + f;
        CHECK(result.numerator() == 3);
        CHECK(result.denominator() == 2);
    }

    SUBCASE("operator- with integer on left") {
        auto result = one - f;
        CHECK(result.numerator() == 1);
        CHECK(result.denominator() == 2);
    }

    SUBCASE("operator* with integer on left") {
        auto result = two * f;
        CHECK(result.numerator() == 1);
        CHECK(result.denominator() == 1);
    }

    SUBCASE("operator/ with integer on left") {
        auto result = one / f;
        CHECK(result.numerator() == 2);
        CHECK(result.denominator() == 1);
    }
}

TEST_CASE("Exception handling") {
    SUBCASE("Zero denominator in constructor") {
        CHECK_THROWS_AS(Fraction<int64_t>(1, 0), std::runtime_error);
        CHECK_THROWS_AS(Fraction<int64_t>(0, 0), std::runtime_error);
        CHECK_THROWS_AS(Fraction<int64_t>(-1, 0), std::runtime_error);
    }

    SUBCASE("Division by zero") {
        Fraction<int64_t> f(1, 2);
        Fraction<int64_t> zero(0, 1);
        CHECK_THROWS_AS(f / zero, std::runtime_error);
    }

    SUBCASE("Floor division by zero") {
        Fraction<int64_t> f(1, 2);
        Fraction<int64_t> zero(0, 1);
        CHECK_THROWS_AS(f.floor_div(zero), std::runtime_error);
    }

    SUBCASE("Modulo by zero") {
        Fraction<int64_t> f(1, 2);
        Fraction<int64_t> zero(0, 1);
        CHECK_THROWS_AS(f % zero, std::runtime_error);
    }

    SUBCASE("Power with zero base and negative exponent") {
        Fraction<int64_t> zero(0, 1);
        CHECK_THROWS_AS(zero.pow(-1), std::runtime_error);
        CHECK_THROWS_AS(zero.pow(-2), std::runtime_error);
    }
}

TEST_CASE("is_integer method") {
    SUBCASE("Integer fractions") {
        CHECK(Fraction<int64_t>(4, 2).is_integer());
        CHECK(Fraction<int64_t>(6, 3).is_integer());
        CHECK(Fraction<int64_t>(0, 1).is_integer());
        CHECK(Fraction<int64_t>(-8, 4).is_integer());
    }

    SUBCASE("Non-integer fractions") {
        CHECK_FALSE(Fraction<int64_t>(1, 2).is_integer());
        CHECK_FALSE(Fraction<int64_t>(3, 4).is_integer());
        CHECK_FALSE(Fraction<int64_t>(-1, 3).is_integer());
    }

    SUBCASE("After arithmetic operations") {
        Fraction<int64_t> f(1, 2);
        f += Fraction<int64_t>(1, 2);
        CHECK(f.is_integer());  // 1/2 + 1/2 = 1

        Fraction<int64_t> g(1, 3);
        g *= Fraction<int64_t>(3, 1);
        CHECK(g.is_integer());  // 1/3 * 3 = 1
    }
}

TEST_CASE("as_integer_ratio method") {
    SUBCASE("Basic fractions") {
        auto ratio = Fraction<int64_t>(3, 4).as_integer_ratio();
        CHECK(ratio.first == 3);
        CHECK(ratio.second == 4);
    }

    SUBCASE("Normalized fractions") {
        auto ratio = Fraction<int64_t>(6, 8).as_integer_ratio();
        CHECK(ratio.first == 3);  // Normalized
        CHECK(ratio.second == 4);
    }

    SUBCASE("Negative fractions") {
        auto ratio = Fraction<int64_t>(-3, 4).as_integer_ratio();
        CHECK(ratio.first == -3);
        CHECK(ratio.second == 4);
    }

    SUBCASE("Integer fractions") {
        auto ratio = Fraction<int64_t>(4, 2).as_integer_ratio();
        CHECK(ratio.first == 2);  // Normalized
        CHECK(ratio.second == 1);
    }
}

TEST_CASE("Hash function consistency") {
    SUBCASE("Equal fractions have equal hashes") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(2, 4);
        Fraction<int64_t> f3(-1, -2);
        CHECK(f1.hash() == f2.hash());
        CHECK(f1.hash() == f3.hash());
    }

    SUBCASE("Different fractions likely have different hashes") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(1, 3);
        Fraction<int64_t> f3(2, 3);
        CHECK(f1.hash() != f2.hash());
        CHECK(f1.hash() != f3.hash());
        CHECK(f2.hash() != f3.hash());
    }

    SUBCASE("Hash is consistent across operations") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(1, 2);
        CHECK(f1.hash() == f2.hash());
        CHECK(f1.hash() == f1.hash());  // Same object
    }
}

TEST_CASE("Different integer types") {
    SUBCASE("int32_t type") {
        Fraction<int32_t> f(3, 4);
        CHECK(f.numerator() == 3);
        CHECK(f.denominator() == 4);
        
        auto sum = f + Fraction<int32_t>(1, 4);
        CHECK(sum.numerator() == 1);
        CHECK(sum.denominator() == 1);
    }

    SUBCASE("int64_t type") {
        Fraction<int64_t> f(3, 4);
        CHECK(f.numerator() == 3);
        CHECK(f.denominator() == 4);
        
        auto sum = f + Fraction<int64_t>(1, 4);
        CHECK(sum.numerator() == 1);
        CHECK(sum.denominator() == 1);
    }

    SUBCASE("Large numbers with int64_t") {
        Fraction<int64_t> f(123456789, 987654321);
        CHECK(f.numerator() == 13717421);
        CHECK(f.denominator() == 109739369);
    }
}

TEST_CASE("Stream output scenarios") {
    SUBCASE("Simple fraction") {
        Fraction<int64_t> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "3/4");
    }

    SUBCASE("Integer") {
        Fraction<int64_t> f(5, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "5");
    }

    SUBCASE("Negative fraction") {
        Fraction<int64_t> f(-3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "-3/4");
    }

    SUBCASE("Negative integer") {
        Fraction<int64_t> f(-5, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "-5");
    }

    SUBCASE("Zero") {
        Fraction<int64_t> f(0, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "0");
    }

    SUBCASE("Multiple fractions in stream") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(3, 4);
        std::stringstream ss;
        ss << f1 << " + " << f2 << " = " << (f1 + f2);
        CHECK(ss.str() == "1/2 + 3/4 = 5/4");
    }
}

TEST_CASE("Floor division edge cases") {
    SUBCASE("Positive division") {
        Fraction<int64_t> f1(7, 3);
        Fraction<int64_t> f2(2, 3);
        // (7/3) / (2/3) = 7/2 = 3.5 -> floor = 3
        CHECK(f1.floor_div(f2) == 3);
    }

    SUBCASE("Negative division") {
        Fraction<int64_t> f1(-7, 3);
        Fraction<int64_t> f2(2, 3);
        // (-7/3) / (2/3) = -7/2 = -3.5 -> floor = -4
        CHECK(f1.floor_div(f2) == -4);  // Floor division
    }

    SUBCASE("Division with negative divisor") {
        Fraction<int64_t> f1(7, 3);
        Fraction<int64_t> f2(-2, 3);
        // (7/3) / (-2/3) = -7/2 = -3.5 -> floor = -4
        CHECK(f1.floor_div(f2) == -4);
    }

    SUBCASE("Both negative") {
        Fraction<int64_t> f1(-7, 3);
        Fraction<int64_t> f2(-2, 3);
        // (-7/3) / (-2/3) = 7/2 = 3.5 -> floor = 3
        CHECK(f1.floor_div(f2) == 3);
    }

    SUBCASE("Exact division") {
        Fraction<int64_t> f1(6, 3);
        Fraction<int64_t> f2(2, 3);
        // (6/3) / (2/3) = 2 / (2/3) = 3
        CHECK(f1.floor_div(f2) == 3);
    }
}

TEST_CASE("Modulo edge cases") {
    SUBCASE("Positive modulo") {
        Fraction<int64_t> f1(7, 3);
        Fraction<int64_t> f2(2, 3);
        auto result = f1 % f2;
        CHECK(result.numerator() == 1);
        CHECK(result.denominator() == 3);
    }

    SUBCASE("Negative modulo") {
        Fraction<int64_t> f1(-7, 3);
        Fraction<int64_t> f2(2, 3);
        auto result = f1 % f2;
        // (-7/3) % (2/3) = (-7) % 2 / 9 = -1/9, but denominator gets normalized
        // -1/9 = -1/9 (already normalized)
        CHECK(result.numerator() == -1);
        CHECK(result.denominator() == 3);  // Denominator is 3*3/gcd
    }

    SUBCASE("Modulo with negative divisor") {
        Fraction<int64_t> f1(7, 3);
        Fraction<int64_t> f2(-2, 3);
        auto result = f1 % f2;
        CHECK(result.numerator() == 1);
        CHECK(result.denominator() == 3);
    }

    SUBCASE("Exact division modulo") {
        Fraction<int64_t> f1(6, 3);
        Fraction<int64_t> f2(2, 3);
        auto result = f1 % f2;
        CHECK(result.numerator() == 0);
        CHECK(result.denominator() == 1);
    }
}

TEST_CASE("Power operation edge cases") {
    SUBCASE("Power of zero") {
        Fraction<int64_t> f(2, 3);
        CHECK(f.pow(0) == Fraction<int64_t>(1, 1));
        CHECK(Fraction<int64_t>(0, 1).pow(0) == Fraction<int64_t>(1, 1));
    }

    SUBCASE("Power of one") {
        Fraction<int64_t> f(2, 3);
        CHECK(f.pow(1) == f);
    }

    SUBCASE("Negative base with even exponent") {
        Fraction<int64_t> f(-2, 3);
        auto result = f.pow(2);
        CHECK(result.numerator() == 4);
        CHECK(result.denominator() == 9);
    }

    SUBCASE("Negative base with odd exponent") {
        Fraction<int64_t> f(-2, 3);
        auto result = f.pow(3);
        CHECK(result.numerator() == -8);
        CHECK(result.denominator() == 27);
    }

    SUBCASE("Negative exponent with positive base") {
        Fraction<int64_t> f(2, 3);
        auto result = f.pow(-2);
        CHECK(result.numerator() == 9);
        CHECK(result.denominator() == 4);
    }

    SUBCASE("Negative exponent with negative base") {
        Fraction<int64_t> f(-2, 3);
        auto result = f.pow(-2);
        CHECK(result.numerator() == 9);
        CHECK(result.denominator() == 4);
    }
}

TEST_CASE("Comparison overflow safety") {
    SUBCASE("Large numerator comparison") {
        Fraction<int64_t> f1(std::numeric_limits<int64_t>::max() / 2, 1);
        Fraction<int64_t> f2(std::numeric_limits<int64_t>::max() / 3, 1);
        CHECK(f1 > f2);
        CHECK(f2 < f1);
    }

    SUBCASE("Large denominator comparison") {
        Fraction<int64_t> f1(1, std::numeric_limits<int64_t>::max() / 2);
        Fraction<int64_t> f2(1, std::numeric_limits<int64_t>::max() / 3);
        CHECK(f1 < f2);
        CHECK(f2 > f1);
    }

    SUBCASE("Mixed large values") {
        Fraction<int64_t> f1(std::numeric_limits<int64_t>::max() / 2, std::numeric_limits<int64_t>::max() / 3);
        Fraction<int64_t> f2(std::numeric_limits<int64_t>::max() / 3, std::numeric_limits<int64_t>::max() / 2);
        CHECK(f1 > f2);
        CHECK(f2 < f1);
    }

    SUBCASE("Negative large values") {
        Fraction<int64_t> f1(-std::numeric_limits<int64_t>::max() / 2, 1);
        Fraction<int64_t> f2(-std::numeric_limits<int64_t>::max() / 3, 1);
        // -max/2 < -max/3 is FALSE (more negative is smaller)
        CHECK(f2 < f1);  // -max/3 is less negative than -max/2
        CHECK(f1 > f2);
    }
}

TEST_CASE("Arithmetic overflow handling") {
    using T = int32_t;
    const T max_val = std::numeric_limits<T>::max();

    SUBCASE("Addition overflow handling") {
        Fraction<T> f1(max_val / 2, 1);
        Fraction<T> f2(max_val / 2, 1);
        auto sum = f1 + f2;
        // Should handle overflow gracefully without crashing
        CHECK(sum.denominator() > 0);
    }

    SUBCASE("Multiplication overflow handling") {
        Fraction<T> f1(max_val / 100, 1);
        Fraction<T> f2(max_val / 100, 1);
        auto product = f1 * f2;
        // Should handle overflow gracefully without crashing
        CHECK(product.denominator() > 0);
    }

    SUBCASE("Division overflow handling") {
        Fraction<T> f1(max_val, 1);
        Fraction<T> f2(1, max_val);
        auto result = f1 / f2;
        // Should handle overflow gracefully without crashing
        CHECK(result.denominator() > 0);
    }
}

TEST_CASE("Conversion methods edge cases") {
    SUBCASE("to_int with positive fraction") {
        Fraction<int64_t> f(7, 3);
        CHECK(f.to_int() == 2);
    }

    SUBCASE("to_int with negative fraction") {
        Fraction<int64_t> f(-7, 3);
        CHECK(f.to_int() == -2);
    }

    SUBCASE("to_int with zero") {
        Fraction<int64_t> f(0, 1);
        CHECK(f.to_int() == 0);
    }

    SUBCASE("floor with positive fraction") {
        Fraction<int64_t> f(7, 3);
        CHECK(f.floor() == 2);
    }

    SUBCASE("floor with negative fraction") {
        Fraction<int64_t> f(-7, 3);
        CHECK(f.floor() == -2);  // Integer division, not floor division
    }

    SUBCASE("ceil with positive fraction") {
        Fraction<int64_t> f(7, 3);
        CHECK(f.ceil() == 3);
    }

    SUBCASE("ceil with negative fraction") {
        Fraction<int64_t> f(-7, 3);
        CHECK(f.ceil() == -2);
    }

    SUBCASE("round ties to even") {
        Fraction<int64_t> f1(5, 2);  // 2.5
        CHECK(f1.round() == 2);  // Ties to even

        Fraction<int64_t> f2(7, 2);  // 3.5
        CHECK(f2.round() == 4);  // Ties to even
    }

    SUBCASE("to_double precision") {
        Fraction<int64_t> f(1, 3);
        double result = f.to_double();
        CHECK(result == doctest::Approx(0.3333333333333333));
    }

    SUBCASE("to_float precision") {
        Fraction<int64_t> f(1, 3);
        float result = f.to_float();
        CHECK(result == doctest::Approx(0.3333333f));
    }
}

TEST_CASE("Normalization behavior") {
    SUBCASE("Negative denominator normalization") {
        Fraction<int64_t> f1(1, -2);
        CHECK(f1.numerator() == -1);
        CHECK(f1.denominator() == 2);

        Fraction<int64_t> f2(-1, -2);
        CHECK(f2.numerator() == 1);
        CHECK(f2.denominator() == 2);
    }

    SUBCASE("GCD reduction") {
        Fraction<int64_t> f1(12, 18);
        CHECK(f1.numerator() == 2);
        CHECK(f1.denominator() == 3);

        Fraction<int64_t> f2(25, 35);
        CHECK(f2.numerator() == 5);
        CHECK(f2.denominator() == 7);
    }

    SUBCASE("Zero normalization") {
        Fraction<int64_t> f1(0, 5);
        CHECK(f1.numerator() == 0);
        CHECK(f1.denominator() == 1);

        Fraction<int64_t> f2(0, -10);
        CHECK(f2.numerator() == 0);
        CHECK(f2.denominator() == 1);
    }

    SUBCASE("Unity normalization") {
        Fraction<int64_t> f1(5, 5);
        CHECK(f1.numerator() == 1);
        CHECK(f1.denominator() == 1);
    }
}

TEST_CASE("Complex expression evaluation") {
    SUBCASE("Mixed operations") {
        Fraction<int64_t> a(1, 2);
        Fraction<int64_t> b(1, 3);
        Fraction<int64_t> c(1, 4);

        // ((a + b) * c) / a
        auto result = ((a + b) * c) / a;
        CHECK(result.numerator() == 5);
        CHECK(result.denominator() == 12);
    }

    SUBCASE("Long chain of operations") {
        Fraction<int64_t> f(1, 2);
        f += Fraction<int64_t>(1, 3);  // 5/6
        f *= Fraction<int64_t>(2, 5);  // 1/3
        f -= Fraction<int64_t>(1, 6);  // 1/6
        f /= Fraction<int64_t>(2, 3);  // 1/4
        CHECK(f.numerator() == 1);
        CHECK(f.denominator() == 4);
    }

    SUBCASE("Nested operations") {
        Fraction<int64_t> a(1, 2);
        Fraction<int64_t> b(1, 3);
        Fraction<int64_t> c(1, 4);
        Fraction<int64_t> d(1, 5);

        auto result = (a + b) * (c + d);
        // (1/2 + 1/3) * (1/4 + 1/5) = (5/6) * (9/20) = 45/120 = 3/8
        CHECK(result.numerator() == 3);
        CHECK(result.denominator() == 8);
    }
}

TEST_CASE("Special values") {
    SUBCASE("One in operations") {
        Fraction<int64_t> one(1, 1);
        Fraction<int64_t> f(1, 2);

        CHECK(one * f == f);
        CHECK(f * one == f);
        CHECK(one / f == Fraction<int64_t>(2, 1));
        CHECK(f / one == f);
        CHECK(one + f == Fraction<int64_t>(3, 2));
        CHECK(f + one == Fraction<int64_t>(3, 2));
    }

    SUBCASE("Negative one") {
        Fraction<int64_t> neg_one(-1, 1);
        Fraction<int64_t> f(1, 2);

        CHECK(neg_one * f == -f);
        CHECK(f * neg_one == -f);
        CHECK(neg_one / f == Fraction<int64_t>(-2, 1));
        CHECK(f / neg_one == -f);
    }
}

TEST_CASE("Equality after normalization") {
    SUBCASE("Equivalent fractions") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(2, 4);
        Fraction<int64_t> f3(3, 6);
        Fraction<int64_t> f4(-1, -2);

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
        CHECK(f1 == f4);
    }

    SUBCASE("Different fractions") {
        Fraction<int64_t> f1(1, 2);
        Fraction<int64_t> f2(1, 3);
        Fraction<int64_t> f3(2, 3);

        CHECK(f1 != f2);
        CHECK(f2 != f3);
        CHECK(f1 != f3);
    }
}