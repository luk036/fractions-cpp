#include <doctest/doctest.h>

#include <fractions/fractions.hpp>
#include <fractions/extfractions.hpp>
#include <sstream>

using namespace fractions;

TEST_CASE("Fraction<int> construction") {
    SUBCASE("default constructor") {
        Fraction<int> f;
        CHECK_EQ(f.numer(), 0);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("single argument copy constructor") {
        int n = 5;
        Fraction<int> f(n);
        CHECK_EQ(f.numer(), 5);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("single argument rvalue constructor") {
        Fraction<int> f(7);
        CHECK_EQ(f.numer(), 7);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("single argument negative") {
        Fraction<int> f(-3);
        CHECK_EQ(f.numer(), -3);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("two argument constructor normalizes") {
        Fraction<int> f(2, 4);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("two argument constructor negative numerator") {
        Fraction<int> f(-1, 2);
        CHECK_EQ(f.numer(), -1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("two argument constructor negative denominator flips sign") {
        Fraction<int> f(1, -2);
        CHECK_EQ(f.numer(), -1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("two argument constructor both negative") {
        Fraction<int> f(-1, -2);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("two argument constructor reduces") {
        Fraction<int> f(6, 8);
        CHECK_EQ(f.numer(), 3);
        CHECK_EQ(f.denom(), 4);
    }

    SUBCASE("two argument constructor zero numerator") {
        Fraction<int> f(0, 5);
        CHECK_EQ(f.numer(), 0);
        CHECK_EQ(f.denom(), 1);
    }
}

TEST_CASE("Fraction<int> accessors") {
    Fraction<int> f(3, 4);
    CHECK_EQ(f.numer(), 3);
    CHECK_EQ(f.denom(), 4);

    CHECK(noexcept(f.numer()));
    CHECK(noexcept(f.denom()));
}

TEST_CASE("Fraction<int> keep_denom_positive") {
    SUBCASE("already positive") {
        Fraction<int> f(1, 2);
        f.keep_denom_positive();
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("negative denominator") {
        Fraction<int> f(1, -2);
        f.keep_denom_positive();
        CHECK_EQ(f.numer(), -1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("both negative") {
        Fraction<int> f(-1, -2);
        f.keep_denom_positive();
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

}

TEST_CASE("Fraction<int> reduce") {
    SUBCASE("already reduced") {
        Fraction<int> f(1, 2);
        auto g = f.reduce();
        CHECK_EQ(g, 1);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("reducible") {
        Fraction<int> f(2, 4);
        auto g = f.reduce();
        CHECK_EQ(g, 1);
    }

    SUBCASE("zero numerator") {
        Fraction<int> f(0, 5);
        auto g = f.reduce();
        CHECK_EQ(g, 1);
    }
}

TEST_CASE("Fraction<int> normalize") {
    SUBCASE("return value is gcd") {
        Fraction<int> f(2, 4);
        auto g = f.normalize();
        CHECK_EQ(g, 1);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("already normalized") {
        Fraction<int> f(1, 2);
        auto g = f.normalize();
        CHECK_EQ(g, 1);
    }

    SUBCASE("negative denominator") {
        Fraction<int> f(2, -4);
        auto g = f.normalize();
        CHECK_EQ(g, 1);
        CHECK_EQ(f.numer(), -1);
        CHECK_EQ(f.denom(), 2);
    }

    SUBCASE("zero numerator") {
        Fraction<int> f(0, 5);
        auto g = f.normalize();
        CHECK_EQ(g, 1);
        CHECK_EQ(f.numer(), 0);
        CHECK_EQ(f.denom(), 1);
    }

    SUBCASE("non-trivial gcd") {
        Fraction<int> f(4, 8);
        f.normalize();
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }
}

TEST_CASE("Fraction<int> cross product") {
    Fraction<int> a(1, 2);
    Fraction<int> b(3, 4);
    Fraction<int> c(2, 4);
    Fraction<int> neg(-1, 2);

    SUBCASE("basic") {
        CHECK_EQ(a.cross(b), -2);
        CHECK_EQ(b.cross(a), 2);
    }

    SUBCASE("equal fractions") {
        CHECK_EQ(a.cross(c), 0);
        CHECK_EQ(a.cross(a), 0);
    }

    SUBCASE("anti-symmetry") {
        CHECK_EQ(a.cross(b), -b.cross(a));
    }

    SUBCASE("with negative") {
        CHECK_EQ(a.cross(neg), 4);
        CHECK_EQ(neg.cross(a), -4);
    }

    SUBCASE("with zero") {
        Fraction<int> zero(0, 1);
        CHECK_EQ(a.cross(zero), 1);
        CHECK_EQ(zero.cross(a), -1);
    }
}

TEST_CASE("Fraction<int> equality") {
    Fraction<int> a(1, 2);
    Fraction<int> b(2, 4);
    Fraction<int> c(3, 4);
    Fraction<int> neg(-1, 2);
    Fraction<int> zero(0, 1);

    SUBCASE("Fraction == Fraction") {
        CHECK_EQ(a, a);
        CHECK_EQ(a, b);
        CHECK_EQ(b, a);
        CHECK_FALSE(a == c);
    }

    SUBCASE("Fraction == T") {
        Fraction<int> one(1, 1);
        CHECK_EQ(one, 1);
        CHECK_EQ(1, one);
        CHECK_FALSE(a == 1);
        CHECK_FALSE(1 == a);
    }

    SUBCASE("zero == 0") {
        CHECK_EQ(zero, 0);
        CHECK_EQ(0, zero);
    }

    SUBCASE("negative") {
        CHECK_FALSE(neg == 1);
    }

    SUBCASE("inequality") {
        CHECK_NE(a, c);
        CHECK_NE(c, a);
        CHECK_FALSE(a != b);
    }
}

TEST_CASE("Fraction<int> less than") {
    Fraction<int> a(1, 2);
    Fraction<int> b(1, 3);
    Fraction<int> c(2, 4);
    Fraction<int> neg(-1, 2);

    SUBCASE("Fraction < Fraction") {
        CHECK_LT(b, a);
        CHECK_FALSE(a < b);
        CHECK_FALSE(a < c);
    }

    SUBCASE("Fraction < T when denom == 1") {
        Fraction<int> one(1, 1);
        CHECK_LT(one, 2);
        CHECK_FALSE(one < 1);
    }

    SUBCASE("Fraction < T when denom != 1 and rhs != 0") {
        CHECK_LT(a, 1);
        CHECK_FALSE(a < 0);
    }

    SUBCASE("Fraction < T when rhs == 0") {
        CHECK_LT(neg, 0);
        CHECK_FALSE(a < 0);
    }

    SUBCASE("T < Fraction") {
        CHECK_LT(0, a);
        CHECK_FALSE(1 < a);
    }

    SUBCASE("T < Fraction with denom != 1 and lhs != 0") {
        CHECK_LT(1, Fraction<int>(3, 2));
        CHECK_FALSE(2 < Fraction<int>(3, 2));
    }

    SUBCASE("T < Fraction when rhs denom == 1") {
        Fraction<int> one(1, 1);
        CHECK_LT(0, one);
        CHECK_FALSE(2 < one);
    }

    SUBCASE("T < Fraction when lhs == 0") {
        CHECK_LT(0, a);
        CHECK(neg < 0);
    }
}

TEST_CASE("Fraction<int> greater than") {
    Fraction<int> a(1, 2);
    Fraction<int> b(1, 3);

    SUBCASE("Fraction > Fraction") {
        CHECK_GT(a, b);
        CHECK_FALSE(b > a);
    }

    SUBCASE("Fraction > T") {
        CHECK_GT(a, 0);
        CHECK_FALSE(a > 1);
    }

    SUBCASE("T > Fraction") {
        CHECK_GT(1, a);
        CHECK_FALSE(0 > a);
    }
}

TEST_CASE("Fraction<int> less than or equal") {
    Fraction<int> a(1, 2);
    Fraction<int> b(2, 4);
    Fraction<int> c(3, 4);
    Fraction<int> neg(-1, 2);

    SUBCASE("Fraction <= Fraction") {
        CHECK_LE(a, b);
        CHECK_LE(a, c);
        CHECK_LE(a, a);
        CHECK_FALSE(c <= a);
    }

    SUBCASE("Fraction <= T") {
        CHECK_LE(a, 1);
        CHECK_LE(neg, 0);
        CHECK_FALSE(a <= -1);
    }

    SUBCASE("T <= Fraction") {
        CHECK_LE(0, a);
        CHECK_LE(1, Fraction<int>(3, 2));
        CHECK_FALSE(2 <= a);
    }
}

TEST_CASE("Fraction<int> greater than or equal") {
    Fraction<int> a(1, 2);
    Fraction<int> b(2, 4);
    Fraction<int> c(3, 4);
    Fraction<int> neg(-1, 2);

    SUBCASE("Fraction >= Fraction") {
        CHECK_GE(a, b);
        CHECK_GE(c, a);
        CHECK_GE(a, a);
        CHECK_FALSE(a >= c);
    }

    SUBCASE("Fraction >= T") {
        CHECK_GE(a, 0);
        CHECK_GE(a, -1);
        CHECK_FALSE(a >= 1);
    }

    SUBCASE("T >= Fraction") {
        CHECK_GE(1, a);
        CHECK_GE(0, neg);
        CHECK_FALSE(0 >= a);
    }
}

TEST_CASE("Fraction<int> arithmetic") {
    Fraction<int> a(1, 2);
    Fraction<int> b(1, 3);
    Fraction<int> neg(-1, 2);

    SUBCASE("addition Fraction + Fraction") {
        CHECK_EQ(a + b, Fraction<int>(5, 6));
        CHECK_EQ(a + a, Fraction<int>(1, 1));
        CHECK_EQ(a + neg, Fraction<int>(0, 1));
    }

    SUBCASE("addition Fraction + T") {
        CHECK_EQ(a + 1, Fraction<int>(3, 2));
        CHECK_EQ(a + 0, a);
    }

    SUBCASE("addition T + Fraction") {
        CHECK_EQ(1 + a, Fraction<int>(3, 2));
        CHECK_EQ(0 + a, a);
    }

    SUBCASE("subtraction Fraction - Fraction") {
        CHECK_EQ(a - b, Fraction<int>(1, 6));
        CHECK_EQ(a - a, Fraction<int>(0, 1));
    }

    SUBCASE("subtraction Fraction - T") {
        CHECK_EQ(a - 1, neg);
        CHECK_EQ(a - 0, a);
    }

    SUBCASE("subtraction T - Fraction") {
        CHECK_EQ(1 - a, Fraction<int>(1, 2));
        CHECK_EQ(0 - a, neg);
    }

    SUBCASE("multiplication Fraction * Fraction") {
        CHECK_EQ(a * b, Fraction<int>(1, 6));
        CHECK_EQ(a * a, Fraction<int>(1, 4));
    }

    SUBCASE("multiplication Fraction * T") {
        CHECK_EQ(a * 2, Fraction<int>(1, 1));
        CHECK_EQ(a * 0, Fraction<int>(0, 1));
    }

    SUBCASE("multiplication T * Fraction") {
        CHECK_EQ(2 * a, Fraction<int>(1, 1));
        CHECK_EQ(0 * a, Fraction<int>(0, 1));
    }

    SUBCASE("division Fraction / Fraction") {
        CHECK_EQ(a / b, Fraction<int>(3, 2));
        CHECK_EQ(a / a, Fraction<int>(1, 1));
    }

    SUBCASE("division Fraction / T") {
        CHECK_EQ(a / 2, Fraction<int>(1, 4));
    }

    SUBCASE("division T / Fraction") {
        CHECK_EQ(1 / a, Fraction<int>(2, 1));
    }

    SUBCASE("negation") {
        CHECK_EQ(-a, neg);
        CHECK_EQ(-neg, a);
        CHECK_EQ(-Fraction<int>(0, 1), Fraction<int>(0, 1));
    }
}

TEST_CASE("Fraction<int> compound assignment") {
    SUBCASE("+= Fraction") {
        auto f = Fraction<int>(1, 2);
        f += Fraction<int>(1, 3);
        CHECK_EQ(f, Fraction<int>(5, 6));
    }

    SUBCASE("+= Fraction same denominator") {
        auto f = Fraction<int>(1, 4);
        f += Fraction<int>(2, 4);
        CHECK_EQ(f, Fraction<int>(3, 4));
    }

    SUBCASE("+= T") {
        auto f = Fraction<int>(1, 2);
        f += 1;
        CHECK_EQ(f, Fraction<int>(3, 2));
    }

    SUBCASE("+= T with denom == 1") {
        auto f = Fraction<int>(1, 1);
        f += 2;
        CHECK_EQ(f, Fraction<int>(3, 1));
    }

    SUBCASE("-= Fraction") {
        auto f = Fraction<int>(1, 2);
        f -= Fraction<int>(1, 3);
        CHECK_EQ(f, Fraction<int>(1, 6));
    }

    SUBCASE("-= Fraction same denominator") {
        auto f = Fraction<int>(3, 4);
        f -= Fraction<int>(1, 4);
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("-= T") {
        auto f = Fraction<int>(3, 2);
        f -= 1;
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("-= T with denom == 1") {
        auto f = Fraction<int>(3, 1);
        f -= 2;
        CHECK_EQ(f, Fraction<int>(1, 1));
    }

    SUBCASE("*= Fraction") {
        auto f = Fraction<int>(2, 3);
        f *= Fraction<int>(3, 4);
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("*= T") {
        auto f = Fraction<int>(1, 2);
        f *= 3;
        CHECK_EQ(f, Fraction<int>(3, 2));
    }

    SUBCASE("/= Fraction") {
        auto f = Fraction<int>(2, 3);
        f /= Fraction<int>(3, 4);
        CHECK_EQ(f, Fraction<int>(8, 9));
    }

    SUBCASE("/= T") {
        auto f = Fraction<int>(3, 2);
        f /= 3;
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("/= T compatibility with /= Fraction") {
        auto f = Fraction<int>(1, 2);
        f /= 2;
        CHECK_EQ(f, Fraction<int>(1, 4));
    }
}

TEST_CASE("Fraction<int> increment decrement") {
    SUBCASE("prefix ++") {
        auto f = Fraction<int>(1, 2);
        CHECK_EQ(++f, Fraction<int>(3, 2));
        CHECK_EQ(f, Fraction<int>(3, 2));
    }

    SUBCASE("postfix ++") {
        auto f = Fraction<int>(1, 2);
        CHECK_EQ(f++, Fraction<int>(1, 2));
        CHECK_EQ(f, Fraction<int>(3, 2));
    }

    SUBCASE("prefix --") {
        auto f = Fraction<int>(3, 2);
        CHECK_EQ(--f, Fraction<int>(1, 2));
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("postfix --") {
        auto f = Fraction<int>(3, 2);
        CHECK_EQ(f--, Fraction<int>(3, 2));
        CHECK_EQ(f, Fraction<int>(1, 2));
    }

    SUBCASE("increment zero") {
        auto f = Fraction<int>(0, 1);
        ++f;
        CHECK_EQ(f, Fraction<int>(1, 1));
    }

    SUBCASE("decrement unity") {
        auto f = Fraction<int>(1, 1);
        --f;
        CHECK_EQ(f, Fraction<int>(0, 1));
    }

    SUBCASE("multiple increments") {
        auto f = Fraction<int>(0, 1);
        ++f;
        ++f;
        ++f;
        CHECK_EQ(f, Fraction<int>(3, 1));
    }
}

TEST_CASE("Fraction<int> reciprocal") {
    SUBCASE("positive fraction") {
        auto f = Fraction<int>(2, 3);
        f.reciprocal();
        CHECK_EQ(f, Fraction<int>(3, 2));
    }

    SUBCASE("negative fraction") {
        auto f = Fraction<int>(-2, 3);
        f.reciprocal();
        CHECK_EQ(f, Fraction<int>(-3, 2));
    }

    SUBCASE("double reciprocal") {
        auto f = Fraction<int>(3, 5);
        f.reciprocal();
        f.reciprocal();
        CHECK_EQ(f, Fraction<int>(3, 5));
    }

    SUBCASE("reciprocal of unity") {
        auto f = Fraction<int>(1, 1);
        f.reciprocal();
        CHECK_EQ(f, Fraction<int>(1, 1));
    }

    SUBCASE("reciprocal keeps denom positive") {
        auto f = Fraction<int>(1, 2);
        f.reciprocal();
        CHECK_EQ(f.denom(), 1);
    }
}

TEST_CASE("Fraction<int> stream output") {
    SUBCASE("basic fraction") {
        Fraction<int> f(3, 4);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(3/4)");
    }

    SUBCASE("negative") {
        Fraction<int> f(-3, 4);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(-3/4)");
    }

    SUBCASE("zero") {
        Fraction<int> f(0, 1);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(0/1)");
    }

    SUBCASE("unity") {
        Fraction<int> f(1, 1);
        std::stringstream ss;
        ss << f;
        CHECK_EQ(ss.str(), "(1/1)");
    }

    SUBCASE("multiple in stream") {
        Fraction<int> a(1, 2);
        Fraction<int> b(3, 4);
        std::stringstream ss;
        ss << a << " + " << b;
        CHECK_EQ(ss.str(), "(1/2) + (3/4)");
    }
}

TEST_CASE("Fraction<int> with different int types") {
    SUBCASE("long") {
        Fraction<long> f(2, 4);
        CHECK_EQ(f.numer(), 1);
        CHECK_EQ(f.denom(), 2);
    }
}

TEST_CASE("Fraction<int> rvalue operators") {
    Fraction<int> f(1, 2);

    SUBCASE("int&& + Fraction") {
        auto r = 1 + f;
        CHECK_EQ(r, Fraction<int>(3, 2));
    }

    SUBCASE("int&& - Fraction") {
        auto r = 1 - f;
        CHECK_EQ(r, Fraction<int>(1, 2));
    }

    SUBCASE("int&& * Fraction") {
        auto r = 2 * f;
        CHECK_EQ(r, Fraction<int>(1, 1));
    }

    SUBCASE("int&& comparison") {
        Fraction<int> one(1, 1);
        CHECK(1 == one);
    }
}

TEST_CASE("Fraction<int> addition same denominator shortcut") {
    SUBCASE("+ with same denominator") {
        Fraction<int> a(1, 4);
        Fraction<int> b(2, 4);
        CHECK_EQ(a + b, Fraction<int>(3, 4));
    }

    SUBCASE("+= with same denominator") {
        auto f = Fraction<int>(1, 4);
        f += Fraction<int>(2, 4);
        CHECK_EQ(f, Fraction<int>(3, 4));
    }

    SUBCASE("-= with same denominator") {
        auto f = Fraction<int>(3, 4);
        f -= Fraction<int>(1, 4);
        CHECK_EQ(f, Fraction<int>(1, 2));
    }
}

TEST_CASE("Fraction<int> complex expressions") {
    Fraction<int> a(1, 2);
    Fraction<int> b(1, 3);
    Fraction<int> c(1, 4);

    SUBCASE("chain operations") {
        auto f = Fraction<int>(1, 2);
        f += Fraction<int>(1, 3);
        f *= Fraction<int>(2, 5);
        f -= Fraction<int>(1, 6);
        f /= Fraction<int>(2, 3);
        CHECK_EQ(f, Fraction<int>(1, 4));
    }

    SUBCASE("nested expression") {
        auto r = ((a + b) * c) / a;
        CHECK_EQ(r, Fraction<int>(5, 12));
    }
}

TEST_CASE("Fraction<int> operator- with T") {
    Fraction<int> a(1, 2);

    SUBCASE("int - Fraction") {
        CHECK_EQ(1 - a, Fraction<int>(1, 2));
        CHECK_EQ(0 - a, Fraction<int>(-1, 2));
    }

    SUBCASE("int&& - Fraction") {
        CHECK_EQ(3 - a, Fraction<int>(5, 2));
    }
}

TEST_CASE("Fraction<int> operator< Fraction < T edge cases") {
    SUBCASE("denom == 1 path") {
        Fraction<int> f(3, 1);
        CHECK_LT(f, 5);
    }

    SUBCASE("denom != 1 && rhs == 0 path") {
        Fraction<int> neg(-1, 2);
        CHECK_LT(neg, 0);
    }

    SUBCASE("denom != 1 && rhs != 0") {
        Fraction<int> f(1, 2);
        CHECK_LT(f, 1);
        CHECK_FALSE(f < 0);
    }
}

TEST_CASE("Fraction<int> operator< T < Fraction edge cases") {
    SUBCASE("rhs denom == 1 path") {
        Fraction<int> f(3, 1);
        CHECK_LT(0, f);
    }

    SUBCASE("rhs denom != 1 && lhs == 0 path") {
        Fraction<int> f(1, 2);
        CHECK_LT(0, f);
    }

    SUBCASE("rhs denom != 1 && lhs != 0") {
        Fraction<int> f(1, 2);
        CHECK_LT(-1, f);
    }
}

TEST_CASE("Fraction<int> operator> edge cases") {
    Fraction<int> f(1, 2);

    SUBCASE("T > Fraction") {
        CHECK_GT(1, f);
        CHECK_FALSE(0 > f);
    }

    SUBCASE("Fraction > T") {
        CHECK_GT(f, 0);
        CHECK_FALSE(f > 1);
    }
}

TEST_CASE("Fraction<int> comparison completeness ordering") {
    Fraction<int> vals[] = {
        Fraction<int>(-3, 2),
        Fraction<int>(-1, 1),
        Fraction<int>(-1, 2),
        Fraction<int>(0, 1),
        Fraction<int>(1, 3),
        Fraction<int>(1, 2),
        Fraction<int>(2, 3),
        Fraction<int>(1, 1),
        Fraction<int>(3, 2),
    };

    for (int i = 0; i < 8; ++i) {
        CHECK_LT(vals[i], vals[i + 1]);
        CHECK_GT(vals[i + 1], vals[i]);
        CHECK_LE(vals[i], vals[i + 1]);
        CHECK_GE(vals[i + 1], vals[i]);
        CHECK_NE(vals[i], vals[i + 1]);
    }

    for (auto& v : vals) {
        CHECK_EQ(v, v);
        CHECK_LE(v, v);
        CHECK_GE(v, v);
        CHECK_FALSE(v != v);
    }
}

TEST_CASE("Fraction<long long> reduce and normalize with large numbers") {
    long long num = 2000000000LL;
    long long den = 4000000000LL;
    Fraction<long long> f(num, den);
    CHECK_EQ(f.numer(), 1);
    CHECK_EQ(f.denom(), 2);
}

TEST_CASE("Fraction<int> cross product for comparison") {
    Fraction<int> a(1, 2);
    Fraction<int> b(1, 3);

    CHECK_GT(a.cross(b), 0);
    CHECK_LT(b.cross(a), 0);
}

TEST_CASE("Fraction<int> zero operations") {
    Fraction<int> zero(0, 1);
    Fraction<int> pos(3, 4);

    CHECK_EQ(zero + zero, zero);
    CHECK_EQ(zero - zero, zero);
    CHECK_EQ(zero * zero, zero);
    CHECK_EQ(zero + pos, pos);
    CHECK_EQ(pos + zero, pos);
    CHECK_EQ(zero * pos, zero);
    CHECK_EQ(pos * zero, zero);
    CHECK_EQ(zero / pos, zero);
    CHECK_EQ(pos - zero, pos);
}

TEST_CASE("Fraction<int> unity operations") {
    Fraction<int> one(1, 1);
    Fraction<int> pos(3, 4);

    CHECK_EQ(one * one, one);
    CHECK_EQ(one / one, one);
    CHECK_EQ(one + one, Fraction<int>(2, 1));
    CHECK_EQ(one - one, Fraction<int>(0, 1));
    CHECK_EQ(one * pos, pos);
    CHECK_EQ(pos * one, pos);
    CHECK_EQ(one / pos, Fraction<int>(4, 3));
    CHECK_EQ(pos / one, pos);
}

TEST_CASE("Fraction<int> negative fractions") {
    Fraction<int> n1(-1, 2);
    Fraction<int> n2(-3, 4);
    Fraction<int> pos(1, 2);

    CHECK_EQ(n1 + n2, Fraction<int>(-5, 4));
    CHECK_EQ(n1 - n2, Fraction<int>(1, 4));
    CHECK_EQ(n1 * n2, Fraction<int>(3, 8));
    CHECK_EQ(n1 / n2, Fraction<int>(2, 3));
    CHECK_EQ(n1 + pos, Fraction<int>(0, 1));
    CHECK_EQ(pos + n1, Fraction<int>(0, 1));
    CHECK_EQ(-n1, pos);
    CHECK_EQ(-pos, n1);
}

TEST_CASE("Fraction<int> cross with negative") {
    Fraction<int> n1(-1, 2);
    Fraction<int> n2(-1, 3);
    CHECK_EQ(n1.cross(n2), -1);
    CHECK_EQ(n2.cross(n1), 1);
}



TEST_CASE("Fraction<int> divide by self gives unity") {
    Fraction<int> f(3, 5);
    CHECK_EQ(f / f, Fraction<int>(1, 1));
}

TEST_CASE("Fraction<int> subtract self gives zero") {
    Fraction<int> f(3, 5);
    CHECK_EQ(f - f, Fraction<int>(0, 1));
}

TEST_CASE("Fraction<int> multiply by reciprocal gives unity") {
    Fraction<int> f(3, 5);
    auto r = f;
    r.reciprocal();
    CHECK_EQ(f * r, Fraction<int>(1, 1));
}

TEST_CASE("Fraction<int> large denominators") {
    SUBCASE("large denominator addition") {
        Fraction<int> a(1, 1000000);
        Fraction<int> b(1, 1000000);
        CHECK_EQ(a + b, Fraction<int>(1, 500000));
    }

    SUBCASE("large denominator multiplication") {
        Fraction<int> a(1, 1000);
        Fraction<int> b(1, 1000);
        CHECK_EQ(a * b, Fraction<int>(1, 1000000));
    }
}

TEST_CASE("Fraction<int> gcd based addition") {
    Fraction<int> a(1, 6);
    Fraction<int> b(1, 4);
    CHECK_EQ(a + b, Fraction<int>(5, 12));
}

TEST_CASE("Fraction<int> division 0/0") {
    Fraction<int> zero(0, 1);
    Fraction<int> result = zero / zero;
    CHECK_EQ(result.numer(), 0);
    CHECK_EQ(result.denom(), 1);
}

TEST_CASE("abs free function with unsigned") {
    CHECK_EQ(abs(5U), 5U);
    CHECK_EQ(abs(0U), 0U);
    CHECK_EQ(abs(255U), 255U);
}

TEST_CASE("abs free function with signed") {
    CHECK_EQ(abs(5), 5);
    CHECK_EQ(abs(-5), 5);
    CHECK_EQ(abs(0), 0);
}

TEST_CASE("gcd_recur free function") {
    CHECK_EQ(gcd_recur(12, 8), 4);
    CHECK_EQ(gcd_recur(8, 12), 4);
    CHECK_EQ(gcd_recur(0, 8), 8);
    CHECK_EQ(gcd_recur(8, 0), 8);
    CHECK_EQ(gcd_recur(0, 0), 0);
    CHECK_EQ(gcd_recur(13, 5), 1);
    CHECK_EQ(gcd_recur(-12, 8), 4);
    CHECK_EQ(gcd_recur(12, -8), 4);
    CHECK_EQ(gcd_recur(-12, -8), 4);
}

TEST_CASE("gcd free function") {
    CHECK_EQ(gcd(12, 8), 4);
    CHECK_EQ(gcd(0, 8), 8);
    CHECK_EQ(gcd(8, 0), 8);
    CHECK_EQ(gcd(0, 0), 0);
    CHECK_EQ(gcd(-12, 8), 4);
}

TEST_CASE("lcm free function") {
    CHECK_EQ(lcm(12, 8), 24);
    CHECK_EQ(lcm(0, 8), 0);
    CHECK_EQ(lcm(8, 0), 0);
    CHECK_EQ(lcm(0, 0), 0);
    CHECK_EQ(lcm(-12, 8), 24);
    CHECK_EQ(lcm(13, 5), 65);
}

TEST_CASE("abs with ExtFraction via fractions.hpp includes") {
    ExtFraction<int> pos(3, 4);
    CHECK_EQ(abs(pos), pos);

    ExtFraction<int> neg(-3, 4);
    CHECK_EQ(abs(neg), ExtFraction<int>(3, 4));

    ExtFraction<int> zero(0, 1);
    CHECK_EQ(abs(zero), zero);
}
