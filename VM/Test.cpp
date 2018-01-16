#include "Test.h"
#include "Decoder.h"
#include "Disassembly.h"
#include "Machine.h"
#include "OpCode.h"
#include "ReadBinaryFile.h"
#include "Value.h"
#include <iostream>
#include <string>

using namespace std;

void TestDecoderReadFile(string binaryFile)
{
	try
	{
        auto stream = ReadBinaryFile("/home/jasonhe/Documents/MyCode/CPP/Qt/"
                                     "build-Cygni-Desktop-Debug/TestCases/" +
                                     binaryFile);
        Decoder decoder(stream);
		decoder.Decode();

        Machine machine(100, decoder);
		machine.Run(0);
	}
	catch (const wchar_t* ex)
	{
		wcout << ex << endl;
	}
}

void TestDisassembly(string binaryFile)
{
	try
	{
        auto stream = ReadBinaryFile("/home/jasonhe/Documents/MyCode/CPP/Qt/"
                                     "build-Cygni-Desktop-Debug/TestCases/" +
                                     binaryFile);
        Decoder decoder(stream);
        decoder.Decode();
        Disassembly disassembly(decoder);
		disassembly.ReadAll();
	}
	catch (const wchar_t* ex)
	{
		wcout << ex << endl;
	}
}
