# GEMINI.md

## Project Overview

This is a C++ project that implements a fraction library. It uses modern CMake practices for building, testing, and dependency management. The core of the project is a single-header library, `fractions.hpp`, which provides a `FastFraction` class with support for arithmetic and comparison operations.

The project is well-structured, with separate directories for the library code (`include`, `source`), tests (`test`), and a standalone example (`standalone`). It also includes configuration for continuous integration, code coverage, and code formatting.

## Building and Running

The project uses CMake for building. Here are the key commands:

### Build Everything

This is the recommended way to build all targets, including the library, tests, and standalone executable.

```bash
cmake -S all -B build
cmake --build build
```

### Run Tests

After building, you can run the tests with the following command:

```bash
./build/test/FractionsTests
```

### Run Standalone Executable

The standalone executable demonstrates the usage of the library.

```bash
./build/standalone/Fractions --help
```

### Code Formatting

The project uses `clang-format` and `cmake-format` to enforce a consistent code style.

```bash
# Check formatting
cmake --build build --target format

# Fix formatting
cmake --build build --target fix-format
```

## Development Conventions

*   **CMake:** The project follows modern CMake practices, using `CPM.cmake` for dependency management.
*   **Testing:** Tests are written using the doctest framework and are located in the `test` directory.
*   **Code Style:** Code style is enforced by `clang-format` and `cmake-format`. Configuration files (`.clang-format`, `.cmake-format`) are present in the root directory.
*   **Continuous Integration:** The project uses GitHub Actions for CI. The workflow files are in the `.github/workflows` directory.
