Sure, here's an example of a generic Fraction class in C++ that accepts different negative integer types, along with a corresponding test module:

Fraction.h

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
class Fraction {
public:
    Fraction(T num, T den) : num_(num), den_(den) {}


    T getNum() { return num_; }
    T getDen() { return den_; }

    Fraction<T> reduce() {
        T gcd = std::gcd(num_, den_);
        return Fraction<T>(num_ / gcd, den_ / gcd);
    }

    Fraction<T> add(const Fraction<T>& other) {
        return Fraction<T>(num_ * other.den_ + other.num_ * den_, den_ * other.den_);
    }

    Fraction<T> sub(const Fraction<T>& other) {
        return Fraction<T>(num_ * other.den_ - other.num_ * den_, den_ * other.den_);
    }

    Fraction<T> mul(const Fraction<T>& other) {
        return Fraction<T>(num_ * other.num_, den_ * other.den_);
    }

    Fraction<T> div(const Fraction<T>& other) {
        return Fraction<T>(num_ * other.den_, den_ * other.num_);
    }

private:
    T num_;
    T den_;
};
```

FractionTest.cpp

```
#include <gtest/gtest.h>
#include "Fraction.h"

TEST(FractionTest, Construction) {
    Fraction<int> f1(1, 2);
    EXPECT_EQ(f1.getNum(), 1);
    EXPECT_EQ(f1.getDen(), 2);
}

TEST(FractionTest, Reduction) {
    Fraction<int> f1(4, 8);
    Fraction<int> f2 = f1.reduce();
    EXPECT_EQ(f2.getNum(), 1);
    EXPECT_EQ(f2.getDen(), 2);
}

TEST(FractionTest, Addition) {
    Fraction<int> f1(1, 2);
    Fraction<int> f2(3, 4);
    Fraction<int> sum = f1.add(f2);
    EXPECT_EQ(sum.getNum(), 7);
    EXPECT_EQ(sum.getDen(), 8);
}

TEST(FractionTest, Subtraction) {
    Fraction<int> f1(1, 2);
    Fraction<int> f2(3, 4);
    Fraction<int> diff = f1.sub(f2);
    EXPECT_EQ(diff.getNum(), -1);
    EXPECT_EQ(diff.getDen(), 2);
}

TEST(FractionTest, Multiplication) {
    Fraction<int> f1(1, 2);
    Fraction<int> f2(3, 4);
    Fraction<int> product = f1.mul(f2);
    EXPECT_EQ(product.getNum(), 3);
    EXPECT_EQ(product.getDen(), 8);
}

TEST(FractionTest, Division) {
    Fraction<int> f1(1, 2);
    Fraction<int> f2(3, 4);
    Fraction<int> quotient = f1.div(f2);
    EXPECT_EQ(quotient.getNum(), 2);
    EXPECT_EQ(quotient.getDen(), 3);
}

TEST(FractionTest, NegativeNumbers) {
    Fraction<int> f1(-1, 2);
    Fraction<int> f2(3, 4);
    Fraction<int> sum = f1.add(f2);
    EXPECT_EQ(sum.getNum(), -1);
    EXPECT_EQ(sum.getDen(), 2);
}
```

In this implementation, we've made the `Fraction` class generic over an integer type `T`. We've also updated the constructor to take two parameters of type `T` and `T` respectively. Additionally, we've updated the `reduce` method to return a `Fraction` object with the reduced numerator and denominator.

In the test module, we've written six test cases that exercise the different methods of the `Fraction` class, just like in the previous example. However, this time we've used the `int` type to create the fractions.

Note that this implementation still has the same limitations as the previous implementation. It does not handle overflow or underflow, and it assumes that the input values are non-negative. Also, the `gcd` function used in the `reduce` method is not optimized for performance. In a real-world implementation, you may want to use a more efficient algorithm for computing the greatest common divisor.

Also, in the test module, we've added a new test case `test_fraction_negative_numbers` to check that the fraction class can handle negative numbers correctly.

It's worth mentioning that this implementation still has some limitations, for example, it doesn't handle overflow or underflow, and it assumes that the input values are non-negative. Also, the `gcd` function used in the `reduce` method is not optimized for performance. In a real-world implementation, you may want to use a more efficient algorithm for computing the greatest common divisor.
