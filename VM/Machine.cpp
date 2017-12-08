#include "Machine.h"
#include "OpCode.h"
#include <iostream>

using namespace std;

Machine::Machine(ConstantPool pool, i32 staticSize)
	:code{nullptr}, pc{0}, sp{0}, fp{0}, ret{-1}, running{true}, pool{pool}
{
	static_v.reserve(staticSize);
	memory.reserve(1000000);
}

void Machine::Run(i32 entry)
{
	pc = entry;

	MainLoop();
}

void Machine::LoadProgram(vector<byte>* code)
{
	this->code = code;
}

int Machine::ReadUShort()
{
	return ((*code)[pc + 1] << 8) + (*code)[pc];
}

void Machine::MainLoop()
{
	int n = code->size();
	while (pc < n)
	{
		OpCode op = (OpCode) (*code)[pc];
		pc++;

		switch (op)
		{
			// push small number
			case OpCode::push_i32_1byte:
			{
				memory[sp].i32_v = (*code)[pc];
				pc++;
				sp++;
				break;
			}
			case OpCode::push_i32_2byte:
			{
				memory[sp].i32_v = ReadUShort();
				pc += 2;
				sp++;
				break;
			}
			case OpCode::push_f64_0:
			{
				memory[sp].f64_v = 0.0;
				sp++;
				break;
			}
			case OpCode::push_f64_1:
			{
				memory[sp].f64_v = 1.0;
				sp++;
				break;
			}

			case OpCode::push_i32:
			{
				memory[sp].i32_v = pool[ReadUShort()].i32_v;
				pc += 2;
				sp++;
				break;
			}
			case OpCode::push_f64:
			{
				memory[sp].f64_v = pool[ReadUShort()].f64_v;
				pc += 2;
				sp++;
				break;
			}

			case OpCode::push_static_i32:
			{
				memory[sp].i32_v = static_v[ReadUShort()].i32_v;
				pc += 2;
				sp++;
				break;
			}
			case OpCode::push_static_f64:
			{
				memory[sp].f64_v = static_v[ReadUShort()].f64_v;
				pc += 2;
				sp++;
				break;
			}
			case OpCode::pop_static_i32:
			{
				memory[ReadUShort()].i32_v = memory[sp].i32_v;
				pc += 2;
				sp--;
				break;
			}
			case OpCode::pop_static_f64:
			{
				memory[ReadUShort()].f64_v = memory[sp].f64_v;
				pc += 2;
				sp--;
				break;
			}

			case OpCode::push_stack_i32:
			{
				memory[sp].i32_v = memory[fp + ReadUShort()].i32_v;
				pc += 2;
				sp++;
				break;
			}
			case OpCode::push_stack_f64:
			{
				memory[sp].f64_v = memory[fp + ReadUShort()].f64_v;
				pc += 2;
				sp++;
				break;
			}
			case OpCode::pop_stack_i32:
			{
				memory[fp + ReadUShort()].i32_v = memory[sp].i32_v;
				pc += 2;
				sp--;
				break;
			}
			case OpCode::pop_stack_f64:
			{
				memory[fp + ReadUShort()].f64_v = memory[sp].f64_v;
				pc += 2;
				sp--;
				break;
			}

			// int arithmetic operation
			case OpCode::add_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v + memory[sp].i32_v;
				sp--;
				wcout << memory[sp-1].i32_v << L" + " << memory[sp].i32_v << endl;
				break;
			}
			case OpCode::sub_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v - memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::mul_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v * memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::div_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v / memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::mod_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v % memory[sp].i32_v;
				sp--;
				break;
			}

			// double arithmetic operation
			case OpCode::add_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v + memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::sub_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v - memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::mul_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v * memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::div_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v / memory[sp].f64_v;
				sp--;
				break;
			}
			/*case OpCode::mod_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v % memory[sp].f64_v;
				sp--;
				break;
			}*/

			// int comparision
			case OpCode::gt_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v > memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::lt_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v < memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::ge_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v >= memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::le_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v <= memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::eq_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v == memory[sp].i32_v;
				sp--;
				break;
			}
			case OpCode::ne_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v != memory[sp].i32_v;
				sp--;
				break;
			}

			// float comparision
			case OpCode::gt_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v > memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::lt_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v < memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::ge_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v >= memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::le_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v <= memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::eq_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v == memory[sp].f64_v;
				sp--;
				break;
			}
			case OpCode::ne_f64:
			{
				memory[sp - 1].f64_v = memory[sp - 1].f64_v != memory[sp].f64_v;
				sp--;
				break;
			}

			// minus operation
			case OpCode::minus_i32:
			{
				memory[sp].i32_v = -memory[sp].i32_v;
				break;
			}
			case OpCode::minus_f64:
			{
				memory[sp].f64_v = -memory[sp].f64_v;
				break;
			}

			// cast
			case OpCode::cast_int32_to_float64:
			{
				memory[sp].f64_v = (f64)memory[sp].i32_v;
				break;
			}
			case OpCode::cast_float64_to_int32:
			{
				memory[sp].i32_v = (i32)memory[sp].f64_v;
				break;
			}

			// jump
			case OpCode::jump:
			{
				pc = ReadUShort();
				break;
			}
			case OpCode::jump_if_true:
			{
				if (memory[sp].i32_v)
				{
					pc = ReadUShort();
				}
				break;
			}
			case OpCode::jump_if_false:
			{
				if (!memory[sp].i32_v)
				{
					pc = ReadUShort();
				}
				break;
			}

			default:
			{
				wcout << opcode_to_wstring(op) << endl;
				throw L"error opcode";
			}
		}
	}
}

