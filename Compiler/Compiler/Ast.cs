using System;
using System.Collections.Generic;

namespace Compiler
{
    public enum Kind
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,
        GreaterThan,
        LessThan,
        GreaterThanOrEqual,
        LessThanOrEqual,
        Equal,
        NotEqual,
        And,
        Or,
        Not,
        UnaryPlus,
        UnaryMinus,
        IfThen,
        IfElse,
        Constant,
        Block,
        Name,
        Return,
        Var,
        Def,
        Assign,
        Call,
        While,
        DefClass,
        DefModule,
        TypeSpecifier,
        MemberAccess,
        MemberAssign,
        New
    };

    public enum Access
    {
        Local,  // for local variables and functions
        Public,
        Private,
        Protected
    };

    public class Position
    {
        public int startLine;
        public int startColumn;
        public int endLine;
        public int endColumn;

        public Position(int startLine, int startColumn, int endLine, int endColumn)
        {
            this.startLine = startLine;
            this.startColumn = startColumn;
            this.endLine = endLine;
            this.endColumn = endColumn;
        }

        public override string ToString()
        {
            return String.Join(", ", startLine + 1, startColumn + 1, endLine + 1, endColumn + 1);
        }
    };

    public class Ast
    {
        public Kind kind;
        public Position position;
        public int id;

        public static int astId = 0;
        public Ast(Kind kind, Position position)
        {
            this.kind = kind;
            this.position = position;
            this.id = astId;
            astId++;
        }
    };

    public class Binary : Ast
    {
        public Ast left;
        public Ast right;

        public Binary(Position position, Kind kind, Ast left, Ast right) :
                base(kind, position)
        {
            this.left = left;
            this.right = right;
        }
    };

    public class Unary : Ast
    {
        public Ast operand;

        public Unary(Position position, Kind kind, Ast operand) : base(kind, position)
        {
            this.operand = operand;
        }
    };

    public class IfThen : Ast
    {
        public Ast condition;
        public Ast ifTrue;

        public IfThen(Position position, Ast condition, Ast ifTrue) : base(Kind.IfThen, position)
        {
            this.condition = condition;
            this.ifTrue = ifTrue;
        }
    };

    public class IfElse : Ast
    {
        public Ast condition;
        public Ast ifTrue;
        public Ast ifFalse;

        public IfElse(Position position,
               Ast condition,
               Ast ifTrue,
               Ast ifFalse) : base(Kind.IfElse, position)
        {
            this.condition = condition;
            this.ifTrue = ifTrue;
            this.ifFalse = ifFalse;
        }
    };

    public class Constant : Ast
    {
        public Object value;
        public Constant(Position position, Object value) : base(Kind.Constant, position)
        {
            this.value = value;
        }
    };

    public class Block : Ast
    {
        public List<Ast> expressions;

        public Block(Position position, List<Ast> expressions) : base(Kind.Block, position)
        {
            this.expressions = expressions;
        }
    };

    public class Name : Ast
    {
        public String name;

        public Name(Position position, String name) : base(Kind.Name, position)
        {
            this.name = name;
        }
    };

    public class Return : Ast
    {
        public Ast value;

        public Return(Position position, Ast value) : base(Kind.Return, position)
        {
            this.value = value;
        }
    };

    public class While : Ast
    {
        public Ast condition;
        public Ast body;

        public While(Position position, Ast condition, Ast body) : base(Kind.While, position)
        {
            this.condition = condition;
            this.body = body;
        }
    };

    public class Call : Ast
    {
        public Ast function;
        public List<Ast> arguments;

        public Call(Position position, Ast function, List<Ast> arguments) : base(Kind.Call, position)
        {
            this.function = function;
            this.arguments = arguments;
        }
    };

    public class TypeSpecifier : Ast
    {
        public String name;
        public List<TypeSpecifier> parameters;

        public TypeSpecifier(Position position, String name) : base(Kind.TypeSpecifier, position)
        {
            this.name = name;
            this.parameters = new List<TypeSpecifier>();
        }

        public TypeSpecifier(Position position, String name, List<TypeSpecifier> parameters) : base(Kind.TypeSpecifier, position)
        {
            this.name = name;
            this.parameters = parameters;
        }
    };

    public class Var : Ast
    {
        public String name;
        public TypeSpecifier type; // nullable
        public Ast value;// nullable
        public Access access;

        public Var(Position position, String name, TypeSpecifier type, Ast value) : base(Kind.Var, position)
        {
            this.name = name;
            this.type = type;
            this.value = value;
            this.access = Access.Local;
        }
    };

    public class Parameter
    {
        public String name;
        public TypeSpecifier type;

        public Parameter(String name, TypeSpecifier type)
        {
            this.name = name;
            this.type = type;
        }
    };

    public class Def : Ast
    {
        public String name;
        public List<Parameter> parameters;
        public TypeSpecifier type;
        public Ast body;
        public Access access;

        public Def(Position position, String name, List<Parameter> parameters, TypeSpecifier type, Ast body) : base(Kind.Def, position)
        {
            this.name = name;
            this.parameters = parameters;
            this.type = type;
            this.body = body;
            this.access = Access.Local;
        }

        public static TypeSpecifier MakeFunctionType(Position position, List<Parameter> parameters, TypeSpecifier returnType)
        {
            List<TypeSpecifier> exps = new List<TypeSpecifier>();
            foreach (Parameter p in parameters)
            {
                exps.Add(p.type);
            }
            exps.Add(returnType);
            return new TypeSpecifier(position, "Function", exps);
        }
    };

    public class DefClass : Ast
    {
        public String name;
        public List<Var> fields;
        public List<Def> methods;

        public DefClass(Position position, String name, List<Var> fields, List<Def> methods) : base(Kind.DefClass, position)
        {
            this.name = name;
            this.fields = fields;
            this.methods = methods;
        }
    };

    public class DefModule : Ast
    {
        public String name;
        public List<Var> fields;
        public List<Def> methods;

        public DefModule(Position position, String name, List<Var> fields, List<Def> methods) : base(Kind.DefModule, position)
        {
            this.name = name;
            this.fields = fields;
            this.methods = methods;
        }
    };

    public class MemberInfo
    {
        public String name;
        public MemberInfo(String name)
        {
            this.name = name;
        }

        public override string ToString()
        {
            return name;
        }
    }
    public class MemberAccess : Ast
    {
        public Ast expression;
        public MemberInfo member;
        public MemberAccess(Position position, Ast expression, MemberInfo member)
            : base(Kind.MemberAccess, position)
        {
            this.expression = expression;
            this.member = member;
        }
    }
    public class MemberAssign : Ast
    {
        public Ast expression;
        public MemberInfo member;
        public Ast value;
        public MemberAssign(Position position, Ast expression, MemberInfo member, Ast value)
            : base(Kind.MemberAssign, position)
        {
            this.expression = expression;
            this.member = member;
            this.value = value;
        }
    }
    public class New: Ast
    {
        public TypeSpecifier type;
        public List<Ast> arguments;

        public New(Position position, TypeSpecifier type, List<Ast> arguments) : base(Kind.New, position)
        {
            this.type = type;
            this.arguments = arguments;
        }
    }
    public class Program
    {
        public String path;
        public List<DefClass> classes;
        public List<DefModule> modules;

        public Program(String path, List<DefClass> classes, List<DefModule> modules)
        {
            this.path = path;
            this.classes = classes;
            this.modules = modules;
        }
    };

}
