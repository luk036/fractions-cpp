#include <doctest/doctest.h>

#include <fractions/fractions.hpp>
#include <limits>
#include <ostream>

using namespace fractions;

TEST_CASE("Fraction<long long> stress tests") {
    SUBCASE("Large numbers") {
        long long large_num = std::numeric_limits<long long>::max() / 2;
        long long large_den = std::numeric_limits<long long>::max() / 3;
        Fraction<long long> f1(large_num, large_den);
        Fraction<long long> f2(large_den, large_num);

        CHECK_NOTHROW(f1 + f2);
        CHECK_NOTHROW(f1 - f2);
        CHECK_NOTHROW(f1 * f2);
        CHECK_NOTHROW(f1 / f2);
    }

    SUBCASE("Repeated operations") {
        Fraction<long long> f(1, 2);
        for (int i = 0; i < 1000; ++i) {
            f += Fraction<long long>(1, 1000);
        }
        // The result should be 1/2 + 1000/1000 = 1/2 + 1 = 3/2
        CHECK(f == Fraction<long long>(3, 2));
    }

    SUBCASE("Normalization with large numbers") {
        long long large_common_divisor = 123456789;
        long long num = 2 * large_common_divisor;
        long long den = 3 * large_common_divisor;
        Fraction<long long> f(num, den);
        CHECK(f.numer() == 2);
        CHECK(f.denom() == 3);
    }
}
