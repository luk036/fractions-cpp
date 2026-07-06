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

