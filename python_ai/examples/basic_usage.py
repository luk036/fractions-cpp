"""
Basic usage examples for the fractions library.

This script demonstrates the basic functionality of the Fraction class
and utility functions.
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from fractions import Fraction, gcd, lcm, abs_value


def basic_fraction_operations():
    """Demonstrate basic fraction operations."""
    print("=== Basic Fraction Operations ===")
    
    # Create fractions
    f1 = Fraction[int](1, 2)  # 1/2
    f2 = Fraction[int](3, 4)  # 3/4
    f3 = Fraction[int](5)     # 5/1
    
    print(f"f1 = {f1}")
    print(f"f2 = {f2}")
    print(f"f3 = {f3}")
    
    # Addition
    result = f1 + f2
    print(f"f1 + f2 = {result}")  # 5/4
    
    # Subtraction
    result = f1 - f2
    print(f"f1 - f2 = {result}")  # -1/4
    
    # Multiplication
    result = f1 * f2
    print(f"f1 * f2 = {result}")  # 3/8
    
    # Division
    result = f1 / f2
    print(f"f1 / f2 = {result}")  # 2/3
    
    # Negation
    result = -f1
    print(f"-f1 = {result}")  # -1/2
    
    print()


def comparison_operations():
    """Demonstrate comparison operations."""
    print("=== Comparison Operations ===")
    
    f1 = Fraction[int](1, 2)
    f2 = Fraction[int](2, 4)  # Equal to f1
    f3 = Fraction[int](3, 4)
    
    print(f"f1 = {f1}, f2 = {f2}, f3 = {f3}")
    
    # Equality
    print(f"f1 == f2: {f1 == f2}")  # True
    print(f"f1 == f3: {f1 == f3}")  # False
    
    # Less than
    print(f"f1 < f3: {f1 < f3}")    # True
    
    # Greater than
    print(f"f3 > f1: {f3 > f1}")    # True
    
    # Comparison with integers
    print(f"f1 == 0: {f1 == 0}")    # False
    print(f"f1 < 1: {f1 < 1}")      # True
    print(f"1 > f1: {1 > f1}")      # True
    
    print()


def zero_denominator_examples():
    """Demonstrate operations with zero denominators."""
    print("=== Zero Denominator Examples ===")
    
    # Create special fractions
    infinity = Fraction[int](1, 0)  # Represents infinity
    zero = Fraction[int](0, 1)      # Represents zero
    nan = Fraction[int](0, 0)       # Represents NaN
    
    print(f"infinity = {infinity}")
    print(f"zero = {zero}")
    print(f"nan = {nan}")
    
    # Operations with zero denominators
    result = zero / nan  # 0/0 = 0/1 (zero divided by zero is zero)
    print(f"zero / nan = {result}")
    
    result = infinity * zero  # Infinity * 0 = NaN
    print(f"infinity * zero = {result}")
    
    print()


def utility_function_examples():
    """Demonstrate utility functions."""
    print("=== Utility Function Examples ===")
    
    # GCD examples
    print(f"gcd(12, 8) = {gcd(12, 8)}")      # 4
    print(f"gcd(0, 8) = {gcd(0, 8)}")        # 8
    print(f"gcd(-12, 8) = {gcd(-12, 8)}")    # 4
    
    # LCM examples
    print(f"lcm(4, 6) = {lcm(4, 6)}")        # 12
    print(f"lcm(0, 6) = {lcm(0, 6)}")        # 0
    
    # Absolute value examples
    print(f"abs_value(5) = {abs_value(5)}")        # 5
    print(f"abs_value(-5) = {abs_value(-5)}")      # 5
    print(f"abs_value(0) = {abs_value(0)}")        # 0
    
    print()


def advanced_examples():
    """Demonstrate advanced features."""
    print("=== Advanced Examples ===")
    
    f1 = Fraction[int](2, 3)
    f2 = Fraction[int](3, 4)
    
    # Cross product
    cross_result = f1.cross(f2)
    print(f"cross({f1}, {f2}) = {cross_result}")  # -1
    
    # Reciprocal
    f1.reciprocal()
    print(f"reciprocal of (2/3) = {f1}")  # (3/2)
    
    # In-place operations
    f2 += Fraction[int](1, 4)
    print(f"3/4 + 1/4 = {f2}")  # (1/1)
    
    print()


def main():
    """Run all examples."""
    print("Fractions Library - Python Implementation")
    print("=" * 50)
    print()
    
    basic_fraction_operations()
    comparison_operations()
    zero_denominator_examples()
    utility_function_examples()
    advanced_examples()
    
    print("All examples completed!")


if __name__ == "__main__":
    main()