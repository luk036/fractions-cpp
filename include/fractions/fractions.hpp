#pragma once

/** @file include/fractions.hpp
 *  This is a C++ Library header.
 */

// #include <boost/operators.hpp>
// #include <cmath>
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
   * @brief absolute (unsigned)
   *
   * @tparam T
   * @param[in] a
   */
  template <typename T> CONSTEXPR14 auto abs(const T &a) ->
      typename std::enable_if<std::is_unsigned<T>::value, T>::type {
    return a;
  }

  /**
   * @brief absolute (signed)
   *
   * @tparam T
   * @param[in] a
   */
  template <typename T> CONSTEXPR14 auto abs(const T &a) ->
      typename std::enable_if<!std::is_unsigned<T>::value, T>::type {
    return (a < T(0)) ? -a : a;
  }

  /**
   * @brief Greatest common divider
   *
   * @tparam _Mn
   * @param[in] __m
   * @param[in] __n
   * @return _Mn
   */
  template <typename _Mn> CONSTEXPR14 auto gcd_recur(const _Mn &__m, const _Mn &__n) -> _Mn {
    if (__n == 0) {
      return abs(__m);
    }
    return gcd_recur(__n, __m % __n);
  }

  /**
   * @brief Greatest common divider
   *
   * @tparam _Mn
   * @param[in] __m
   * @param[in] __n
   * @return _Mn
   */
  template <typename _Mn> CONSTEXPR14 auto gcd(const _Mn &__m, const _Mn &__n) -> _Mn {
    if (__m == 0) {
      return abs(__n);
    }
    return gcd_recur(__m, __n);
  }

  /**
   * @brief Least common multiple
   *
   * @tparam _Mn
   * @param[in] __m
   * @param[in] __n
   * @return _Mn
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
     * @brief Construct a new Fraction object
     *
     * @param[in] numer
     * @param[in] denom
     */
    CONSTEXPR14 Fraction(Z numer, Z denom) : _numer{std::move(numer)}, _denom{std::move(denom)} {
      this->normalize();
    }

    /**
     * @brief normalize to a canonical form
     *
     * denominator is always non-negative and co-prime with numerator
     */
    CONSTEXPR14 auto normalize() -> Z {
      this->normalize1();
      return this->normalize2();
    }

    /**
     * @brief normalize to a canonical form
     *
     * denominator is always non-negative
     */
    CONSTEXPR14 void normalize1() {
      if (this->_denom < Z(0)) {
        this->_numer = -this->_numer;
        this->_denom = -this->_denom;
      }
    }

    /**
     * @brief normalize to a canonical form
     *
     * denominator is always co-prime with numerator
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
     * @brief Construct a new Fraction object
     *
     * @param[in] numer
     */
    CONSTEXPR14 explicit Fraction(Z &&numer) : _numer{std::move(numer)}, _denom(Z(1)) {}

    /**
     * @brief Construct a new Fraction object
     *
     * @param[in] numer
     */
    CONSTEXPR14 explicit Fraction(const Z &numer) : _numer{numer}, _denom(1) {}

    /**
     * @brief Construct a new Fraction object
     *
     * @param[in] numer
     */
    CONSTEXPR14 Fraction() : _numer(0), _denom(1) {}

    /**
     * @brief
     *
     * @return const Z&
     */
    CONSTEXPR14 auto numer() const noexcept -> const Z & { return _numer; }

    /**
     * @brief
     *
     * @return const Z&
     */
    CONSTEXPR14 auto denom() const noexcept -> const Z & { return _denom; }

    /**
     * @brief cross product
     *
     * @param[in] rhs
     * @return Z
     */
    CONSTEXPR14 auto cross(const Fraction &rhs) const -> Z {
      return this->_numer * rhs._denom - this->_denom * rhs._numer;
    }

    /** @name Comparison operators
     *  ==, !=, <, >, <=, >= etc.
     */
    ///@{

    /**
     * @brief Equal to
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
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
     * @brief Less than
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
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
     * @brief Less than
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
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
     * @brief Equal to
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
     */
    friend CONSTEXPR14 auto operator==(const Z &lhs, const Fraction &rhs) -> bool {
      return rhs == lhs;
    }

    /**
     * @brief Equal to
     *
     * @param[in] rhs
     * @return true
     * @return false
     */

    /**
     * @brief Equal to
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
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
     * @brief Less than
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
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
     * @brief
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator!=(const Fraction &rhs) const -> bool { return !(*this == rhs); }

    /**
     * @brief Greater than
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator>(const Fraction &rhs) const -> bool { return rhs < *this; }

    /**
     * @brief Greater than or euqal to
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator>=(const Fraction &rhs) const -> bool { return !(*this < rhs); }

    /**
     * @brief Less than or equal to
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator<=(const Fraction &rhs) const -> bool { return !(rhs < *this); }

    /**
     * @brief Greater than
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator>(const Z &rhs) const -> bool { return rhs < *this; }

    /**
     * @brief Less than or equal to
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator<=(const Z &rhs) const -> bool { return !(rhs < *this); }

    /**
     * @brief Greater than or equal to
     *
     * @param[in] rhs
     * @return true
     * @return false
     */
    CONSTEXPR14 auto operator>=(const Z &rhs) const -> bool { return !(*this < rhs); }

    /**
     * @brief Greater than
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
     */
    friend CONSTEXPR14 auto operator>(const Z &lhs, const Fraction &rhs) -> bool {
      return rhs < lhs;
    }

    /**
     * @brief Less than or equal to
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
     */
    friend CONSTEXPR14 auto operator<=(const Z &lhs, const Fraction &rhs) -> bool {
      return !(rhs < lhs);
    }

    /**
     * @brief Greater than or euqal to
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return true
     * @return false
     */
    friend CONSTEXPR14 auto operator>=(const Z &lhs, const Fraction &rhs) -> bool {
      return !(lhs < rhs);
    }

    ///@}

    /**
     * @brief reciprocal
     *
     */
    CONSTEXPR14 void reciprocal() {
      std::swap(this->_numer, this->_denom);
      this->normalize1();
    }

    /**
     * @brief multiply and assign
     *
     * @param[in] rhs
     * @return Fraction&
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
     * @brief multiply
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator*(Fraction lhs, const Fraction &rhs) -> Fraction {
      return lhs *= rhs;
    }

    /**
     * @brief multiply and assign
     *
     * @param[in] rhs
     * @return Fraction&
     */
    CONSTEXPR14 auto operator*=(Z rhs) -> Fraction & {
      std::swap(this->_numer, rhs);
      this->normalize2();
      this->_numer *= rhs;
      return *this;
    }

    /**
     * @brief multiply
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator*(Fraction lhs, const Z &rhs) -> Fraction { return lhs *= rhs; }

    /**
     * @brief multiply
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator*(const Z &lhs, Fraction rhs) -> Fraction { return rhs *= lhs; }

    /**
     * @brief divide and assign
     *
     * @param[in] rhs
     * @return Fraction&
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
     * @brief divide
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator/(Fraction lhs, const Fraction &rhs) -> Fraction {
      return lhs /= rhs;
    }

    /**
     * @brief divide and assign
     *
     * @param[in] rhs
     * @return Fraction&
     */
    CONSTEXPR14 auto operator/=(Z rhs) -> Fraction & {
      std::swap(this->_denom, rhs);
      this->normalize();
      this->_denom *= rhs;
      return *this;
    }

    /**
     * @brief divide
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator/(Fraction lhs, const Z &rhs) -> Fraction { return lhs /= rhs; }

    /**
     * @brief divide
     *
     * @param[in] lhs
     * @param[in] rhs
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator/(const Z &lhs, Fraction rhs) -> Fraction {
      rhs.reciprocal();
      return rhs *= lhs;
    }

    /**
     * @brief Negate
     *
     * @return Fraction
     */
    CONSTEXPR14 auto operator-() const -> Fraction {
      auto res = Fraction(*this);
      res._numer = -res._numer;
      return res;
    }

    /**
     * @brief Add
     *
     * @param[in] rhs
     * @return Fraction
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
     * @brief Subtract
     *
     * @param[in] frac
     * @return Fraction
     */
    CONSTEXPR14 auto operator-(const Fraction &frac) const -> Fraction { return *this + (-frac); }

    /**
     * @brief Add
     *
     * @param[in] frac
     * @param[in] i
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator+(Fraction frac, const Z &i) -> Fraction { return frac += i; }

    /**
     * @brief Add
     *
     * @param[in] i
     * @param[in] frac
     * @return Fraction
     */
    friend CONSTEXPR14 auto operator+(const Z &i, Fraction frac) -> Fraction { return frac += i; }

    /**
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    CONSTEXPR14 auto operator-(const Z &i) const -> Fraction { return *this + (-i); }

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
