#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Test_Lexer.hpp"

using namespace cygni;

TEST_CASE("test the lexer", "[Lexer]") {
    SECTION("define an variable with initial value of an integer") {
		std::vector<Token> tokens = ParseTokensFromText(U"var x = 20");

        REQUIRE(tokens.size() == 5);
        REQUIRE(SameToken(tokens[0], Tag::Var));
        REQUIRE(SameToken(tokens[1], Tag::Identifier));
        REQUIRE(SameToken(tokens[2], Tag::Assign));
        REQUIRE(SameToken(tokens[3], Tag::Integer, std::u32string(U"20")));
        REQUIRE(SameToken(tokens[4], Tag::Eof));
    }

    SECTION("define an variable with initial value of an character") {
		std::vector<Token> tokens = ParseTokensFromText(U"var a: Char = 'b'");

        REQUIRE(tokens.size() == 7);
        REQUIRE(SameToken(tokens[0], Tag::Var));
        REQUIRE(SameToken(tokens[1], Tag::Identifier));
        REQUIRE(SameToken(tokens[2], Tag::Colon));
        REQUIRE(SameToken(tokens[3], Tag::Identifier));
        REQUIRE(SameToken(tokens[4], Tag::Assign));
        REQUIRE(SameToken(tokens[5], Tag::Character, std::u32string(U"b")));
        REQUIRE(SameToken(tokens[6], Tag::Eof));
    }
}
