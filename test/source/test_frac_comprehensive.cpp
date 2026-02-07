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

        CHECK(zero + zero == zero);
        CHECK(zero - zero == zero);
        CHECK(zero * zero == zero);
        CHECK(zero + pos == pos);
        CHECK(pos + zero == pos);
        CHECK(zero - pos == -pos);
        CHECK(pos - zero == pos);
        CHECK(zero * pos == zero);
        CHECK(pos * zero == zero);
        CHECK(zero * neg == zero);
        CHECK(neg * zero == zero);
    }

    SUBCASE("Unity fraction (1/1) properties") {
        ExtFraction<int> one(1, 1);
        ExtFraction<int> pos(3, 4);

        CHECK(one * one == one);
        CHECK(one / one == one);
        CHECK(one + one == ExtFraction<int>(2, 1));
        CHECK(one - one == ExtFraction<int>(0, 1));
        CHECK(one * pos == pos);
        CHECK(pos * one == pos);
        CHECK(one / pos == ExtFraction<int>(4, 3));
        CHECK(pos / one == pos);
    }

    SUBCASE("Negative fractions") {
        ExtFraction<int> neg1(-1, 2);
        ExtFraction<int> neg2(-3, 4);
        ExtFraction<int> pos(1, 2);

        CHECK(neg1 + neg2 == ExtFraction<int>(-5, 4));
        CHECK(neg1 - neg2 == ExtFraction<int>(1, 4));
        CHECK(neg1 * neg2 == ExtFraction<int>(3, 8));
        CHECK(neg1 / neg2 == ExtFraction<int>(2, 3));
        CHECK(neg1 + pos == ExtFraction<int>(0, 1));
        CHECK(pos + neg1 == ExtFraction<int>(0, 1));
        CHECK(-neg1 == pos);
        CHECK(-pos == neg1);
    }

    SUBCASE("Fractions that normalize to same value") {
        ExtFraction<int> f1(2, 4);
        ExtFraction<int> f2(3, 6);
        ExtFraction<int> f3(4, 8);
        ExtFraction<int> f4(-2, -4);

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
        CHECK(f1 == f4);
        CHECK(f1.numer() == 1);
        CHECK(f1.denom() == 2);
    }

    SUBCASE("Zero denominator (infinity-like) cases") {
        ExtFraction<int> inf(1, 0);
        ExtFraction<int> neg_inf(-1, 0);
        ExtFraction<int> nan(0, 0);
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> pos(3, 4);

        CHECK(inf == inf);
        CHECK(neg_inf == neg_inf);
        CHECK(nan == nan);
        CHECK(zero < inf);
        CHECK(pos < inf);
        CHECK(neg_inf < zero);
        CHECK(neg_inf < pos);
        CHECK(inf * pos == inf);
        CHECK(pos * inf == inf);
        CHECK(inf / zero == inf);
        CHECK(inf / pos == inf);
        CHECK(inf + inf == inf);
        CHECK(neg_inf + neg_inf == neg_inf);
    }
}

TEST_CASE("ExtFraction overflow safety") {
    SUBCASE("Safe multiplication with large numbers") {
        ExtFraction<int> f1(1000000, 2000000);
        CHECK(f1.numer() == 1);
        CHECK(f1.denom() == 2);
    }

    SUBCASE("Safe addition with large denominators") {
        ExtFraction<int> f1(1, 1000000);
        ExtFraction<int> f2(1, 1000000);
        auto result = f1 + f2;
        CHECK(result == ExtFraction<int>(1, 500000));
    }

    SUBCASE("Safe division with large numbers") {
        ExtFraction<int> f1(1000000, 1);
        ExtFraction<int> f2(2000000, 1);
        auto result = f1 / f2;
        CHECK(result == ExtFraction<int>(1, 2));
    }
}

TEST_CASE("ExtFraction fraction with integer operations") {
    SUBCASE("Fraction with integer operations") {
        ExtFraction<int> f(3, 4);

        CHECK(f + 1 == ExtFraction<int>(7, 4));
        CHECK(1 + f == ExtFraction<int>(7, 4));
        CHECK(f - 1 == ExtFraction<int>(-1, 4));
        CHECK(1 - f == ExtFraction<int>(1, 4));
        CHECK(f * 2 == ExtFraction<int>(3, 2));
        CHECK(2 * f == ExtFraction<int>(3, 2));
        CHECK(f / 2 == ExtFraction<int>(3, 8));
        CHECK(2 / f == ExtFraction<int>(8, 3));
    }
}

TEST_CASE("ExtFraction comparison edge cases") {
    SUBCASE("Comparing with zero") {
        ExtFraction<int> zero(0, 1);
        ExtFraction<int> pos(1, 2);
        ExtFraction<int> neg(-1, 2);
        ExtFraction<int> large_pos(1000, 1);
        ExtFraction<int> small_pos(1, 1000);

        CHECK(zero == 0);
        CHECK(0 == zero);
        CHECK(pos > 0);
        CHECK(0 < pos);
        CHECK(neg < 0);
        CHECK(0 > neg);
        CHECK(small_pos > 0);
        CHECK(0 < small_pos);
        CHECK(large_pos > 0);
        CHECK(0 < large_pos);
    }

    SUBCASE("Comparing very close fractions") {
        ExtFraction<int> f1(999, 1000);
        ExtFraction<int> f2(998, 999);
        ExtFraction<int> f3(1000, 1001);

        CHECK(f1 > f2);
        CHECK(f1 < f3);
        CHECK(f2 < f1);
        CHECK(f3 > f1);
    }

    SUBCASE("Transitivity of comparisons") {
        ExtFraction<int> a(1, 4);
        ExtFraction<int> b(1, 3);
        ExtFraction<int> c(1, 2);

        CHECK(a < b);
        CHECK(b < c);
        CHECK(a < c);
        CHECK(c > b);
        CHECK(b > a);
        CHECK(c > a);
    }
}

TEST_CASE("ExtFraction arithmetic precision") {
    SUBCASE("Repeated multiplication and division") {
        ExtFraction<int> f(1, 2);
        f *= 3;
        CHECK(f == ExtFraction<int>(3, 2));
        f /= 3;
        CHECK(f == ExtFraction<int>(1, 2));
    }

    SUBCASE("Chain of operations") {
        ExtFraction<int> f(1, 2);
        f += ExtFraction<int>(1, 3);
        CHECK(f == ExtFraction<int>(5, 6));
        f *= ExtFraction<int>(2, 5);
        CHECK(f == ExtFraction<int>(1, 3));
        f -= ExtFraction<int>(1, 6);
        CHECK(f == ExtFraction<int>(1, 6));
        f /= ExtFraction<int>(2, 3);
        CHECK(f == ExtFraction<int>(1, 4));
    }

    SUBCASE("Complex expression evaluation") {
        ExtFraction<int> a(1, 2);
        ExtFraction<int> b(1, 3);
        ExtFraction<int> c(1, 4);

        // ((a + b) * c) / a
        // (1/2 + 1/3) * 1/4 / (1/2) = (5/6) * 1/4 / (1/2) = 5/24 / (1/2) = 5/24 * 2 = 10/24 = 5/12
        auto result = ((a + b) * c) / a;
        CHECK(result == ExtFraction<int>(5, 12));
    }
}

TEST_CASE("ExtFraction normalization behavior") {
    SUBCASE("Negative denominator normalization") {
        ExtFraction<int> f1(1, -2);
        CHECK(f1.numer() == -1);
        CHECK(f1.denom() == 2);

        ExtFraction<int> f2(-1, -2);
        CHECK(f2.numer() == 1);
        CHECK(f2.denom() == 2);

        ExtFraction<int> f3(-3, -6);
        CHECK(f3.numer() == 1);
        CHECK(f3.denom() == 2);
    }

    SUBCASE("GCD reduction") {
        ExtFraction<int> f1(12, 18);
        CHECK(f1.numer() == 2);
        CHECK(f1.denom() == 3);

        ExtFraction<int> f2(25, 35);
        CHECK(f2.numer() == 5);
        CHECK(f2.denom() == 7);

        ExtFraction<int> f3(100, 150);
        CHECK(f3.numer() == 2);
        CHECK(f3.denom() == 3);
    }

    SUBCASE("Zero normalization") {
        ExtFraction<int> f1(0, 5);
        CHECK(f1.numer() == 0);
        CHECK(f1.denom() == 1);

        ExtFraction<int> f2(0, -10);
        CHECK(f2.numer() == 0);
        CHECK(f2.denom() == 1);
    }
}

TEST_CASE("ExtFraction cross product method") {
    SUBCASE("Cross product basic functionality") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(3, 4);
        ExtFraction<int> f3(2, 4);

        CHECK(f1.cross(f2) == -2);
        CHECK(f2.cross(f1) == 2);
        CHECK(f1.cross(f3) == 0);
        CHECK(f1.cross(f1) == 0);
    }

    SUBCASE("Cross product for comparison") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(1, 3);

        // If a/b < c/d, then a*d - b*c < 0
        CHECK(f1.cross(f2) > 0);  // 1/2 > 1/3
        CHECK(f2.cross(f1) < 0);
    }
}

TEST_CASE("ExtFraction reciprocal method") {
    SUBCASE("Reciprocal of positive fractions") {
        ExtFraction<int> f1(2, 3);
        f1.reciprocal();
        CHECK(f1 == ExtFraction<int>(3, 2));

        ExtFraction<int> f2(1, 4);
        f2.reciprocal();
        CHECK(f2 == ExtFraction<int>(4, 1));
    }

    SUBCASE("Reciprocal of negative fractions") {
        ExtFraction<int> f1(-2, 3);
        f1.reciprocal();
        CHECK(f1 == ExtFraction<int>(-3, 2));

        ExtFraction<int> f2(2, -3);
        f2.reciprocal();
        CHECK(f2 == ExtFraction<int>(-3, 2));
    }

    SUBCASE("Double reciprocal returns original") {
        ExtFraction<int> f(3, 5);
        f.reciprocal();
        f.reciprocal();
        CHECK(f == ExtFraction<int>(3, 5));
    }
}

TEST_CASE("ExtFraction increment/decrement operators") {
    SUBCASE("Prefix increment") {
        ExtFraction<int> f(1, 2);
        CHECK(++f == ExtFraction<int>(3, 2));
        CHECK(f == ExtFraction<int>(3, 2));
    }

    SUBCASE("Postfix increment") {
        ExtFraction<int> f(1, 2);
        CHECK(f++ == ExtFraction<int>(1, 2));
        CHECK(f == ExtFraction<int>(3, 2));
    }

    SUBCASE("Prefix decrement") {
        ExtFraction<int> f(3, 2);
        CHECK(--f == ExtFraction<int>(1, 2));
        CHECK(f == ExtFraction<int>(1, 2));
    }

    SUBCASE("Postfix decrement") {
        ExtFraction<int> f(3, 2);
        CHECK(f-- == ExtFraction<int>(3, 2));
        CHECK(f == ExtFraction<int>(1, 2));
    }

    SUBCASE("Multiple increments/decrements") {
        ExtFraction<int> f(0, 1);
        ++f;
        ++f;
        CHECK(f == ExtFraction<int>(2, 1));
        --f;
        CHECK(f == ExtFraction<int>(1, 1));
        f++;
        f++;
        CHECK(f == ExtFraction<int>(3, 1));
    }
}

TEST_CASE("ExtFraction compound assignment operators") {
    SUBCASE("operator+= with fraction") {
        ExtFraction<int> f(1, 2);
        f += ExtFraction<int>(1, 3);
        CHECK(f == ExtFraction<int>(5, 6));
    }

    SUBCASE("operator-= with fraction") {
        ExtFraction<int> f(1, 2);
        f -= ExtFraction<int>(1, 3);
        CHECK(f == ExtFraction<int>(1, 6));
    }

    SUBCASE("operator*= with fraction") {
        ExtFraction<int> f(2, 3);
        f *= ExtFraction<int>(3, 4);
        CHECK(f == ExtFraction<int>(1, 2));
    }

    SUBCASE("operator/= with fraction") {
        ExtFraction<int> f(2, 3);
        f /= ExtFraction<int>(3, 4);
        CHECK(f == ExtFraction<int>(8, 9));
    }

    SUBCASE("operator+= with integer") {
        ExtFraction<int> f(1, 2);
        f += 1;
        CHECK(f == ExtFraction<int>(3, 2));
    }

    SUBCASE("operator-= with integer") {
        ExtFraction<int> f(3, 2);
        f -= 1;
        CHECK(f == ExtFraction<int>(1, 2));
    }

    SUBCASE("operator*= with integer") {
        ExtFraction<int> f(1, 2);
        f *= 3;
        CHECK(f == ExtFraction<int>(3, 2));
    }

    SUBCASE("operator/= with integer") {
        ExtFraction<int> f(3, 2);
        f /= 3;
        CHECK(f == ExtFraction<int>(1, 2));
    }
}

TEST_CASE("ExtFraction stream output") {
    SUBCASE("Basic stream output") {
        ExtFraction<int> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(3/4)");
    }

    SUBCASE("Negative fraction stream output") {
        ExtFraction<int> f(-3, 4);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(-3/4)");
    }

    SUBCASE("Zero fraction stream output") {
        ExtFraction<int> f(0, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(0/1)");
    }

    SUBCASE("Unity fraction stream output") {
        ExtFraction<int> f(1, 1);
        std::stringstream ss;
        ss << f;
        CHECK(ss.str() == "(1/1)");
    }
}

TEST_CASE("ExtFraction with different integer types") {
    SUBCASE("long type") {
        ExtFraction<long> f(1000000000L, 2000000000L);
        CHECK(f.numer() == 1);
        CHECK(f.denom() == 2);

        ExtFraction<long> g(123456789L, 987654321L);
        g.normalize();
        CHECK(g.numer() == 13717421);
        CHECK(g.denom() == 109739369);
    }
}

TEST_CASE("ExtFraction special arithmetic cases") {
    SUBCASE("Adding fractions that sum to zero") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(-1, 2);
        CHECK(f1 + f2 == ExtFraction<int>(0, 1));
    }

    SUBCASE("Multiplying by reciprocal gives unity") {
        ExtFraction<int> f(3, 5);
        ExtFraction<int> f_recip = f;
        f_recip.reciprocal();
        CHECK(f * f_recip == ExtFraction<int>(1, 1));
    }

    SUBCASE("Dividing by same fraction gives unity") {
        ExtFraction<int> f(3, 5);
        CHECK(f / f == ExtFraction<int>(1, 1));
    }

    SUBCASE("Subtracting same fraction gives zero") {
        ExtFraction<int> f(3, 5);
        CHECK(f - f == ExtFraction<int>(0, 1));
    }
}

TEST_CASE("ExtFraction abs function") {
    SUBCASE("abs of positive fractions") {
        ExtFraction<int> f1(3, 4);
        CHECK(abs(f1) == f1);

        ExtFraction<int> f2(5, 2);
        CHECK(abs(f2) == f2);
    }

    SUBCASE("abs of negative fractions") {
        ExtFraction<int> f1(-3, 4);
        CHECK(abs(f1) == ExtFraction<int>(3, 4));

        ExtFraction<int> f2(3, -4);
        CHECK(abs(f2) == ExtFraction<int>(3, 4));

        ExtFraction<int> f3(-3, -4);
        CHECK(abs(f3) == ExtFraction<int>(3, 4));
    }

    SUBCASE("abs of zero") {
        ExtFraction<int> f(0, 1);
        CHECK(abs(f) == f);
    }
}

TEST_CASE("ExtFraction constructor variants") {
    SUBCASE("Default constructor") {
        ExtFraction<int> f;
        CHECK(f.numer() == 0);
        CHECK(f.denom() == 1);
    }

    SUBCASE("Single argument constructor (int)") {
        ExtFraction<int> f1(5);
        CHECK(f1.numer() == 5);
        CHECK(f1.denom() == 1);

        ExtFraction<int> f2(-3);
        CHECK(f2.numer() == -3);
        CHECK(f2.denom() == 1);

        ExtFraction<int> f3(0);
        CHECK(f3.numer() == 0);
        CHECK(f3.denom() == 1);
    }

    SUBCASE("Two argument constructor") {
        ExtFraction<int> f1(1, 2);
        CHECK(f1.numer() == 1);
        CHECK(f1.denom() == 2);

        ExtFraction<int> f2(4, 2);
        CHECK(f2.numer() == 2);
        CHECK(f2.denom() == 1);

        ExtFraction<int> f3(0, 5);
        CHECK(f3.numer() == 0);
        CHECK(f3.denom() == 1);
    }
}

TEST_CASE("ExtFraction equality and inequality special cases") {
    SUBCASE("Equal fractions with different representations") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(2, 4);
        ExtFraction<int> f3(3, 6);
        ExtFraction<int> f4(-1, -2);

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
        CHECK(f1 == f4);
    }

    SUBCASE("Inequality checks") {
        ExtFraction<int> f1(1, 2);
        ExtFraction<int> f2(1, 3);
        ExtFraction<int> f3(2, 3);

        CHECK(f1 != f2);
        CHECK(f1 != f3);
        CHECK(f2 != f3);
    }

    SUBCASE("Zero equality") {
        ExtFraction<int> f1(0, 1);
        ExtFraction<int> f2(0, 2);
        ExtFraction<int> f3(0, 5);

        CHECK(f1 == f2);
        CHECK(f2 == f3);
        CHECK(f1 == f3);
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
            CHECK(f[i] < f[i + 1]);
            CHECK(f[i + 1] > f[i]);
            CHECK(f[i] <= f[i + 1]);
            CHECK(f[i + 1] >= f[i]);
        }

        for (int i = 0; i < 9; ++i) {
            CHECK(f[i] <= f[i]);
            CHECK(f[i] >= f[i]);
            CHECK(f[i] == f[i]);
            CHECK_FALSE(f[i] != f[i]);
        }
    }
}
