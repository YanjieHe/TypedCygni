using System;
using System.Collections.Generic;
using Op = Compiler.Op;
namespace Disassembler
{
    public class Instruction
    {
        public Op op;
        public int byteCount;
        public ushort value;
        public Instruction(Op op, int byteCount, ushort value)
        {
            this.op = op;
            this.byteCount = byteCount;
            this.value = value;
        }
        public override string ToString()
        {
            if (byteCount == 0)
            {
                return op.ToString();
            }
            else
            {
                return op.ToString() + " " + value;
            }
        }
    }
    public class Disassembler
    {
        public Disassembler()
        {
        }
        static HashSet<Op> zeroByte = new HashSet<Op>
        {
            Op.ADD_I32,Op.SUB_I32,Op.MUL_I32,Op.DIV_I32,Op.MOD_I32,Op.GT_I32,Op.LT_I32,Op.GE_I32,Op.LE_I32,Op.EQ_I32,Op.NE_I32,Op.MINUS_I32,
            Op.ADD_I64,Op.SUB_I64,Op.MUL_I64,Op.DIV_I64,Op.MOD_I64,Op.GT_I64,Op.LT_I64,Op.GE_I64,Op.LE_I64,Op.EQ_I64,Op.NE_I64,Op.MINUS_I64,
            Op.ADD_F32,Op.SUB_F32,Op.MUL_F32,Op.DIV_F32,Op.MOD_F32,Op.GT_F32,Op.LT_F32,Op.GE_F32,Op.LE_F32,Op.EQ_F32,Op.NE_F32,Op.MINUS_F32,
            Op.ADD_F64,Op.SUB_F64,Op.MUL_F64,Op.DIV_F64,Op.MOD_F64,Op.GT_F64,Op.LT_F64,Op.GE_F64,Op.LE_F64,Op.EQ_F64,Op.NE_F64,Op.MINUS_F64,
            Op.RETURN_I32, Op.RETURN_I64,Op.RETURN_F32, Op.RETURN_F64,
            Op.CALL,

        };
        static HashSet<Op> oneByte = new HashSet<Op>
        {
        };
        static HashSet<Op> twoBytes = new HashSet<Op>
        {
            Op.PUSH_CONST_I32, Op.PUSH_LOCAL_I32, Op.PUSH_FIELD_I32, Op.POP_LOCAL_I32,
            Op.PUSH_CONST_I64, Op.PUSH_LOCAL_I64, Op.PUSH_FIELD_I64, Op.POP_LOCAL_I64,
            Op.PUSH_CONST_F32, Op.PUSH_LOCAL_F32, Op.PUSH_FIELD_F32, Op.POP_LOCAL_F32,
            Op.PUSH_CONST_F64, Op.PUSH_LOCAL_F64, Op.PUSH_FIELD_F64, Op.POP_LOCAL_F64,

            Op.JUMP, Op.JUMP_IF_FALSE, Op.JUMP_IF_TRUE,
            Op.PUSH_FUNCTION,Op.PUSH_MEMBER_FUNCTION,Op.PUSH_MODULE,
            Op.PUSH_FIELD_I32, Op.PUSH_FIELD_I64, Op.PUSH_FIELD_F32, Op.PUSH_FIELD_F64,Op.PUSH_FIELD_OBJECT,
            Op.POP_FIELD_I32, Op.POP_FIELD_I64, Op.POP_FIELD_F32, Op.POP_FIELD_F64,Op.POP_FIELD_OBJECT,
            Op.NEW
        };
        public List<Instruction> CodeToInstructions(List<byte> code)
        {
            List<Instruction> instructions = new List<Instruction>();
            int i = 0;
            while (i < code.Count)
            {
                Op op = (Op)code[i];
                if (zeroByte.Contains(op))
                {
                    instructions.Add(new Instruction(op, 0, 0));
                    i++;
                }
                else if (oneByte.Contains(op))
                {
                    instructions.Add(new Instruction(op, 1, code[i + 1]));
                    i += 2;
                }
                else if (twoBytes.Contains(op))
                {
                    instructions.Add(new Instruction(op, 2, (ushort)(code[i + 1] + (code[i + 2] << 8))));
                    i += 3;
                }
                else
                {
                    throw new OverflowException(op.ToString());
                }
            }
            return instructions;
        }
        public void ShowInstructions(List<Instruction> instructions, int indentation)
        {
            int index = 0;
            foreach (var instruction in instructions)
            {
                for (int i = 0; i < indentation; i++)
                {
                    Console.Write(" ");
                }
                Console.Write("{0}: ", index);
                Console.WriteLine(instruction);
                index = index + instruction.byteCount + 1;
            }
        }
    }
}
