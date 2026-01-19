# Fractions Python Library

A Python implementation of the fractions library with type annotations, converted from the C++ implementation. This library supports zero denominators, allowing representation of infinity and NaN values.

## Features

- **Type Annotations**: Full support for Python type hints
- **Zero Denominator Support**: Allows fractions with zero denominators to represent infinity and NaN
- **Comprehensive Operations**: Addition, subtraction, multiplication, division, comparison, and more
- **Automatic Normalization**: Fractions are automatically normalized to their simplest form
- **Cross Product Calculation**: Compute the cross product of two fractions
- **Reciprocal Calculation**: Calculate the reciprocal of a fraction

## Installation

Simply copy the `fractions` directory to your project or add it to your Python path.

## Usage

### Basic Usage

```python
from fractions import Fraction

# Create fractions
f1 = Fraction[int](1, 2)  # 1/2
f2 = Fraction[int](3, 4)  # 3/4
f3 = Fraction[int](5)     # 5/1

# Basic operations
result = f1 + f2  # 5/4
result = f1 - f2  # -1/4
result = f1 * f2  # 3/8
result = f1 / f2  # 2/3

# Comparison
if f1 < f2:
    print("f1 is less than f2")

# String representation
print(f1)  # (1/2)
```

### Zero Denominator Support

```python
from fractions import Fraction

# Create fractions with zero denominators
infinity = Fraction[int](1, 0)  # Represents infinity
zero = Fraction[int](0, 1)      # Represents zero
nan = Fraction[int](0, 0)       # Represents NaN

# Operations with zero denominators
result = zero / nan  # 0/0 = 0/1 (zero divided by zero is zero)
```

### Using Utility Functions

```python
from fractions import gcd, lcm, abs_value

# GCD calculation
result = gcd(12, 8)  # 4

# LCM calculation
result = lcm(4, 6)   # 12

# Absolute value
result = abs_value(-5)  # 5
```

## API Reference

### Fraction Class

#### Constructors

- `Fraction[T](numer, denom)`: Create a fraction with numerator and denominator
- `Fraction[T](numer)`: Create a fraction with numerator and denominator 1
- `Fraction[T](other)`: Copy constructor

#### Methods

- `numer()`: Get the numerator
- `denom()`: Get the denominator
- `cross(other)`: Compute cross product with another fraction
- `reciprocal()`: Invert the fraction

#### Operators

- Arithmetic: `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`, unary `-`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`

### Utility Functions

- `gcd(a, b)`: Greatest common divisor
- `lcm(a, b)`: Least common multiple
- `abs_value(val)`: Absolute value

## Testing

Run the tests with:

```bash
python -m unittest test_fractions.py
```

## Differences from C++ Implementation

1. **Type System**: Uses Python's type hints instead of C++ templates
2. **Method Names**: Python-style method names (e.g., `numer()` instead of `numer()`)
3. **Operator Overloading**: Python's magic methods for operator overloading
4. **Memory Management**: Automatic garbage collection instead of manual memory management

## License

This project is licensed under the MIT License - see the LICENSE file for details.