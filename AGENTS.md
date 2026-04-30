# AGENTS.md - Agentic Coding Guidelines for fractions-cpp

This file provides context for agentic coding agents operating in this repository.

## Project Overview

- **Language**: C++ (header-only library with optional source)
- **C++ Standard**: C++11 (primary), C++17 (tests)
- **Build System**: CMake (3.14-3.22) + xmake
- **Test Framework**: doctest

## Build Commands

### Building the Library

```bash
# Using CMake
cmake -S . -B build
cmake --build build

# Using xmake
xmake
```

### Running Tests

```bash
# Using CMake (recommended)
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# Or run the test executable directly
./build/test/FractionsTests

# Run a single test case
./build/test/FractionsTests -tc="ExtFraction<int> add"

# Using xmake
xmake test
```

### Code Formatting

```bash
# Check formatting (CMake)
cmake -S test -B build/test
cmake --build build/test --target format

# Apply formatting fixes
cmake --build build/test --target fix-format

# Also supports direct clang-format:
clang-format -i --style=file source/*.cpp
```

### Additional Build Options

```bash
# Code coverage
cmake -S test -B build/test -DENABLE_TEST_COVERAGE=1
cmake --build build/test

# Sanitizers (Address, Memory, Undefined, Thread, Leak)
cmake -S test -B build/test -DUSE_SANITIZER=Address

# Static analyzers (clang-tidy, iwyu, cppcheck)
cmake -S test -B build/test -DUSE_STATIC_ANALYZER=clang-tidy

# Build all subprojects at once
cmake -S all -B build
cmake --build build
```

## Code Style Guidelines

### Formatting (`.clang-format`)

- **Based On**: Google
- **Column Limit**: 100 characters
- **Indent Width**: 4 spaces
- **Brace Breaking**: Attach
- **Namespace Indentation**: All

### Naming Conventions

- **Classes/Types**: `PascalCase` (e.g., `Fraction`, `ExtFraction`)
- **Functions**: `PascalCase` (e.g., `normalize()`, `gcd_recur()`)
- **Member variables**: `_camelCase` with leading underscore (e.g., `_numer`, `_denom`)
- **Constants**: `SCREAMING_SNAKE_CASE`
- **Templates**: `PascalCase` with single letter fallback (e.g., `template<typename T>`)

### Imports and Includes

- Use `#include <fractions/fractions.hpp>` for the main library
- Use `#include <fractions/extfractions.hpp>` for extended functionality
- STL headers: `#include <type_traits>`, `#include <utility>`, etc.
- External libs: `#include <fmt/format.h>`, `#include <spdlog/spdlog.h>`

### Code Patterns

**Header guards**: Use `#pragma once`

**_constexpr handling**:
```cpp
#if __cpp_constexpr >= 201304
#    define CONSTEXPR14 constexpr
#else
#    define CONSTEXPR14 inline
#endif
```

**SFINAE for overloads**:
```cpp
template <typename T> auto abs(const T& val_a) -> 
    typename std::enable_if<std::is_unsigned<T>::value, T>::type { ... }
```

**Member initialization** (prefer in-class initializers):
```cpp
CONSTEXPR14 Fraction() : _numer(0), _denom(1) {}
```

**Friend operators** (for mixed-type comparisons):
```cpp
friend CONSTEXPR14 auto operator==(const Fraction& lhs, const T& rhs) -> bool { ... }
```

**Stream output**:
```cpp
template <typename _Stream>
friend auto operator<<(_Stream& out_stream, const Fraction& frac) -> _Stream& {
    out_stream << "(" << frac.numer() << "/" << frac.denom() << ")";
    return out_stream;
}
```

### Error Handling

- **No exceptions**: This library uses assertions and return values
- **Division by zero**: Handled specially (0/0 = 0/1 allowed)
- Use `CHECK()` and `CHECK_EQ()` from doctest for test assertions
- Never use `try/catch` - this is a header-only math library

### Documentation

- Use Doxygen-style comments with `@`, `@file`, `@tparam`, etc.
- Include ASCII diagrams in docstrings for math operations:
```cpp
/** 
 * ```svgbob
 *    a     c     a*d + b*c
 *   --- + --- = ---------
 *    b     d       b*d
 * ```
 */
```

## Test Structure

### Test File Organization

- Located in `test/source/`
- Main test file: `test_frac.cpp`
- Extended tests: `test_frac_extended.cpp`, `test_pyfractions.cpp`
- Comprehensive: `test_frac_comprehensive.cpp`, `test_frac_gaps.cpp`
- Property-based: `test_frac_property.cpp`

### Test Patterns

```cpp
#include <doctest/doctest.h>

TEST_CASE("Fraction<int> operation") {
    const auto a = Fraction<int>{3, 4};
    const auto b = Fraction<int>{5, 6};
    CHECK_EQ(a + b, Fraction<int>{19, 12});
}
```

### Running Specific Tests

```bash
# Run tests matching a pattern
./build/test/FractionsTests -tc="*add*"

# List all test cases
./build/test/FractionsTests -ltc

# Exit after first failure
./build/test/FractionsTests -eaf
```

## Library Structure

```
include/fractions/
  fractions.hpp      - Core Fraction template
  extfractions.hpp  - Extended functionality
  pyfractions.hpp  - Python-compatible interface

source/
  (empty - header-only library)

test/source/
  test_frac.cpp    - Basic tests
```

## Key Implementation Details

### Normalization

Always normalize after construction/operations:
- Denominator must be positive
- Numerator and denominator must be coprime

### Zero Denominator Handling

This library ALLOWS zero denominators:
- `0/0` = `0/1` (identity)
- `n/0` = infinity representation (keeps numerator sign)
- Division by zero returns zero numerator with denominator 1

### Arithmetic Operations

- Use cross-multiplication for comparison: `a/b < c/d` → `a*d < b*c`
- LCM for addition: `(a*lcm(b,d)/b) + (c*lcm(b,d)/d)`
- GCD reduction for simplification

## Dependencies

- **fmt** (12.1.0) - String formatting
- **spdlog** (1.17.0) - Logging (optional)
- **doctest** (2.4.11) - Testing framework
- **RapidCheck** (master) - Property-based testing (tests only)

## Git Workflow

- Branches: `main` for releases, feature branches for development
- Commits: Conventional commit messages
- Never commit build artifacts or files in `build/` directory