#include "TypeChecker.hpp"

// Ptr<Type> TypeRegister::Register(const Ptr<Ast>& node,
//                                  Ptr<Scope<Ptr<Type>>>& scope) {
//   auto record = [this, node](const Ptr<Type>& type) -> Ptr<Type> {
//     this->typeRecord.insert({node->id, type});
//     return type;
//   };
//   switch (node->kind) {
//     case Kind::Add:
//     case Kind::Subtract:
//     case Kind::Multiply:
//     case Kind::Divide:
//     case Kind::Modulo:
//     case Kind::GreaterThan:
//     case Kind::LessThan:
//     case Kind::GreaterThanOrEqual:
//     case Kind::LessThanOrEqual:
//     case Kind::Equal:
//     case Kind::NotEqual:
//       return record(CheckBinaryArithmetic(Cast<Binary>(node), scope));
//     case Kind::UnaryPlus:
//     case Kind::UnaryMinus:
//     case Kind::Not:
//       return record(CheckUnaryArithmetic(Cast<Unary>(node), scope));
//     case Kind::And:
//       throw NotSupportedException();
//     case Kind::Or:
//       throw NotSupportedException();
//     case Kind::IfThen:
//       return record(CheckIfThen(Cast<IfThen>(node), scope));
//     case Kind::IfElse:
//       return record(CheckIfElse(Cast<IfElse>(node), scope));
//     case Kind::Constant:
//       return record(CheckConstant(Cast<Constant>(node)));
//     case Kind::Block:
//       return record(CheckBlock(Cast<Block>(node), scope));
//     case Kind::Name:
//       return record(CheckName(Cast<Name>(node), scope));
//     case Kind::Return:
//       return record(CheckReturn(Cast<Return>(node), scope));
//     case Kind::Var:
//       return record(CheckVar(Cast<Var>(node), scope));
//     case Kind::Def:
//       return record(CheckDef(Cast<Def>(node), scope));
//     case Kind::Assign:
//       return record(CheckAssign(Cast<Assign>(node), scope));
//     case Kind::Call:
//       return record(CheckCall(Cast<Call>(node), scope));
//     case Kind::While:
//       break;
//     case Kind::DefClass:
//       return record(CheckClass(Cast<DefClass>(node), scope));
//     case Kind::DefModule:
//       return record(CheckModule(Cast<DefModule>(node), scope));
//     case Kind::TypeExpr:
//       return record(CheckTypeExpression(Cast<TypeExpression>(node), scope));
//   }
//   throw NotImplementedException();
// }

void TypeRegister::RegisterModule(const Ptr<DefModule>& node,
                                  Ptr<Scope<Ptr<Type>>>& scope) {
  for (const auto& variable : node->fields) {
    RegisterVariable(variable, scope);
  }
  for (const auto& function : node->methods) {
    RegisterFunction(function, scope);
  }
  Table<String, Ptr<Type>> variables;
  Table<String, Ptr<FunctionType>> functions;
  for (const auto& variable : node->fields) {
    const auto& type = typeRecord[variable->id];
    variables.Add(variable->name, type);
  }
  for (const auto& function : node->methods) {
    const auto& type = typeRecord[function->id];
    functions.Add(function->name, Cast<FunctionType>(type));
  }
  auto moduleType = New<ModuleType>(node->name);
  typeRecord[node->id] = moduleType;
}