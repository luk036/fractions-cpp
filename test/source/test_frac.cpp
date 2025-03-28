/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <fractions/fractions.hpp>
#include <ostream>

using namespace fractions;

// auto operator<=>(const cpp_int& a, const cpp_int& b) -> std::strong_ordering
// {
//     if (operator<(a, b)) return std::strong_ordering::less;
//     if (operator>(a, b)) return std::strong_ordering::greater;
//     return std::strong_ordering::equivalent;
// }

TEST_CASE("Fraction<int> add") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK_EQ(a + b, Fraction<int>{19, 12});
}

TEST_CASE("Fraction<int> subtract") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK_EQ(a - b, Fraction<int>{-1, 12});
}

TEST_CASE("Fraction<int> multiply") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK_EQ(a * b, Fraction<int>{15, 24});
}

TEST_CASE("Fraction<int> divide") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK_EQ(a / b, Fraction<int>{9, 10});
}

TEST_CASE("Fraction<int> neg") {
    const auto a = Fraction<int>{3, 4};
    CHECK_EQ(-a, Fraction<int>{-3, 4});
}

TEST_CASE("Fraction<int> abs") {
    const auto a = Fraction<int>{-3, 4};
    CHECK_EQ(abs(a), Fraction<int>{3, 4});
}

TEST_CASE("Fraction<int> less than") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK(a < b);
}

TEST_CASE("Fraction<int> greater than") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK(!(a > b));
}

TEST_CASE("Fraction<int>::operator==") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{6, 8};
    CHECK(a == b);
}

TEST_CASE("Fraction<int>::operator== with zero denominator") {
    const auto a = Fraction<int>{0, 1};
    const auto b = Fraction<int>{0, 2};
    CHECK(a == b);
}

TEST_CASE("Fraction<int>::operator== with zero numerator and denominator") {
    const auto a = Fraction<int>{0, 1};
    CHECK(a == a);
}

TEST_CASE("Fraction<int>::operator== with zero numerator and non-zero denominator") {
    const auto a = Fraction<int>{0, 1};
    const auto b = Fraction<int>{0, 2};
    CHECK(a == b);
}

TEST_CASE("Fraction<int>::operator== with non-zero numerator and zero denominator") {
    const auto a = Fraction<int>{1, 0};
    CHECK(a == a);
}

TEST_CASE("Fraction<int>::operator== with non-zero numerator and non-zero denominator") {
    const auto a = Fraction<int>{1, 2};
    CHECK(a == a);
}

TEST_CASE("Fraction<int>::operator<=") {
    const auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    CHECK(a <= b);
}

TEST_CASE("Fraction<int>::operator>=") {
    const auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    CHECK(a >= b);
}

TEST_CASE("Fraction<int>::operator<= with zero denominator and non-zero numerator") {
    const auto a = Fraction<int>{1, 0};
    CHECK(a <= a);
}

TEST_CASE("Fraction<int>::operator>= with zero denominator and non-zero numerator") {
    const auto a = Fraction<int>{1, 0};
    CHECK(a >= a);
}

TEST_CASE("Fraction<int>::operator+=") {
    auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    a += b;
    CHECK_EQ(a, Fraction<int>{1, 1});
}

TEST_CASE("Fraction<int>::operator-=") {
    auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    a -= b;
    CHECK_EQ(a, Fraction<int>{0, 1});
}

TEST_CASE("Fraction<int>::operator*=") {
    auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    a *= b;
    CHECK_EQ(a, Fraction<int>{1, 4});
}

TEST_CASE("Fraction<int>::operator/=") {
    auto a = Fraction<int>{1, 2};
    const auto b = Fraction<int>{2, 4};
    a /= b;
    CHECK_EQ(a, Fraction<int>{1, 1});
}

TEST_CASE("Fraction<int>::operator++") {
    auto a = Fraction<int>{1, 2};
    CHECK_EQ(++a, Fraction<int>{3, 2});
}

TEST_CASE("Fraction<int>::operator--") {
    auto a = Fraction<int>{1, 2};
    CHECK_EQ(--a, Fraction<int>{-1, 2});
}

TEST_CASE("Fraction<int>::operator++ with postfix") {
    auto a = Fraction<int>{1, 2};
    CHECK_EQ(a++, Fraction<int>{1, 2});
}

TEST_CASE("Fraction<int>::operator-- with postfix") {
    auto a = Fraction<int>{1, 2};
    CHECK_EQ(a--, Fraction<int>{1, 2});
}

TEST_CASE("Fraction<int>::operator* with zero denominator and non-zero numerator") {
    const auto a = Fraction<int>{1, 0};
    CHECK(a * a == a);
}

TEST_CASE("Fraction<int>::operator* with zero numerator and non-zero denominator") {
    const auto a = Fraction<int>{0, 1};
    CHECK(a * a == a);
}

TEST_CASE("Fraction") {
    const auto a = 3U;
    const auto b = 4U;
    const auto c = 5U;
    const auto d = 6U;

    const auto p = Fraction<int>{a, b};
    const auto q = Fraction<int>{c, d};

    CHECK(p == Fraction<int>(30U, 40U));
    CHECK(p + q == Fraction<int>(19U, 12U));
    CHECK((p - q) + q == p);
    // CHECK(p != 0);
}

TEST_CASE("Fraction Special Cases") {
    const auto posf = Fraction<int>{3, 4};
    const auto inf = Fraction<int>{1, 0};
    const auto nan = Fraction<int>{0, 0};
    const auto zero = Fraction<int>{0, 1};

    CHECK(-inf < zero);
    CHECK(zero < inf);
    CHECK(-inf < posf);
    CHECK(posf < inf);
    CHECK(inf == inf);
    CHECK(-inf < inf);
    CHECK(inf == inf * posf);
    CHECK(inf == inf * inf);
    CHECK(inf == posf / zero);
    CHECK(inf == inf / zero);
    CHECK(nan == nan);
    CHECK(nan == inf * zero);
    CHECK(nan == -inf * zero);
    CHECK(nan == inf / inf);
    CHECK(nan == nan * zero);
    CHECK(nan == nan * nan);
    CHECK(inf == inf + inf);
    CHECK(nan == inf - inf);
    CHECK(inf - posf == inf);
    CHECK(-inf + posf == -inf);
}


TEST_CASE("abs function") {
    SUBCASE("unsigned types") {
        CHECK(abs(5u) == 5u);
        CHECK(abs(0u) == 0u);
    }

    SUBCASE("signed types") {
        CHECK(abs(5) == 5);
        CHECK(abs(-5) == 5);
        CHECK(abs(0) == 0);
    }
}

TEST_CASE("gcd function") {
    CHECK(gcd(12, 8) == 4);
    CHECK(gcd(12, 4) == 4);
    CHECK(gcd(4, 4) == 4);
    CHECK(gcd(0, 8) == 8);
    CHECK(gcd(8, 0) == 8);
    CHECK(gcd(13, 5) == 1);
    CHECK(gcd(-12, 8) == 4);
    CHECK(gcd(12, -8) == 4);
    CHECK(gcd(-12, -8) == 4);
}

TEST_CASE("lcm function") {
    CHECK(lcm(12, 8) == 24);
    CHECK(lcm(12, 4) == 12);
    CHECK(lcm(4, 4) == 4);
    CHECK(lcm(0, 8) == 0);
    CHECK(lcm(8, 0) == 0);
    CHECK(lcm(13, 5) == 65);
    CHECK(lcm(-12, 8) == 24);
    CHECK(lcm(12, -8) == 24);
    CHECK(lcm(-12, -8) == 24);
}

TEST_CASE("Fraction construction") {
    SUBCASE("default constructor") {
        Fraction<int> f;
        CHECK(f.numer() == 0);
        CHECK(f.denom() == 1);
    }

    SUBCASE("single argument constructor") {
        Fraction<int> f1(5);
        CHECK(f1.numer() == 5);
        CHECK(f1.denom() == 1);

        Fraction<int> f2(-3);
        CHECK(f2.numer() == -3);
        CHECK(f2.denom() == 1);
    }

    SUBCASE("two argument constructor") {
        Fraction<int> f1(1, 2);
        CHECK(f1.numer() == 1);
        CHECK(f1.denom() == 2);

        Fraction<int> f2(-1, 2);
        CHECK(f2.numer() == -1);
        CHECK(f2.denom() == 2);

        Fraction<int> f3(1, -2);
        CHECK(f3.numer() == -1);
        CHECK(f3.denom() == 2);

        Fraction<int> f4(-1, -2);
        CHECK(f4.numer() == 1);
        CHECK(f4.denom() == 2);

        Fraction<int> f5(2, 4);
        CHECK(f5.numer() == 1);
        CHECK(f5.denom() == 2);

        Fraction<int> f6(0, 5);
        CHECK(f6.numer() == 0);
        CHECK(f6.denom() == 1);
    }
}

TEST_CASE("Fraction comparison operators") {
    Fraction<int> half(1, 2);
    Fraction<int> quarter(1, 4);
    Fraction<int> two_quarters(2, 4);
    Fraction<int> negative_half(-1, 2);
    Fraction<int> one(1);

    SUBCASE("equality") {
        CHECK(half == half);
        CHECK(half == two_quarters);
        CHECK(two_quarters == half);
        CHECK(one == 1);
        CHECK(1 == one);
        CHECK_FALSE(half == quarter);
        CHECK_FALSE(half == one);
        CHECK_FALSE(half == 1);
        CHECK_FALSE(1 == half);
    }

    SUBCASE("inequality") {
        CHECK(half != quarter);
        CHECK(quarter != half);
        CHECK(half != one);
        CHECK(one != half);
        CHECK(half != 1);
        CHECK(1 != half);
        CHECK_FALSE(half != two_quarters);
    }

    SUBCASE("less than") {
        CHECK(quarter < half);
        CHECK(negative_half < quarter);
        CHECK(negative_half < 0);
        CHECK(0 < half);
        CHECK_FALSE(half < quarter);
        CHECK_FALSE(half < half);
    }

    SUBCASE("greater than") {
        CHECK(half > quarter);
        CHECK(quarter > negative_half);
        CHECK(half > 0);
        CHECK(0 > negative_half);
        CHECK_FALSE(quarter > half);
        CHECK_FALSE(half > half);
    }

    SUBCASE("less than or equal") {
        CHECK(quarter <= half);
        CHECK(half <= half);
        CHECK(half <= two_quarters);
        CHECK(negative_half <= 0);
        CHECK(0 <= half);
        CHECK_FALSE(half <= quarter);
    }

    SUBCASE("greater than or equal") {
        CHECK(half >= quarter);
        CHECK(half >= half);
        CHECK(two_quarters >= half);
        CHECK(half >= 0);
        CHECK(0 >= negative_half);
        CHECK_FALSE(quarter >= half);
    }
}

TEST_CASE("Fraction arithmetic operations") {
    Fraction<int> half(1, 2);
    Fraction<int> quarter(1, 4);
    Fraction<int> three_quarters(3, 4);
    Fraction<int> one(1);
    Fraction<int> two(2);
    Fraction<int> negative_half(-1, 2);

    SUBCASE("addition") {
        CHECK(half + quarter == three_quarters);
        CHECK(half + half == one);
        CHECK(half + 1 == Fraction<int>(3, 2));
        CHECK(1 + half == Fraction<int>(3, 2));
        CHECK(half + negative_half == Fraction<int>(0, 1));
    }

    SUBCASE("subtraction") {
        CHECK(half - quarter == quarter);
        CHECK(three_quarters - half == quarter);
        CHECK(half - half == Fraction<int>(0, 1));
        CHECK(half - 1 == negative_half);
        CHECK(1 - half == half);
    }

    SUBCASE("multiplication") {
        CHECK(half * half == quarter);
        CHECK(half * two == one);
        CHECK(two * half == one);
        CHECK(half * 2 == one);
        CHECK(2 * half == one);
        CHECK(half * negative_half == Fraction<int>(-1, 4));
    }

    SUBCASE("division") {
        CHECK(half / half == one);
        CHECK(one / half == two);
        CHECK(half / 2 == quarter);
        CHECK(2 / half == Fraction<int>(4, 1));
        CHECK(one / negative_half == Fraction<int>(-2, 1));
    }

    SUBCASE("negation") {
        CHECK(-half == negative_half);
        CHECK(-negative_half == half);
        CHECK(-one == Fraction<int>(-1, 1));
    }

    SUBCASE("compound assignment") {
        Fraction<int> f(1, 2);
        f += quarter;
        CHECK(f == three_quarters);
        
        f -= half;
        CHECK(f == quarter);
        
        f *= two;
        CHECK(f == half);
        
        f /= half;
        CHECK(f == one);
        
        f += 1;
        CHECK(f == two);
        
        f -= 1;
        CHECK(f == one);
        
        f *= 2;
        CHECK(f == two);
        
        f /= 2;
        CHECK(f == one);
    }

    SUBCASE("increment/decrement") {
        Fraction<int> f(3, 2);
        CHECK(++f == Fraction<int>(5, 2));
        CHECK(f-- == Fraction<int>(5, 2));
        CHECK(f == Fraction<int>(3, 2));
        CHECK(--f == half);
        CHECK(f++ == half);
        CHECK(f == Fraction<int>(3, 2));
    }
}

TEST_CASE("Fraction utility methods") {
    Fraction<int> half(1, 2);
    Fraction<int> negative_half(-1, 2);
    Fraction<int> two_fourths(2, 4);

    SUBCASE("reciprocal") {
        half.reciprocal();
        CHECK(half == Fraction<int>(2, 1));
        half.reciprocal();
        CHECK(half == Fraction<int>(1, 2));
        
        negative_half.reciprocal();
        CHECK(negative_half == Fraction<int>(-2, 1));
    }

    SUBCASE("cross product") {
        CHECK(half.cross(two_fourths) == 0);
        CHECK(half.cross(Fraction<int>(3, 4)) == -2);
        CHECK(half.cross(negative_half) == 4);
    }

    SUBCASE("normalization") {
        Fraction<int> f(2, -4);
        CHECK(f.numer() == -1);
        CHECK(f.denom() == 2);
        
        f = Fraction<int>(-2, -4);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }
}

TEST_CASE("Fraction with different types") {
    SUBCASE("long") {
        Fraction<long> f(1000000000L, 2000000000L);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    SUBCASE("unsigned") {
        Fraction<unsigned> f(3u, 6u);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);
    }

    // SUBCASE("short") {
    //     Fraction<short> f(3, 6);
    //     CHECK(f.numer() == 1);
    //     CHECK(f.denom() == 2);
    // }
}