using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;

namespace Compiler
{
    public enum Op
    {
        PUSH_CONST_I32,
        PUSH_CONST_I64,
        PUSH_CONST_F32,
        PUSH_CONST_F64,
        PUSH_CONST_STRING,

        PUSH_LOCAL_I32,
        PUSH_LOCAL_I64,
        PUSH_LOCAL_F32,
        PUSH_LOCAL_F64,
        PUSH_LOCAL_OBJECT,

        POP_LOCAL_I32,
        POP_LOCAL_I64,
        POP_LOCAL_F32,
        POP_LOCAL_F64,
        POP_LOCAL_OBJECT,

        ADD_I32,
        SUB_I32,
        MUL_I32,
        DIV_I32,
        MOD_I32,

        GT_I32,
        LT_I32,
        GE_I32,
        LE_I32,
        EQ_I32,
        NE_I32,

        MINUS_I32,

        ADD_I64,
        SUB_I64,
        MUL_I64,
        DIV_I64,
        MOD_I64,

        GT_I64,
        LT_I64,
        GE_I64,
        LE_I64,
        EQ_I64,
        NE_I64,

        MINUS_I64,

        ADD_F32,
        SUB_F32,
        MUL_F32,
        DIV_F32,
        MOD_F32,

        GT_F32,
        LT_F32,
        GE_F32,
        LE_F32,
        EQ_F32,
        NE_F32,

        MINUS_F32,

        ADD_F64,
        SUB_F64,
        MUL_F64,
        DIV_F64,
        MOD_F64,

        GT_F64,
        LT_F64,
        GE_F64,
        LE_F64,
        EQ_F64,
        NE_F64,

        MINUS_F64,

        JUMP,

        JUMP_IF_TRUE,

        JUMP_IF_FALSE,
        RETURN_I32,
        RETURN_I64,
        RETURN_F32,
        RETURN_F64,
        PUSH_FIELD_I32,
        PUSH_FIELD_I64,
        PUSH_FIELD_F32,
        PUSH_FIELD_F64,
        PUSH_FIELD_OBJECT,

        PUSH_MODULE,
        PUSH_MEMBER_FUNCTION,
        PUSH_FUNCTION,
        CALL,
        TAIL_CALL,
    }


    public enum OpType
    {
        I32,
        I64,
        F32,
        F64,
        BOOL,
        CHAR,
        STRING,
        ARRAY
    }
    public class CompilerException : Exception
    {
        public Position position;
        public CompilerException(Position position, String message) : base(message)
        {
            this.position = position;
        }
    }
    public class Rule<T>
    {
        public List<Type> arguments;
        public T result;
        public Rule(List<Type> arguments, T result)
        {
            this.arguments = arguments;
            this.result = result;
        }

        public bool Match(List<Type> args)
        {
            if (arguments.Count == args.Count)
            {
                for (int i = 0; i < args.Count; i++)
                {
                    if (!arguments[i].Equals(args[i]))
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    public class RuleSet<T>
    {
        public Dictionary<String, List<Rule<T>>> rules;
        public RuleSet()
        {
            this.rules = new Dictionary<String, List<Rule<T>>>();
        }
        public void AddRule(String key, Type[] types, T result)
        {
            if (types.Length > 0)
            {
                var rule = new Rule<T>(types.ToList(), result);
                if (rules.ContainsKey(key))
                {
                    rules[key].Add(rule);
                }
                else
                {
                    rules.Add(key, new List<Rule<T>> { rule });
                }
            }
            else
            {
                throw new ArgumentException();
            }
        }
        public bool TryMatch(String key, List<Type> args, out T result)
        {
            foreach (var rule in rules[key])
            {
                if (rule.Match(args))
                {
                    result = rule.result;
                    return true;
                }
            }
            result = default(T);
            return false;
        }
        public bool TryMatch(String key, Type[] args, out T result)
        {
            foreach (var rule in rules[key])
            {
                if (rule.Match(args.ToList()))
                {
                    result = rule.result;
                    return true;
                }
            }
            result = default(T);
            return false;
        }
        public int Count
        {
            get { return rules.Count; }
        }
    }
    public class Compiler
    {
        Dictionary<int, Type> typeMap;
        Dictionary<int, Location> locationMap;
        Dictionary<int, List<Object>> constantPoolMap;
        public Compiler(Dictionary<int, Type> typeMap, Dictionary<int, Location> locationMap, Dictionary<int, List<Object>> constantPoolMap)
        {
            this.typeMap = typeMap;
            this.locationMap = locationMap;
            this.constantPoolMap = constantPoolMap;
            if (ruleSet.Count == 0)
            {
                ruleSet.AddRule("+", new Type[] { Type.INT, Type.INT }, Op.ADD_I32);
                ruleSet.AddRule("-", new Type[] { Type.INT, Type.INT }, Op.SUB_I32);
                ruleSet.AddRule("*", new Type[] { Type.INT, Type.INT }, Op.MUL_I32);
                ruleSet.AddRule("/", new Type[] { Type.INT, Type.INT }, Op.DIV_I32);
                ruleSet.AddRule("%", new Type[] { Type.INT, Type.INT }, Op.MOD_I32);

                ruleSet.AddRule(">", new Type[] { Type.INT, Type.INT }, Op.GT_I32);
                ruleSet.AddRule("<", new Type[] { Type.INT, Type.INT }, Op.LT_I32);
                ruleSet.AddRule(">=", new Type[] { Type.INT, Type.INT }, Op.GE_I32);
                ruleSet.AddRule("<=", new Type[] { Type.INT, Type.INT }, Op.LE_I32);
                ruleSet.AddRule("==", new Type[] { Type.INT, Type.INT }, Op.EQ_I32);
                ruleSet.AddRule("!=", new Type[] { Type.INT, Type.INT }, Op.NE_I32);

                ruleSet.AddRule("-", new Type[] { Type.INT }, Op.MINUS_I32);

                ruleSet.AddRule("+", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.ADD_F64);
                ruleSet.AddRule("-", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.SUB_F64);
                ruleSet.AddRule("*", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.MUL_F64);
                ruleSet.AddRule("/", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.DIV_F64);
                ruleSet.AddRule("%", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.MOD_F64);

                ruleSet.AddRule(">", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.GT_F64);
                ruleSet.AddRule("<", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.LT_F64);
                ruleSet.AddRule(">=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.GE_F64);
                ruleSet.AddRule("<=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.LE_F64);
                ruleSet.AddRule("==", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.EQ_F64);
                ruleSet.AddRule("!=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.NE_F64);

                ruleSet.AddRule("-", new Type[] { Type.DOUBLE }, Op.MINUS_F64);

                ruleSet.AddRule("return", new Type[] { Type.INT }, Op.RETURN_I32);
                ruleSet.AddRule("return", new Type[] { Type.LONG }, Op.RETURN_I64);
                ruleSet.AddRule("return", new Type[] { Type.FLOAT }, Op.RETURN_F32);
                ruleSet.AddRule("return", new Type[] { Type.DOUBLE }, Op.RETURN_F64);

            }
        }
        public static RuleSet<Op> ruleSet = new RuleSet<Op>();
        public static Dictionary<Kind, String> opString = new Dictionary<Kind, string>
        {
            {Kind.Add, "+" },
            {Kind.Subtract, "-"},
            {Kind.Multiply, "*"},
            {Kind.Divide, "/"},
            {Kind.Modulo, "%"},
            {Kind.GreaterThan, ">"},
            {Kind.LessThan, "<"},
            {Kind.GreaterThanOrEqual, ">="},
            {Kind.LessThanOrEqual, "<="},
            {Kind.Equal, "=="},
            {Kind.NotEqual, "!="},

            {Kind.UnaryPlus, "+"},
            {Kind.UnaryMinus, "-"},

            {Kind.And, "and"},
            {Kind.Or, "or"},
            {Kind.Not, "not"}
        };

        public List<byte> Compile(Program program)
        {
            List<byte> code = new List<byte>();
            EmitString(code, program.path);
            EmitU16(code, program.modules.Count);
            EmitU16(code, program.classes.Count);
            foreach (var module in program.modules)
            {
                CompileModule(module, code);
            }
            return code;
        }

        void CompileNode(Ast node, List<byte> code)
        {
            switch (node.kind)
            {
                case Kind.Add:
                case Kind.Subtract:
                case Kind.Multiply:
                case Kind.Divide:
                case Kind.Modulo:
                case Kind.GreaterThan:
                case Kind.LessThan:
                case Kind.GreaterThanOrEqual:
                case Kind.LessThanOrEqual:
                case Kind.Equal:
                case Kind.NotEqual:
                    CompileBinary((Binary)node, code);
                    break;
                case Kind.And:
                    break;
                case Kind.Or:
                    break;
                case Kind.Not:
                case Kind.UnaryPlus:
                case Kind.UnaryMinus:
                    CompileUnary((Unary)node, code);
                    break;
                case Kind.IfThen:
                    CompileIfThen((IfThen)node, code);
                    break;
                case Kind.IfElse:
                    CompileIfElse((IfElse)node, code);
                    break;
                case Kind.Constant:
                    CompileConstant((Constant)node, code);
                    break;
                case Kind.Block:
                    CompileBlock((Block)node, code);
                    break;
                case Kind.Name:
                    CompileName((Name)node, code);
                    break;
                case Kind.Return:
                    CompileReturn((Return)node, code);
                    break;
                case Kind.Var:
                    CompileVar((Var)node, code);
                    break;
                case Kind.Def:
                    CompileDef((Def)node, code);
                    break;
                case Kind.Assign:
                    CompileAssign((Binary)node, code);
                    break;
                case Kind.Call:
                    CompileCall((Call)node, code);
                    break;
                case Kind.While:
                    CompileWhile((While)node, code);
                    break;
                case Kind.DefClass:
                    break;
                case Kind.DefModule:
                    break;
                case Kind.TypeSpecifier:
                    break;
                case Kind.MemberAccess:
                    CompileMemberAccess((MemberAccess)node, code);
                    break;
                default:
                    break;
            }
        }

        Type T(Ast node)
        {
            return typeMap[node.id];
        }

        void CompileBinary(Binary node, List<byte> code)
        {
            CompileNode(node.left, code);
            CompileNode(node.right, code);
            if (ruleSet.TryMatch(opString[node.kind], new Type[] { T(node.left), T(node.right) }, out Op op))
            {
                EmitOp(code, op);
            }
            else
            {
                throw new CompilerException(node.position, node.kind.ToString());
            }
        }
        void CompileUnary(Unary node, List<byte> code)
        {
            CompileNode(node.operand, code);
            if (ruleSet.TryMatch(opString[node.kind], new Type[] { T(node.operand) }, out Op op))
            {
                EmitOp(code, op);
            }
            else
            {
                throw new CompilerException(node.position, node.kind.ToString());
            }
        }

        void CompileIfThen(IfThen node, List<byte> code)
        {
            CompileNode(node.condition, code);
            EmitOp(code, Op.JUMP_IF_FALSE);
            int index = code.Count;
            EmitU16(code, 0); /* hold the place */

            CompileNode(node.ifTrue, code);
            int dest = code.Count;
            RewriteU16(code, index, dest);
        }

        void CompileIfElse(IfElse node, List<byte> code)
        {
            CompileNode(node.condition, code);
            EmitOp(code, Op.JUMP_IF_FALSE);
            int index0 = code.Count;
            EmitU16(code, 0); /* hold the place */

            CompileNode(node.ifTrue, code);
            EmitOp(code, Op.JUMP);
            int index1 = code.Count;
            EmitU16(code, 0); /* hold another place */

            int dest0 = code.Count;
            RewriteU16(code, index0, dest0);
            CompileNode(node.ifFalse, code);

            int dest1 = code.Count;
            RewriteU16(code, index1, dest1);
        }

        void CompileConstant(Constant node, List<byte> code)
        {
            Location location = locationMap[node.id];
            Object value = node.value;

            if (value is int)
            {
                EmitOp(code, Op.PUSH_CONST_I32);
                EmitU16(code, location.offset);
            }
            else if (value is long)
            {
                EmitOp(code, Op.PUSH_CONST_I64);
                EmitU16(code, location.offset);
            }
            else if (value is float)
            {
                EmitOp(code, Op.PUSH_CONST_F32);
                EmitU16(code, location.offset);
            }
            else if (value is double)
            {
                EmitOp(code, Op.PUSH_CONST_F64);
                EmitU16(code, location.offset);
            }
            else
            {
                throw new TimeZoneNotFoundException(); // impossible branch
            }
        }

        void CompileBlock(Block node, List<byte> code)
        {
            foreach (var exp in node.expressions)
            {
                CompileNode(exp, code);
            }
        }

        void CompileName(Name node, List<byte> code)
        {
            Location location = locationMap[node.id];
            Type type = T(node);
            if (type.GetTypeCode() == TypeCode.FUNCTION)
            {
                if (location.kind == LocationKind.Module)
                {
                    EmitOp(code, Op.PUSH_FUNCTION);
                    EmitU16(code, location.offset);
                }
                else if (location.kind == LocationKind.Class)
                {
                    // TO DO
                    throw new EntryPointNotFoundException();
                }
                else
                {
                    throw new MulticastNotSupportedException();
                }
            }
            else if (type.GetTypeCode() == TypeCode.MODULE)
            {
                EmitOp(code, Op.PUSH_MODULE);
                EmitU16(code, location.offset);
            }
            else
            {
                if (location.kind == LocationKind.Function)
                {
                    if (type.GetTypeCode() == TypeCode.INT)
                    {
                        EmitOp(code, Op.PUSH_LOCAL_I32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.FLOAT)
                    {
                        EmitOp(code, Op.PUSH_LOCAL_F32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.LONG)
                    {
                        EmitOp(code, Op.PUSH_LOCAL_I64);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.DOUBLE)
                    {
                        EmitOp(code, Op.PUSH_LOCAL_F64);
                        EmitU16(code, location.offset);
                    }
                    else
                    {
                        throw new NotSupportedException();
                    }
                }
                else if (location.kind == LocationKind.Module)
                {
                    if (type.GetTypeCode() == TypeCode.INT)
                    {
                        EmitOp(code, Op.PUSH_FIELD_I32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.FLOAT)
                    {
                        EmitOp(code, Op.PUSH_FIELD_F32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.LONG)
                    {
                        EmitOp(code, Op.PUSH_FIELD_I64);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.DOUBLE)
                    {
                        EmitOp(code, Op.PUSH_FIELD_F64);
                        EmitU16(code, location.offset);
                    }
                    else
                    {
                        throw new NotSupportedException();
                    }
                }
                else
                {
                    throw new MulticastNotSupportedException();
                }
            }
        }

        void CompileReturn(Return node, List<byte> code)
        {
            CompileNode(node.value, code);
            if (ruleSet.TryMatch("return", new Type[] { T(node.value) }, out Op op))
            {
                EmitOp(code, op);
            }
            else
            {
                throw new CompilerException(node.position, node.kind.ToString());
            }
        }

        void CompileVar(Var node, List<byte> code)
        {
            if (node.value != null)
            {
                CompileNode(node.value, code);
                Location location = locationMap[node.id];
                Type type = T(node);
                if (location.kind == LocationKind.Function)
                {
                    if (type.GetTypeCode() == TypeCode.INT)
                    {
                        EmitOp(code, Op.POP_LOCAL_I32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.FLOAT)
                    {
                        EmitOp(code, Op.POP_LOCAL_F32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.LONG)
                    {
                        EmitOp(code, Op.POP_LOCAL_I64);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.DOUBLE)
                    {
                        EmitOp(code, Op.POP_LOCAL_F64);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.BOOL)
                    {
                        EmitOp(code, Op.POP_LOCAL_I32);
                        EmitU16(code, location.offset);
                    }
                    else if (type.GetTypeCode() == TypeCode.CHAR)
                    {
                        EmitOp(code, Op.POP_LOCAL_I32);
                        EmitU16(code, location.offset);
                    }
                    else
                    {
                        throw new NotSupportedException();
                    }
                }
                else
                {
                    throw new NotSupportedException();
                }
            }
            else
            {
                throw new NotSupportedException();
            }
        }

        void CompileDef(Def node, List<byte> code)
        {
            EmitString(code, node.name);
            var locals = AstVisitor.Visit(node.body)
            .Count(n => n.kind == Kind.Var);
            EmitU16(code, locals);
            EmitU16(code, 0); // stack size: unknown for now
            EmitU16(code, node.parameters.Count);

            List<byte> functionCode = new List<byte>();
            CompileNode(node.body, functionCode);
            EmitU16(code, functionCode.Count);
            code.AddRange(functionCode);
        }
        void CompileAssign(Binary node, List<byte> code)
        {
            // add support for field assignment and array access
            Location location = locationMap[node.left.id];
            Type type = T(node.left);
            CompileNode(node.right, code);
            if (location.kind == LocationKind.Function)
            {
                if (type.GetTypeCode() == TypeCode.INT)
                {
                    EmitOp(code, Op.POP_LOCAL_I32);
                    EmitU16(code, location.offset);
                }
                else if (type.GetTypeCode() == TypeCode.FLOAT)
                {
                    EmitOp(code, Op.POP_LOCAL_F32);
                    EmitU16(code, location.offset);
                }
                else if (type.GetTypeCode() == TypeCode.LONG)
                {
                    EmitOp(code, Op.POP_LOCAL_I64);
                    EmitU16(code, location.offset);
                }
                else if (type.GetTypeCode() == TypeCode.DOUBLE)
                {
                    EmitOp(code, Op.POP_LOCAL_F64);
                    EmitU16(code, location.offset);
                }
                else if (type.GetTypeCode() == TypeCode.BOOL)
                {
                    EmitOp(code, Op.POP_LOCAL_I32);
                    EmitU16(code, location.offset);
                }
                else if (type.GetTypeCode() == TypeCode.CHAR)
                {
                    EmitOp(code, Op.POP_LOCAL_I32);
                    EmitU16(code, location.offset);
                }
                else
                {
                    throw new NotSupportedException();
                }
            }
            else
            {
                throw new NotSupportedException();
            }
        }
        void CompileCall(Call node, List<byte> code)
        {
            foreach (var arg in node.arguments)
            {
                CompileNode(arg, code);
            }
            CompileNode(node.function, code);
            EmitOp(code, Op.CALL);
        }

        void CompileWhile(While node, List<byte> code)
        {
            int index0 = code.Count;
            CompileNode(node.condition, code);
            EmitOp(code, Op.JUMP_IF_FALSE);
            int index1 = code.Count;
            EmitU16(code, 0); /* hold the place */

            CompileNode(node.body, code);
            EmitOp(code, Op.JUMP);
            EmitU16(code, index0);

            int dest1 = code.Count;
            RewriteU16(code, index1, dest1);
        }

        void CompileModule(DefModule node, List<byte> code)
        {
            List<Object> constantPool = constantPoolMap[node.id];
            EmitString(code, node.name);
            EmitU16(code, constantPool.Count);
            EmitU16(code, node.fields.Count);
            EmitU16(code, node.methods.Count);
            foreach (var obj in constantPool)
            {
                if (obj is int)
                {
                    EmitOpType(code, OpType.I32);
                    code.AddRange(BitConverter.GetBytes((int)obj));
                }
                else if (obj is long)
                {
                    EmitOpType(code, OpType.I64);
                    code.AddRange(BitConverter.GetBytes((long)obj));
                }
                else if (obj is float)
                {
                    EmitOpType(code, OpType.F32);
                    code.AddRange(BitConverter.GetBytes((float)obj));
                }
                else if (obj is double)
                {
                    EmitOpType(code, OpType.F64);
                    code.AddRange(BitConverter.GetBytes((double)obj));
                }
                else if (obj is bool)
                {
                    throw new EntryPointNotFoundException();
                }
                else if (obj is char)
                {
                    throw new DllNotFoundException();
                }
                else if (obj is string)
                {
                    EmitOpType(code, OpType.STRING);
                    EmitString(code, (string)obj);
                }
                else
                {
                    throw new NotSupportedException();
                }
            }
            // foreach (var field in node.fields)
            // {
            //     CompileNode(field, code);
            // }
            foreach (var function in node.methods)
            {
                CompileDef(function, code);
            }
        }

        void CompileMemberAccess(MemberAccess node, List<byte> code)
        {
            CompileNode(node.expression, code);
            EmitOp(code, Op.PUSH_MEMBER_FUNCTION);
            Location location = locationMap[node.id];
            EmitU16(code, location.offset);
        }

        /************************************ Emit ************************************/
        static void EmitOp(List<byte> code, Op op)
        {
            Console.WriteLine("emit {0}", op);
            code.Add((byte)op);
        }

        static void EmitU16(List<byte> code, int value)
        {
            if (0 <= value && value < 65536)
            {
                /* big endian */
                int high = value >> 8;
                int low = value % 256;
                code.Add((byte)low);
                code.Add((byte)high);
            }
            else
            {
                throw new ArgumentOutOfRangeException("u16 value");
            }
        }

        static void RewriteU16(List<byte> code, int index, int value)
        {
            if (0 <= value && value < 65536)
            {
                /* big endian */
                int high = value >> 8;
                int low = value % 256;
                code[index] = (byte)low;
                code[index + 1] = (byte)high;
            }
            else
            {
                throw new ArgumentOutOfRangeException("u16 value");
            }
        }

        static void EmitString(List<byte> code, String value)
        {
            if (value.Length >= 0 && value.Length < 65536)
            {
                byte[] bytes = Encoding.Convert(Encoding.Default, Encoding.UTF8, Encoding.Default.GetBytes(value));
                EmitU16(code, bytes.Length);
                foreach (var b in bytes)
                {
                    code.Add(b);
                }
            }
            else
            {
                throw new ArgumentOutOfRangeException("string literal too long");
            }
        }

        static void EmitI32(List<byte> code, int value)
        {
            for (int i = 0; i < 4; i++)
            {
                code.Add((byte)((value >> (i * 8)) % 256));
            }
        }

        static void EmitOpType(List<byte> code, OpType opType)
        {
            code.Add((byte)opType);
        }
    }
}
