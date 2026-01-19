"""
Fractions library for Python with type annotations.

This module provides a Fraction class that supports zero denominators,
allowing representation of infinity and NaN values, similar to the C++
implementation.
"""

from .fraction import Fraction
from .utils import gcd, lcm, abs_value

__all__ = ['Fraction', 'gcd', 'lcm', 'abs_value']