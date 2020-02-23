#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "Test_Lexer.hpp"
#include <locale>
#include <codecvt>
#include <iostream>

using namespace cygni;
using std::cout;
using std::endl;


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

    SECTION("test hello world") {
        std::vector<Token> tokens = ParseTokensFromSource("test-code-snippets/hello_world.cyg");

        REQUIRE(tokens.size() == 21);
        REQUIRE(SameToken(tokens[0], Tag::Module));
        REQUIRE(SameToken(tokens[1], Tag::Identifier));
        REQUIRE(SameToken(tokens[2], Tag::LeftBrace));
        REQUIRE(SameToken(tokens[3], Tag::Def));
        REQUIRE(SameToken(tokens[4], Tag::Identifier));
        REQUIRE(SameToken(tokens[5], Tag::LeftParenthesis));
        REQUIRE(SameToken(tokens[6], Tag::RightParenthesis));
        REQUIRE(SameToken(tokens[7], Tag::Colon));
        REQUIRE(SameToken(tokens[8], Tag::Identifier));
        REQUIRE(SameToken(tokens[9], Tag::LeftBrace));
        REQUIRE(SameToken(tokens[10], Tag::Identifier));
        REQUIRE(SameToken(tokens[11], Tag::Dot));
        REQUIRE(SameToken(tokens[12], Tag::Identifier));
        REQUIRE(SameToken(tokens[13], Tag::Dot));
        REQUIRE(SameToken(tokens[14], Tag::Identifier));
        REQUIRE(SameToken(tokens[15], Tag::LeftParenthesis));
        REQUIRE(SameToken(tokens[16], Tag::String));
        REQUIRE(SameToken(tokens[17], Tag::RightParenthesis));
        REQUIRE(SameToken(tokens[18], Tag::RightBrace));
        REQUIRE(SameToken(tokens[19], Tag::RightBrace));
        REQUIRE(SameToken(tokens[20], Tag::Eof));
    }
}
