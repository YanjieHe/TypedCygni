#include "StringBuilder.h"
#include "Test.h"
#include <iostream>
#include <locale>

using namespace std;

// int main(int argc, char *argv[])
int main()
{
    locale::global(locale(""));
	// TestStringBuilder();
	// TestLexer();
	// TestType();
	// TestParser();
	// TestScope();
	// TestTypeChecker();
    TestCompiler();
	return 0;
}
