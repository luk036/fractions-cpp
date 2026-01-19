#!/usr/bin/env python3
"""
Test script to verify type annotations work correctly.

This script tests that the Fraction class properly supports type annotations
and can be used with mypy for type checking.
"""

import sys
import os

# Add the parent directory to the path to avoid conflicts with built-in fractions
sys.path.insert(0, os.path.dirname(__file__))

from fractions import Fraction, gcd, lcm, abs_value
from typing import Union


def test_type_annotations() -> None:
    """Test that type annotations work correctly."""
    # Test with int type
    int_frac: Fraction[int] = Fraction[int](1, 2)
    print(f"int_frac: {int_frac}")
    
    # Test with float type
    float_frac: Fraction[float] = Fraction[float](1.5, 2.5)
    print(f"float_frac: {float_frac}")
    
    # Test utility functions with type annotations
    gcd_result: int = gcd(12, 8)
    print(f"gcd_result: {gcd_result}")
    
    lcm_result: int = lcm(4, 6)
    print(f"lcm_result: {lcm_result}")
    
    abs_result: int = abs_value(-5)
    print(f"abs_result: {abs_result}")
    
    # Test operations with type annotations
    result: Fraction[int] = int_frac + Fraction[int](1, 3)
    print(f"result: {result}")


def main() -> None:
    """Run the type annotation tests."""
    print("Type Annotation Tests")
    print("=" * 25)
    print()
    
    test_type_annotations()
    
    print()
    print("Type annotation tests completed!")
    print()
    print("To run type checking with mypy:")
    print("  mypy type_check_test.py")


if __name__ == "__main__":
    main()