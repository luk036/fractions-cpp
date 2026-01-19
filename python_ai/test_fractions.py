"""
Comprehensive tests for the fractions library.

This module contains tests for the Fraction class and utility functions,
mirroring the tests from the C++ implementation.
"""

import unittest
from fractions import Fraction, gcd, lcm, abs_value


class TestFraction(unittest.TestCase):
    """Test cases for the Fraction class."""
    
    def test_constructor(self):
        """Test Fraction constructors."""
        # Constructor with numerator and denominator
        f1 = Fraction[int](1, 2)
        self.assertEqual(f1.numer(), 1)
        self.assertEqual(f1.denom(), 2)
        
        # Constructor with only numerator (denominator = 1)
        f2 = Fraction[int](3)
        self.assertEqual(f2.numer(), 3)
        self.assertEqual(f2.denom(), 1)
        
        # Default constructor (0/1)
        f3 = Fraction[int](0)
        self.assertEqual(f3.numer(), 0)
        self.assertEqual(f3.denom(), 1)
        
        # Copy constructor
        f4 = Fraction[int](f1)
        self.assertEqual(f4.numer(), 1)
        self.assertEqual(f4.denom(), 2)
    
    def test_normalize(self):
        """Test fraction normalization."""
        # Test reduction
        f1 = Fraction[int](6, 8)
        self.assertEqual(f1.numer(), 3)
        self.assertEqual(f1.denom(), 4)
        
        # Test negative denominator
        f2 = Fraction[int](1, -2)
        self.assertEqual(f2.numer(), -1)
        self.assertEqual(f2.denom(), 2)
        
        # Test both negative
        f3 = Fraction[int](-3, -4)
        self.assertEqual(f3.numer(), 3)
        self.assertEqual(f3.denom(), 4)
        
        # Test zero denominator (allowed)
        f4 = Fraction[int](1, 0)
        self.assertEqual(f4.numer(), 1)
        self.assertEqual(f4.denom(), 0)
    
    def test_cross(self):
        """Test cross product calculation."""
        f1 = Fraction[int](1, 2)
        f2 = Fraction[int](3, 4)
        self.assertEqual(f1.cross(f2), -2)
        
        f3 = Fraction[int](1, 2)
        f4 = Fraction[int](1, 2)
        self.assertEqual(f3.cross(f4), 0)
        
        f5 = Fraction[int](1, 2)
        f6 = Fraction[int](-1, 2)
        self.assertEqual(f5.cross(f6), 4)
    
    def test_reciprocal(self):
        """Test reciprocal calculation."""
        f1 = Fraction[int](2, 3)
        f1.reciprocal()
        self.assertEqual(f1.numer(), 3)
        self.assertEqual(f1.denom(), 2)
        
        # Test with negative denominator
        f2 = Fraction[int](2, -3)
        f2.reciprocal()
        self.assertEqual(f2.numer(), -3)
        self.assertEqual(f2.denom(), 2)
        # Already normalized after reciprocal
    
    def test_comparison_operators(self):
        """Test comparison operators."""
        f1 = Fraction[int](1, 2)
        f2 = Fraction[int](2, 4)  # Equal to f1
        f3 = Fraction[int](3, 4)
        f4 = Fraction[int](1, 3)
        
        # Equality
        self.assertTrue(f1 == f2)
        self.assertFalse(f1 == f3)
        self.assertTrue(f1 != f3)
        
        # Less than
        self.assertTrue(f1 < f3)
        self.assertTrue(f4 < f1)
        self.assertFalse(f1 < f2)
        
        # Greater than
        self.assertTrue(f3 > f1)
        self.assertTrue(f1 > f4)
        self.assertFalse(f1 > f2)
        
        # Less than or equal
        self.assertTrue(f1 <= f2)
        self.assertTrue(f1 <= f3)
        self.assertFalse(f3 <= f1)
        
        # Greater than or equal
        self.assertTrue(f1 >= f2)
        self.assertTrue(f3 >= f1)
        self.assertFalse(f1 >= f3)
        
        # Comparison with integers
        self.assertFalse(f1 == 0)  # 1/2 != 0
        self.assertFalse(f1 == 1)  # 1/2 != 1
        self.assertTrue(Fraction[int](2, 1) == 2)  # 2/1 == 2
        self.assertTrue(f1 < 1)
        self.assertFalse(f1 < 0)
        self.assertTrue(1 > f1)
        self.assertFalse(0 > f1)
    
    def test_arithmetic_operators(self):
        """Test arithmetic operators."""
        f1 = Fraction[int](1, 2)
        f2 = Fraction[int](1, 3)
        f3 = Fraction[int](3, 4)
        
        # Addition
        result = f1 + f2
        self.assertEqual(result, Fraction[int](5, 6))
        
        result = f1 + 1
        self.assertEqual(result, Fraction[int](3, 2))
        
        result = 1 + f1
        self.assertEqual(result, Fraction[int](3, 2))
        
        # Subtraction
        result = f1 - f2
        self.assertEqual(result, Fraction[int](1, 6))
        
        result = f1 - 1
        self.assertEqual(result, Fraction[int](-1, 2))
        
        result = 1 - f1
        self.assertEqual(result, Fraction[int](1, 2))
        
        # Multiplication
        result = f1 * f2
        self.assertEqual(result, Fraction[int](1, 6))
        
        result = f1 * 2
        self.assertEqual(result, Fraction[int](1, 1))
        
        result = 2 * f1
        self.assertEqual(result, Fraction[int](1, 1))
        
        # Division
        result = f1 / f2
        self.assertEqual(result, Fraction[int](3, 2))
        
        result = f1 / 2
        self.assertEqual(result, Fraction[int](1, 4))
        
        result = 2 / f1
        self.assertEqual(result, Fraction[int](4, 1))
        
        # Negation
        result = -f1
        self.assertEqual(result, Fraction[int](-1, 2))
    
    def test_inplace_operators(self):
        """Test in-place arithmetic operators."""
        f1 = Fraction[int](1, 2)
        f2 = Fraction[int](1, 3)
        
        # In-place addition
        f1 += f2
        self.assertEqual(f1, Fraction[int](5, 6))
        
        f1 = Fraction[int](1, 2)
        f1 += 1
        self.assertEqual(f1, Fraction[int](3, 2))
        
        # In-place subtraction
        f1 -= f2
        self.assertEqual(f1, Fraction[int](7, 6))
        
        f1 -= 1
        self.assertEqual(f1, Fraction[int](1, 6))
        
        # In-place multiplication
        f1 = Fraction[int](1, 2)
        f1 *= f2
        self.assertEqual(f1, Fraction[int](1, 6))
        
        f1 = Fraction[int](1, 2)
        f1 *= 2
        self.assertEqual(f1, Fraction[int](1, 1))
        
        # In-place division
        f1 = Fraction[int](1, 2)
        f1 /= f2
        self.assertEqual(f1, Fraction[int](3, 2))
        
        f1 = Fraction[int](1, 2)
        f1 /= 2
        self.assertEqual(f1, Fraction[int](1, 4))
    
    def test_zero_denominator(self):
        """Test operations with zero denominators."""
        f1 = Fraction[int](1, 0)  # Infinity
        f2 = Fraction[int](0, 1)  # Zero
        f3 = Fraction[int](0, 0)  # NaN
        
        # Division by zero
        result = f2 / f3  # 0/0 = 0/1 (zero divided by zero is zero)
        self.assertEqual(result, Fraction[int](0, 1))
        
        # Multiplication with zero denominator
        result = f1 * f2
        self.assertEqual(result, Fraction[int](0, 0))  # Infinity * 0 = NaN
    
    def test_string_representation(self):
        """Test string representation."""
        f1 = Fraction[int](1, 2)
        self.assertEqual(str(f1), "(1/2)")
        self.assertEqual(repr(f1), "Fraction[int](1, 2)")


class TestUtils(unittest.TestCase):
    """Test cases for utility functions."""
    
    def test_abs_value(self):
        """Test abs_value function."""
        self.assertEqual(abs_value(5), 5)
        self.assertEqual(abs_value(-5), 5)
        self.assertEqual(abs_value(0), 0)
        self.assertEqual(abs_value(5.5), 5.5)
        self.assertEqual(abs_value(-5.5), 5.5)
    
    def test_gcd(self):
        """Test gcd function."""
        self.assertEqual(gcd(12, 8), 4)
        self.assertEqual(gcd(12, 4), 4)
        self.assertEqual(gcd(4, 4), 4)
        self.assertEqual(gcd(0, 8), 8)
        self.assertEqual(gcd(8, 0), 8)
        self.assertEqual(gcd(0, 0), 0)
        self.assertEqual(gcd(-12, 8), 4)
        self.assertEqual(gcd(12, -8), 4)
        self.assertEqual(gcd(-12, -8), 4)
    
    def test_lcm(self):
        """Test lcm function."""
        self.assertEqual(lcm(4, 6), 12)
        self.assertEqual(lcm(0, 6), 0)
        self.assertEqual(lcm(6, 0), 0)
        self.assertEqual(lcm(0, 0), 0)
        self.assertEqual(lcm(-4, 6), 12)
        self.assertEqual(lcm(4, -6), 12)
        self.assertEqual(lcm(-4, -6), 12)


if __name__ == '__main__':
    unittest.main()