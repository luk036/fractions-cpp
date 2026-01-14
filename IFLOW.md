# iFlow Context File

## Project Overview

This is a C++ library project called **fractions-cpp** that provides a FastFraction class for handling rational numbers. The library allows operations with fractions, including addition, subtraction, multiplication, division, comparison, and more. It supports special cases like allowing zero denominators and handles various numeric types.

The project follows modern CMake practices and includes:
- A header-only library implementation
- Integrated test suite using doctest
- Support for continuous integration
- Code formatting with clang-format and cmake-format
- Dependency management via CPM.cmake
- Documentation generation with Doxygen

## Key Files and Structure

### Library Implementation
- `include/fractions/fractions.hpp` - Main header file containing the FastFraction class template and related functions (gcd, lcm, abs)

### Build Configuration
- `CMakeLists.txt` - Main CMake configuration for the library
- `cmake/CPM.cmake` - Dependency management via CPM.cmake
- `cmake/tools.cmake` - Additional tools for sanitizers, static analyzers, etc.

### Tests
- `test/source/test_frac.cpp` - Main test file with comprehensive tests for FastFraction operations
- `test/source/test_frac_extended.cpp` - Extended tests for mixed types and other features
- `test/source/main.cpp` - Test runner entry point

### Documentation and Configuration
- `README.md` - Project documentation and usage instructions
- `.clang-format` - C++ code formatting configuration
- `.cmake-format` - CMake code formatting configuration

## Building and Running

### Standalone Target
```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/Fractions --help
```

### Testing
```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test
```

### Building Everything at Once
```bash
cmake -S all -B build
cmake --build build
```

### Code Formatting
```bash
# Check formatting
cmake --build build --target format

# Apply formatting
cmake --build build --target fix-format
```

## Development Conventions

- C++11 standard is used (set in CMakeLists.txt)
- Code formatting enforced by clang-format and cmake-format
- Modern CMake practices with CPM for dependency management
- Header-only library design
- Comprehensive test coverage using doctest
- Zero denominators are allowed in the FastFraction class

## Special Features

- Fractions can have zero denominators (representing infinity/nan values)
- Automatic normalization of fractions
- Cross product calculation for fractions
- Support for various numeric types (int, long, short, etc.)
- Comprehensive operator overloading for arithmetic and comparison operations

## Dependencies

- doctest (for testing)
- fmt library (for formatting)
- CPM.cmake (for dependency management)