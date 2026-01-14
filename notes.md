Sure, here's an example of a generic FastFraction class in C++ that accepts different negative integer types, along with a corresponding test module:

FastFraction.h

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
class FastFraction {
public:
    FastFraction(T num, T den) : num_(num), den_(den) {}


    T getNum() { return num_; }
    T getDen() { return den_; }

    FastFraction<T> reduce() {
        T gcd = std::gcd(num_, den_);
        return FastFraction<T>(num_ / gcd, den_ / gcd);
    }

    FastFraction<T> add(const FastFraction<T>& other) {
        return FastFraction<T>(num_ * other.den_ + other.num_ * den_, den_ * other.den_);
    }

    FastFraction<T> sub(const FastFraction<T>& other) {
        return FastFraction<T>(num_ * other.den_ - other.num_ * den_, den_ * other.den_);
    }

    FastFraction<T> mul(const FastFraction<T>& other) {
        return FastFraction<T>(num_ * other.num_, den_ * other.den_);
    }

    FastFraction<T> div(const FastFraction<T>& other) {
        return FastFraction<T>(num_ * other.den_, den_ * other.num_);
    }

private:
    T num_;
    T den_;
};
```

FractionTest.cpp

```
#include <gtest/gtest.h>
#include "FastFraction.h"

TEST(FractionTest, Construction) {
    FastFraction<int> f1(1, 2);
    EXPECT_EQ(f1.getNum(), 1);
    EXPECT_EQ(f1.getDen(), 2);
}

TEST(FractionTest, Reduction) {
    FastFraction<int> f1(4, 8);
    FastFraction<int> f2 = f1.reduce();
    EXPECT_EQ(f2.getNum(), 1);
    EXPECT_EQ(f2.getDen(), 2);
}

TEST(FractionTest, Addition) {
    FastFraction<int> f1(1, 2);
    FastFraction<int> f2(3, 4);
    FastFraction<int> sum = f1.add(f2);
    EXPECT_EQ(sum.getNum(), 7);
    EXPECT_EQ(sum.getDen(), 8);
}

TEST(FractionTest, Subtraction) {
    FastFraction<int> f1(1, 2);
    FastFraction<int> f2(3, 4);
    FastFraction<int> diff = f1.sub(f2);
    EXPECT_EQ(diff.getNum(), -1);
    EXPECT_EQ(diff.getDen(), 2);
}

TEST(FractionTest, Multiplication) {
    FastFraction<int> f1(1, 2);
    FastFraction<int> f2(3, 4);
    FastFraction<int> product = f1.mul(f2);
    EXPECT_EQ(product.getNum(), 3);
    EXPECT_EQ(product.getDen(), 8);
}

TEST(FractionTest, Division) {
    FastFraction<int> f1(1, 2);
    FastFraction<int> f2(3, 4);
    FastFraction<int> quotient = f1.div(f2);
    EXPECT_EQ(quotient.getNum(), 2);
    EXPECT_EQ(quotient.getDen(), 3);
}

TEST(FractionTest, NegativeNumbers) {
    FastFraction<int> f1(-1, 2);
    FastFraction<int> f2(3, 4);
    FastFraction<int> sum = f1.add(f2);
    EXPECT_EQ(sum.getNum(), -1);
    EXPECT_EQ(sum.getDen(), 2);
}
```

In this implementation, we've made the `FastFraction` class generic over an integer type `T`. We've also updated the constructor to take two parameters of type `T` and `T` respectively. Additionally, we've updated the `reduce` method to return a `FastFraction` object with the reduced numerator and denominator.

In the test module, we've written six test cases that exercise the different methods of the `FastFraction` class, just like in the previous example. However, this time we've used the `int` type to create the fractions.

Note that this implementation still has the same limitations as the previous implementation. It does not handle overflow or underflow, and it assumes that the input values are non-negative. Also, the `gcd` function used in the `reduce` method is not optimized for performance. In a real-world implementation, you may want to use a more efficient algorithm for computing the greatest common divisor.

Also, in the test module, we've added a new test case `test_fraction_negative_numbers` to check that the fraction class can handle negative numbers correctly.

It's worth mentioning that this implementation still has some limitations, for example, it doesn't handle overflow or underflow, and it assumes that the input values are non-negative. Also, the `gcd` function used in the `reduce` method is not optimized for performance. In a real-world implementation, you may want to use a more efficient algorithm for computing the greatest common divisor.
