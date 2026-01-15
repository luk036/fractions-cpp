#include <doctest/doctest.h>

#include <fractions/extfractions.hpp>
#include <limits>
#include <ostream>

using namespace fractions;

TEST_CASE("ExtFraction<long long> stress tests") {
    SUBCASE("Large numbers") {
        long long large_num = std::numeric_limits<long long>::max() / 2;
        long long large_den = std::numeric_limits<long long>::max() / 3;
        ExtFraction<long long> f1(large_num, large_den);
        ExtFraction<long long> f2(large_den, large_num);

        CHECK_NOTHROW(f1 + f2);
        CHECK_NOTHROW(f1 - f2);
        CHECK_NOTHROW(f1 * f2);
        CHECK_NOTHROW(f1 / f2);
    }

    SUBCASE("Repeated operations") {
        ExtFraction<long long> f(1, 2);
        for (int i = 0; i < 1000; ++i) {
            f += ExtFraction<long long>(1, 1000);
        }
        // The result should be 1/2 + 1000/1000 = 1/2 + 1 = 3/2
        CHECK(f == ExtFraction<long long>(3, 2));
    }

    SUBCASE("Normalization with large numbers") {
        long long large_common_divisor = 123456789;
        long long num = 2 * large_common_divisor;
        long long den = 3 * large_common_divisor;
        ExtFraction<long long> f(num, den);
        CHECK(f.numer() == 2);
        CHECK(f.denom() == 3);
    }
}
