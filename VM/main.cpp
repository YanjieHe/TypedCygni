#include "Any.h"
#include "Test.h"
#include <array>
#include <iostream>
#include <locale>

using namespace std;

// int main(int argc, char* argv[])
int main()
{
	locale loc("");
	locale::global(loc);
    TestDisassembly("factorial.bin");
    TestDecoderReadFile("factorial.bin");

	return 0;
}
