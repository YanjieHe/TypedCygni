#include "Test.h"
#include "Machine.h"
#include "OpCode.h"
#include "Any.h"
#include "Decoder.h"

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
