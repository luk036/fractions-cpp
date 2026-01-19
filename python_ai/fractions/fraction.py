"""
Fraction class implementation with type annotations.

This module provides a Fraction class that supports zero denominators,
allowing representation of infinity and NaN values, similar to the C++
implementation.
"""

from typing import TypeVar, Generic, Union
from .utils import gcd, lcm, abs_value

# Define a generic type variable for numeric types
T = TypeVar('T', int, float, complex)


class Fraction(Generic[T]):
    """
    A class representing a fraction with a numerator and denominator.
    
    This class supports zero denominators, allowing representation of
    infinity and NaN values. The fraction is always kept in a normalized
    form where the denominator is positive and the numerator and denominator
    are coprime.
    
    Attributes:
        _numer: The numerator of the fraction.
        _denom: The denominator of the fraction.
    
    Examples:
        >>> f = Fraction[int](1, 2)
        >>> f.numer()
        1
        >>> f.denom()
        2
        
        >>> f = Fraction[int](3)  # Equivalent to Fraction[int](3, 1)
        >>> f.numer()
        3
        >>> f.denom()
        1
    """
    
    def __init__(self, numer: Union[T, 'Fraction[T]'], denom: T = None) -> None:
        """
        Constructs a new Fraction object.
        
        Args:
            numer: The numerator of the fraction, or another Fraction to copy.
            denom: The denominator of the fraction. If None and numer is a T,
                   creates a fraction with denominator 1.
                   
        Examples:
            >>> f1 = Fraction[int](1, 2)  # 1/2
            >>> f2 = Fraction[int](3)     # 3/1
            >>> f3 = Fraction[int](f1)    # Copy of f1
        """
        if isinstance(numer, Fraction):
            # Copy constructor
            self._numer = numer._numer
            self._denom = numer._denom
        elif denom is None:
            # Constructor with only numerator (denominator = 1)
            self._numer = numer
            self._denom = T(1) if hasattr(T, '__call__') else 1
        else:
            # Constructor with numerator and denominator
            self._numer = numer
            self._denom = denom
            self.normalize()
    
    def normalize(self) -> T:
        """
        Normalizes the fraction to a canonical form where the denominator
        is always non-negative and co-prime with the numerator.
        
        Returns:
            The greatest common divisor of the original numerator and denominator.
            
        Examples:
            >>> f = Fraction[int](6, 8)  # Will be normalized to 3/4
            >>> f.numer()
            3
            >>> f.denom()
            4
            
            >>> f = Fraction[int](-3, -4)  # Will be normalized to 3/4
            >>> f.numer()
            3
            >>> f.denom()
            4
        """
        self._keep_denom_positive()
        return self._reduce()
    
    def _keep_denom_positive(self) -> None:
        """
        Normalizes the fraction by making the denominator positive.
        If the denominator is negative, both numerator and denominator are negated.
        This ensures the denominator is always positive.
        """
        if self._denom < 0:
            self._numer = -self._numer
            self._denom = -self._denom
    
    def _reduce(self) -> T:
        """
        Normalizes the fraction to a canonical form by making the numerator
        and denominator coprime. The gcd of the numerator and denominator
        is computed and used to divide out any common factors.
        
        Returns:
            The greatest common divisor of the original numerator and denominator.
            
        Examples:
            >>> f = Fraction[int](2, 6)  # Will be reduced to 1/3
            >>> f.numer()
            1
            >>> f.denom()
            3
        """
        common = gcd(self._numer, self._denom)
        if common != 1 and common != 0:
            self._numer //= common
            self._denom //= common
        return common
    
    def numer(self) -> T:
        """
        Gets the numerator of the fraction.
        
        Returns:
            The numerator of the fraction.
        """
        return self._numer
    
    def denom(self) -> T:
        """
        Gets the denominator of the fraction.
        
        Returns:
            The denominator of the fraction.
        """
        return self._denom
    
    def cross(self, other: 'Fraction[T]') -> T:
        """
        Computes the cross product of this fraction and another fraction.
        
        The cross product is defined as:
            cross(a/b, c/d) = a*d - b*c
        
        This can be used to compute the determinant of a 2x2 matrix with
        the two fractions as the entries on the diagonal.
        
        Args:
            other: The right-hand fraction to compute the cross product with.
            
        Returns:
            The computed cross product.
            
        Examples:
            >>> Fraction[int](1, 2).cross(Fraction[int](3, 4))
            -2
            >>> Fraction[int](1, 2).cross(Fraction[int](1, 2))
            0
            >>> Fraction[int](1, 2).cross(Fraction[int](-1, 2))
            0
        """
        return self._numer * other._denom - self._denom * other._numer
    
    def reciprocal(self) -> None:
        """
        Inverts the numerator and denominator of the fraction to calculate
        its reciprocal.
        
        Examples:
            >>> f = Fraction[int](2, 3)
            >>> f.reciprocal()
            >>> f.numer()
            3
            >>> f.denom()
            2
        """
        self._numer, self._denom = self._denom, self._numer
        self._keep_denom_positive()
    
    # Comparison operators
    def __eq__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """
        Compares this fraction to another value for equality.
        
        Args:
            other: The value to compare against (Fraction or scalar).
            
        Returns:
            True if the values are equal, False otherwise.
        """
        if isinstance(other, Fraction):
            return self._numer == other._numer and self._denom == other._denom
        else:
            return self._numer == other and self._denom == 1
    
    def __lt__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """
        Compares this fraction to another value for less than.
        
        Args:
            other: The value to compare against (Fraction or scalar).
            
        Returns:
            True if this fraction is less than the other value, False otherwise.
        """
        if isinstance(other, Fraction):
            if self._denom == other._denom:
                return self._numer < other._numer
            
            # Convert to common denominator
            lhs = Fraction(self)
            rhs = Fraction(other)
            
            # Swap denominators and reduce
            lhs._denom, rhs._numer = rhs._numer, lhs._denom
            lhs._reduce()
            rhs._reduce()
            lhs._denom, rhs._numer = rhs._numer, lhs._denom
            
            return lhs._numer * rhs._denom < lhs._denom * rhs._numer
        else:
            if self._denom == 1 or other == 0:
                return self._numer < other
            
            # Convert to common denominator
            lhs = Fraction(self)
            lhs._denom, other = other, lhs._denom
            lhs._reduce()
            return lhs._numer < lhs._denom * other
    
    def __le__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """Less than or equal comparison."""
        return self == other or self < other
    
    def __gt__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """Greater than comparison."""
        if isinstance(other, Fraction):
            return other < self
        else:
            return other < self
    
    def __ge__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """Greater than or equal comparison."""
        return self == other or self > other
    
    def __ne__(self, other: Union[T, 'Fraction[T]']) -> bool:
        """Not equal comparison."""
        return not self == other
    
    # Arithmetic operators
    def __neg__(self) -> 'Fraction[T]':
        """
        Negates this Fraction by negating its numerator.
        
        Returns:
            A new Fraction with the negated numerator.
        """
        result = Fraction(self)
        result._numer = -result._numer
        return result
    
    def __add__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """
        Adds this Fraction to another value.
        
        Args:
            other: The value to add (Fraction or scalar).
            
        Returns:
            A new Fraction containing the sum.
        """
        if isinstance(other, Fraction):
            if self._denom == other._denom:
                result = Fraction(self._numer + other._numer, self._denom)
                return result
            
            common = gcd(self._denom, other._denom)
            if common == 0:
                result = Fraction(other._denom * self._numer + self._denom * other._numer, 0)
                return result
            
            left = self._denom // common
            right = other._denom // common
            denom = self._denom * right
            numer = right * self._numer + left * other._numer
            result = Fraction(numer, denom)
            return result
        else:
            # Add scalar
            if self._denom == 1:
                return Fraction(self._numer + other, 1)
            
            result = Fraction(self._numer + other * self._denom, self._denom)
            return result
    
    def __radd__(self, other: T) -> 'Fraction[T]':
        """Reverse addition for scalar + Fraction."""
        return self + other
    
    def __sub__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """
        Subtracts another value from this Fraction.
        
        Args:
            other: The value to subtract (Fraction or scalar).
            
        Returns:
            A new Fraction containing the difference.
        """
        return self + (-other)
    
    def __rsub__(self, other: T) -> 'Fraction[T]':
        """Reverse subtraction for scalar - Fraction."""
        return other + (-self)
    
    def __mul__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """
        Multiplies this Fraction by another value.
        
        Args:
            other: The value to multiply by (Fraction or scalar).
            
        Returns:
            A new Fraction containing the product.
        """
        if isinstance(other, Fraction):
            # Normalize before multiplication
            lhs = Fraction(self)
            rhs = Fraction(other)
            
            # Swap numerators and reduce
            lhs._numer, rhs._numer = rhs._numer, lhs._numer
            lhs._reduce()
            rhs._reduce()
            
            result = Fraction(lhs._numer * rhs._numer, lhs._denom * rhs._denom)
            return result
        else:
            # Multiply by scalar
            lhs = Fraction(self)
            lhs._numer, other = other, lhs._numer
            lhs._reduce()
            result = Fraction(lhs._numer * other, lhs._denom)
            return result
    
    def __rmul__(self, other: T) -> 'Fraction[T]':
        """Reverse multiplication for scalar * Fraction."""
        return self * other
    
    def __truediv__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """
        Divides this Fraction by another value.
        
        Args:
            other: The value to divide by (Fraction or scalar).
            
        Returns:
            A new Fraction containing the quotient.
        """
        if isinstance(other, Fraction):
            # Special case: 0/0 = 0/1 (zero divided by zero is zero)
            if self._numer == 0 and other._numer == 0:
                return Fraction(0, 1)
            
            # Division is multiplication by reciprocal
            lhs = Fraction(self)
            rhs = Fraction(other)
            
            # Swap denominator of lhs with numerator of rhs
            lhs._denom, rhs._numer = rhs._numer, lhs._denom
            lhs.normalize()
            rhs._reduce()
            
            result = Fraction(lhs._numer * rhs._denom, lhs._denom * rhs._numer)
            return result
        else:
            # Divide by scalar
            lhs = Fraction(self)
            lhs._denom, other = other, lhs._denom
            lhs.normalize()
            result = Fraction(lhs._numer, lhs._denom * other)
            return result
    
    def __rtruediv__(self, other: T) -> 'Fraction[T]':
        """Reverse division for scalar / Fraction."""
        rhs = Fraction(self)
        rhs.reciprocal()
        return rhs * other
    
    # In-place arithmetic operators
    def __iadd__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """In-place addition."""
        if isinstance(other, Fraction):
            if self._denom == other._denom:
                self._numer += other._numer
                self._reduce()
                return self
            
            # Find common denominator
            other_copy = Fraction(other)
            self._denom, other_copy._numer = other_copy._numer, self._denom
            common_n = self._reduce()
            common_d = other_copy._reduce()
            self._denom, other_copy._numer = other_copy._numer, self._denom
            
            self._numer = self._numer * other_copy._denom + self._denom * other_copy._numer
            self._denom *= other_copy._denom
            self._denom, common_d = common_d, self._denom
            self._reduce()
            self._numer *= common_n
            self._denom *= common_d
            self._reduce()
            return self
        else:
            # Add scalar
            if self._denom == 1:
                self._numer += other
                return self
            
            other_copy = other
            self._denom, other_copy = other_copy, self._denom
            common_n = self._reduce()
            self._denom, other_copy = other_copy, self._denom
            self._numer += other_copy * self._denom
            self._numer *= common_n
            self._reduce()
            return self
    
    def __isub__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """In-place subtraction."""
        return self.__iadd__(-other)
    
    def __imul__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """In-place multiplication."""
        if isinstance(other, Fraction):
            # Swap numerators and reduce
            self._numer, other._numer = other._numer, self._numer
            self._reduce()
            other._reduce()
            
            self._numer *= other._numer
            self._denom *= other._denom
            self.normalize()
            return self
        else:
            # Multiply by scalar
            self._numer, other = other, self._numer
            self._reduce()
            self._numer *= other
            self.normalize()
            return self
    
    def __itruediv__(self, other: Union[T, 'Fraction[T]']) -> 'Fraction[T]':
        """In-place division."""
        if isinstance(other, Fraction):
            # Special case: 0/0 = 0/1 (zero divided by zero is zero)
            if self._numer == 0 and other._numer == 0:
                self._denom = 1
                return self
            
            # Swap denominator with numerator of other
            self._denom, other._numer = other._numer, self._denom
            self.normalize()
            other._reduce()
            
            self._numer *= other._denom
            self._denom *= other._numer
            self.normalize()
            return self
        else:
            # Divide by scalar
            self._denom, other = other, self._denom
            self.normalize()
            self._denom *= other
            return self
    
    # Increment and decrement operators
    def __inc__(self) -> 'Fraction[T]':
        """Pre-increment operator (++self)."""
        self._numer += self._denom
        return self
    
    def __dec__(self) -> 'Fraction[T]':
        """Pre-decrement operator (--self)."""
        self._numer -= self._denom
        return self
    
    # String representation
    def __str__(self) -> str:
        """String representation of the fraction."""
        return f"({self._numer}/{self._denom})"
    
    def __repr__(self) -> str:
        """Official string representation of the fraction."""
        return f"Fraction[{type(self._numer).__name__}]({self._numer}, {self._denom})"