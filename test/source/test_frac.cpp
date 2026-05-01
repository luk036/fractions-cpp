/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <fractions/extfractions.hpp>
#include <ostream>

using namespace fractions;

// auto operator<=>(const cpp_int& a, const cpp_int& b) -> std::strong_ordering
// {
//     if (operator<(a, b)) return std::strong_ordering::less;
//     if (operator>(a, b)) return std::strong_ordering::greater;
//     return std::strong_ordering::equivalent;
// }

TEST_CASE("ExtFraction<int> add") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_EQ(a + b, ExtFraction<int>{19, 12});
}

TEST_CASE("ExtFraction<int> subtract") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_EQ(a - b, ExtFraction<int>{-1, 12});
}

TEST_CASE("ExtFraction<int> multiply") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_EQ(a * b, ExtFraction<int>{15, 24});
}

TEST_CASE("ExtFraction<int> divide") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_EQ(a / b, ExtFraction<int>{9, 10});
}

TEST_CASE("ExtFraction<int> neg") {
    const auto a = ExtFraction<int>{3, 4};
    CHECK_EQ(-a, ExtFraction<int>{-3, 4});
}

TEST_CASE("ExtFraction<int> abs") {
    const auto a = ExtFraction<int>{-3, 4};
    CHECK_EQ(abs(a), ExtFraction<int>{3, 4});
}

TEST_CASE("ExtFraction<int> less than") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_LT(a, b);
}

TEST_CASE("ExtFraction<int> greater than") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{5, 6};
    CHECK_FALSE(a > b);
}

TEST_CASE("ExtFraction<int>::operator==") {
    const auto a = ExtFraction<int>{3, 4};
    const auto b = ExtFraction<int>{6, 8};
    CHECK_EQ(a, b);
}

TEST_CASE("ExtFraction<int>::operator== with zero denominator") {
    const auto a = ExtFraction<int>{0, 1};
    const auto b = ExtFraction<int>{0, 2};
    CHECK_EQ(a, b);
}

TEST_CASE("ExtFraction<int>::operator== with zero numerator and denominator") {
    const auto a = ExtFraction<int>{0, 1};
    CHECK_EQ(a, a);
}

TEST_CASE("ExtFraction<int>::operator== with zero numerator and non-zero denominator") {
    const auto a = ExtFraction<int>{0, 1};
    const auto b = ExtFraction<int>{0, 2};
    CHECK_EQ(a, b);
}

TEST_CASE("ExtFraction<int>::operator== with non-zero numerator and zero denominator") {
    const auto a = ExtFraction<int>{1, 0};
    CHECK_EQ(a, a);
}

TEST_CASE("ExtFraction<int>::operator== with non-zero numerator and non-zero denominator") {
    const auto a = ExtFraction<int>{1, 2};
    CHECK_EQ(a, a);
}

TEST_CASE("ExtFraction<int>::operator<=") {
    const auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    CHECK_LE(a, b);
}

TEST_CASE("ExtFraction<int>::operator>=") {
    const auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    CHECK_GE(a, b);
}

TEST_CASE("ExtFraction<int>::operator<= with zero denominator and non-zero numerator") {
    const auto a = ExtFraction<int>{1, 0};
    CHECK_LE(a, a);
}

TEST_CASE("ExtFraction<int>::operator>= with zero denominator and non-zero numerator") {
    const auto a = ExtFraction<int>{1, 0};
    CHECK_GE(a, a);
}

TEST_CASE("ExtFraction<int>::operator+=") {
    auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    a += b;
    CHECK_EQ(a, ExtFraction<int>{1, 1});
}

TEST_CASE("ExtFraction<int>::operator-=") {
    auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    a -= b;
    CHECK_EQ(a, ExtFraction<int>{0, 1});
}

TEST_CASE("ExtFraction<int>::operator*=") {
    auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    a *= b;
    CHECK_EQ(a, ExtFraction<int>{1, 4});
}

TEST_CASE("ExtFraction<int>::operator/=") {
    auto a = ExtFraction<int>{1, 2};
    const auto b = ExtFraction<int>{2, 4};
    a /= b;
    CHECK_EQ(a, ExtFraction<int>{1, 1});
}

TEST_CASE("ExtFraction<int>::operator++") {
    auto a = ExtFraction<int>{1, 2};
    CHECK_EQ(++a, ExtFraction<int>{3, 2});
}

TEST_CASE("ExtFraction<int>::operator--") {
    auto a = ExtFraction<int>{1, 2};
    CHECK_EQ(--a, ExtFraction<int>{-1, 2});
}

TEST_CASE("ExtFraction<int>::operator++ with postfix") {
    auto a = ExtFraction<int>{1, 2};
    CHECK_EQ(a++, ExtFraction<int>{1, 2});
}

TEST_CASE("ExtFraction<int>::operator-- with postfix") {
    auto a = ExtFraction<int>{1, 2};
    CHECK_EQ(a--, ExtFraction<int>{1, 2});
}

TEST_CASE("ExtFraction<int>::operator* with zero denominator and non-zero numerator") {
    const auto a = ExtFraction<int>{1, 0};
    CHECK_EQ(a * a, a);
}

TEST_CASE("ExtFraction<int>::operator* with zero numerator and non-zero denominator") {
    const auto a = ExtFraction<int>{0, 1};
    CHECK_EQ(a * a, a);
}

TEST_CASE("ExtFraction") {
    const auto val_a = 3U;
    const auto val_b = 4U;
    const auto val_c = 5U;
    const auto val_d = 6U;

    const auto p = ExtFraction<int>{val_a, val_b};
    const auto q = ExtFraction<int>{val_c, val_d};

    CHECK_EQ(p, ExtFraction<int>(30U, 40U));
    CHECK_EQ(p + q, ExtFraction<int>(19U, 12U));
    CHECK_EQ((p - q) + q, p);
    // CHECK_NE(p, 0);
}

TEST_CASE("ExtFraction Special Cases") {
    const auto posf = ExtFraction<int>{3, 4};
    const auto inf = ExtFraction<int>{1, 0};
    const auto nan = ExtFraction<int>{0, 0};
    const auto zero = ExtFraction<int>{0, 1};

    CHECK_LT(-inf, zero);
    CHECK_LT(zero, inf);
    CHECK_LT(-inf, posf);
    CHECK_LT(posf, inf);
    CHECK_EQ(inf, inf);
    CHECK_LT(-inf, inf);
    CHECK_EQ(inf, inf * posf);
    CHECK_EQ(inf, inf * inf);
    CHECK_EQ(inf, posf / zero);
    CHECK_EQ(inf, inf / zero);
    CHECK_EQ(nan, nan);
    CHECK_EQ(nan, inf * zero);
    CHECK_EQ(nan, -inf * zero);
    CHECK_EQ(nan, inf / inf);
    CHECK_EQ(nan, nan * zero);
    CHECK_EQ(nan, nan * nan);
    CHECK_EQ(inf, inf + inf);
    CHECK_EQ(nan, inf - inf);
    CHECK_EQ(inf - posf, inf);
    CHECK_EQ(-inf + posf, -inf);
}

TEST_CASE("abs function") {
    SUBCASE("unsigned types") {
        CHECK_EQ(abs(5u), 5u);
        CHECK_EQ(abs(0u), 0u);
    }

    SUBCASE("signed types") {
        CHECK_EQ(abs(5), 5);
        CHECK_EQ(abs(-5), 5);
        CHECK_EQ(abs(0), 0);
    }
}

TEST_CASE("gcd function") {
    CHECK_EQ(gcd(12, 8), 4);
    CHECK_EQ(gcd(12, 4), 4);
    CHECK_EQ(gcd(4, 4), 4);
    CHECK_EQ(gcd(0, 8), 8);
    CHECK_EQ(gcd(8, 0), 8);
    CHECK_EQ(gcd(13, 5), 1);
    CHECK_EQ(gcd(-12, 8), 4);
    CHECK_EQ(gcd(12, -8), 4);
    CHECK_EQ(gcd(-12, -8), 4);
}

TEST_CASE("lcm function") {
    CHECK_EQ(lcm(12, 8), 24);
    CHECK_EQ(lcm(12, 4), 12);
    CHECK_EQ(lcm(4, 4), 4);
    CHECK_EQ(lcm(0, 8), 0);
    CHECK_EQ(lcm(8, 0), 0);
    CHECK_EQ(lcm(13, 5), 65);
    CHECK_EQ(lcm(-12, 8), 24);
    CHECK_EQ(lcm(12, -8), 24);
    CHECK_EQ(lcm(-12, -8), 24);
}

TEST_CASE("ExtFraction construction") {
    SUBCASE("default constructor") {
        ExtFraction<int> f;
        CHECK_EQ(f.numer(), 0);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("single argument constructor") {
        ExtFraction<int> f1(5);
        CHECK_EQ(f1.numer(), 5);
        CHECK_EQ(f1.denom(), 1);

        ExtFraction<int> f2(-3);
        CHECK_EQ(f2.numer(), -3);
        CHECK_EQ(f2.denom(), 1);
    }

    SUBCASE("two argument constructor") {
        ExtFraction<int> f1(1, 2);
        CHECK_EQ(f1.numer(), 1);
        CHECK_EQ(f1.denom(), 2);

        ExtFraction<int> f2(-1, 2);
        CHECK_EQ(f2.numer(), -1);
        CHECK_EQ(f2.denom(), 2);

        ExtFraction<int> f3(1, -2);
        CHECK_EQ(f3.numer(), -1);
        CHECK_EQ(f3.denom(), 2);

        ExtFraction<int> f4(-1, -2);
        CHECK_EQ(f4.numer(), 1);
        CHECK_EQ(f4.denom(), 2);

        ExtFraction<int> f5(2, 4);
        CHECK_EQ(f5.numer(), 1);
        CHECK_EQ(f5.denom(), 2);

        ExtFraction<int> f6(0, 5);
        CHECK_EQ(f6.numer(), 0);
        CHECK_EQ(f6.denom(), 1);
    }
}

TEST_CASE("ExtFraction comparison operators") {
    ExtFraction<int> half(1, 2);
    ExtFraction<int> quarter(1, 4);
    ExtFraction<int> two_quarters(2, 4);
    ExtFraction<int> negative_half(-1, 2);
    ExtFraction<int> one(1);

    SUBCASE("equality") {
        CHECK_EQ(half, half);
        CHECK_EQ(half, two_quarters);
        CHECK_EQ(two_quarters, half);
        CHECK_EQ(one, 1);
        CHECK_EQ(1, one);
        CHECK_FALSE(half == quarter);
        CHECK_FALSE(half == one);
        CHECK_FALSE(half == 1);
        CHECK_FALSE(1 == half);
    }

    SUBCASE("inequality") {
        CHECK_NE(half, quarter);
        CHECK_NE(quarter, half);
        CHECK_NE(half, one);
        CHECK_NE(one, half);
        // CHECK_NE(half, 1);
        // CHECK_NE(1, half);
        CHECK_FALSE(half != two_quarters);
    }

    SUBCASE("less than") {
        CHECK_LT(quarter, half);
        CHECK_LT(negative_half, quarter);
        CHECK_LT(negative_half, 0);
        CHECK_LT(0, half);
        CHECK_FALSE(half < quarter);
        CHECK_FALSE(half < half);
    }

    SUBCASE("greater than") {
        CHECK_GT(half, quarter);
        CHECK_GT(quarter, negative_half);
        CHECK_GT(half, 0);
        CHECK_GT(0, negative_half);
        CHECK_FALSE(quarter > half);
        CHECK_FALSE(half > half);
    }

    SUBCASE("less than or equal") {
        CHECK_LE(quarter, half);
        CHECK_LE(half, half);
        CHECK_LE(half, two_quarters);
        CHECK_LE(negative_half, 0);
        CHECK_LE(0, half);
        CHECK_FALSE(half <= quarter);
    }

    SUBCASE("greater than or equal") {
        CHECK_GE(half, quarter);
        CHECK_GE(half, half);
        CHECK_GE(two_quarters, half);
        CHECK_GE(half, 0);
        CHECK_GE(0, negative_half);
        CHECK_FALSE(quarter >= half);
    }
}

TEST_CASE("ExtFraction arithmetic operations") {
    ExtFraction<int> half(1, 2);
    ExtFraction<int> quarter(1, 4);
    ExtFraction<int> three_quarters(3, 4);
    ExtFraction<int> one(1);
    ExtFraction<int> two(2);
    ExtFraction<int> negative_half(-1, 2);

    SUBCASE("addition") {
        CHECK_EQ(half + quarter, three_quarters);
        CHECK_EQ(half + half, one);
        CHECK_EQ(half + 1, ExtFraction<int>(3, 2));
        CHECK_EQ(1 + half, ExtFraction<int>(3, 2));
        CHECK_EQ(half + negative_half, ExtFraction<int>(0, 1));
    }

    SUBCASE("subtraction") {
        CHECK_EQ(half - quarter, quarter);
        CHECK_EQ(three_quarters - half, quarter);
        CHECK_EQ(half - half, ExtFraction<int>(0, 1));
        CHECK_EQ(half - 1, negative_half);
        CHECK_EQ(1 - half, half);
    }

    SUBCASE("multiplication") {
        CHECK_EQ(half * half, quarter);
        CHECK_EQ(half * two, one);
        CHECK_EQ(two * half, one);
        CHECK_EQ(half * 2, one);
        CHECK_EQ(2 * half, one);
        CHECK_EQ(half * negative_half, ExtFraction<int>(-1, 4));
    }

    SUBCASE("division") {
        CHECK_EQ(half / half, one);
        CHECK_EQ(one / half, two);
        CHECK_EQ(half / 2, quarter);
        CHECK_EQ(2 / half, ExtFraction<int>(4, 1));
        CHECK_EQ(one / negative_half, ExtFraction<int>(-2, 1));
    }

    SUBCASE("negation") {
        CHECK_EQ(-half, negative_half);
        CHECK_EQ(-negative_half, half);
        CHECK_EQ(-one, ExtFraction<int>(-1, 1));
    }

    SUBCASE("compound assignment") {
        ExtFraction<int> f(1, 2);
        f += quarter;
        CHECK_EQ(f, three_quarters);

        f -= half;
        CHECK_EQ(f, quarter);

        f *= two;
        CHECK_EQ(f, half);

        f /= half;
        CHECK_EQ(f, one);

        f += 1;
        CHECK_EQ(f, two);

        f -= 1;
        CHECK_EQ(f, one);

        f *= 2;
        CHECK_EQ(f, two);

        f /= 2;
        CHECK_EQ(f, one);
    }

    SUBCASE("increment/decrement") {
        ExtFraction<int> f(3, 2);
        CHECK_EQ(++f, ExtFraction<int>(5, 2));
        CHECK_EQ(f--, ExtFraction<int>(5, 2));
        CHECK_EQ(f, ExtFraction<int>(3, 2));
        CHECK_EQ(--f, half);
        CHECK_EQ(f++, half);
        CHECK_EQ(f, ExtFraction<int>(3, 2));
    }
}

TEST_CASE("ExtFraction utility methods") {
    ExtFraction<int> half(1, 2);
    ExtFraction<int> negative_half(-1, 2);
    ExtFraction<int> two_fourths(2, 4);

    SUBCASE("reciprocal") {
        half.reciprocal();
        CHECK_EQ(half, ExtFraction<int>(2, 1));
        half.reciprocal();
        CHECK_EQ(half, ExtFraction<int>(1, 2));

        negative_half.reciprocal();
        CHECK_EQ(negative_half, ExtFraction<int>(-2, 1));
    }

    SUBCASE("cross product") {
        CHECK_EQ(half.cross(two_fourths), 0);
        CHECK_EQ(half.cross(ExtFraction<int>(3, 4)), -2);
        CHECK_EQ(half.cross(negative_half), 4);
    }

    SUBCASE("normalization") {
        ExtFraction<int> f(2, -4);
        CHECK_EQ(f.numer(), -1);
        CHECK_EQ(f.denom(), 2);

        f = ExtFraction<int>(-2, -4);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }
}

TEST_CASE("ExtFraction with different types") {
    SUBCASE("long") {
        ExtFraction<long> f(1000000000L, 2000000000L);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    // SUBCASE("unsigned") {
    //     ExtFraction<unsigned> f(3u, 6u);
    //     CHECK_EQ(f.numer(), 1);
    //     CHECK_EQ(f.denom(), 2);
    // }

    // SUBCASE("short") {
    //     ExtFraction<short> f(3, 6);
    //     CHECK_EQ(f.numer(), 1);
    //     CHECK_EQ(f.denom(), 2);
    // }
}

TEST_CASE("ExtFraction operator<") {
    const auto val_a = 3;
    const auto val_b = -4;
    const auto val_c = 5;
    const auto val_d = 6;

    const auto p = ExtFraction<int>{val_a, val_b};
    const auto q = ExtFraction<int>{val_c, val_d};

    CHECK_LT(p, q);
    // CHECK_NE(p, 0);
}
