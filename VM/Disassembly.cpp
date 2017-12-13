#include "Disassembly.h"
#include "OpCode.h"
#include <iostream>
#include <fstream>

using namespace std;

Disassembly::Disassembly(string path)
{
	ifstream file(path, ifstream::binary);
	if (file)
	{
		byte b;
		while (!file.eof())
		{
			file.read((char*)&b, sizeof(b));
			stream.push_back(b);
		}
		file.close();
		stream.pop_back();
		wcout << "stream size: " << stream.size() << endl;
	}
	else
	{
		wcout << "cannot open binary file" << endl;
		throw "cannot open binary file" + path;
	}
}

void Disassembly::ReadCode()
{
	i32 i = ReadFunction(0);	
	ReadCode(i);
}

i32 Disassembly::ReadCode(i32 offset)
{
	i32 i = offset;
	while (i < stream.size())
	{
		OpCode op = (OpCode) stream.at(i);
		if (op == OpCode::function_end)
		{
			i++;
			wcout << L"*** end of function ***" << endl << endl;
			return i;
		}
		else if (OperandSize(op) == 0)
		{
			wcout << i - offset << ": ";
			wcout << opcode_to_wstring(op) << endl;
			i++;
		}
		else if (OperandSize(op) == 1)
		{
			wcout << i - offset << ": ";
			wcout << opcode_to_wstring(op) << L"  ";
			i++;
			wcout << stream.at(i) << endl;
			i++;
		}
		else if (OperandSize(op) == 2)
		{
			wcout << i - offset << ": ";
			wcout << opcode_to_wstring(op) << L"  ";
			i++;
			wcout << ReadUShort(i) << endl;
			i += 2;
		}
		else
		{
			wcout << L"not implemented opcode: " + opcode_to_wstring(op) << endl;
			throw L"not implemented opcode: " + opcode_to_wstring(op);
		}
	}
	return i;
}

i32 Disassembly::ReadFunction(i32 offset)
{
	i32 i = offset;
	while (i < stream.size())
	{
		OpCode op = (OpCode) stream.at(i);
		if (op == OpCode::function_begin)
		{
			i++;
			wcout << endl;
			wcout << L"*** begin of function ***" << endl;
			i32 args_size = ReadUShort(i);
			i += 2;
			i32 stack = ReadUShort(i);
			i += 2;
			i32 locals = stack - args_size;
			wcout << L"args_size: " << args_size << endl;
			wcout << L"stack: " << stack << endl;
			wcout << L"locals: " << locals << endl;
			i = ReadCode(i);
		}
		else
		{
			return i;
		}
	}
	return i;
}

i32 Disassembly::ReadUShort(i32 offset)
{
	return (stream[offset + 1] << 8) + stream[offset];
}

i32 Disassembly::ReadInt32(i32 offset)
{
	return 
		(stream[offset + 3] << 24)
		+ (stream[offset + 2] << 16)
		+ (stream[offset + 1] << 8)
		+ stream[offset];
}

i64 Disassembly::ReadInt64(i32 offset)
{
	i64 n = 0;
	for (i32 i = 0; i < sizeof(i64); ++i)
	{
		n <<= 8;
		n |= (stream[offset + i]);
	}
	return n;
}

f64 Disassembly::ReadFloat64(i32 offset)
{
	union 
	{
		i64 i64_v;
		f64 f64_v;
	} value;
	value.i64_v = ReadInt64(offset);
	return value.f64_v;
}
