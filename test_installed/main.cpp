#include <fractions/fractions.hpp>
#include <iostream>

auto main() -> int {
    const auto a = fractions::Fraction<int>{1, 2};
    const auto b = fractions::Fraction<int>{1, 3};
    const auto c = a + b;

    std::cout << "fractions-cpp installed test: " << a << " + " << b << " = " << c << "\n";

    return (c == fractions::Fraction<int>{5, 6}) ? 0 : 1;
}
