cmake_minimum_required(VERSION 3.14)
project(FractionsBenchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Benchmark executable
add_executable(bench_comparison bench_comparison.cpp)

# Link libraries
target_link_libraries(bench_comparison benchmark::benchmark benchmark::benchmark_main)

# Set output directory
set_target_properties(bench_comparison PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)