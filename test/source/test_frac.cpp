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
  const auto p = Fraction<int>{3, 4};
  const auto inf = Fraction<int>{1, 0};
  const auto nan = Fraction<int>{0, 0};
  const auto zero = Fraction<int>{0, 1};

  CHECK(-inf < zero);
  CHECK(zero < inf);
  CHECK(-inf < p);
  CHECK(p < inf);
  CHECK(inf == inf);
  CHECK(-inf < inf);
  CHECK(inf == inf * p);
  CHECK(inf == inf * inf);
  CHECK(inf == p / zero);
  CHECK(inf == inf / zero);
  CHECK(nan == nan);
  CHECK(nan == inf * zero);
  CHECK(nan == -inf * zero);
  CHECK(nan == inf / inf);
  CHECK(nan == nan * zero);
  CHECK(nan == nan * nan);
  CHECK(inf == inf + inf);
  CHECK(nan == inf - inf);
  CHECK(inf - p == inf);
  CHECK(-inf + p == -inf);
}
