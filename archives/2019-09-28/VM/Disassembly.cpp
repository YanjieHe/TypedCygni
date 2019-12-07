#include "Disassembly.h"
#include "BinaryStreamReader.h"
#include "Exception.h"
#include "OpCode.h"
#include <fstream>
#include <iostream>
using namespace std;

Disassembly::Disassembly(Decoder& decoder) : decoder{decoder}
{
}

void Disassembly::ReadAll()
{
    ReadFunctions();
    ReadCode(decoder.topCode);
}

void Disassembly::ReadCode(ByteCode code)
{
    vector<byte> vec = code.ToVector();
    BinaryStreamReader reader(vec);
    while (!reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Read());
        if (OperandSize(op) == 0)
		{
            wcout << OpCodeToString(op) << endl;
		}
		else if (OperandSize(op) == 1)
		{
			wcout << OpCodeToString(op) << L"  ";
            wcout << L"byte: " << reader.Read() << endl;
		}
		else if (OperandSize(op) == 2)
		{
			wcout << OpCodeToString(op) << L"  ";
            wcout << L"ushort: " << reader.ReadUShort() << endl;
		}
		else
		{
            wcout << L"not implemented opcode: " + OpCodeToString(op) << endl;
			throw L"not implemented opcode: " + OpCodeToString(op);
		}
	}
}

void Disassembly::ReadFunctions()
{
    for (Function& f : decoder.functions)
    {
        wstring name = f.name;
        i32 args_size = f.parametersSize;
        i32 stack = f.frameSize;
        i32 locals = stack - args_size;
        wcout << name << endl;
        wcout << L"args_size: " << args_size << endl;
        wcout << L"stack: " << stack << endl;
        wcout << L"locals: " << locals << endl;
        ReadCode(f.code);
    }
}
