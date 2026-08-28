#include <doctest/doctest.h>

#include <fractions/fractions.hpp>
#include <ostream>

using namespace fractions;

TEST_CASE("Fraction<int> factories") {
    const auto f = Fraction<int>::from(2, 4);
    CHECK_EQ(f.numer(), 1);
    CHECK_EQ(f.denom(), 2);
    const auto r = Fraction<int>::from_raw(2, 4);
    CHECK_EQ(r.numer(), 2);
    CHECK_EQ(r.denom(), 4);
    CHECK_EQ(Fraction<int>::zero(), Fraction<int>(0, 1));
    CHECK_EQ(Fraction<int>::one(), Fraction<int>(1, 1));
}

TEST_CASE("Fraction<int> predicates and setters") {
    auto f = Fraction<int>(3, 4);
    CHECK(f.is_positive());
    CHECK_FALSE(f.is_negative());
    CHECK_FALSE(f.is_zero());
    CHECK_FALSE(f.is_one());
    CHECK_FALSE(f.is_infinite());
    CHECK_FALSE(f.is_nan());
    f.set_zero();
    CHECK(f.is_zero());
    f.set_one();
    CHECK(f.is_one());
    f.set_infinite();
    CHECK(f.is_infinite());
    f.set_nan();
    CHECK(f.is_nan());
    const auto neg = Fraction<int>(-3, 4);
    CHECK(neg.is_negative());
}

TEST_CASE("Fraction<int> abs signum inv") {
    const auto f = Fraction<int>(3, 4);
    CHECK_EQ(f.abs(), f);
    CHECK_EQ((-f).abs(), f);
    CHECK_EQ(f.signum(), Fraction<int>::one());
    CHECK_EQ((-f).signum(), -Fraction<int>::one());
    CHECK_EQ(Fraction<int>::zero().signum(), Fraction<int>::zero());
    CHECK_EQ(f.inv(), Fraction<int>(4, 3));
}

TEST_CASE("Fraction<int> scalar inequality") {
    const auto f = Fraction<int>(3, 4);
    CHECK(f != 1);
    CHECK(1 != f);
    CHECK_FALSE(f != Fraction<int>(3, 4));
}