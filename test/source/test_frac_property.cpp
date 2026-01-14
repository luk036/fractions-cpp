/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>
#include <rapidcheck.h>
#include <fractions/fastfractions.hpp>
#include <sstream>

using namespace fractions;

// Property-based tests for FastFraction class
// These tests verify mathematical properties that should hold for all valid inputs

TEST_CASE("Property-based tests for FastFraction operations") {
    
    SUBCASE("Addition commutativity: a + b == b + a") {
        rc::check("fraction addition is commutative", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-100, 100);
            const auto b_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            
            RC_ASSERT(a + b == b + a);
        });
    }
    
    SUBCASE("Addition associativity: (a + b) + c == a + (b + c)") {
        rc::check("fraction addition is associative", []() {
            const auto a_num = *rc::gen::inRange(-50, 50);
            const auto a_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-50, 50);
            const auto b_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto c_num = *rc::gen::inRange(-50, 50);
            const auto c_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            FastFraction<int> c(c_num, c_den);
            
            RC_ASSERT((a + b) + c == a + (b + c));
        });
    }
    
    SUBCASE("Addition identity: a + 0 == a") {
        rc::check("zero is additive identity", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> zero(0, 1);
            
            RC_ASSERT(a + zero == a);
            RC_ASSERT(zero + a == a);
        });
    }
    
    SUBCASE("Multiplication commutativity: a * b == b * a") {
        rc::check("fraction multiplication is commutative", []() {
            const auto a_num = *rc::gen::inRange(-50, 50);
            const auto a_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-50, 50);
            const auto b_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            
            RC_ASSERT(a * b == b * a);
        });
    }
    
    SUBCASE("Multiplication associativity: (a * b) * c == a * (b * c)") {
        rc::check("fraction multiplication is associative", []() {
            const auto a_num = *rc::gen::inRange(-20, 20);
            const auto a_den = *rc::gen::inRange(1, 20);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-20, 20);
            const auto b_den = *rc::gen::inRange(1, 20);  // Avoid zero denominator
            const auto c_num = *rc::gen::inRange(-20, 20);
            const auto c_den = *rc::gen::inRange(1, 20);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            FastFraction<int> c(c_num, c_den);
            
            RC_ASSERT((a * b) * c == a * (b * c));
        });
    }
    
    SUBCASE("Multiplication identity: a * 1 == a") {
        rc::check("one is multiplicative identity", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> one(1, 1);
            
            RC_ASSERT(a * one == a);
            RC_ASSERT(one * a == a);
        });
    }
    
    SUBCASE("Distributive property: a * (b + c) == a * b + a * c") {
        rc::check("multiplication distributes over addition", []() {
            const auto a_num = *rc::gen::inRange(-30, 30);
            const auto a_den = *rc::gen::inRange(1, 30);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-30, 30);
            const auto b_den = *rc::gen::inRange(1, 30);  // Avoid zero denominator
            const auto c_num = *rc::gen::inRange(-30, 30);
            const auto c_den = *rc::gen::inRange(1, 30);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            FastFraction<int> c(c_num, c_den);
            
            RC_ASSERT(a * (b + c) == a * b + a * c);
        });
    }
    
    SUBCASE("Subtraction as addition of inverse: a - b == a + (-b)") {
        rc::check("subtraction equals addition of negation", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-100, 100);
            const auto b_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            
            RC_ASSERT(a - b == a + (-b));
        });
    }
    
    SUBCASE("Division as multiplication by reciprocal: a / b == a * (1/b)") {
        rc::check("division equals multiplication by reciprocal", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-100, -1);  // Non-zero numerator
            const auto b_den = *rc::gen::inRange(1, 100);   // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            FastFraction<int> b_recip = b;
            b_recip.reciprocal();
            
            RC_ASSERT(a / b == a * b_recip);
        });
    }
    
    SUBCASE("Double negation: -(-a) == a") {
        rc::check("double negation returns original", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            
            RC_ASSERT(-(-a) == a);
        });
    }
    
    SUBCASE("Reciprocal of reciprocal: reciprocal(reciprocal(a)) == a") {
        rc::check("double reciprocal returns original", []() {
            const auto a_num = *rc::gen::inRange(1, 100);  // Non-zero numerator
            const auto a_den = *rc::gen::inRange(1, 100);  // Non-zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> a_recip = a;
            a_recip.reciprocal();
            a_recip.reciprocal();
            
            RC_ASSERT(a_recip == a);
        });
    }
    
    SUBCASE("Multiplication by zero: a * 0 == 0") {
        rc::check("multiplication by zero yields zero", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> zero(0, 1);
            
            RC_ASSERT(a * zero == zero);
            RC_ASSERT(zero * a == zero);
        });
    }
    
    SUBCASE("Addition of inverse: a + (-a) == 0") {
        rc::check("adding a number to its inverse yields zero", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> zero(0, 1);
            
            RC_ASSERT(a + (-a) == zero);
        });
    }
    
    SUBCASE("Cross product property: a.cross(b) == -b.cross(a)") {
        rc::check("cross product is anti-symmetric", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-100, 100);
            const auto b_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            
            RC_ASSERT(a.cross(b) == -b.cross(a));
        });
    }
    
    SUBCASE("Cross product with equal fractions: a.cross(a) == 0") {
        rc::check("cross product of equal fractions is zero", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            
            RC_ASSERT(a.cross(a) == 0);
        });
    }
    
    SUBCASE("Normalization consistency: equivalent fractions are equal") {
        rc::check("equivalent fractions are equal after normalization", []() {
            const auto a_num = *rc::gen::inRange(-50, 50);
            const auto a_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto multiplier = *rc::gen::inRange(-10, 10);
            RC_PRE(multiplier != 0);  // Multiplier must not be zero
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(a_num * multiplier, a_den * multiplier);
            
            RC_ASSERT(a == b);
        });
    }
    
    SUBCASE("Comparison transitivity: if a < b and b < c then a < c") {
        rc::check("less than is transitive", []() {
            const auto a_num = *rc::gen::inRange(-50, 50);
            const auto a_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto b_num = *rc::gen::inRange(-50, 50);
            const auto b_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            const auto c_num = *rc::gen::inRange(-50, 50);
            const auto c_den = *rc::gen::inRange(1, 50);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> b(b_num, b_den);
            FastFraction<int> c(c_num, c_den);
            
            // Only test when the precondition holds
            RC_PRE(a < b && b < c);
            
            RC_ASSERT(a < c);
        });
    }
    
    SUBCASE("Absolute value property: abs(a) >= 0") {
        rc::check("absolute value is non-negative", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> a_abs = abs(a);
            
            RC_ASSERT(a_abs >= FastFraction<int>(0, 1));
        });
    }
    
    SUBCASE("Absolute value of absolute value: abs(abs(a)) == abs(a)") {
        rc::check("absolute value is idempotent", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> a_abs = abs(a);
            
            RC_ASSERT(abs(a_abs) == a_abs);
        });
    }
}

TEST_CASE("Property-based tests for edge cases") {
    
    SUBCASE("Zero fraction properties") {
        rc::check("zero fraction has special properties", []() {
            const auto a_num = *rc::gen::inRange(-100, 100);
            const auto a_den = *rc::gen::inRange(1, 100);  // Avoid zero denominator
            
            FastFraction<int> a(a_num, a_den);
            FastFraction<int> zero(0, 1);
            
            RC_ASSERT(a + zero == a);
            RC_ASSERT(a * zero == zero);
            RC_ASSERT(zero * a == zero);
            
            // Zero divided by non-zero is zero
            RC_ASSERT(zero / a == zero);
        });
    }
    
    SUBCASE("Infinity-like properties (zero denominator)") {
        rc::check("fractions with zero denominator behave consistently", []() {
            const auto a_num = *rc::gen::inRange(-10, -1);  // Non-zero numerator
            const auto b_num = *rc::gen::inRange(1, 10);    // Non-zero numerator
            
            FastFraction<int> infinity_a(a_num, 0);
            FastFraction<int> infinity_b(b_num, 0);
            FastFraction<int> zero(0, 1);
            
            // Infinity times any non-zero should be infinity-like
            FastFraction<int> normal(1, 2);
            FastFraction<int> result1 = infinity_a * normal;
            RC_ASSERT(result1.denom() == 0);
            
            // Infinity divided by infinity should be NaN-like
            FastFraction<int> result2 = infinity_a / infinity_b;
            RC_ASSERT(result2.numer() == 0 && result2.denom() == 0);
            
            // Infinity times zero should be NaN-like
            FastFraction<int> result3 = infinity_a * zero;
            RC_ASSERT(result3.numer() == 0 && result3.denom() == 0);
        });
    }
}

TEST_CASE("Property-based tests for GCD and LCM functions") {
    
    SUBCASE("GCD commutativity: gcd(a, b) == gcd(b, a)") {
        rc::check("gcd is commutative", []() {
            const auto a = *rc::gen::inRange(-1000, 1000);
            const auto b = *rc::gen::inRange(-1000, 1000);
            RC_PRE(a != 0 || b != 0);  // At least one is non-zero
            
            RC_ASSERT(gcd(a, b) == gcd(b, a));
        });
    }
    
    SUBCASE("GCD associativity: gcd(gcd(a, b), c) == gcd(a, gcd(b, c))") {
        rc::check("gcd is associative", []() {
            const auto a = *rc::gen::inRange(-500, 500);
            const auto b = *rc::gen::inRange(-500, 500);
            const auto c = *rc::gen::inRange(-500, 500);
            RC_PRE(a != 0 || b != 0 || c != 0);  // At least one is non-zero
            
            RC_ASSERT(gcd(gcd(a, b), c) == gcd(a, gcd(b, c)));
        });
    }
    
    SUBCASE("GCD identity: gcd(a, 0) == |a|") {
        rc::check("zero is identity for gcd", []() {
            const auto a = *rc::gen::inRange(-1000, 1000);
            
            RC_ASSERT(gcd(a, 0) == abs(a));
            RC_ASSERT(gcd(0, a) == abs(a));
        });
    }
    
    SUBCASE("LCM commutativity: lcm(a, b) == lcm(b, a)") {
        rc::check("lcm is commutative", []() {
            const auto a = *rc::gen::inRange(-100, 100);
            const auto b = *rc::gen::inRange(-100, 100);
            RC_PRE(a != 0 && b != 0);  // Both non-zero for LCM
            
            RC_ASSERT(lcm(a, b) == lcm(b, a));
        });
    }
    
    SUBCASE("LCM zero property: lcm(a, 0) == 0") {
        rc::check("lcm with zero is zero", []() {
            const auto a = *rc::gen::inRange(-1000, 1000);
            
            RC_ASSERT(lcm(a, 0) == 0);
            RC_ASSERT(lcm(0, a) == 0);
        });
    }
    
    SUBCASE("GCD-LCM relationship: gcd(a, b) * lcm(a, b) == |a * b|") {
        rc::check("gcd and lcm relationship", []() {
            const auto a = *rc::gen::inRange(1, 1000);  // Positive to avoid overflow
            const auto b = *rc::gen::inRange(1, 1000);  // Positive to avoid overflow
            RC_PRE(a != 0 && b != 0);  // Both non-zero
            
            RC_ASSERT(gcd(a, b) * lcm(a, b) == abs(a * b));
        });
    }
}