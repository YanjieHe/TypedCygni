#include "Machine.h"
#include "OpCode.h"
#include <iostream>

using namespace std;

Machine::Machine(ConstantPool pool, i32 staticSize)
	:code{nullptr}, pc{0}, sp{-1}, fp{0}, ret{-1}, running{true}, pool{pool}
{
	static_v.reserve(staticSize);
	memory.reserve(1000000);
	for (int i = 0; i < 1000000; i++)
	{
		memory[i].i32_v = -100;
	}
}

void Machine::Run(i32 entry)
{
	pc = entry;

	MainLoop();
	wcout << "result = " << static_v[0].i32_v << endl;
}

void Machine::LoadProgram(ByteCode* code)
{
	this->globalCode = code;
	this->code = code;
}

void Machine::LoadFunction(Function f)
{
	functions.push_back(f);
}

i32 Machine::ReadUShort()
{
	return ((*code)[pc + 1] << 8) + (*code)[pc];
}

void Machine::MainLoop()
{
//	i32 n = code->size();

	while (pc < (i32) code->size())
	{
		OpCode op = (OpCode) (*code)[pc];
		pc++;

		wcout << L"memory sp = " << sp << L" fp = " << fp << endl;
		for (int i = 0; i < 20; i++)
		{
			wcout << memory[i].i32_v << L"  ";
		}
		wcout << endl;
		wcout << L"run: " << opcode_to_wstring(op) << endl;
		switch (op)
		{
			// push small number
			case OpCode::push_i32_1byte:
			{
				sp++;
				memory[sp].i32_v = (*code)[pc];
				pc++;
				break;
			}
			case OpCode::push_i32_2byte:
			{
				sp++;
				memory[sp].i32_v = ReadUShort();
				pc += 2;
				break;
			}
			case OpCode::push_f64_0:
			{
				sp++;
				memory[sp].f64_v = 0.0;
				break;
			}
			case OpCode::push_f64_1:
			{
				sp++;
				memory[sp].f64_v = 1.0;
				break;
			}

			case OpCode::push_i32:
			{
				sp++;
				memory[sp].i32_v = pool[ReadUShort()].i32_v;
				pc += 2;
				break;
			}
			case OpCode::push_f64:
			{
				sp++;
				memory[sp].f64_v = pool[ReadUShort()].f64_v;
				pc += 2;
				break;
			}
			case OpCode::push_function:
			{
				sp++;
				memory[sp].i32_v = ReadUShort();
				pc += 2;
				break;
			}

			case OpCode::push_static_i32:
			{
				sp++;
				memory[sp].i32_v = static_v[ReadUShort()].i32_v;
				pc += 2;
				break;
			}
			case OpCode::push_static_f64:
			{
				sp++;
				memory[sp].f64_v = static_v[ReadUShort()].f64_v;
				pc += 2;
				break;
			}
			case OpCode::pop_static_i32:
			{
				static_v[ReadUShort()].i32_v = memory[sp].i32_v;
				sp--;
				pc += 2;
				break;
			}
			case OpCode::pop_static_f64:
			{
				static_v[ReadUShort()].f64_v = memory[sp].f64_v;
				sp--;
				pc += 2;
				break;
			}

			case OpCode::push_stack_i32:
			{
				sp++;
				memory[sp].i32_v = memory[fp + ReadUShort()].i32_v;
				pc += 2;
				break;
			}
			case OpCode::push_stack_f64:
			{
				sp++;
				memory[sp].f64_v = memory[fp + ReadUShort()].f64_v;
				pc += 2;
				break;
			}
			case OpCode::pop_stack_i32:
			{
				memory[fp + ReadUShort()].i32_v = memory[sp].i32_v;
				sp--;
				pc += 2;
				break;
			}
			case OpCode::pop_stack_f64:
			{
				memory[fp + ReadUShort()].f64_v = memory[sp].f64_v;
				sp--;
				pc += 2;
				break;
			}

			// i32 arithmetic operation
			case OpCode::add_i32:
			{
				memory[sp - 1].i32_v = memory[sp - 1].i32_v + memory[sp].i32_v;
				sp--;
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

			// f64 arithmetic operation
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

			// i32 comparision
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

			// f64 comparision
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
					sp--;
					pc = ReadUShort();
				}
				else
				{
					sp--;
					pc += 2;
				}
				break;
			}
			case OpCode::jump_if_false:
			{
				if (!memory[sp].i32_v)
				{
					sp--;
					pc = ReadUShort();
				}
				else
				{
					sp--;
					pc += 2;
				}
				break;
			}

			case OpCode::invoke:
			{
				wcout << "invoke" << endl;
				i32 args_size = ReadUShort();
				pc += 2;
				i32 function_id = memory[sp].i32_v;
				sp--;

				Function& f = functions[function_id];
				wcout << "args_size = " << f.parametersSize << endl;
				wcout << "frame_size = " << f.frameSize << endl;
				fp = sp - f.parametersSize + 1;
				sp = fp + f.frameSize;

				ret = pc;
				pc = 0;
				call_stack.push(code);
				wcout << "call_stack: " << call_stack.size() << endl;
				code = &(f.code);
				break;
			}

			case OpCode::return_i32:
			{
				pc = ret;
				code = call_stack.top();
				call_stack.pop();
				memory[fp].i32_v = memory[sp].i32_v;
				wcout << "stack_peek: " << memory[sp].i32_v << endl;
				sp = fp;
				break;
			}
			case OpCode::return_f64:
			{
				pc = ret;
				code = call_stack.top();
				call_stack.pop();
				memory[fp].f64_v = memory[sp].f64_v;
				sp = fp;
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

