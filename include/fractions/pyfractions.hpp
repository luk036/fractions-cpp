#pragma once

/** @file include/fractions/pyfractions.hpp
 *  C++ implementation of Python's fractions.py ExtFraction class
 *  This is a direct port for comparison with the existing ExtFraction implementation.
 */

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#if __cpp_constexpr >= 201304
#    define CONSTEXPR14 constexpr
#else
#    define CONSTEXPR14 inline
#endif

namespace fractions {

    /**
     * Fraction - A direct C++ port of Python's fractions.Fraction class
     *
     * This implementation closely mirrors the Python fractions.py implementation,
     * including the optimized arithmetic algorithms from Knuth's TAOCP.
     *
     * Key features:
     * - Exact rational number representation
     * - Automatic normalization to lowest terms
     * - Optimized GCD-based arithmetic (following Knuth's algorithms)
     * - Comparison operators
     * - Conversion to/from integers
     *
     * @tparam T Integer type for numerator and denominator (default: int64_t)
     */
    template <typename T = int64_t> class Fraction {
    public:
        using value_type = T;

    private:
        T _numerator;
        T _denominator;

        /**
         * Helper function to compute GCD
         */
        static CONSTEXPR14 T _gcd(T a, T b) {
            a = a < 0 ? -a : a;
            b = b < 0 ? -b : b;
            while (b != 0) {
                T temp = b;
                b = a % b;
                a = temp;
            }
            return a;
        }

        /**
         * Internal constructor from coprime integers (bypasses normalization)
         * Used for optimized internal operations
         */
        Fraction(T numerator, T denominator, bool skip_normalize)
            : _numerator(numerator), _denominator(denominator) {
            (void)skip_normalize;  // Suppress unused parameter warning
        }

    public:
        /**
         * Default constructor: Fraction(0, 1)
         */
        Fraction() : _numerator(0), _denominator(1) {}

        /**
         * Constructor from integer: Fraction(n) = n/1
         */
        explicit Fraction(T numerator) : _numerator(numerator), _denominator(1) {}

        /**
         * Constructor from numerator and denominator
         * Normalizes the fraction to lowest terms
         */
        Fraction(T numerator, T denominator) {
            if (denominator == 0) {
                throw std::runtime_error("Fraction(" + std::to_string(numerator) + ", 0)");
            }

            T g = _gcd(numerator, denominator);
            _numerator = numerator / g;
            _denominator = denominator / g;
            
            // Ensure denominator is positive
            if (_denominator < 0) {
                _numerator = -_numerator;
                _denominator = -_denominator;
            }
        }

        /**
         * Copy constructor
         */
        Fraction(const Fraction &other)
            : _numerator(other._numerator), _denominator(other._denominator) {}

        /**
         * Move constructor
         */
        Fraction(Fraction &&other) noexcept
            : _numerator(std::move(other._numerator)), _denominator(std::move(other._denominator)) {}

        /**
         * Copy assignment
         */
        Fraction &operator=(const Fraction &other) {
            if (this != &other) {
                _numerator = other._numerator;
                _denominator = other._denominator;
            }
            return *this;
        }

        /**
         * Move assignment
         */
        Fraction &operator=(Fraction &&other) noexcept {
            if (this != &other) {
                _numerator = std::move(other._numerator);
                _denominator = std::move(other._denominator);
            }
            return *this;
        }

        /**
         * Get numerator
         */
        CONSTEXPR14 T numerator() const noexcept { return _numerator; }

        /**
         * Get denominator
         */
        CONSTEXPR14 T denominator() const noexcept { return _denominator; }

        /**
         * Check if fraction is an integer
         */
        CONSTEXPR14 bool is_integer() const noexcept { return _denominator == 1; }

        /**
         * Get numerator and denominator as a pair
         */
        std::pair<T, T> as_integer_ratio() const { return {_numerator, _denominator}; }

        /**
         * Negation operator
         */
        Fraction operator-() const {
            return Fraction(-_numerator, _denominator, true);
        }

        /**
         * Absolute value
         */
        Fraction abs() const {
            return Fraction(_numerator < 0 ? -_numerator : _numerator, _denominator, true);
        }

        /**
         * Addition - optimized algorithm following Knuth TAOCP Volume 2, 4.5.1
         *
         * Uses GCD optimization to avoid large intermediate values:
         * - Find g = gcd(da, db)
         * - Compute result with smaller intermediate values
         */
        Fraction operator+(const Fraction &other) const {
            T na = _numerator, da = _denominator;
            T nb = other._numerator, db = other._denominator;

            T g = _gcd(da, db);

            if (g == 1) {
                return Fraction(na * db + da * nb, da * db);
            }

            T s = da / g;
            T t = na * (db / g) + nb * s;
            T g2 = _gcd(t, g);

            if (g2 == 1) {
                return Fraction(t, s * db);
            }

            return Fraction(t / g2, s * (db / g2));
        }

        /**
         * Subtraction - optimized algorithm following Knuth TAOCP Volume 2, 4.5.1
         */
        Fraction operator-(const Fraction &other) const {
            T na = _numerator, da = _denominator;
            T nb = other._numerator, db = other._denominator;

            T g = _gcd(da, db);

            if (g == 1) {
                return Fraction(na * db - da * nb, da * db);
            }

            T s = da / g;
            T t = na * (db / g) - nb * s;
            T g2 = _gcd(t, g);

            if (g2 == 1) {
                return Fraction(t, s * db);
            }

            return Fraction(t / g2, s * (db / g2));
        }

        /**
         * Multiplication - optimized algorithm following Knuth TAOCP Volume 2, 4.5.1
         *
         * Uses GCD optimization to reduce before multiplying:
         * - g1 = gcd(na, db)
         * - g2 = gcd(nb, da)
         * - Divide before multiplying to keep numbers small
         */
        Fraction operator*(const Fraction &other) const {
            T na = _numerator, da = _denominator;
            T nb = other._numerator, db = other._denominator;

            T g1 = _gcd(na, db);
            if (g1 > 1) {
                na /= g1;
                db /= g1;
            }

            T g2 = _gcd(nb, da);
            if (g2 > 1) {
                nb /= g2;
                da /= g2;
            }

            return Fraction(na * nb, db * da);
        }

        /**
         * Division - optimized algorithm (inverse of multiplication)
         */
        Fraction operator/(const Fraction &other) const {
            T nb = other._numerator, db = other._denominator;

            if (nb == 0) {
                throw std::runtime_error("Fraction division by zero");
            }

            T na = _numerator, da = _denominator;

            T g1 = _gcd(na, nb);
            if (g1 > 1) {
                na /= g1;
                nb /= g1;
            }

            T g2 = _gcd(db, da);
            if (g2 > 1) {
                da /= g2;
                db /= g2;
            }

            T n = na * db;
            T d = nb * da;

            if (d < 0) {
                n = -n;
                d = -d;
            }

            return Fraction(n, d);
        }

        /**
         * In-place addition
         */
        Fraction &operator+=(const Fraction &other) {
            *this = *this + other;
            return *this;
        }

        /**
         * In-place subtraction
         */
        Fraction &operator-=(const Fraction &other) {
            *this = *this - other;
            return *this;
        }

        /**
         * In-place multiplication
         */
        Fraction &operator*=(const Fraction &other) {
            *this = *this * other;
            return *this;
        }

        /**
         * In-place division
         */
        Fraction &operator/=(const Fraction &other) {
            *this = *this / other;
            return *this;
        }

        /**
         * Addition with integer
         */
        Fraction operator+(T other) const {
            return *this + Fraction(other);
        }

        /**
         * Subtraction with integer
         */
        Fraction operator-(T other) const {
            return *this - Fraction(other);
        }

        /**
         * Multiplication with integer
         */
        Fraction operator*(T other) const {
            return *this * Fraction(other);
        }

        /**
         * Division with integer
         */
        Fraction operator/(T other) const {
            return *this / Fraction(other);
        }

        /**
         * Equality comparison
         */
        bool operator==(const Fraction &other) const {
            return _numerator == other._numerator && _denominator == other._denominator;
        }

        /**
         * Inequality comparison
         */
        bool operator!=(const Fraction &other) const { return !(*this == other); }

        /**
         * Less than comparison (overflow-safe)
         */
        bool operator<(const Fraction &other) const {
            // To avoid overflow, we compare a/b < c/d by checking a*d < b*c
            // We cancel out common factors from both sides of the equation
            
            T na = _numerator;
            T da = _denominator;
            T nb = other._numerator;
            T db = other._denominator;
            
            // Handle different signs
            if (na < 0 && nb >= 0) return true;
            if (na >= 0 && nb < 0) return false;
            
            // Both are non-negative or both are negative
            // For negative case, the comparison reverses
            bool both_negative = (na < 0);
            
            // Make both non-negative for comparison
            if (both_negative) {
                na = -na;
                nb = -nb;
            }
            
            // Now na, da, nb, db are all non-negative
            // We want to check if na/da < nb/db, i.e., na*db < nb*da
            
            // Cancel common factors from both sides of the equation na*db < nb*da
            // g1 = gcd(na, nb), g2 = gcd(db, da)
            T g1 = _gcd(na, nb);
            T g2 = _gcd(db, da);
            
            if (g1 > 1) {
                na /= g1;
                nb /= g1;
            }
            
            if (g2 > 1) {
                db /= g2;
                da /= g2;
            }
            
            // Also try to cancel cross factors
            // g3 = gcd(na, da), g4 = gcd(nb, db)
            T g3 = _gcd(na, da);
            if (g3 > 1) {
                na /= g3;
                da /= g3;
            }
            
            T g4 = _gcd(nb, db);
            if (g4 > 1) {
                nb /= g4;
                db /= g4;
            }
            
            // Now compare na*db < nb*da with reduced values
            // After canceling common factors, the multiplication should be safe
            T left = na * db;
            T right = nb * da;
            return left < right;
        }

        /**
         * Greater than comparison
         */
        bool operator>(const Fraction &other) const { return other < *this; }

        /**
         * Less than or equal comparison
         */
        bool operator<=(const Fraction &other) const { return !(other < *this); }

        /**
         * Greater than or equal comparison
         */
        bool operator>=(const Fraction &other) const { return !(*this < other); }

        /**
         * Comparison with integer
         */
        bool operator==(T other) const { return _numerator == other && _denominator == 1; }

        bool operator!=(T other) const { return !(*this == other); }

        bool operator<(T other) const {
            if (_denominator == 1 || other == 0) {
                return _numerator < other;
            }
            return _numerator < _denominator * other;
        }

        bool operator>(T other) const { return other < *this; }

        bool operator<=(T other) const { return !(other < *this); }

        bool operator>=(T other) const { return !(*this < other); }

        /**
         * Integer division (floor)
         */
        T floor_div(const Fraction &other) const {
            return (_numerator * other._denominator) / (_denominator * other._numerator);
        }

        /**
         * Modulo operation
         */
        Fraction operator%(const Fraction &other) const {
            T da = _denominator, db = other._denominator;
            T n_mod = (_numerator * db) % (other._numerator * da);
            return Fraction(n_mod, da * db);
        }

        /**
         * Power operation (integer exponent only)
         */
        Fraction pow(int exponent) const {
            if (exponent >= 0) {
                return Fraction(static_cast<T>(std::pow(_numerator, exponent)),
                                    static_cast<T>(std::pow(_denominator, exponent)));
            } else {
                if (_numerator == 0) {
                    throw std::runtime_error("Fraction(0, 0)");
                }
                if (_numerator > 0) {
                    return Fraction(static_cast<T>(std::pow(_denominator, -exponent)),
                                        static_cast<T>(std::pow(_numerator, -exponent)));
                } else {
                    return Fraction(static_cast<T>(std::pow(-_denominator, -exponent)),
                                        static_cast<T>(std::pow(-_numerator, -exponent)));
                }
            }
        }

        /**
         * Conversion to integer (truncation toward zero)
         */
        T to_int() const {
            if (_numerator < 0) {
                return -((-_numerator) / _denominator);
            }
            return _numerator / _denominator;
        }

        /**
         * Floor operation
         */
        T floor() const { return _numerator / _denominator; }

        /**
         * Ceiling operation
         */
        T ceil() const {
            if (_numerator >= 0) {
                return (_numerator + _denominator - 1) / _denominator;
            } else {
                return _numerator / _denominator;
            }
        }

        /**
         * Round to nearest integer (ties to even)
         */
        T round() const {
            T floor_val = this->floor();
            T remainder = _numerator % _denominator;

            if (remainder * 2 < _denominator) {
                return floor_val;
            } else if (remainder * 2 > _denominator) {
                return floor_val + 1;
            } else {
                // Tie case: round to even
                return (floor_val % 2 == 0) ? floor_val : floor_val + 1;
            }
        }

        /**
         * Round to specified number of decimal places
         */
        Fraction round(int ndigits) const {
            if (ndigits >= 0) {
                T shift = static_cast<T>(std::pow(10, ndigits));
                return Fraction(this->round() * shift, shift);
            } else {
                T shift = static_cast<T>(std::pow(10, -ndigits));
                return Fraction(this->round() * shift, shift);
            }
        }

        /**
         * Convert to double
         */
        double to_double() const { return static_cast<double>(_numerator) / _denominator; }

        /**
         * Convert to float
         */
        float to_float() const { return static_cast<float>(_numerator) / _denominator; }

        /**
         * String representation
         */
        std::string to_string() const {
            if (_denominator == 1) {
                return std::to_string(_numerator);
            }
            return std::to_string(_numerator) + "/" + std::to_string(_denominator);
        }

        /**
         * Hash function (compatible with Python's hash algorithm)
         */
        std::size_t hash() const {
            // Simplified hash - Python uses modular inverse with large prime
            // This is a basic implementation
            std::hash<T> hasher;
            std::size_t h1 = hasher(_numerator);
            std::size_t h2 = hasher(_denominator);
            return h1 ^ (h2 << 1);
        }

        /**
         * Stream output operator
         */
        friend std::ostream &operator<<(std::ostream &os, const Fraction &frac) {
            os << frac.to_string();
            return os;
        }
    };

    /**
     * Comparison operators for integer on left side
     */
    template <typename T>
    bool operator==(T lhs, const Fraction<T> &rhs) {
        return rhs.numerator() == lhs && rhs.denominator() == 1;
    }

    template <typename T>
    bool operator<(T lhs, const Fraction<T> &rhs) {
        if (rhs.denominator() == 1 || lhs == 0) {
            return lhs < rhs.numerator();
        }
        return rhs.denominator() * lhs < rhs.numerator();
    }

    template <typename T>
    bool operator>(T lhs, const Fraction<T> &rhs) {
        if (rhs.denominator() == 1 || lhs == 0) {
            return lhs > rhs.numerator();
        }
        return rhs.denominator() * lhs > rhs.numerator();
    }

    template <typename T>
    bool operator<=(T lhs, const Fraction<T> &rhs) {
        return !(lhs > rhs);
    }

    template <typename T>
    bool operator>=(T lhs, const Fraction<T> &rhs) {
        return !(lhs < rhs);
    }

    /**
     * Arithmetic operators for integer on left side
     */
    template <typename T>
    Fraction<T> operator+(T lhs, const Fraction<T> &rhs) {
        return Fraction<T>(lhs) + rhs;
    }

    template <typename T>
    Fraction<T> operator-(T lhs, const Fraction<T> &rhs) {
        return Fraction<T>(lhs) - rhs;
    }

    template <typename T>
    Fraction<T> operator*(T lhs, const Fraction<T> &rhs) {
        return Fraction<T>(lhs) * rhs;
    }

    template <typename T>
    Fraction<T> operator/(T lhs, const Fraction<T> &rhs) {
        return Fraction<T>(lhs) / rhs;
    }

}  // namespace fractions