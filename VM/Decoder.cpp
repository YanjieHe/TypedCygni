#include "Decoder.h"
#include "Exception.h"
#include "OpCode.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Decoder::Decoder(vector<byte>& stream) : reader(stream)
{
}

void Decoder::Decode()
{
    wcout << "start decoding" << endl;
    //    LoadConstantPool();
    LoadFunctions();
    while (!reader.IsEof())
    {
        code.push_back(reader.Read());
    }

    wcout << "finish decoding" << endl;
}

void Decoder::LoadConstantPool()
{
    bool done = false;
    while (!done && !reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Read());
		switch (op)
		{
        case OpCode::constant_i32:
        {
            pool.push_back(Any::Int(reader.ReadInt()));
            break;
		}
        case OpCode::constant_i64:
        {
            pool.push_back(Any::Long(reader.ReadLong()));
            break;
        }
        case OpCode::constant_f64:
        {
            pool.push_back(Any::Double(reader.ReadDouble()));
            break;
        }
        default:
        {
            done = true;
            break;
        }
        }
    }
}

void Decoder::LoadFunctions()
{
    while (!reader.IsEof())
	{
        OpCode op = static_cast<OpCode>(reader.Peek());
		if (op == OpCode::function_begin)
        {
            reader.Read();
            wstring name = reader.ReadString();
            i32 parametersSize = reader.ReadUShort();
            i32 frameSize = reader.ReadUShort();
            LoadOneFunction(name, parametersSize, frameSize);
		}
		else
		{
            return;
		}
    }
}

void Decoder::LoadOneFunction(wstring name, i32 parametersSize, i32 frameSize)
{
	bool done = false;
	vector<byte> fcode;
	while (!done)
    {
        OpCode op = static_cast<OpCode>(reader.Peek());
		if (op == OpCode::function_end)
		{
            reader.Read();
			done = true;
		}
		else
		{
            fcode.push_back(reader.Read());
            i32 size = OperandSize(op);
            for (i32 i = 0; i < size; i++)
			{
                fcode.push_back(reader.Read());
            }
		}
	}
    functions.push_back(Function(name, parametersSize, frameSize, fcode));
}
