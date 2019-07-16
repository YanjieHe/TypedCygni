#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "Ast.hpp"
#include "Exception.hpp"
#include "Predef.hpp"
#include "Scope.hpp"
#include "Type.hpp"

/*
Scope:
    **Operator**: Vector<Ptr<FunctionType>>
    **Identifier**: Ptr<Type>
    **Function**: Ptr<FunctionType>
    **Type**: Ptr<Type>
*/

class TypeException : public Exception {
 public:
  Position position;

  TypeException(Position position, String message)
      : Exception(std::move(message)), position{position} {
    AddInfo("position", position.ToString());
  }
};
class TypeRegister {
 private:
  HashMap<int, Ptr<Type>> typeRecord;

 public:
  TypeRegister(HashMap<int, Ptr<Type>>& typeRecord) : typeRecord{typeRecord} {}

  // Ptr<Type> Register(const Ptr<Ast>& node, Ptr<Scope<Ptr<Type>>>& scope);

  void RegisterModule(const Ptr<DefModule>& node, Ptr<Scope<Ptr<Type>>>& scope);

  void RegisterFunction(const Ptr<Def>& node, Ptr<Scope<Ptr<Type>>>& scope) {}

  void RegisterVariable(const Ptr<Var>& node, Ptr<Scope<Ptr<Type>>>& scope) {
    // TO DO
  }

  Ptr<Type> ParseType(const Ptr<TypeExpression>& typeExp,
                      Ptr<Scope<Ptr<Type>>>& scope) {
    if (typeExp->name == "Function") {
      Vector<Ptr<Type>> types;
      types.reserve(typeExp->parameters.size());
      for (const auto& t : typeExp->parameters) {
        types.push_back(ParseType(t, scope));
      }
      if (types.size() == 0) {
        throw TypeException(
            typeExp->position,
            "the number of type parameters of function cannot be one");
      } else {
        auto returnType = types.back();
        types.pop_back();
        return New<FunctionType>(types, returnType);
      }
    } else {
      Optional<Ptr<Type>> result = scope->Lookup(typeExp->name);
      if (result) {
        throw NotImplementedException();
      } else {
        throw TypeException(typeExp->position, "type not defined");
      }
    }
  }
};

// class TypeChecker {
//  public:
//   class ScopeCollection {
//    public:
//     Ptr<Scope<Ptr<Type>>> typeScope;
//     Ptr<Scope<Ptr<Type>>> identifierScope;
//     Ptr<Scope<Vector<Ptr<FunctionType>>>> operatorScope;
//     Ptr<Scope<Ptr<FunctionType>>> functionScope;
//     ScopeCollection() = default;
//   };

//  public:
//   HashMap<int, Ptr<Type>> typeRecord;

//   void CheckProgram(const Program& program, const ScopeCollection& scopes) {
//     for (const auto& _class : program.classes) {
//       Check(_class, scopes);
//     }
//     for (const auto& module : program.modules) {
//       Check(module, scopes);
//     }
//   }

//   Ptr<Type> Check(const Ptr<Ast>& node, const ScopeCollection& scopes) {
//     auto record = [this, node](const Ptr<Type>& type) -> Ptr<Type> {
//       this->typeRecord.insert({node->id, type});
//       return type;
//     };
//     switch (node->kind) {
//       case Kind::Add:
//       case Kind::Subtract:
//       case Kind::Multiply:
//       case Kind::Divide:
//       case Kind::Modulo:
//       case Kind::GreaterThan:
//       case Kind::LessThan:
//       case Kind::GreaterThanOrEqual:
//       case Kind::LessThanOrEqual:
//       case Kind::Equal:
//       case Kind::NotEqual:
//         return record(CheckBinaryArithmetic(Cast<NotEqual>(node), scopes));
//       case Kind::UnaryPlus:
//       case Kind::UnaryMinus:
//         return record(CheckUnaryArithmetic(Cast<UnaryMinus>(node), scopes));
//       case Kind::And:
//         throw NotSupportedException();
//       case Kind::Or:
//         throw NotSupportedException();
//       case Kind::Not:
//         return record(CheckUnaryArithmetic(Cast<Not>(node), scopes));
//       case Kind::IfThen:
//         return record(CheckIfThen(Cast<IfThen>(node), scopes));
//       case Kind::IfElse:
//         return record(CheckIfElse(Cast<IfElse>(node), scopes));
//       case Kind::Constant:
//         return record(CheckConstant(Cast<Constant>(node)));
//       case Kind::Block:
//         return record(CheckBlock(Cast<Block>(node), scopes));
//       case Kind::Name:
//         return record(CheckName(Cast<Name>(node), scopes));
//       case Kind::Return:
//         return record(CheckReturn(Cast<Return>(node), scopes));
//       case Kind::Var:
//         return record(CheckVar(Cast<Var>(node), scopes));
//       case Kind::Def:
//         return record(CheckDef(Cast<Def>(node), scopes));
//       case Kind::Assign:
//         return record(CheckAssign(Cast<Assign>(node), scopes));
//       case Kind::Call:
//         return record(CheckCall(Cast<Call>(node), scopes));
//       case Kind::While:
//         break;
//       case Kind::DefClass:
//         return record(CheckClass(Cast<DefClass>(node), scopes));
//       case Kind::DefModule:
//         return record(CheckModule(Cast<DefModule>(node), scopes));
//       case Kind::TypeExpr:
//         return record(CheckTypeExpression(Cast<TypeExpression>(node), scopes));
//     }
//     throw NotImplementedException();
//   }

//   Ptr<Type> CheckBinaryArithmetic(Ptr<Binary<kind>> node,
//                                   const ScopeCollection& scopes) {
//     static HashMap<Kind, String> binaryOperators = {
//         {Kind::Add, "+"},
//         {Kind::Subtract, "-"},
//         {Kind::Multiply, "*"},
//         {Kind::Divide, "/"},
//         {Kind::Modulo, "%"},
//         {Kind::GreaterThan, ">"},
//         {Kind::LessThan, "<"},
//         {Kind::GreaterThanOrEqual, ">="},
//         {Kind::LessThanOrEqual, "<="},
//         {Kind::Equal, "=="},
//         {Kind::NotEqual, "!="},
//     };
//     Kind kind = node->kind;
//     auto leftType = Check(node->left, scopes);
//     auto rightType = Check(node->right, scopes);
//     auto result = scopes.operatorScope->Lookup(binaryOperators[kind]);
//     if (result) {
//       auto functions = *result;
//       for (const auto& function : functions) {
//         if (MatchParameters(function, {leftType, rightType})) {
//           return function->returnType;
//         }
//       }
//       throw TypeException(node->position, String("arithmetic type '") +
//                                               KindToString(kind) + String("'"));
//     } else {
//       throw TypeException(node->position, String("arithmetic computation '") +
//                                               KindToString(kind) +
//                                               String("' not supported"));
//     }
//   }

//   Ptr<Type> CheckUnaryArithmetic(Ptr<Unary<kind>> node,
//                                  const ScopeCollection& scopes) {
//     static HashMap<Kind, String> unaryOperators = {
//         {Kind::UnaryPlus, "+"}, {Kind::UnaryMinus, "-"}, {Kind::Not, "not"}};
//     Kind kind = node->kind;
//     auto operandType = Check(node->operand, scopes);
//     auto opChar = unaryOperators[kind];
//     auto result = scopes.operatorScope->Lookup(unaryOperators[kind]);
//     if (result) {
//       auto functions = *result;
//       for (const auto& function : functions) {
//         if (MatchParameters(function, {operandType})) {
//           return function->returnType;
//         }
//       }
//       throw TypeException(node->position, String("arithmetic type '") +
//                                               KindToString(kind) + String("'"));
//     } else {
//       throw TypeException(node->position, String("arithmetic type '") +
//                                               KindToString(kind) + String("'"));
//     }
//   }

//   static bool MatchParameters(const Ptr<FunctionType>& function,
//                               const Vector<Ptr<Type>>& arguments) {
//     auto comparator = [](const Ptr<Type>& x, const Ptr<Type>& y) -> bool {
//       return x->Equals(y);
//     };
//     return std::equal(function->parameters.begin(), function->parameters.end(),
//                       arguments.begin(), arguments.end(), comparator);
//   }

//   static Ptr<Type> CheckConstant(const Ptr<Constant>& node) {
//     switch (node->constantType) {
//       case Constant::ConstantType::Int32Type:
//         return Type::INT;
//       case Constant::ConstantType::Int64Type:
//         return Type::LONG;
//       case Constant::ConstantType::FloatType:
//         return Type::FLOAT;
//       case Constant::ConstantType::DoubleType:
//         return Type::DOUBLE;
//       case Constant::ConstantType::BooleanType:
//         return Type::BOOL;
//       case Constant::ConstantType::CharType:
//         return Type::CHAR;
//       case Constant::ConstantType::StringType:
//         return Type::STRING;
//       default:
//         throw NotImplementedException();
//     }
//   }

//   Ptr<Type> CheckBlock(const Ptr<Block>& node, const ScopeCollection& scopes) {
//     Ptr<Type> result = Type::VOID;
//     for (const auto& expression : node->expressions) {
//       result = Check(expression, scopes);
//     }
//     return result;
//   }

//   static Ptr<Type> CheckName(const Ptr<Name>& node,
//                              const ScopeCollection& scopes) {
//     auto result = scopes.identifierScope->Lookup(node->name);
//     if (result) {
//       return *result;
//     } else {
//       throw TypeException(node->position,
//                           "identifier '" + node->name + "' not defined");
//     }
//   }

//   Ptr<Type> CheckTypeExpression(const Ptr<TypeExpression>& expression,
//                                 const ScopeCollection& scopes) {
//     if (expression->name == "Function") {
//       Vector<Ptr<Type>> parameters;
//       if (expression->parameters.empty()) {
//         throw TypeException(expression->position,
//                             "parameters count do not match");
//       }
//       int n = expression->parameters.size();
//       for (int i = 0; i < n - 1; i++) {
//         const auto& p = expression->parameters.at(i);
//         parameters.push_back(CheckTypeExpression(p, scopes));
//       }
//       auto returnType =
//           CheckTypeExpression(expression->parameters.back(), scopes);
//       return New<FunctionType>(parameters, returnType);
//     } else {
//       auto result = scopes.typeScope->Lookup(expression->name);
//       if (result) {
//         auto type = *result;
//         Vector<Ptr<Type>> parameters =
//             Enumerate::Map(expression->parameters,
//                            [this, &scopes](const Ptr<TypeExpression>& exp) {
//                              return Check(exp, scopes);
//                            });
//         if (expression->parameters.empty()) {
//           return type;
//         } else if (type->GetTypeCode() == TypeCode::FUNCTION) {
//           auto functionType = Cast<FunctionType>(type);
//           Vector<Ptr<Type>> functionParams = functionType->parameters;
//           functionParams.push_back(functionType->returnType);
//           auto comparator = [](const Ptr<Type>& x, const Ptr<Type>& y) -> bool {
//             return x->Equals(y);
//           };
//           if (std::equal(functionParams.begin(), functionParams.end(),
//                          parameters.begin(), parameters.end(), comparator)) {
//             return functionType;
//           } else {
//             throw TypeException(expression->position, "Type not match");
//           }
//         } else {
//           // Generic Classes
//           throw NotImplementedException();
//         }
//       } else {
//         throw TypeException(expression->position, "type not defined");
//       }
//     }
//   }

//   Ptr<Type> CheckVar(const Ptr<Var>& node, const ScopeCollection& scopes) {
//     if (node->value) {
//       auto value = Check(*(node->value), scopes);
//       if (node->type) {
//         auto declaration = Check(*(node->type), scopes);
//         if (value->Equals(declaration)) {
//           scopes.identifierScope->Put(node->name, value);
//           return declaration;
//         } else {
//           throw TypeException(node->position, "variable type");
//         }
//       } else {
//         scopes.identifierScope->Put(node->name, value);
//         return value;
//       }
//     } else {
//       if (node->type) {
//         auto declaration = Check(*(node->type), scopes);
//         scopes.identifierScope->Put(node->name, declaration);
//         return declaration;
//       } else {
//         throw TypeException(node->position, "variable type missing");
//       }
//     }
//   }

//   Ptr<Type> CheckCall(const Ptr<Call>& node, const ScopeCollection& scopes) {
//     auto function = Check(node->function, scopes);
//     Vector<Ptr<Type>> arguments;
//     arguments.reserve(node->arguments.size());
//     for (const auto& arg : node->arguments) {
//       arguments.push_back(Check(arg, scopes));
//     }
//     if (function->GetTypeCode() == TypeCode::FUNCTION) {
//       if (MatchParameters(Cast<FunctionType>(function), arguments)) {
//         return Cast<FunctionType>(function)->returnType;
//       } else {
//         throw TypeException(node->position, "function call arguments' type");
//       }
//     } else {
//       throw TypeException(node->position, "unable to call");
//     }
//   }

//   Ptr<Type> CheckModule(const Ptr<DefModule>& node,
//                         const ScopeCollection& scopes) {
//     HashMap<String, Ptr<Type>> fields;
//     HashMap<String, Ptr<Type>> methods;
//     for (const Ptr<Var>& var : node->fields) {
//       fields.insert({var->name, Check(*(var->type), scopes)});
//     }
//     for (const Ptr<Def>& def : node->methods) {
//       methods.insert({def->name, Check(def->type, scopes)});
//     }
//     auto module = New<ModuleType>(fields, methods);
//     scopes.identifierScope->Put(node->name, module);
//     for (const auto& field : node->fields) {
//       Check(field, scopes);
//     }
//     for (const auto& method : node->methods) {
//       Check(method, scopes);
//     }
//     return Type::VOID;
//   }

//   Ptr<Type> CheckClass(const Ptr<DefClass>& node,
//                        const ScopeCollection& scopes) {
//     HashMap<String, Ptr<Type>> fields;
//     HashMap<String, Ptr<Type>> methods;
//     for (const Ptr<Var>& var : node->fields) {
//       fields.insert({var->name, Check(*(var->type), scopes)});
//     }
//     for (const Ptr<Def>& def : node->methods) {
//       methods.insert({def->name, Check(def->type, scopes)});
//     }
//     auto _class = New<ClassType>(fields, methods);
//     scopes.identifierScope->Put(node->name, _class);
//     for (const auto& field : node->fields) {
//       Check(field, scopes);
//     }
//     for (const auto& method : node->methods) {
//       Check(method, scopes);
//     }
//     return Type::VOID;
//   }

//   Ptr<Type> CheckIfThen(const Ptr<IfThen>& node,
//                         const ScopeCollection& scopes) {
//     auto condition = Check(node->condition, scopes);
//     if (condition->GetTypeCode() == TypeCode::BOOL) {
//       Check(node->ifTrue, scopes);
//       return Type::VOID;
//     } else {
//       throw TypeException(node->position, "condition must be boolean type");
//     }
//   }

//   Ptr<Type> CheckIfElse(const Ptr<IfElse>& node,
//                         const ScopeCollection& scopes) {
//     auto condition = Check(node->condition, scopes);
//     if (condition->GetTypeCode() == TypeCode::BOOL) {
//       Check(node->ifTrue, scopes);
//       Check(node->ifFalse, scopes);
//       return Type::VOID;
//     } else {
//       throw TypeException(node->position, "condition must be boolean type");
//     }
//   }

//   Ptr<Type> CheckReturn(const Ptr<Return>& node,
//                         const ScopeCollection& scopes) {
//     auto value = Check(node->value, scopes);
//     auto result = scopes.typeScope->Lookup("**Function**");
//     if (result) {
//       auto function = Cast<FunctionType>(*result);
//       if (value->Equals(function->returnType)) {
//         return value;
//       } else {
//         throw TypeException(node->position, "return type does not match");
//       }
//     } else {
//       throw TypeException(node->position,
//                           "return statement should be in a function");
//     }
//   }

//   Ptr<Type> CheckDef(const Ptr<Def>& node, const ScopeCollection& scopes) {
//     auto functionType = Check(node->type, scopes);
//     auto functionScope = New<Scope<Ptr<Type>>>(scopes.identifierScope);
//     auto typeScope = New<Scope<Ptr<Type>>>(scopes.typeScope);
//     scopes.identifierScope->Put(node->name, functionType);
//     typeScope->Put("**Function**", functionType);

//     for (const auto& parameter : node->parameters) {
//       functionScope->Put(parameter.name, Check(parameter.type, scopes));
//     }
//     auto newScopes = scopes;
//     newScopes.typeScope = typeScope;
//     newScopes.identifierScope = functionScope;
//     Check(node->body, newScopes);
//     return Type::VOID;
//   }

//   Ptr<Type> CheckAssign(const Ptr<Assign>& node,
//                         const ScopeCollection& scopes) {
//     auto right = Check(node->right, scopes);
//     if (node->left->kind == Kind::Name) {
//       auto name = Cast<Name>(node->left);
//       auto result = scopes.identifierScope->Lookup(name->name);
//       if (result) {
//         if ((*result)->Equals(right)) {
//           return right;
//         } else {
//           throw TypeException(node->position,
//                               "assignment: types are not matched");
//         }
//       } else {
//         throw TypeException(node->position, "variable is not defined");
//       }
//     } else {
//       throw TypeException(node->position,
//                           "left part of the assignment must be a variable");
//     }
//   }
// };

#endif  // TYPE_CHECKER