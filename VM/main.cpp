#include "Test.h"
#include "Any.h"
#include <iostream>
#include <locale>

using namespace std;

typedef unsigned char* byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++)
	{
		cout << i << ": " << (int)start[i] << endl;
	}
}

void show_ushort(unsigned short x)
{
	show_bytes((byte_pointer) &x, sizeof(unsigned short));
}

void show_int(i32 x)
{
	show_bytes((byte_pointer) &x, sizeof(i32));
}

int main(int argc, char *argv[])
{
	show_ushort(65534);
	cout << "+++++++++++++++++++++++++++++" << endl;
	show_int(998888918);
	cout << "+++++++++++++++++++++++++++++" << endl;
	locale loc("");
	locale::global(loc);

	// TestDisassembly();
	TestDecoderReadFile();
	return 0;
}
