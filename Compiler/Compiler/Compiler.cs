using System;
using System.Collections.Generic;
using System.Linq;

namespace Compiler
{
    public enum Op
    {
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
        public Compiler(Dictionary<int, Type> typeMap)
        {
            this.typeMap = typeMap;
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
                ruleSet.AddRule("!=", new Type[] { Type.INT, Type.INT }, Op.EQ_I32);
                ruleSet.AddRule("==", new Type[] { Type.INT, Type.INT }, Op.NE_I32);

                ruleSet.AddRule("+", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.ADD_F64);
                ruleSet.AddRule("-", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.SUB_F64);
                ruleSet.AddRule("*", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.MUL_F64);
                ruleSet.AddRule("/", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.DIV_F64);
                ruleSet.AddRule("%", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.MOD_F64);

                ruleSet.AddRule(">", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.GT_F64);
                ruleSet.AddRule("<", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.LT_F64);
                ruleSet.AddRule(">=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.GE_F64);
                ruleSet.AddRule("<=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.LE_F64);
                ruleSet.AddRule("!=", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.EQ_F64);
                ruleSet.AddRule("==", new Type[] { Type.DOUBLE, Type.DOUBLE }, Op.NE_F64);

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
        public void Compile(Program program)
        {

        }

        void CompileNode(Ast node, List<byte> code)
        {
            switch (node.kind)
            {
                case Kind.Add:
                    break;
                case Kind.Subtract:
                    break;
                case Kind.Multiply:
                    break;
                case Kind.Divide:
                    break;
                case Kind.Modulo:
                    break;
                case Kind.GreaterThan:
                    break;
                case Kind.LessThan:
                    break;
                case Kind.GreaterThanOrEqual:
                    break;
                case Kind.LessThanOrEqual:
                    break;
                case Kind.Equal:
                    break;
                case Kind.NotEqual:
                    break;
                case Kind.And:
                    break;
                case Kind.Or:
                    break;
                case Kind.Not:
                    break;
                case Kind.UnaryPlus:
                    break;
                case Kind.UnaryMinus:
                    break;
                case Kind.IfThen:
                    break;
                case Kind.IfElse:
                    break;
                case Kind.Constant:
                    break;
                case Kind.Block:
                    break;
                case Kind.Name:
                    break;
                case Kind.Return:
                    break;
                case Kind.Var:
                    break;
                case Kind.Def:
                    break;
                case Kind.Assign:
                    break;
                case Kind.Call:
                    break;
                case Kind.While:
                    break;
                case Kind.DefClass:
                    break;
                case Kind.DefModule:
                    break;
                case Kind.TypeSpecifier:
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
        static void EmitOp(List<byte> code, Op op)
        {
            code.Add((byte)op);
        }
    }
}
