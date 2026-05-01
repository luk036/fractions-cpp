/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <fractions/extfractions.hpp>
#include <limits>
#include <sstream>
#include <type_traits>

using namespace fractions;

TEST_CASE("ExtFraction boundary and edge cases") {
    SUBCASE("Zero fraction in all operations") {
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> pos(3, 4);
        ExtFraction<int> neg(-3, 4);

        CHECK_EQ(zero + zero, zero);
        CHECK_EQ(zero - zero, zero);
        CHECK_EQ(zero * zero, zero);
        CHECK_EQ(zero + pos, pos);
        CHECK_EQ(pos + zero, pos);
        CHECK_EQ(zero - pos, -pos);
        CHECK_EQ(pos - zero, pos);
        CHECK_EQ(zero * pos, zero);
        CHECK_EQ(pos * zero, zero);
        CHECK_EQ(zero * neg, zero);
        CHECK_EQ(neg * zero, zero);
    }

    SUBCASE("Unity fraction (1/1) properties") {
        ExtFraction<int> one(1, 1);
        ExtFraction<int> pos(3, 4);

        CHECK_EQ(one * one, one);
        CHECK_EQ(one / one, one);
        CHECK_EQ(one + one, ExtFraction<int>(2, 1));
        CHECK_EQ(one - one, ExtFraction<int>(0, 1));
        CHECK_EQ(one * pos, pos);
        CHECK_EQ(pos * one, pos);
        CHECK_EQ(one / pos, ExtFraction<int>(4, 3));
        CHECK_EQ(pos / one, pos);
    }

    SUBCASE("Negative fractions") {
        ExtFraction<int> neg1(-1, 2);
        ExtFraction<int> neg2(-3, 4);
        ExtFraction<int> pos(1, 2);

        CHECK_EQ(neg1 + neg2, ExtFraction<int>(-5, 4));
        CHECK_EQ(neg1 - neg2, ExtFraction<int>(1, 4));
        CHECK_EQ(neg1 * neg2, ExtFraction<int>(3, 8));
        CHECK_EQ(neg1 / neg2, ExtFraction<int>(2, 3));
        CHECK_EQ(neg1 + pos, ExtFraction<int>(0, 1));
        CHECK_EQ(pos + neg1, ExtFraction<int>(0, 1));
        CHECK_EQ(-neg1, pos);
        CHECK_EQ(-pos, neg1);
    }

    SUBCASE("Fractions that normalize to same value") {
        ExtFraction<int> f1(2, 4);
        ExtFraction<int> f2(3, 6);
        ExtFraction<int> f3(4, 8);
        ExtFraction<int> f4(-2, -4);

        CHECK_EQ(f1, f2);
        CHECK_EQ(f2, f3);
        CHECK_EQ(f1, f3);
        CHECK_EQ(f1, f4);
        CHECK_EQ(f1.numer(), 1);
        CHECK_EQ(f1.denom(), 2);
    }

    SUBCASE("Zero denominator (infinity-like) cases") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> neg_inf(-1, 0);
        ExtFraction<int> nan(0, 0);
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> pos(3, 4);

        CHECK_EQ(inf, inf);
        CHECK_EQ(neg_inf, neg_inf);
        CHECK_EQ(nan, nan);
        CHECK_LT(zero, inf);
        CHECK_LT(pos, inf);
        CHECK_LT(neg_inf, zero);
        CHECK_LT(neg_inf, pos);
        CHECK_EQ(inf * pos, inf);
        CHECK_EQ(pos * inf, inf);
        CHECK_EQ(inf / zero, inf);
        CHECK_EQ(inf / pos, inf);
        CHECK_EQ(inf + inf, inf);
        CHECK_EQ(neg_inf + neg_inf, neg_inf);
    }
}

TEST_CASE("ExtFraction overflow safety") {
    SUBCASE("Safe multiplication with large numbers") {
        ExtFraction<int> f1(1000000, 2000000);
        CHECK_EQ(f1.numer(), 1);
        CHECK_EQ(f1.denom(), 2);
    }

    SUBCASE("Safe addition with large denominators") {
        ExtFraction<int> f1(1, 1000000);
        ExtFraction<int> f2(1, 1000000);
        auto result = f1 + f2;
        CHECK_EQ(result, ExtFraction<int>(1, 500000));
    }

    SUBCASE("Safe division with large numbers") {
        ExtFraction<int> f1(1000000, 1);
        ExtFraction<int> f2(2000000, 1);
        auto result = f1 / f2;
        CHECK_EQ(result, ExtFraction<int>(1, 2));
    }
}

TEST_CASE("ExtFraction fraction with integer operations") {
    SUBCASE("Fraction with integer operations") {
        ExtFraction<int> f(3, 4);

        CHECK_EQ(f + 1, ExtFraction<int>(7, 4));
        CHECK_EQ(1 + f, ExtFraction<int>(7, 4));
        CHECK_EQ(f - 1, ExtFraction<int>(-1, 4));
        CHECK_EQ(1 - f, ExtFraction<int>(1, 4));
        CHECK_EQ(f * 2, ExtFraction<int>(3, 2));
        CHECK_EQ(2 * f, ExtFraction<int>(3, 2));
        CHECK_EQ(f / 2, ExtFraction<int>(3, 8));
        CHECK_EQ(2 / f, ExtFraction<int>(8, 3));
    }
}

TEST_CASE("ExtFraction comparison edge cases") {
    SUBCASE("Comparing with zero") {
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> pos(1, 2);
        ExtFraction<int> neg(-1, 2);
        ExtFraction<int> large_pos(1000, 1);
        ExtFraction<int> small_pos(1, 1000);

        CHECK_EQ(zero, 0);
        CHECK_EQ(0, zero);
        CHECK_GT(pos, 0);
        CHECK_LT(0, pos);
        CHECK_LT(neg, 0);
        CHECK_GT(0, neg);
        CHECK_GT(small_pos, 0);
        CHECK_LT(0, small_pos);
        CHECK_GT(large_pos, 0);
        CHECK_LT(0, large_pos);
    }

    SUBCASE("Comparing very close fractions") {
        ExtFraction<int> f1(999, 1000);
        ExtFraction<int> f2(998, 999);
        ExtFraction<int> f3(1000, 1001);

        CHECK_GT(f1, f2);
        CHECK_LT(f1, f3);
        CHECK_LT(f2, f1);
        CHECK_GT(f3, f1);
    }

    SUBCASE("Transitivity of comparisons") {
        ExtFraction<int> a(1, 4);
        ExtFraction<int> b(1, 3);
        ExtFraction<int> c(1, 2);

        CHECK_LT(a, b);
        CHECK_LT(b, c);
        CHECK_LT(a, c);
        CHECK_GT(c, b);
        CHECK_GT(b, a);
        CHECK_GT(c, a);
    }
}

TEST_CASE("ExtFraction arithmetic precision") {
    SUBCASE("Repeated multiplication and division") {
        ExtFraction<int> f(1, 2);
        f *= 3;
        CHECK_EQ(f, ExtFraction<int>(3, 2));
        f /= 3;
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }

    SUBCASE("Chain of operations") {
        ExtFraction<int> f(1, 2);
        f += ExtFraction<int>(1, 3);
        CHECK_EQ(f, ExtFraction<int>(5, 6));
        f *= ExtFraction<int>(2, 5);
        CHECK_EQ(f, ExtFraction<int>(1, 3));
        f -= ExtFraction<int>(1, 6);
        CHECK_EQ(f, ExtFraction<int>(1, 6));
        f /= ExtFraction<int>(2, 3);
        CHECK_EQ(f, ExtFraction<int>(1, 4));
    }

    SUBCASE("Complex expression evaluation") {
        ExtFraction<int> a(1, 2);
        ExtFraction<int> b(1, 3);
        ExtFraction<int> c(1, 4);

        // ((a + b) * c) / a
        // (1/2 + 1/3) * 1/4 / (1/2) = (5/6) * 1/4 / (1/2) = 5/24 / (1/2) = 5/24 * 2 = 10/24 = 5/12
        auto result = ((a + b) * c) / a;
        CHECK_EQ(result, ExtFraction<int>(5, 12));
    }
}

TEST_CASE("ExtFraction normalization behavior") {
    SUBCASE("Negative denominator normalization") {
        ExtFraction<int> f1(1, -2);
        CHECK_EQ(f1.numer(), -1);
        CHECK_EQ(f1.denom(), 2);

        ExtFraction<int> f2(-1, -2);
        CHECK_EQ(f2.numer(), 1);
        CHECK_EQ(f2.denom(), 2);

        ExtFraction<int> f3(-3, -6);
        CHECK_EQ(f3.numer(), 1);
        CHECK_EQ(f3.denom(), 2);
    }

    SUBCASE("GCD reduction") {
        ExtFraction<int> f1(12, 18);
        CHECK_EQ(f1.numer(), 2);
        CHECK_EQ(f1.denom(), 3);

        ExtFraction<int> f2(25, 35);
        CHECK_EQ(f2.numer(), 5);
        CHECK_EQ(f2.denom(), 7);

        ExtFraction<int> f3(100, 150);
        CHECK_EQ(f3.numer(), 2);
        CHECK_EQ(f3.denom(), 3);
    }

    SUBCASE("Zero normalization") {
        ExtFraction<int> f1(0, 5);
        CHECK_EQ(f1.numer(), 0);
        CHECK_EQ(f1.denom(), 1);

        ExtFraction<int> f2(0, -10);
        CHECK_EQ(f2.numer(), 0);
        CHECK_EQ(f2.denom(), 1);
    }
}

TEST_CASE("ExtFraction cross product method") {
    SUBCASE("Cross product basic functionality") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(3, 4);
        ExtFraction<int> f3(2, 4);

        CHECK_EQ(f1.cross(f2), -2);
        CHECK_EQ(f2.cross(f1), 2);
        CHECK_EQ(f1.cross(f3), 0);
        CHECK_EQ(f1.cross(f1), 0);
    }

    SUBCASE("Cross product for comparison") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(1, 3);

        // If a/b < c/d, then a*d - b*c < 0
        CHECK_GT(f1.cross(f2), 0);  // 1/2 > 1/3
        CHECK_LT(f2.cross(f1), 0);
    }
}

TEST_CASE("ExtFraction reciprocal method") {
    SUBCASE("Reciprocal of positive fractions") {
        ExtFraction<int> f1(2, 3);
        f1.reciprocal();
        CHECK_EQ(f1, ExtFraction<int>(3, 2));

        ExtFraction<int> f2(1, 4);
        f2.reciprocal();
        CHECK_EQ(f2, ExtFraction<int>(4, 1));
    }

    SUBCASE("Reciprocal of negative fractions") {
        ExtFraction<int> f1(-2, 3);
        f1.reciprocal();
        CHECK_EQ(f1, ExtFraction<int>(-3, 2));

        ExtFraction<int> f2(2, -3);
        f2.reciprocal();
        CHECK_EQ(f2, ExtFraction<int>(-3, 2));
    }

    SUBCASE("Double reciprocal returns original") {
        ExtFraction<int> f(3, 5);
        f.reciprocal();
        f.reciprocal();
        CHECK_EQ(f, ExtFraction<int>(3, 5));
    }
}

TEST_CASE("ExtFraction increment/decrement operators") {
    SUBCASE("Prefix increment") {
        ExtFraction<int> f(1, 2);
        CHECK_EQ(++f, ExtFraction<int>(3, 2));
        CHECK_EQ(f, ExtFraction<int>(3, 2));
    }

    SUBCASE("Postfix increment") {
        ExtFraction<int> f(1, 2);
        CHECK_EQ(f++, ExtFraction<int>(1, 2));
        CHECK_EQ(f, ExtFraction<int>(3, 2));
    }

    SUBCASE("Prefix decrement") {
        ExtFraction<int> f(3, 2);
        CHECK_EQ(--f, ExtFraction<int>(1, 2));
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }

    SUBCASE("Postfix decrement") {
        ExtFraction<int> f(3, 2);
        CHECK_EQ(f--, ExtFraction<int>(3, 2));
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }

    SUBCASE("Multiple increments/decrements") {
        ExtFraction<int> f(0, 1);
        ++f;
        ++f;
        CHECK_EQ(f, ExtFraction<int>(2, 1));
        --f;
        CHECK_EQ(f, ExtFraction<int>(1, 1));
        f++;
        f++;
        CHECK_EQ(f, ExtFraction<int>(3, 1));
    }
}

TEST_CASE("ExtFraction compound assignment operators") {
    SUBCASE("operator+= with fraction") {
        ExtFraction<int> f(1, 2);
        f += ExtFraction<int>(1, 3);
        CHECK_EQ(f, ExtFraction<int>(5, 6));
    }

    SUBCASE("operator-= with fraction") {
        ExtFraction<int> f(1, 2);
        f -= ExtFraction<int>(1, 3);
        CHECK_EQ(f, ExtFraction<int>(1, 6));
    }

    SUBCASE("operator*= with fraction") {
        ExtFraction<int> f(2, 3);
        f *= ExtFraction<int>(3, 4);
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }

    SUBCASE("operator/= with fraction") {
        ExtFraction<int> f(2, 3);
        f /= ExtFraction<int>(3, 4);
        CHECK_EQ(f, ExtFraction<int>(8, 9));
    }

    SUBCASE("operator+= with integer") {
        ExtFraction<int> f(1, 2);
        f += 1;
        CHECK_EQ(f, ExtFraction<int>(3, 2));
    }

    SUBCASE("operator-= with integer") {
        ExtFraction<int> f(3, 2);
        f -= 1;
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }

    SUBCASE("operator*= with integer") {
        ExtFraction<int> f(1, 2);
        f *= 3;
        CHECK_EQ(f, ExtFraction<int>(3, 2));
    }

    SUBCASE("operator/= with integer") {
        ExtFraction<int> f(3, 2);
        f /= 3;
        CHECK_EQ(f, ExtFraction<int>(1, 2));
    }
}

TEST_CASE("ExtFraction stream output") {
    SUBCASE("Basic stream output") {
        ExtFraction<int> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(3/4)");
    }

    SUBCASE("Negative fraction stream output") {
        ExtFraction<int> f(-3, 4);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(-3/4)");
    }

    SUBCASE("Zero fraction stream output") {
        ExtFraction<int> f(0, 1);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(0/1)");
    }

    SUBCASE("Unity fraction stream output") {
        ExtFraction<int> f(1, 1);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(1/1)");
    }
}

TEST_CASE("ExtFraction with different integer types") {
    SUBCASE("long type") {
        ExtFraction<long> f(1000000000L, 2000000000L);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);

        ExtFraction<long> g(123456789L, 987654321L);
        g.normalize();
        CHECK_EQ(g.numer(), 13717421);
        CHECK_EQ(g.denom(), 109739369);
    }
}

TEST_CASE("ExtFraction special arithmetic cases") {
    SUBCASE("Adding fractions that sum to zero") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(-1, 2);
        CHECK_EQ(f1 + f2, ExtFraction<int>(0, 1));
    }

    SUBCASE("Multiplying by reciprocal gives unity") {
        ExtFraction<int> f(3, 5);
        ExtFraction<int> f_recip = f;
        f_recip.reciprocal();
        CHECK_EQ(f * f_recip, ExtFraction<int>(1, 1));
    }

    SUBCASE("Dividing by same fraction gives unity") {
        ExtFraction<int> f(3, 5);
        CHECK_EQ(f / f, ExtFraction<int>(1, 1));
    }

    SUBCASE("Subtracting same fraction gives zero") {
        ExtFraction<int> f(3, 5);
        CHECK_EQ(f - f, ExtFraction<int>(0, 1));
    }
}

TEST_CASE("ExtFraction abs function") {
    SUBCASE("abs of positive fractions") {
        ExtFraction<int> f1(3, 4);
        CHECK_EQ(abs(f1), f1);

        ExtFraction<int> f2(5, 2);
        CHECK_EQ(abs(f2), f2);
    }

    SUBCASE("abs of negative fractions") {
        ExtFraction<int> f1(-3, 4);
        CHECK_EQ(abs(f1), ExtFraction<int>(3, 4));

        ExtFraction<int> f2(3, -4);
        CHECK_EQ(abs(f2), ExtFraction<int>(3, 4));

        ExtFraction<int> f3(-3, -4);
        CHECK_EQ(abs(f3), ExtFraction<int>(3, 4));
    }

    SUBCASE("abs of zero") {
        ExtFraction<int> f(0, 1);
        CHECK_EQ(abs(f), f);
    }
}

TEST_CASE("ExtFraction constructor variants") {
    SUBCASE("Default constructor") {
        ExtFraction<int> f;
        CHECK_EQ(f.numer(), 0);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("Single argument constructor (int)") {
        ExtFraction<int> f1(5);
        CHECK_EQ(f1.numer(), 5);
        CHECK_EQ(f1.denom(), 1);

        ExtFraction<int> f2(-3);
        CHECK_EQ(f2.numer(), -3);
        CHECK_EQ(f2.denom(), 1);

        ExtFraction<int> f3(0);
        CHECK_EQ(f3.numer(), 0);
        CHECK_EQ(f3.denom(), 1);
    }

    SUBCASE("Two argument constructor") {
        ExtFraction<int> f1(1, 2);
        CHECK_EQ(f1.numer(), 1);
        CHECK_EQ(f1.denom(), 2);

        ExtFraction<int> f2(4, 2);
        CHECK_EQ(f2.numer(), 2);
        CHECK_EQ(f2.denom(), 1);

        ExtFraction<int> f3(0, 5);
        CHECK_EQ(f3.numer(), 0);
        CHECK_EQ(f3.denom(), 1);
    }
}

TEST_CASE("ExtFraction equality and inequality special cases") {
    SUBCASE("Equal fractions with different representations") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(2, 4);
        ExtFraction<int> f3(3, 6);
        ExtFraction<int> f4(-1, -2);

        CHECK_EQ(f1, f2);
        CHECK_EQ(f2, f3);
        CHECK_EQ(f1, f3);
        CHECK_EQ(f1, f4);
    }

    SUBCASE("Inequality checks") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(1, 3);
        ExtFraction<int> f3(2, 3);

        CHECK_NE(f1, f2);
        CHECK_NE(f1, f3);
        CHECK_NE(f2, f3);
    }

    SUBCASE("Zero equality") {
        ExtFraction<int> f1(0, 1);
        ExtFraction<int> f2(0, 2);
        ExtFraction<int> f3(0, 5);

        CHECK_EQ(f1, f2);
        CHECK_EQ(f2, f3);
        CHECK_EQ(f1, f3);
    }
}

TEST_CASE("ExtFraction ordering completeness") {
    SUBCASE("Complete ordering with various fractions") {
        ExtFraction<int> f[] = {
            ExtFraction<int>(-3, 2),  // -1.5
            ExtFraction<int>(-1, 1),  // -1.0
            ExtFraction<int>(-1, 2),  // -0.5
            ExtFraction<int>(0, 1),   // 0.0
            ExtFraction<int>(1, 3),   // 0.333...
            ExtFraction<int>(1, 2),   // 0.5
            ExtFraction<int>(2, 3),   // 0.666...
            ExtFraction<int>(1, 1),   // 1.0
            ExtFraction<int>(3, 2)    // 1.5
        };

        for (int i = 0; i < 8; ++i) {
            CHECK_LT(f[i], f[i + 1]);
            CHECK_GT(f[i + 1], f[i]);
            CHECK_LE(f[i], f[i + 1]);
            CHECK_GE(f[i + 1], f[i]);
        }

        for (int i = 0; i < 9; ++i) {
            CHECK_LE(f[i], f[i]);
            CHECK_GE(f[i], f[i]);
            CHECK_EQ(f[i], f[i]);
            CHECK_FALSE(f[i] != f[i]);
        }
    }
}
