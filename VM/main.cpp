#include "Test.h"
#include "Value.h"
#include <array>
#include <iostream>
#include <locale>

using namespace std;

// int main(int argc, char* argv[])
int main()
{
	locale loc("");
	locale::global(loc);
    TestDisassembly("fib.bin");
    TestDecoderReadFile("fib.bin");

	return 0;
}
