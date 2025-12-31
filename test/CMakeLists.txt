cmake_minimum_required(VERSION 3.14...3.22)

project(FractionsTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:doctest/doctest@2.4.11")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")
CPMAddPackage(
  NAME RapidCheck
  GITHUB_REPOSITORY emil-e/rapidcheck
  GIT_TAG master
  OPTIONS "RAPIDCHECK_BUILD_EXAMPLES=OFF" "RAPIDCHECK_BUILD_TESTS=OFF"
)

if(TEST_INSTALLED_VERSION)
  find_package(Fractions REQUIRED)
else()
  CPMAddPackage(NAME Fractions SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} doctest::doctest Fractions::Fractions rapidcheck)
target_include_directories(${PROJECT_NAME} PRIVATE ${RapidCheck_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(Fractions PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(Fractions PUBLIC /W4 /WX /wd4819 /wd4996)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add FractionsTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

include(../cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(Fractions PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(Fractions PUBLIC -fprofile-arcs -ftest-coverage)
endif()
