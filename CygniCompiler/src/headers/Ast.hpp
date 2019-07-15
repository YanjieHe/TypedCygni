#ifndef AST_HPP
#define AST_HPP

#include "Optional.hpp"
#include "Predef.hpp"
#include "String.hpp"
#include "Type.hpp"

enum class Kind {
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
  TypeExpr
};

enum class Access {
  Local,  // for local variables and functions
  Public,
  Private,
  Protected
};

class Position {
 public:
  int startLine;
  int startColumn;
  int endLine;
  int endColumn;

  Position(int startLine, int startColumn, int endLine, int endColumn);

  String ToString();
};

class Ast {
 public:
  Kind kind;
  Position position;
  int id;

  Ast(Kind kind, Position position);
};

class Binary : public Ast {
 public:
  Ptr<Ast> left;
  Ptr<Ast> right;

  Binary(Position position, Kind kind, Ptr<Ast> left, Ptr<Ast> right);
};

class Unary : public Ast {
 public:
  Ptr<Ast> operand;

  Unary(Position position, Kind kind, Ptr<Ast> operand);
};

class IfThen : public Ast {
 public:
  Ptr<Ast> condition;
  Ptr<Ast> ifTrue;

  IfThen(Position position, Ptr<Ast> condition, Ptr<Ast> ifTrue);
};

class IfElse : public Ast {
 public:
  Ptr<Ast> condition;
  Ptr<Ast> ifTrue;
  Ptr<Ast> ifFalse;

  IfElse(Position position,
         Ptr<Ast> condition,
         Ptr<Ast> ifTrue,
         Ptr<Ast> ifFalse);
};

class Constant : public Ast {
 public:
  enum class ConstantType {
    Int32Type,
    Int64Type,
    FloatType,
    DoubleType,
    BooleanType,
    CharType,
    StringType
  };
  ConstantType constantType;
  String value;

  Constant(Position position, ConstantType constantType, String value);
};

class Block : public Ast {
 public:
  Vector<Ptr<Ast>> expressions;

  Block(Position position, Vector<Ptr<Ast>> expressions);
};

class Name : public Ast {
 public:
  String name;

  Name(Position position, String name);
};

class Return : public Ast {
 public:
  Ptr<Ast> value;

  Return(Position position, Ptr<Ast> value);
};

class While : public Ast {
 public:
  Ptr<Ast> condition;
  Ptr<Ast> body;

  While(Position position, Ptr<Ast> condition, Ptr<Ast> body);
};

class Call : public Ast {
 public:
  Ptr<Ast> function;
  Vector<Ptr<Ast>> arguments;

  Call(Position position, Ptr<Ast> function, Vector<Ptr<Ast>> arguments);
};

class TypeExpression : public Ast {
 public:
  String name;
  Vector<Ptr<TypeExpression>> parameters;

  TypeExpression(Position position, String name);

  TypeExpression(Position position,
                 String name,
                 Vector<Ptr<TypeExpression>> parameters);
};

class Var : public Ast {
 public:
  String name;
  Optional<Ptr<TypeExpression>> type;
  Optional<Ptr<Ast>> value;
  Access access;

  Var(Position position,
      String name,
      Optional<Ptr<TypeExpression>> type,
      Optional<Ptr<Ast>> value);
};

class Parameter {
 public:
  String name;
  Ptr<TypeExpression> type;

  Parameter(String name, Ptr<TypeExpression> type);
};

class Def : public Ast {
 public:
  String name;
  Vector<Parameter> parameters;
  Ptr<TypeExpression> type;
  Ptr<Ast> body;
  Access access;

  Def(Position position,
      String name,
      Vector<Parameter> parameters,
      Ptr<TypeExpression> type,
      Ptr<Ast> body);

  static Ptr<TypeExpression> MakeFunctionType(Position position,
                                              Vector<Parameter> parameters,
                                              Ptr<TypeExpression> returnType);
};

class DefClass : public Ast {
 public:
  String name;
  Vector<Ptr<Var>> fields;
  Vector<Ptr<Def>> methods;

  DefClass(Position position,
           String name,
           Vector<Ptr<Var>> fields,
           Vector<Ptr<Def>> methods);
};

class DefModule : public Ast {
 public:
  String name;
  Vector<Ptr<Var>> fields;
  Vector<Ptr<Def>> methods;

  DefModule(Position position,
            String name,
            Vector<Ptr<Var>> fields,
            Vector<Ptr<Def>> methods);
};

class Program {
 public:
  String path;
  Vector<Ptr<DefClass>> classes;
  Vector<Ptr<DefModule>> modules;

  Program(String path,
          Vector<Ptr<DefClass>> classes,
          Vector<Ptr<DefModule>> modules);
};

String KindToString(Kind kind);

#endif  // AST_HPP