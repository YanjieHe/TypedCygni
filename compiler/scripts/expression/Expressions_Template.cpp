#include "Enum.hpp"
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using std::optional;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::unordered_map;
using std::vector;
using std::weak_ptr;
using Byte = uint8_t;

class Position {
public:
  int startLine;
  int startCol;
  int endLine;
  int endCol;

  Position() : startLine{0}, startCol{0}, endLine{0}, endCol{0} {}
  Position(int startLine, int startCol, int endLine, int endCol)
      : startLine{startLine}, startCol{startCol}, endLine{endLine},
        endCol{endCol} {}
};

class MemberDeclaration {
public:
  virtual Position Pos() const = 0;
  virtual MemberKind Kind() const = 0;
  virtual AccessModifier GetAccessModifier() const = 0;
};

class FieldMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  string name;
  ExpPtr initializer;
  int fieldIndex;
  weak_ptr<ClassInfo> classInfo;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::FIELD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

class MethodMember : public MemberDeclaration {
public:
  Position pos;
  AccessModifier accessModifier;
  bool isConstructor;
  bool isAbstract;
  bool isVirtual;
  bool isOverride;
  int methodIndex;
  FunctionType functionType; // 'this' is the first parameter
  weak_ptr<ClassInfo> classInfo;

  Position Pos() const override { return pos; }
  MemberKind Kind() const override { return MemberKind::METHOD; }
  AccessModifier GetAccessModifier() const override { return accessModifier; }
};

enum class ExpressionType {
  INT = 0,
  LONG = 1,
  FLOAT = 2,
  DOUBLE = 3,
  BOOLEAN = 4,
  CHAR = 5,
  STRING = 6,
  ADD = 7,
  SUBTRACT = 8,
  MULTIPLY = 9,
  DIVIDE = 10,
  MODULO = 11,
  GT = 12,
  LT = 13,
  GE = 14,
  LE = 15,
  EQ = 16,
  NE = 17,
  LOGICAL_AND = 18,
  LOGICAL_OR = 19,
  LOGICAL_NOT = 20,
  PLUS = 21,
  MINUS = 22,
  INVOKE = 23,
  IDENTIFIER = 24,
  CONVERT = 25,
  MEMBER = 26,
  NEW = 27
};

class Statement;
class Expression;

using ExpPtr = shared_ptr<Expression>;
using StatementPtr = shared_ptr<Statement>;

class Statement {
public:
  virtual Position Pos() const = 0;
  virtual StatementType GetStatementType() const = 0;
};

class Expression : public Statement {
public:
  virtual ExpressionType NodeType() const = 0;
  StatementType GetStatementType() const override {
    return StatementType::EXPRESSION;
  }
};

class BinaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  ExpPtr left;
  ExpPtr right;
};

class ConstantExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  string value;
};

class UnaryExpression : public Expression {
public:
  Position pos;
  ExpressionType nodeType;
  ExpPtr operand;
};

class InvocationExpression : public Expression {
public:
  Position pos;
  ExpPtr function;
  vector<ExpPtr> args;
};

class IdentifierExpression : public Expression {
public:
  Position pos;
  string identifier;
};

class ConversionExpression : public Expression {
public:
  Position pos;
  ExpPtr expression;
};

class IfThenStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  shared_ptr<BlockStatement> ifTrue;
};

class IfElseStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  shared_ptr<BlockStatement> ifTrue;
  shared_ptr<BlockStatement> ifFalse;
};

class WhileStatement : public Statement {
public:
  Position pos;
  ExpPtr condition;
  shared_ptr<BlockStatement> body;
};

class BlockStatement : public Statement {
public:
  Position pos;
  vector<StatementPtr> statements;
};

class ReturnStatement : public Statement {
public:
  Position pos;
  ExpPtr value;
};

class BreakStatement : public Statement {
public:
  Position pos;
};

class AssignStatement : public Statement {
public:
  Position pos;
  AssignmentKind kind;
  ExpPtr left;
  ExpPtr value;
};

class VarDeclStatement : public Statement {
public:
  Position pos;
  string identifier;
  optional<TypePtr> type;
  optional<ExpPtr> value;
};

class MemberExpression : public Expression {
public:
  Position pos;
  ExpPtr object;
  shared_ptr<MemberDeclaration> declaration;
};

class NewExpression : public Expression {
public:
  Position pos;
  shared_ptr<MethodMember> constructor;
  vector<ExpPtr> args;
};

class NewArrayExpression : public Expression {
public:
  TypePtr type;
};

class ClassInfo {
public:
  bool isAbstract;
  bool isInterface;
  string packageName;
  string name;
  vector<weak_ptr<ClassInfo>> extendList;
  weak_ptr<ClassInfo> superClass;
  vector<weak_ptr<ClassInfo>> interfaceList;
  vector<shared_ptr<FieldMember>> fields;
  vector<shared_ptr<MethodMember>> methods;
  vector<shared_ptr<FieldMember>> staticVariables;
  vector<shared_ptr<MethodMember>> staticFunctions;
};

class Parameter {
public:
  Position pos;
  string name;
  TypePtr type;
};

class FunctionInfo {
public:
  TypePtr type;
  string packageName;
  string name;
  vector<Parameter> parameters;
  shared_ptr<BlockStatement> block;
  vector<shared_ptr<VarDeclStatement>> localVariables;
};

class Import {
public:
  Position pos;
  string packageName;
};

class Rename {
public:
  Position pos;
  string packageName;
  string originalName;
  string newName;
};

class CodeFile {
public:
  string path;
  string packageName;
  vector<Import> importList;
  vector<Rename> renameList;
  vector<shared_ptr<ClassInfo>> classList;
};

class Program {
public:
  vector<shared_ptr<CodeFile>> codeFiles;
};