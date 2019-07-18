using System;
using System.Collections.Generic;
using System.Linq;
namespace Compiler
{
    public class TypeRegister
    {
        Dictionary<int, Scope> scopeMap;
        public TypeRegister(Dictionary<int, Scope> scopeMap)
        {
            this.scopeMap = scopeMap;
        }

        public void Register(Program program, Scope scope)
        {
            foreach (var module in program.modules)
            {
                var type = new ModuleType(module.name, module);
                scope.Insert(module.id.ToString(), "IDENTIFIER_TYPE", module);
            }
            foreach (var _class in program.classes)
            {
                var type = new ClassType(_class.name, _class);
                scope.Insert(_class.id.ToString(), "TYPE", _class);
            }
            program.modules.ForEach(m => RegisterModule(m, scope));
            program.classes.ForEach(c => RegisterClass(c, scope));
        }

        void RegisterModule(DefModule module, Scope parent)
        {
            Scope scope = new Scope(parent);
            module.methods.ForEach(f => RegisterFunction(f, scope));
            module.fields.ForEach(v => RegisterVariable(v, scope));
            scopeMap[module.id] = scope;
        }

        void RegisterClass(DefClass _class, Scope parent)
        {
            Scope scope = new Scope(parent);
            _class.methods.ForEach(f => RegisterFunction(f, scope));
            _class.fields.ForEach(v => RegisterVariable(v, scope));
            scopeMap[_class.id] = scope;
        }

        void RegisterFunction(Def function, Scope scope)
        {
            scope.Insert(function.name, "IDENTIFIER_TYPE", RegisterTypeSpecifier(function.type, scope));
        }

        void RegisterVariable(Var variable, Scope scope)
        {
            scope.Insert(variable.name, "IDENTIFIER_TYPE", RegisterTypeSpecifier(variable.type, scope));
        }

        Type RegisterTypeSpecifier(TypeSpecifier typeSpecifier, Scope scope)
        {
            if (typeSpecifier.name == "Function")
            {
                var items = typeSpecifier.parameters.Select(p => RegisterTypeSpecifier(p, scope)).ToList();
                return new FunctionType(items.Take(items.Count - 1).ToList(), items.Last());
            }
            else
            {
                Object result = scope.Lookup(typeSpecifier.name, "TYPE");
                if (result == null)
                {
                    throw new TypeException(typeSpecifier.position, "type not defined");
                }
                else
                {
                    return (Type)result;
                }
            }
        }
    }
    public class TypeRule
    {
        public List<Type> arguments;
        public Type returnType;
        public TypeRule(List<Type> arguments, Type returnType)
        {
            this.arguments = arguments;
            this.returnType = returnType;
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
    public class TypeRuleSet
    {
        public Dictionary<String, List<TypeRule>> rules;
        public TypeRuleSet()
        {
            this.rules = new Dictionary<string, List<TypeRule>>();
        }
        public void AddRule(String key, params Type[] types)
        {
            if (types.Length > 0)
            {
                List<Type> args = new List<Type>(types.Length - 1);
                for (int i = 0; i < types.Length - 1; i++)
                {
                    args.Add(types[i]);
                }
                var rule = new TypeRule(args, types.Last());
                if (rules.ContainsKey(key))
                {
                    rules[key].Add(rule);
                }
                else
                {
                    rules.Add(key, new List<TypeRule> { rule });
                }
            }
            else
            {
                throw new ArgumentException();
            }
        }
        public Type TryMatch(String key, List<Type> args)
        {
            foreach (var rule in rules[key])
            {
                if (rule.Match(args))
                {
                    return rule.returnType;
                }
            }
            return null;
        }
        public Type TryMatch(String key, params Type[] args)
        {
            foreach (var rule in rules[key])
            {
                if (rule.Match(args.ToList()))
                {
                    return rule.returnType;
                }
            }
            return null;
        }
        public int Count
        {
            get { return rules.Count; }
        }
    }

    public class TypeException : Exception
    {
        public Position position;
        public TypeException(Position position, String message) : base(message)
        {
            this.position = position;
        }
    }

    public class TypeChecker
    {
        Dictionary<int, Scope> scopeMap;
        Dictionary<int, Type> typeMap;
        public TypeChecker(Dictionary<int, Scope> scopeMap, Dictionary<int, Type> typeMap)
        {
            this.scopeMap = scopeMap;
            this.typeMap = typeMap;
            if (ruleSet.Count == 0)
            {
                ruleSet.AddRule("+", Type.INT, Type.INT, Type.INT);
                ruleSet.AddRule("-", Type.INT, Type.INT, Type.INT);
                ruleSet.AddRule("*", Type.INT, Type.INT, Type.INT);
                ruleSet.AddRule("/", Type.INT, Type.INT, Type.INT);
                ruleSet.AddRule("%", Type.INT, Type.INT, Type.INT);
                ruleSet.AddRule(">", Type.INT, Type.INT, Type.BOOL);
                ruleSet.AddRule("<", Type.INT, Type.INT, Type.BOOL);
                ruleSet.AddRule(">=", Type.INT, Type.INT, Type.BOOL);
                ruleSet.AddRule("<=", Type.INT, Type.INT, Type.BOOL);
                ruleSet.AddRule("==", Type.INT, Type.INT, Type.BOOL);
                ruleSet.AddRule("!=", Type.INT, Type.INT, Type.BOOL);

                ruleSet.AddRule("+", Type.INT, Type.INT);
                ruleSet.AddRule("-", Type.INT, Type.INT);

                ruleSet.AddRule("and", Type.BOOL, Type.BOOL, Type.BOOL);
                ruleSet.AddRule("or", Type.BOOL, Type.BOOL, Type.BOOL);
                ruleSet.AddRule("not", Type.BOOL, Type.BOOL);

                ruleSet.AddRule("+", Type.FLOAT, Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule("-", Type.FLOAT, Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule("*", Type.FLOAT, Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule("/", Type.FLOAT, Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule("%", Type.FLOAT, Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule(">", Type.FLOAT, Type.FLOAT, Type.BOOL);
                ruleSet.AddRule("<", Type.FLOAT, Type.FLOAT, Type.BOOL);
                ruleSet.AddRule(">=", Type.FLOAT, Type.FLOAT, Type.BOOL);
                ruleSet.AddRule("<=", Type.FLOAT, Type.FLOAT, Type.BOOL);
                ruleSet.AddRule("==", Type.FLOAT, Type.FLOAT, Type.BOOL);
                ruleSet.AddRule("!=", Type.FLOAT, Type.FLOAT, Type.BOOL);

                ruleSet.AddRule("+", Type.FLOAT, Type.FLOAT);
                ruleSet.AddRule("-", Type.FLOAT, Type.FLOAT);

                ruleSet.AddRule("+", Type.LONG, Type.LONG, Type.LONG);
                ruleSet.AddRule("-", Type.LONG, Type.LONG, Type.LONG);
                ruleSet.AddRule("*", Type.LONG, Type.LONG, Type.LONG);
                ruleSet.AddRule("/", Type.LONG, Type.LONG, Type.LONG);
                ruleSet.AddRule("%", Type.LONG, Type.LONG, Type.LONG);
                ruleSet.AddRule(">", Type.LONG, Type.LONG, Type.BOOL);
                ruleSet.AddRule("<", Type.LONG, Type.LONG, Type.BOOL);
                ruleSet.AddRule(">=", Type.LONG, Type.LONG, Type.BOOL);
                ruleSet.AddRule("<=", Type.LONG, Type.LONG, Type.BOOL);
                ruleSet.AddRule("==", Type.LONG, Type.LONG, Type.BOOL);
                ruleSet.AddRule("!=", Type.LONG, Type.LONG, Type.BOOL);

                ruleSet.AddRule("+", Type.LONG, Type.LONG);
                ruleSet.AddRule("-", Type.LONG, Type.LONG);

                ruleSet.AddRule("+", Type.DOUBLE, Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule("-", Type.DOUBLE, Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule("*", Type.DOUBLE, Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule("/", Type.DOUBLE, Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule("%", Type.DOUBLE, Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule(">", Type.DOUBLE, Type.DOUBLE, Type.BOOL);
                ruleSet.AddRule("<", Type.DOUBLE, Type.DOUBLE, Type.BOOL);
                ruleSet.AddRule(">=", Type.DOUBLE, Type.DOUBLE, Type.BOOL);
                ruleSet.AddRule("<=", Type.DOUBLE, Type.DOUBLE, Type.BOOL);
                ruleSet.AddRule("==", Type.DOUBLE, Type.DOUBLE, Type.BOOL);
                ruleSet.AddRule("!=", Type.DOUBLE, Type.DOUBLE, Type.BOOL);

                ruleSet.AddRule("+", Type.DOUBLE, Type.DOUBLE);
                ruleSet.AddRule("-", Type.DOUBLE, Type.DOUBLE);
            }
        }

        public static TypeRuleSet ruleSet = new TypeRuleSet();
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

        public void Check(Program program, Scope scope)
        {
            foreach (var module in program.modules)
            {
                Check(module, scope);
            }
            foreach (var _class in program.classes)
            {
                Check(_class, scope);
            }
        }

        Type Check(Ast node, Scope scope)
        {
            Type type = CheckNode(node, scope);
            typeMap[node.id] = type;
            return type;
        }

        Type CheckNode(Ast node, Scope scope)
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
                case Kind.And:
                case Kind.Or:
                    return CheckBinary((Binary)node, scope);
                case Kind.Not:
                case Kind.UnaryPlus:
                case Kind.UnaryMinus:
                    return CheckUnary((Unary)node, scope);
                case Kind.IfThen:
                    return CheckIfThen((IfThen)node, scope);
                case Kind.IfElse:
                    return CheckIfElse((IfElse)node, scope);
                case Kind.Constant:
                    return CheckConstant((Constant)node, scope);
                case Kind.Block:
                    return CheckBlock((Block)node, scope);
                case Kind.Name:
                    return CheckName((Name)node, scope);
                case Kind.Return:
                    return CheckReturn((Return)node, scope);
                case Kind.Var:
                    return CheckVar((Var)node, scope);
                case Kind.Def:
                    return CheckDef((Def)node, scope);
                case Kind.Assign:
                    return CheckAssign((Binary)node, scope);
                case Kind.Call:
                    return CheckCall((Call)node, scope);
                case Kind.While:
                    throw new NotImplementedException();
                case Kind.DefClass:
                    return CheckDefClass((DefClass)node, scope);
                case Kind.DefModule:
                    return CheckDefModule((DefModule)node, scope);
                case Kind.TypeSpecifier:
                    return CheckTypeSpecifier((TypeSpecifier)node, scope);
                default:
                    throw new NotSupportedException();
            }
        }

        Type CheckBinary(Binary node, Scope scope)
        {
            String str = opString[node.kind];
            Type left = Check(node.left, scope);
            Type right = Check(node.right, scope);
            Type result = ruleSet.TryMatch(str, left, right);
            if (result == null)
            {
                throw new TypeException(node.position, str);
            }
            else
            {
                return result;
            }
        }

        Type CheckUnary(Unary node, Scope scope)
        {
            String str = opString[node.kind];
            Type operand = Check(node.operand, scope);
            Type result = ruleSet.TryMatch(str, operand);

            if (result == null)
            {
                throw new TypeException(node.position, str);
            }
            else
            {
                return result;
            }
        }

        Type CheckIfThen(IfThen node, Scope scope)
        {
            Check(node.condition, scope);
            Check(node.ifTrue, scope);
            return Type.VOID;
        }

        Type CheckIfElse(IfElse node, Scope scope)
        {
            Check(node.condition, scope);
            Check(node.ifTrue, scope);
            Check(node.ifFalse, scope);
            return Type.VOID;
        }

        Type CheckConstant(Constant node, Scope scope)
        {
            if (node.value is int)
            {
                return Type.INT;
            }
            else if (node.value is bool)
            {
                return Type.BOOL;
            }
            else if (node.value is String)
            {
                return Type.STRING;
            }
            else
            {
                throw new NotImplementedException();
            }
        }

        Type CheckBlock(Block node, Scope scope)
        {
            foreach (var exp in node.expressions)
            {
                Check(exp, scope);
            }
            return Type.VOID;
        }

        Type CheckName(Name node, Scope scope)
        {
            Object result = scope.Lookup(node.name, "IDENTIFIER_TYPE");
            if (result == null)
            {
                throw new TypeException(node.position, "name not defined");
            }
            else
            {
                return (Type)result;
            }
        }

        Type CheckReturn(Return node, Scope scope)
        {
            Object result = scope.Lookup("**PARENT**", "FUNCTION");
            if (result == null)
            {
                throw new TypeException(node.position, "return statement is not in a function scope");
            }
            else
            {
                Scope functionScope = (Scope)result;
                Type returnType = (Type)functionScope.Lookup("**FUNCTION_INFO**", "RETURN_TYPE");
                Type type = Check(node.value, scope);
                if (type.Equals(returnType))
                {
                    return Type.VOID;
                }
                else
                {
                    throw new TypeException(node.position, "return type mismatch");
                }
            }
        }

        Type CheckAssign(Binary node, Scope scope)
        {
            Type left = Check(node.left, scope);
            Type right = Check(node.right, scope);
            if (left.Equals(right))
            {
                return Type.VOID;
            }
            else
            {
                throw new TypeException(node.position, "assign type mismatch");
            }
        }

        Type CheckVar(Var node, Scope scope)
        {
            if (node.type != null && node.value != null)
            {
                Type value = Check(node.value, scope);
                if (node.type.Equals(value))
                {
                    scope.Insert(node.name, "IDENTIFIER_TYPE", value);
                    return Type.VOID;
                }
                else
                {
                    throw new TypeException(node.position, "variable declaration type mismatch");
                }
            }
            else if (node.type == null && node.value != null)
            {
                Type value = Check(node.value, scope);
                scope.Insert(node.name, "IDENTIFIER_TYPE", value);
                return Type.VOID;
            }
            else if (node.type != null && node.value == null)
            {
                if (node.access == Access.Local)
                {
                    scope.Insert(node.name, "IDENTIFIER_TYPE", Check(node.type, scope));
                }
                return Type.VOID;
            }
            else
            {
                throw new NotSupportedException();
            }
        }

        Type CheckDef(Def node, Scope parent)
        {
            Scope scope = new Scope(parent);
            foreach (var p in node.parameters)
            {
                scope.Insert(p.name, "IDENTIFIER_TYPE", Check(p.type, parent));
            }
            scope.Insert("**FUNCTION_INFO**", "RETURN_TYPE", ((FunctionType)Check(node.type, parent)).returnType);
            scope.Insert("**PARENT**", "FUNCTION", scope);
            Check(node.body, scope);
            return Type.VOID;
        }

        Type CheckCall(Call node, Scope scope)
        {
            Type function = Check(node.function, scope);
            var arguments = node.arguments.Select(arg => Check(arg, scope)).ToList();
            if (function is FunctionType)
            {
                var type = (FunctionType)function;
                if (type.parameters.Zip(arguments, (p, arg) => p.Equals(arg)).All(b => b))
                {
                    return type.returnType;
                }
                else
                {
                    throw new TypeException(node.position, "call arguments type mismatch");
                }
            }
            else
            {
                throw new TypeException(node.position, "call type mismatch");
            }
        }


        Type CheckDefClass(DefClass node, Scope parent)
        {
            Scope scope = scopeMap[node.id].Clone();
            scope.ChangeParent(parent);
            foreach (var variable in node.fields)
            {
                Check(variable, scope);
            }
            foreach (var function in node.methods)
            {
                Check(function, scope);
            }
            return Type.VOID;
        }

        Type CheckDefModule(DefModule node, Scope parent)
        {
            Scope scope = scopeMap[node.id].Clone();
            scope.ChangeParent(parent);
            foreach (var variable in node.fields)
            {
                Check(variable, scope);
            }
            foreach (var function in node.methods)
            {
                Check(function, scope);
            }
            return Type.VOID;
        }

        Type CheckTypeSpecifier(TypeSpecifier node, Scope scope)
        {
            if (node.name == "Function")
            {
                var items = node.parameters.Select(p => Check(p, scope)).ToList();
                return new FunctionType(items.Take(items.Count - 1).ToList(), items.Last());
            }
            else
            {
                Object result = scope.Lookup(node.name, "TYPE");
                if (result == null)
                {
                    throw new TypeException(node.position, "type not defined");
                }
                else
                {
                    return (Type)result;
                }
            }
        }
    }
}