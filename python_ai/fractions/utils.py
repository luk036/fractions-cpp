"""
Utility functions for the fractions library.

This module provides mathematical utility functions used by the Fraction class,
including greatest common divisor (GCD), least common multiple (LCM), and
absolute value functions with support for various numeric types.
"""

from typing import TypeVar, Union

# Define a generic type variable for numeric types
T = TypeVar('T', int, float, complex)

def abs_value(val: T) -> T:
    """
    Returns the absolute value of the input.
    
    For unsigned types, simply returns the input value.
    
    Args:
        val: The input value.
        
    Returns:
        The absolute value of the input.
        
    Examples:
        >>> abs_value(5)
        5
        >>> abs_value(-5)
        5
        >>> abs_value(0)
        0
    """
    if isinstance(val, (int, float)):
        return abs(val)
    return val


def gcd_recur(m: T, n: T) -> T:
    """
    Computes the greatest common divisor (GCD) of two integers recursively
    using Euclid's algorithm.
    
    Args:
        m: The first integer.
        n: The second integer.
        
    Returns:
        The GCD of m and n.
        
    Examples:
        >>> gcd_recur(12, 8)
        4
        >>> gcd_recur(12, 4)
        4
        >>> gcd_recur(4, 4)
        4
    """
    if n == 0:
        return abs_value(m)
    return gcd_recur(n, m % n)


def gcd(m: T, n: T) -> T:
    """
    Computes the greatest common divisor (GCD) of two integers recursively
    using Euclid's algorithm.
    
    Args:
        m: The first integer.
        n: The second integer.
        
    Returns:
        The GCD of m and n.
        
    Examples:
        >>> gcd(0, 8)
        8
        >>> gcd(12, 4)
        4
        >>> gcd(4, 4)
        4
    """
    if m == 0:
        return abs_value(n)
    return gcd_recur(m, n)


def lcm(m: T, n: T) -> T:
    """
    Computes the least common multiple of two integers.
    
    Uses the formula lcm(a, b) = (abs(a) / gcd(a, b)) * abs(b).
    
    Args:
        m: The first integer.
        n: The second integer.
        
    Returns:
        The least common multiple of m and n.
        
    Examples:
        >>> lcm(4, 6)
        12
        
        4 = 2^2
        6 = 2^1 * 3^1
        lcm = 2^2 * 3^1 = 12
    """
    if m == 0 or n == 0:
        return 0
    return (abs_value(m) // gcd(m, n)) * abs_value(n)