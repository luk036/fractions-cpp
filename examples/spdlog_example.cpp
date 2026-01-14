#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "fractions/fastfractions.hpp"

int main() {
    // Create a color multi-threaded logger
    auto console = spdlog::stdout_color_mt("console");
    console->set_level(spdlog::level::debug);

    // Test basic logging
    console->info("Welcome to spdlog example with fractions library!");
    console->debug("Debug message");
    console->warn("Warning message");
    console->error("Error message");

    // Test fractions with logging
    fractions::FastFraction<int> f1(3, 4);
    fractions::FastFraction<int> f2(2, 5);

    console->info("Working with fractions:");
    console->info("f1 = {}/{}", f1._numer, f1._denom);
    console->info("f2 = {}/{}", f2._numer, f2._denom);

    auto sum = f1 + f2;
    console->info("f1 + f2 = {}/{}", sum._numer, sum._denom);

    auto product = f1 * f2;
    console->info("f1 * f2 = {}/{}", product._numer, product._denom);

    console->info("Example completed successfully!");

    return 0;
}