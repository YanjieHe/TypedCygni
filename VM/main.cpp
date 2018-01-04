#include "Any.h"
#include "Test.h"
#include <array>
#include <iostream>
#include <locale>

using namespace std;

int main(int argc, char* argv[])
{
	locale loc("");
	locale::global(loc);

    //	wcout << "argc: " << argc << endl;
    //	wcout << "argv[0]: " << argv[0] << endl;
    //	wcout << "argv[1]: " << argv[1] << endl;
    // TestDisassembly();
	TestDecoderReadFile("factorial.bin");

	return 0;
}
