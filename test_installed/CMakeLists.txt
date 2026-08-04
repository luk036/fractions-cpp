cmake_minimum_required(VERSION 3.14)
project(FractionsInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verify that the installed package can be found and linked via find_package.
find_package(Fractions REQUIRED)

add_executable(fractions_installed_test main.cpp)
target_link_libraries(fractions_installed_test PRIVATE Fractions::Fractions)

enable_testing()
add_test(NAME fractions_installed_test COMMAND fractions_installed_test)
