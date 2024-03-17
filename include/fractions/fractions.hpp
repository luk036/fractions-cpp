#pragma once

/** @file include/fractions.hpp
 *  This is a C++ Library header.
 */

#include <numeric>
#include <type_traits>
#include <utility>

// #include "common_concepts.h"

#if __cpp_constexpr >= 201304
#  define CONSTEXPR14 constexpr
#else
#  define CONSTEXPR14 inline
#endif

namespace fractions {

  /**
   * Returns the absolute value of the input.
   *
   * For unsigned types, simply returns the input value.
   *
   * @tparam T The type of the input parameter.
   * @param[in] a The input value.
   * @return The absolute value of the input.
   */
  template <typename T> CONSTEXPR14 auto abs(const T &a) ->
      typename std::enable_if<std::is_unsigned<T>::value, T>::type {
    return a;
  }

  /**
   * Computes the absolute value of the given signed integer value.
   * Returns the input if it is positive, otherwise returns the negation of the input.
   *
   * @tparam T The type of the input parameter.
   * @param[in] a The input value.
   * @return The absolute value of the input.
   */
  template <typename T> CONSTEXPR14 auto abs(const T &a) ->
      typename std::enable_if<!std::is_unsigned<T>::value, T>::type {
    return (a < T(0)) ? -a : a;
  }

  /**
   * Computes the greatest common divider (GCD) of two integers recursively
   * using Euclid's algorithm.
   *
   * @tparam _Mn The integer type.
   * @param __m The first integer.
   * @param __n The second integer.
   * @return The GCD of __m and __n.
   */
  template <typename _Mn> CONSTEXPR14 auto gcd_recur(const _Mn &__m, const _Mn &__n) -> _Mn {
    if (__n == 0) {
      return abs(__m);
    }
    return gcd_recur(__n, __m % __n);
  }

  /**
   * Computes the greatest common divider (GCD) of two integers recursively using Euclid's
   * algorithm.
   *
   * @tparam _Mn The integer type.
   * @param __m The first integer.
   * @param __n The second integer.
   * @return The GCD of __m and __n.
   */
  template <typename _Mn> CONSTEXPR14 auto gcd(const _Mn &__m, const _Mn &__n) -> _Mn {
    if (__m == 0) {
      return abs(__n);
    }
    return gcd_recur(__m, __n);
  }

  /**
   * Computes the least common multiple of two integers.
   *
   * Uses the formula lcm(a, b) = (abs(a) / gcd(a, b)) * abs(b).
   *
   * @tparam _Mn The integer type.
   * @param __m The first integer.
   * @param __n The second integer.
   * @return The least common multiple of __m and __n.
   */
  template <typename _Mn> CONSTEXPR14 auto lcm(const _Mn &__m, const _Mn &__n) -> _Mn {
    if (__m == 0 || __n == 0) {
      return 0;
    }
    return (abs(__m) / gcd(__m, __n)) * abs(__n);
  }

  /**
   * @brief Fraction
   *
   * @tparam Z
   */
  template <typename Z> struct Fraction {
    Z _numer;
    Z _denom;

    /**
     * Constructs a new Fraction object from the given numerator and denominator.
     *
     * Normalizes the fraction after construction.
     *
     * @param[in] numer The numerator
     * @param[in] denom The denominator
     */
    CONSTEXPR14 Fraction(Z numer, Z denom) : _numer{std::move(numer)}, _denom{std::move(denom)} {
      this->normalize();
    }

    /**
     * Normalizes the fraction to a canonical form where the denominator
     * is always non-negative and co-prime with the numerator.
     */
    CONSTEXPR14 auto normalize() -> Z {
      this->normalize1();
      return this->normalize2();
    }

    /**
     * Normalizes the fraction by making the denominator positive.
     * If the denominator is negative, both numerator and denominator are negated.
     */
    CONSTEXPR14 void normalize1() {
      if (this->_denom < Z(0)) {
        this->_numer = -this->_numer;
        this->_denom = -this->_denom;
      }
    }

    /**
     * Normalizes the fraction to a canonical form by making the numerator
     * and denominator coprime. The gcd of the numerator and denominator
     * is computed and used to divide out any common factors.
     */
    CONSTEXPR14 auto normalize2() -> Z {
      Z common = gcd(this->_numer, this->_denom);
      if (common == Z(1) || common == Z(0)) {
        return common;
      }
      this->_numer /= common;
      this->_denom /= common;
      return common;
    }

    /**
     * Constructs a new Fraction object from the given numerator.
     *
     * The denominator is initialized to 1.
     *
     * @param[in] numer The numerator.
     */
    CONSTEXPR14 explicit Fraction(Z &&numer) : _numer{std::move(numer)}, _denom(Z(1)) {}

    /**
     * Constructs a new Fraction object from the given numerator.
     *
     * The denominator is initialized to 1.
     *
     * @param[in] numer The numerator.
     */
    CONSTEXPR14 explicit Fraction(const Z &numer) : _numer{numer}, _denom(1) {}

    /**
     * Constructs a new Fraction object with numerator initialized to 0 and denominator initialized
     * to 1.
     */
    CONSTEXPR14 Fraction() : _numer(0), _denom(1) {}

    /**
     * Gets the numerator of the fraction.
     *
     * @return A const reference to the numerator.
     */
    CONSTEXPR14 auto numer() const noexcept -> const Z & { return _numer; }

    /**
     * Gets the denominator of the fraction.
     *
     * @return A const reference to the denominator.
     */
    CONSTEXPR14 auto denom() const noexcept -> const Z & { return _denom; }

    /**
     * Computes the cross product of this fraction and another fraction.
     *
     * The cross product is defined as:
     *   cross(a/b, c/d) = a*d - b*c
     *
     * This can be used to compute the determinant of a 2x2 matrix with
     * the two fractions as the entries on the diagonal.
     *
     * @param[in] rhs The right-hand fraction to compute the cross product with.
     * @return The computed cross product.
     */
    CONSTEXPR14 auto cross(const Fraction &rhs) const -> Z {
      return this->_numer * rhs._denom - this->_denom * rhs._numer;
    }

    /** @name Comparison operators
     *  ==, !=, <, >, <=, >= etc.
     */
    ///@{

    /**
     * Compares this fraction to an integer value for equality.
     *
     * Returns true if the fraction is equal to the integer, false otherwise.
     * The fraction is considered equal if its normalized value matches the
     * integer when the denominator is 1.
     *
     * @param lhs The left hand side fraction to compare.
     * @param rhs The right hand side integer to compare.
     * @return True if lhs == rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator==(const Fraction &lhs, const Z &rhs) -> bool {
      if (lhs._denom == Z(1) || rhs == Z(0)) {
        return lhs._numer == rhs;
      }
      auto lhs2{lhs};
      auto rhs2{rhs};
      std::swap(lhs2._denom, rhs2);
      lhs2.normalize2();
      return lhs2._numer < lhs2._denom * rhs2;
    }

    /**
     * Compares this fraction to an integer value for less than.
     *
     * Returns true if this fraction is less than the integer, false otherwise.
     * The fraction is considered less if its normalized value is less than
     * the integer when the denominator is 1.
     *
     * @param lhs The left hand side fraction to compare.
     * @param rhs The right hand side integer to compare.
     * @return True if lhs < rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator<(const Fraction &lhs, const Z &rhs) -> bool {
      if (lhs._denom == Z(1) || rhs == Z(0)) {
        return lhs._numer < rhs;
      }
      auto lhs2{lhs};
      auto rhs2{rhs};
      std::swap(lhs2._denom, rhs2);
      lhs2.normalize2();
      return lhs2._numer < lhs2._denom * rhs2;
    }

    /**
     * Compares an integer value to this fraction for less than.
     *
     * Returns true if the integer is less than this fraction, false otherwise.
     * The fraction is considered greater if its normalized value is greater than
     * the integer when the denominator is 1.
     *
     * @param lhs The left hand side integer to compare.
     * @param rhs The right hand side fraction to compare.
     * @return True if lhs < rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator<(const Z &lhs, const Fraction &rhs) -> bool {
      if (rhs._denom == Z(1) || lhs == Z(0)) {
        return lhs < rhs._numer;
      }
      auto lhs2{lhs};
      auto rhs2{rhs};
      std::swap(rhs2._denom, lhs2);
      rhs2.normalize2();
      return rhs2._denom * lhs2 < rhs2._numer;
    }

    /**
     * Compares an integer value to this fraction for equality.
     *
     * Returns true if the integer is equal to this fraction, false otherwise.
     * Equality is checked by comparing the integer to the normalized value
     * of the fraction.
     *
     * @param lhs The left hand side integer to compare.
     * @param rhs The right hand side fraction to compare.
     * @return True if lhs == rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator==(const Z &lhs, const Fraction &rhs) -> bool {
      return rhs == lhs;
    }

    /**
     * Compares this fraction to another fraction for equality.
     *
     * Returns true if the two fractions represent the same rational value,
     * false otherwise. Equality is checked by normalizing both fractions
     * to a common denominator and comparing the numerators.
     *
     * @param lhs The left hand side fraction to compare.
     * @param rhs The right hand side fraction to compare.
     * @return True if lhs == rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator==(const Fraction &lhs, const Fraction &rhs) -> bool {
      if (lhs._denom == rhs._denom) {
        return lhs._numer == rhs._numer;
      }
      auto lhs2{lhs};
      auto rhs2{rhs};
      std::swap(lhs2._denom, rhs2._numer);
      lhs2.normalize2();
      rhs2.normalize2();
      return lhs2._numer * rhs2._denom == lhs2._denom * rhs2._numer;
    }

    /**
     * Compares this fraction to another fraction for less than.
     *
     * Returns true if this fraction is less than the other fraction, false otherwise.
     * Less than is determined by converting both fractions to a common denominator and
     * comparing the resulting numerators.
     *
     * @param lhs The left hand side fraction to compare.
     * @param rhs The right hand side fraction to compare.
     * @return True if lhs < rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator<(const Fraction &lhs, const Fraction &rhs) -> bool {
      if (lhs._denom == rhs._denom) {
        return lhs._numer < rhs._numer;
      }
      auto lhs2{lhs};
      auto rhs2{rhs};
      std::swap(lhs2._denom, rhs2._numer);
      lhs2.normalize2();
      rhs2.normalize2();
      return lhs2._numer * rhs2._denom < lhs2._denom * rhs2._numer;
    }

    /**
     * Compares this fraction for inequality against another fraction.
     *
     * Returns true if this fraction and the given fraction represent different
     * rational values, false if they are equal. Uses the equality operator to
     * compare the two fractions.
     *
     * @param rhs The right hand side fraction to compare against.
     * @return True if the fractions are not equal, false if they are equal.
     */
    CONSTEXPR14 auto operator!=(const Fraction &rhs) const -> bool { return !(*this == rhs); }

    /**
     * Compares this fraction to another fraction for greater than.
     *
     * Returns true if this fraction is greater than the other fraction, false otherwise.
     * Greater than is determined by converting to a common denominator and comparing
     * the resulting numerators.
     *
     * @param rhs The right hand side fraction to compare.
     * @return True if this fraction is greater than rhs, false otherwise.
     */
    CONSTEXPR14 auto operator>(const Fraction &rhs) const -> bool { return rhs < *this; }

    /**
     * Compares this fraction to another fraction for greater than or equal to.
     *
     * @param rhs The right hand side fraction to compare.
     * @returns True if this fraction is greater than or equal to the other fraction,
     * false otherwise.
     */
    CONSTEXPR14 auto operator>=(const Fraction &rhs) const -> bool { return !(*this < rhs); }

    /**
     * Compares this fraction to another fraction for less than or equal to.
     *
     * @param rhs The right hand side fraction to compare.
     * @returns True if this fraction is less than or equal to the other fraction,
     * false otherwise.
     */
    CONSTEXPR14 auto operator<=(const Fraction &rhs) const -> bool { return !(rhs < *this); }

    /**
     * Compares this fraction to another Z integer for greater than.
     *
     * @param rhs The right hand side fraction to compare.
     * @returns True if this fraction is greater than the other Z integer,
     * false otherwise.
     */
    CONSTEXPR14 auto operator>(const Z &rhs) const -> bool { return rhs < *this; }

    /**
     * Compares this fraction to another Z integer for less than or equal to.
     *
     * @param rhs The right hand side fraction to compare.
     * @returns True if this fraction is less than or equal to the other Z integer,
     * false otherwise.
     */
    CONSTEXPR14 auto operator<=(const Z &rhs) const -> bool { return !(rhs < *this); }

    /**
     * Compares this fraction to another Z integer for greater than or equal to.
     *
     * @param rhs The right hand side fraction to compare.
     * @returns True if this fraction is greater than or equal to the other Z integer,
     * false otherwise.
     */
    CONSTEXPR14 auto operator>=(const Z &rhs) const -> bool { return !(*this < rhs); }

    /**
     * Compares a Z integer to a Fraction for greater than.
     *
     * This is a friend function that allows comparing a Z integer on the left
     * hand side to a Fraction on the right hand side.
     *
     * @param lhs The Z integer on the left hand side.
     * @param rhs The Fraction on the right hand side.
     * @return True if lhs is greater than rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator>(const Z &lhs, const Fraction &rhs) -> bool {
      return rhs < lhs;
    }

    /**
     * Compares a Z integer to a Fraction for less than or equal to.
     *
     * This is a friend function that allows comparing a Z integer on the left
     * hand side to a Fraction on the right hand side.
     *
     * @param lhs The Z integer on the left hand side.
     * @param rhs The Fraction on the right hand side.
     * @return True if lhs is less than or equal to rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator<=(const Z &lhs, const Fraction &rhs) -> bool {
      return !(rhs < lhs);
    }

    /**
     * Compares a Z integer to a Fraction for greater than or equal to.
     *
     * This is a friend function that allows comparing a Z integer on the left
     * hand side to a Fraction on the right hand side.
     *
     * @param lhs The Z integer on the left hand side.
     * @param rhs The Fraction on the right hand side.
     * @return True if lhs is greater than or equal to rhs, false otherwise.
     */
    friend CONSTEXPR14 auto operator>=(const Z &lhs, const Fraction &rhs) -> bool {
      return !(lhs < rhs);
    }

    ///@}

    /**
     * @brief Inverts the numerator and denominator of the fraction to calculate
     * its reciprocal.
     */
    CONSTEXPR14 void reciprocal() {
      std::swap(this->_numer, this->_denom);
      this->normalize1();
    }

    /**
     * Multiplies this Fraction by the given Fraction rhs and assigns the result to this Fraction.
     *
     * Normalizes this Fraction and rhs before multiplying. Updates the numerator and denominator
     * of this Fraction with the result.
     *
     * @param rhs The Fraction to multiply.
     * @return A reference to this Fraction after multiplication.
     */
    CONSTEXPR14 auto operator*=(Fraction rhs) -> Fraction & {
      std::swap(this->_numer, rhs._numer);
      this->normalize2();
      rhs.normalize2();
      this->_numer *= rhs._numer;
      this->_denom *= rhs._denom;
      return *this;
    }

    /**
     * Multiplies the given Fraction lhs by the Fraction rhs.
     *
     * Normalizes lhs and rhs before multiplying. Returns a new Fraction containing
     * the result of the multiplication.
     *
     * @param lhs The left hand Fraction to multiply.
     * @param rhs The right hand Fraction to multiply.
     * @return A new Fraction containing the result of the multiplication.
     */
    friend CONSTEXPR14 auto operator*(Fraction lhs, const Fraction &rhs) -> Fraction {
      return lhs *= rhs;
    }

    /**
     * Multiplies this Fraction by the given integer rhs and assigns the result to this Fraction.
     *
     * Normalizes this Fraction before multiplying. Updates the numerator of
     * this Fraction with the result and retains the existing denominator.
     *
     * @param rhs The integer to multiply.
     * @return A reference to this Fraction after multiplication.
     */
    CONSTEXPR14 auto operator*=(Z rhs) -> Fraction & {
      std::swap(this->_numer, rhs);
      this->normalize2();
      this->_numer *= rhs;
      return *this;
    }

    /**
     * Multiplies the given Fraction lhs by the integer rhs.
     *
     * Normalizes lhs before multiplying. Returns a new Fraction containing
     * the result of the multiplication.
     *
     * @param lhs The left hand Fraction to multiply.
     * @param rhs The right hand integer to multiply.
     * @return A new Fraction containing the result of the multiplication.
     */
    friend CONSTEXPR14 auto operator*(Fraction lhs, const Z &rhs) -> Fraction { return lhs *= rhs; }

    /**
     * Multiplies the Fraction rhs by the integer lhs.
     *
     * Normalizes rhs before multiplying. Returns a new Fraction containing
     * the result of the multiplication.
     *
     * @param lhs The left hand integer to multiply.
     * @param rhs The right hand Fraction to multiply.
     * @return A new Fraction containing the result of the multiplication.
     */
    friend CONSTEXPR14 auto operator*(const Z &lhs, Fraction rhs) -> Fraction { return rhs *= lhs; }

    /**
     * Divides this Fraction by the given Fraction rhs and assigns the result to this Fraction.
     *
     * Swaps the denominator of this Fraction with the numerator of rhs.
     * Normalizes this Fraction and rhs before dividing.
     * Updates the numerator and denominator of this Fraction with the result.
     *
     * @param rhs The Fraction to divide by.
     * @return A reference to this Fraction after division.
     */
    CONSTEXPR14 auto operator/=(Fraction rhs) -> Fraction & {
      std::swap(this->_denom, rhs._numer);
      this->normalize();
      rhs.normalize2();
      this->_numer *= rhs._denom;
      this->_denom *= rhs._numer;
      return *this;
    }

    /**
     * Divides the Fraction lhs by the Fraction rhs.
     *
     * Returns a new Fraction containing the result.
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return A Fraction after division.
     */
    friend CONSTEXPR14 auto operator/(Fraction lhs, const Fraction &rhs) -> Fraction {
      return lhs /= rhs;
    }

    /**
     * Divides this Fraction by the integer rhs and assigns the result to this Fraction.
     *
     * Divides the numerator of this Fraction by rhs and updates it with the result.
     *
     * @param rhs The integer to divide this Fraction by.
     * @return A reference to this Fraction after dividing by rhs.
     */
    CONSTEXPR14 auto operator/=(Z rhs) -> Fraction & {
      std::swap(this->_denom, rhs);
      this->normalize();
      this->_denom *= rhs;
      return *this;
    }

    /**
     * Divides the Fraction lhs by the integer rhs.
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return A Fraction after division.
     */
    friend CONSTEXPR14 auto operator/(Fraction lhs, const Z &rhs) -> Fraction { return lhs /= rhs; }

    /**
     * Divides the integer lhs by the Fraction rhs.
     *
     * Returns a new Fraction containing the result.
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return A Fraction after division.
     */
    friend CONSTEXPR14 auto operator/(const Z &lhs, Fraction rhs) -> Fraction {
      rhs.reciprocal();
      return rhs *= lhs;
    }

    /**
     * Negates this Fraction by negating its numerator.
     *
     * Returns a new Fraction with the negated numerator.
     */
    CONSTEXPR14 auto operator-() const -> Fraction {
      auto res = Fraction(*this);
      res._numer = -res._numer;
      return res;
    }

    /**
     * Adds this Fraction to the Fraction rhs.
     *
     * Finds the lowest common denominator and converts both Fractions to it.
     * Then adds the numerators and returns a new Fraction with the result.
     * Handles zero denominators by returning a Fraction with a zero denominator.
     */
    CONSTEXPR14 auto operator+(const Fraction &rhs) const -> Fraction {
      if (this->_denom == rhs._denom) {
        return Fraction(this->_numer + rhs._numer, this->_denom);
      }
      const auto common = gcd(this->_denom, rhs._denom);
      if (common == Z(0)) {
        return Fraction(rhs._denom * this->_numer + this->_denom * rhs._numer, Z(0));
      }
      const auto l = this->_denom / common;
      const auto r = rhs._denom / common;
      auto d = this->_denom * r;
      auto n = r * this->_numer + l * rhs._numer;
      return Fraction(std::move(n), std::move(d));
    }

    /**
     * Subtracts another Fraction from this Fraction.
     *
     * Negates the passed in Fraction, then adds it to this Fraction.
     *
     * @param[in] other The Fraction to subtract from this one.
     * @return A new Fraction containing the result.
     */
    CONSTEXPR14 auto operator-(const Fraction &other) const -> Fraction { return *this + (-other); }

    /**
     * Adds a Fraction and an integer.
     *
     * Converts the integer to a Fraction and adds it to the Fraction lhs.
     *
     * @param lhs The Fraction to add to.
     * @param rhs The integer to add.
     * @return A new Fraction containing the sum.
     */
    friend CONSTEXPR14 auto operator+(Fraction lhs, const Z &rhs) -> Fraction { return lhs += rhs; }

    /**
     * Adds an integer to a Fraction.
     *
     * Converts the integer rhs to a Fraction and adds it to the Fraction lhs.
     *
     * @param lhs The Fraction to add to.
     * @param rhs The integer to add.
     * @return A new Fraction containing the sum.
     */
    friend CONSTEXPR14 auto operator+(const Z &lhs, Fraction rhs) -> Fraction { return rhs += lhs; }

    /**
     * Subtracts an integer from this Fraction.
     *
     * Converts the integer rhs to a Fraction with denominator 1 and subtracts it from this
     * Fraction.
     *
     * @param rhs The integer to subtract.
     * @return A new Fraction containing the difference.
     */
    CONSTEXPR14 auto operator-(const Z &rhs) const -> Fraction { return *this + (-rhs); }

    /**
     * @brief
     *
     * @param[in] rhs
     * @return Fraction
     */
    CONSTEXPR14 auto operator+=(const Fraction &rhs) -> Fraction & { return *this -= (-rhs); }

    /**
     * @brief
     *
     * @param[in] rhs
     * @return Fraction
     */
    CONSTEXPR14 auto operator-=(const Fraction &rhs) -> Fraction & {
      if (this->_denom == rhs._denom) {
        this->_numer -= rhs._numer;
        this->normalize2();
        return *this;
      }

      auto other{rhs};
      std::swap(this->_denom, other._numer);
      auto common_n = this->normalize2();
      auto common_d = other.normalize2();
      std::swap(this->_denom, other._numer);
      this->_numer = this->cross(other);
      this->_denom *= other._denom;
      std::swap(this->_denom, common_d);
      this->normalize2();
      this->_numer *= common_n;
      this->_denom *= common_d;
      this->normalize2();
      return *this;
    }

    /**
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    CONSTEXPR14 auto operator+=(const Z &i) -> Fraction & { return *this -= (-i); }

    /**
     * @brief
     *
     * @param[in] rhs
     * @return Fraction
     */
    CONSTEXPR14 auto operator-=(const Z &rhs) -> Fraction & {
      if (this->_denom == Z(1)) {
        this->_numer -= rhs;
        return *this;
      }

      auto other{rhs};
      std::swap(this->_denom, other);
      auto common_n = this->normalize2();
      std::swap(this->_denom, other);
      this->_numer -= other * this->_denom;
      this->_numer *= common_n;
      this->normalize2();
      return *this;
    }

    /**
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator-(const Z &c, const Fraction &frac) -> Fraction {
      return c + (-frac);
    }

    /**
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator+(int &&c, const Fraction &frac) -> Fraction {
      return frac + Z(c);
    }

    /**
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator-(int &&c, const Fraction &frac) -> Fraction {
      return (-frac) + Z(c);
    }

    /**
     * @brief
     *
     * @param[in] c
     * @param[in] frac
     * @return Fraction<Z>
     */
    friend CONSTEXPR14 auto operator*(int &&c, const Fraction &frac) -> Fraction {
      return frac * Z(c);
    }

    /**
     * @brief
     *
     * @tparam _Stream
     * @tparam Z
     * @param[in] os
     * @param[in] frac
     * @return _Stream&
     */
    template <typename _Stream> friend auto operator<<(_Stream &os, const Fraction &frac)
        -> _Stream & {
      os << "(" << frac.numer() << "/" << frac.denom() << ")";
      return os;
    }
  };

  // For template deduction
  // typename{Z} Fraction(const Z &, const Z &) noexcept -> Fraction<Z>;

}  // namespace fractions
