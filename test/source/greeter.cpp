#include <doctest/doctest.h>
#include <fractions/fractions.h>
#include <fractions/version.h>

#include <string>

TEST_CASE("Fractions") {
  using namespace fractions;

  Fractions fractions("Tests");

  CHECK(fractions.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(fractions.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(fractions.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(fractions.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("Fractions version") {
  static_assert(std::string_view(FRACTIONS_VERSION) == std::string_view("1.0"));
  CHECK(std::string(FRACTIONS_VERSION) == std::string("1.0"));
}
