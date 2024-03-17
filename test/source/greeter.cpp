#include <doctest/doctest.h>
#include <fractions/version.h>
#include <greeter/greeter.h>

#include <string>

TEST_CASE("Fractions") {
  using namespace fractions;

  Fractions fractions("Tests");

  CHECK(fractions.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(fractions.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(fractions.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(fractions.greet(LanguageCode::FR) == "Bonjour Tests!");
}
