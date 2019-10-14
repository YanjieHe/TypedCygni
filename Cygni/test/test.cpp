#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include "Lexer.hpp"
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

using namespace std;

std::string utf32_to_utf8(const std::u32string& utf32) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    std::string utf8 = cnv.to_bytes(utf32);
    if (cnv.converted() < utf32.size()) {
        throw std::runtime_error("incomplete conversion");
	} else {
		return utf8;
	}
}

std::ostream& operator<< (std::ostream& stream, const std::u32string& utf32) {
	stream << utf32_to_utf8(utf32);
	return stream;
}


unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
