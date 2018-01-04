#include "Test.h"
#include "Any.h"
#include "Decoder.h"
#include "Disassembly.h"
#include "Machine.h"
#include "OpCode.h"
#include <iostream>
#include <string>

using namespace std;

void TestByteCode()
{
    ByteCode code = vector<byte>{static_cast<byte>(OpCode::push_i32_1byte), 32,
                                 static_cast<byte>(OpCode::push_i32_1byte), 43,
                                 static_cast<byte>(OpCode::add_i32)};
    Machine machine(Memory(), 10);
	machine.LoadProgram(&code);
	machine.Run(0);
}

void TestDecoder()
{
    vector<byte> code = {static_cast<byte>(OpCode::constant_i32),
                         10,
                         20,
                         30,
                         40,
                         static_cast<byte>(OpCode::constant_f64),
                         90,
                         23,
                         43,
                         58,
                         12,
                         32,
                         43,
                         55};
	Decoder decoder(code);
	decoder.Decode();
}

void TestDecoderReadFile(string binaryFile)
{
	try
	{
        Decoder decoder("/home/jasonhe/Documents/MyCode/CPP/Qt/"
                        "build-Cygni-Desktop-Debug/TestCases/" +
                        binaryFile);
		decoder.Decode();
        Machine machine(Memory(), 100);
        ByteCode code = decoder.code;
        machine.LoadProgram(&code);
        for (Function f : decoder.functions)
		{
			machine.LoadFunction(f);
		}
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
        Disassembly disassembly("/home/jasonhe/Documents/MyCode/CPP/Qt/"
                                "build-Cygni-Desktop-Debug/TestCases/" +
                                binaryFile);
		disassembly.ReadCode();
	}
	catch (const wchar_t* ex)
	{
		wcout << ex << endl;
	}
}
