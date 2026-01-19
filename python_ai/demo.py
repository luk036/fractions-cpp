#!/usr/bin/env python3
"""
Demo script for the fractions library.

This script demonstrates the key features of the Python fractions library
with type annotations and zero denominator support.
"""

import sys
import os

# Add the parent directory to the path to avoid conflicts with built-in fractions
sys.path.insert(0, os.path.dirname(__file__))

from fractions import Fraction, gcd, lcm, abs_value


def main():
    """Run the demo."""
    print("Fractions Library - Python Implementation with Type Annotations")
    print("=" * 65)
    print()
    
    # Basic operations
    print("Basic Operations:")
    print("-" * 20)
    a = Fraction[int](1, 2)
    b = Fraction[int](3, 4)
    print(f"a = {a}, b = {b}")
    print(f"a + b = {a + b}")
    print(f"a - b = {a - b}")
    print(f"a * b = {a * b}")
    print(f"a / b = {a / b}")
    print()
    
    # Zero denominator support
    print("Zero Denominator Support:")
    print("-" * 30)
    inf = Fraction[int](1, 0)  # Infinity
    zero = Fraction[int](0, 1)  # Zero
    nan = Fraction[int](0, 0)   # NaN
    print(f"infinity = {inf}")
    print(f"zero = {zero}")
    print(f"nan = {nan}")
    print(f"zero / nan = {zero / nan}")
    print()
    
    # Type annotations
    print("Type Annotations:")
    print("-" * 20)
    print("The Fraction class is generic and supports type annotations:")
    print("  Fraction[int] - for integer fractions")
    print("  Fraction[float] - for float fractions")
    print()
    
    # Utility functions
    print("Utility Functions:")
    print("-" * 20)
    print(f"gcd(12, 8) = {gcd(12, 8)}")
    print(f"lcm(4, 6) = {lcm(4, 6)}")
    print(f"abs_value(-5) = {abs_value(-5)}")
    print()
    
    print("Demo completed successfully!")


if __name__ == "__main__":
    main()