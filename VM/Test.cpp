#include "Test.h"
#include "Machine.h"
#include "OpCode.h"
#include "Any.h"
#include "Decoder.h"
#include "Disassembly.h"
#include <string>
#include <iostream>

using namespace std;

void TestByteCode()
{
	vector<byte> code = {
		(byte)OpCode::push_i32_1byte,
		(byte)32,
		(byte)OpCode::push_i32_1byte,
		(byte)43,
		(byte)OpCode::add_i32
	};
	Machine machine(ConstantPool(), 10);
	machine.LoadProgram(&code);
	machine.Run(0);
}

void TestDecoder()
{
	vector<byte> code = {
		(byte)OpCode::constant_i32,
		10, 20, 30, 40,
		(byte)OpCode::constant_f64,
		90, 23, 43, 58, 12, 32, 43, 55
	};
	Decoder decoder(code);
	decoder.Decode();
}

void TestDecoderReadFile()
{
	try
	{
		Decoder decoder("/home/jasonhe/Documents/MyCode/CPP/Cygni/TestCases/factorial.bin");
		decoder.Decode();
		Machine machine(ConstantPool(), 100);
		wcout << L"code size: " << decoder.code.size() << endl;
		machine.LoadProgram(&(decoder.code));
		wcout << "stating loading functions" << endl;
		for (Function f: decoder.functions)
		{
			machine.LoadFunction(f);
		}
		wcout << "finish loading functions" << endl;
		machine.Run(0);
	}
	catch (const wchar_t* ex)
	{
		wcout << ex << endl;
	}
}

void TestDisassembly()
{
	try
	{
		Disassembly disassembly("/home/jasonhe/Documents/MyCode/CPP/Cygni/TestCases/factorial.bin");
		disassembly.ReadCode();
	}
	catch (const wchar_t* ex)
	{
		wcout << ex << endl;
	}
}
