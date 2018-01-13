#include "Disassembly.h"
#include "Exception.h"
#include "OpCode.h"
#include <fstream>
#include <iostream>

using namespace std;

Disassembly::Disassembly(vector<byte>& stream) : reader(stream)
{
}

void Disassembly::ReadAll()
{
    ReadFunction();
    ReadCode();
}

void Disassembly::ReadCode()
{
    while (!reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Read());
		if (op == OpCode::function_end)
        {
			wcout << L"*** end of function ***" << endl << endl;
            return;
		}
		else if (OperandSize(op) == 0)
		{
            wcout << opcode_to_wstring(op) << endl;
		}
		else if (OperandSize(op) == 1)
		{
			wcout << opcode_to_wstring(op) << L"  ";
            wcout << L"byte: " << reader.Read() << endl;
		}
		else if (OperandSize(op) == 2)
		{
			wcout << opcode_to_wstring(op) << L"  ";
            wcout << L"ushort: " << reader.ReadUShort() << endl;
		}
		else
		{
            wcout << L"not implemented opcode: " + opcode_to_wstring(op)
                  << endl;
			throw L"not implemented opcode: " + opcode_to_wstring(op);
		}
	}
}

void Disassembly::ReadFunction()
{
    while (!reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Peek());
		if (op == OpCode::function_begin)
        {
            reader.Read();
            wcout << endl << L"*** begin of function ***" << endl;

            wstring name = reader.ReadString();
            i32 args_size = reader.ReadUShort();
            i32 stack = reader.ReadUShort();
            i32 locals = stack - args_size;
            wcout << name << endl;
			wcout << L"args_size: " << args_size << endl;
			wcout << L"stack: " << stack << endl;
            wcout << L"locals: " << locals << endl;
            ReadCode();
		}
		else
		{
            ReadCode();
		}
	}
}
