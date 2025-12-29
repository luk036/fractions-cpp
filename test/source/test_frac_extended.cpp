
#include <doctest/doctest.h>
#include <fractions/fractions.hpp>
#include <sstream>

using namespace fractions;

template <typename T, typename U>
auto add(const Fraction<T>& lhs, const Fraction<U>& rhs) {
    using common_type = decltype(T() + U());
    auto common_denom = lcm(static_cast<common_type>(lhs.denom()), static_cast<common_type>(rhs.denom()));
    auto lhs_numer = static_cast<common_type>(lhs.numer()) * (common_denom / static_cast<common_type>(lhs.denom()));
    auto rhs_numer = static_cast<common_type>(rhs.numer()) * (common_denom / static_cast<common_type>(rhs.denom()));
    return Fraction<common_type>(lhs_numer + rhs_numer, common_denom);
}

template <typename T, typename U>
auto operator+(const Fraction<T>& lhs, const Fraction<U>& rhs) {
    return add(lhs, rhs);
}

namespace fractions {
    template <> CONSTEXPR14 auto gcd_recur<short>(const short &__m, const short &__n) -> short {
        if (__n == 0) {
            return abs(__m);
        }
        return gcd_recur<short>(__n, __m % __n);
    }
}

TEST_CASE("Fraction with mixed types") {
    Fraction<int> f_int(1, 2);
    Fraction<long> f_long(3, 4);

    SUBCASE("int and long") {
        auto result = f_int + f_long;
        CHECK(result.numer() == 5);
        CHECK(result.denom() == 4);
    }

    SUBCASE("int and short") {
        Fraction<short> f_short(1, 4);
        auto result = f_int + f_short;
        CHECK(result.numer() == 3);
        CHECK(result.denom() == 4);
    }
}

TEST_CASE("Stream insertion") {
    Fraction<int> frac(3, 4);
    std::stringstream ss;
    ss << frac;
    CHECK(ss.str() == "(3/4)");
}



TEST_CASE("Large numbers") {
    Fraction<long long> frac1(1000000000000LL, 2000000000000LL);
    frac1.normalize();
    CHECK(frac1.numer() == 1);
    CHECK(frac1.denom() == 2);
}
