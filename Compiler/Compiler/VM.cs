using System;
using System.Collections.Generic;
using Op = Compiler.Op;
namespace VirtualMachine
{
    public class Union
    {
        public int i32_v;
        public long i64_v;
        public float f32_v;
        public double f64_v;
        public Object pointer;
    }
    public class Value
    {
        public bool isPointer;
        public Union u;
        public Value()
        {
            this.u = new Union();
        }
    }
    public enum TypeTag
    {
        TYPE_I32,
        TYPE_I64,
        TYPE_F32,
        TYPE_F64,
        TYPE_BOOL,
        TYPE_CHAR,
        TYPE_STRING,
        TYPE_ARRAY
    }
    public class TaggedValue
    {
        public TypeTag tag;
        public Union u;
        public TaggedValue()
        {
            this.u = new Union();
        }
    }
    public class Function
    {
        public byte[] name;
        public ushort locals;
        public ushort stack;
        public ushort args_size;
        public byte[] code;
        public TaggedValue[] constantPool;
    }
    public class Module
    {
        public byte[] name;
        public TaggedValue[] constantPool;
        public Value[] variables;
        public Function[] functions;
    }
    public class Class
    {
        public byte[] name;
        public TaggedValue[] constantPool;
        public Value[] fields;
        public Function[] methods;
    }
    public class Program
    {
        public byte[] path;
        public Module[] modules;
        public Class[] classes;
    }
    public class VM
    {
        public Program program;
        public int sp;
        public int pc;
        public int fp;
        public Value[] stack;
        public VM(int stackSize)
        {
            this.stack = new Value[stackSize];
        }
        public static ushort USHORT(byte[] bytes, int index)
        {
            return (ushort)((bytes[index] << 8) + (bytes[index + 1]));
        }
        public static void Run(VM vm, Function entry)
        {
            Function current = entry;
            while (vm.pc < current.code.Length)
            {
                byte op = current.code[vm.pc];
                vm.pc++;
                switch ((Op)op)
                {
                    case Op.PUSH_CONST_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v =
                               current.constantPool[index].u.i32_v;
                            vm.pc += 2;
                            break;
                        }
                    case Op.PUSH_LOCAL_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v =
                                vm.stack[vm.fp + index].u.i32_v;
                            vm.pc += 2;
                            break;
                        }
                    case Op.POP_LOCAL_I32:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.stack[vm.fp + index].u.i32_v =
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            vm.pc += 2;
                            break;
                        }
                    case Op.ADD_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v +
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.SUB_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v -
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.MUL_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v *
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.DIV_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v /
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.MOD_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v =
                                vm.stack[vm.sp - 1].u.i32_v %
                                vm.stack[vm.sp].u.i32_v;
                            vm.sp--;
                            break;
                        }
                    case Op.EQ_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v ==
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.NE_I32:
                        {
                            vm.stack[vm.sp - 1].u.i32_v = Convert.ToInt32(
                                vm.stack[vm.sp - 1].u.i32_v !=
                                vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            break;
                        }
                    case Op.CALL:
                        {
                            Console.Write("a = {0}\n", vm.stack[vm.fp].u.i32_v);
                            Console.Write("load function at: {0}\n", vm.sp);
                            Function function = (Function)(vm.stack[vm.sp].u.pointer);
                            Console.Write("successfully load function\n");
                            int previous_fp = vm.fp;
                            vm.fp = vm.sp - function.args_size;
                            Console.Write("after ... a = {0}\n", vm.stack[vm.fp].u.i32_v);
                            Console.Write("fp = {0}\n", vm.fp);
                            int _base = vm.fp + function.args_size + function.locals;
                            vm.stack[_base].u.i32_v = vm.pc;
                            vm.stack[_base + 1].u.i32_v = previous_fp;
                            vm.stack[_base + 2].u.pointer = current;
                            vm.sp = vm.fp + function.args_size + function.locals + 2;
                            current = function;
                            vm.pc = 0;
                            break;
                        }
                    case Op.JUMP_IF_TRUE:
                        {
                            bool condition = Convert.ToBoolean(vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            if (condition)
                            {
                                vm.pc = USHORT(current.code, vm.pc);
                            }
                            else
                            {
                                vm.pc += 2;
                            }
                            break;
                        }
                    case Op.JUMP_IF_FALSE:
                        {
                            bool condition = Convert.ToBoolean(vm.stack[vm.sp].u.i32_v);
                            vm.sp--;
                            if (!condition)
                            {
                                vm.pc = USHORT(current.code, vm.pc);
                            }
                            else
                            {
                                vm.pc += 2;
                            }
                            break;
                        }
                    case Op.JUMP:
                        {
                            vm.pc = USHORT(current.code, vm.pc);
                            break;
                        }
                    case Op.PUSH_FUNCTION:
                        {
                            ushort index = USHORT(current.code, vm.pc);
                            vm.sp++;
                            vm.stack[vm.sp].u.pointer =
                                current.constantPool[index].u.pointer;
                            Console.Write("function index: {0}\n", index);
                            Console.Write("push function at: {0}\n", vm.sp);

                            vm.pc += 2;
                            break;
                        }
                    case Op.RETURN_I32:
                        {
                            Console.Write("try to return\n");
                            bool result = Convert.ToBoolean(vm.stack[vm.sp].u.i32_v);
                            int _base = vm.fp + current.args_size + current.locals;
                            vm.sp = vm.fp;
                            vm.pc = vm.stack[_base].u.i32_v;
                            vm.fp = vm.stack[_base + 1].u.i32_v;

                            vm.sp++;
                            vm.stack[vm.sp].u.i32_v = Convert.ToInt32(result);
                            Console.Write("result = {0}\n", result);

                            Function previous = (Function)(vm.stack[_base + 2].u.pointer);
                            if (previous == null)
                            {
                                Console.Write("finished!\n");
                                return;
                            }
                            current = previous;
                            break;
                        }
                    default:
                        {
                            Console.Write("not supported operation code");
                            throw new NotSupportedException();
                        }
                }
            }
        }
    }
    public class Parser
    {
        public byte[] bytes;
        public int offset;
        public Parser(byte[] bytecode)
        {
            this.bytes = bytecode;
            this.offset = 0;
        }
        public Program Parse()
        {
            Program program = new Program();
            program.path = ReadString();
            int moduleCount = ReadU16();
            int classCount = ReadU16();
            program.modules = ParseModules(moduleCount);
            return program;
        }

        Module[] ParseModules(int moduleCount)
        {
            Module[] modules = new Module[moduleCount];
            for (int i = 0; i < moduleCount; i++)
            {
                Module module = new Module();
                module.name = ReadString();
                int constantPoolCount = ReadU16();
                int variableCount = ReadU16();
                int functionCount = ReadU16();
                module.constantPool = ParseConstantPool(constantPoolCount);
                module.variables = new Value[variableCount];
                module.functions = ParseFunctions(functionCount, module.constantPool);
                modules[i] = module;
            }
            return modules;
        }

        Function[] ParseFunctions(int functionCount, TaggedValue[] constantPool)
        {
            Function[] functions = new Function[functionCount];
            for (int i = 0; i < functionCount; i++)
            {
                Function function = new Function();
                function.name = ReadString();
                function.locals = ReadU16();
                function.stack = ReadU16();
                function.args_size = ReadU16();
                int codeSize = ReadU16();
                function.code = ReadBytes(codeSize);
                function.constantPool = constantPool;
                functions[i] = function;
            }
            return functions;
        }

        TaggedValue[] ParseConstantPool(int constantCount)
        {
            TaggedValue[] constantPool = new TaggedValue[constantCount];
            for (int i = 0; i < constantCount; i++)
            {
                constantPool[i] = new TaggedValue();
                TypeTag tag = (TypeTag)bytes[offset];
                offset++;
                switch (tag)
                {
                    case TypeTag.TYPE_I32:
                        constantPool[i].u.i32_v = BitConverter.ToInt32(ReadBytes(4), 0);
                        break;
                    case TypeTag.TYPE_I64:
                        constantPool[i].u.i64_v = BitConverter.ToInt64(ReadBytes(8), 0);
                        break;
                    case TypeTag.TYPE_F32:
                        constantPool[i].u.f32_v = BitConverter.ToSingle(ReadBytes(4), 0);
                        break;
                    case TypeTag.TYPE_F64:
                        constantPool[i].u.f64_v = BitConverter.ToDouble(ReadBytes(8), 0);
                        break;
                    case TypeTag.TYPE_STRING:
                        constantPool[i].u.pointer = ReadString();
                        break;
                    default:
                        throw new NotSupportedException();
                }
            }
            return constantPool;
        }

        ushort ReadU16()
        {
            ushort value = (ushort)(bytes[offset] + (bytes[offset + 1] << 8));
            offset = offset + 2;
            return value;
        }

        byte[] ReadString()
        {
            ushort length = ReadU16();
            byte[] characters = ReadBytes(length);
            return characters;
        }

        byte[] ReadBytes(int length)
        {
            byte[] array = new byte[length];
            for (int i = 0; i < length; i++)
            {
                array[i] = bytes[offset + i];
            }
            offset = offset + length;
            return array;
        }
    }
}
