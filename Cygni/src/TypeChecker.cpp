#include "TypeChecker.hpp"
#include "Exception.hpp"
#include <algorithm>

using namespace std;

namespace cygni {
TypeChecker::Rule::Rule(std::u32string functionName,
                        std::vector<TypePtr> parameters, TypePtr returnType)
    : functionName{functionName}, parameters{parameters}, returnType{
                                                              returnType} {}

void TypeChecker::RuleSet::Add(std::u32string functionName,
                               std::vector<TypePtr> parameters,
                               TypePtr returnType) {
  Rule rule{functionName, parameters, returnType};
  if (rules.find(functionName) != rules.end()) {
    rules[functionName].push_back(rule);
  } else {
    rules.insert({functionName, {rule}});
  }
}

std::optional<TypePtr>
TypeChecker::RuleSet::Match(std::u32string functionName,
                            std::vector<TypePtr> parameters) {
  auto matchOneRule = [&parameters](const Rule &rule) -> bool {
    return (rule.parameters.size() == parameters.size()) &&
           (std::equal(rule.parameters.begin(), rule.parameters.end(),
                       parameters.begin(), [](TypePtr x, TypePtr y) -> bool {
                         return x->Equals(y);
                       }));
  };
  if (HasKey(rules, functionName)) {
    for (const auto &rule : rules[functionName]) {
      if (matchOneRule(rule)) {
        return rule.returnType;
      }
    }
    return std::optional<TypePtr>();
  } else {
    return std::optional<TypePtr>();
  }
}

TypeChecker::TypeChecker(Project &project)
    : project{project}, typeGraph{project.typeGraph} {

  ruleSet.Add(U"+", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"+", {Type::Int64(), Type::Int64()}, Type::Int64());
  ruleSet.Add(U"+", {Type::Float32(), Type::Float32()}, Type::Float32());
  ruleSet.Add(U"+", {Type::Float64(), Type::Float64()}, Type::Float64());

  ruleSet.Add(U"-", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"-", {Type::Int64(), Type::Int64()}, Type::Int64());
  ruleSet.Add(U"-", {Type::Float32(), Type::Float32()}, Type::Float32());
  ruleSet.Add(U"-", {Type::Float64(), Type::Float64()}, Type::Float64());

  ruleSet.Add(U"*", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"*", {Type::Int64(), Type::Int64()}, Type::Int64());
  ruleSet.Add(U"*", {Type::Float32(), Type::Float32()}, Type::Float32());
  ruleSet.Add(U"*", {Type::Float64(), Type::Float64()}, Type::Float64());

  ruleSet.Add(U"/", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"/", {Type::Int64(), Type::Int64()}, Type::Int64());
  ruleSet.Add(U"/", {Type::Float32(), Type::Float32()}, Type::Float32());
  ruleSet.Add(U"/", {Type::Float64(), Type::Float64()}, Type::Float64());

  ruleSet.Add(U">", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U">", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U">", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U">", {Type::Float64(), Type::Float64()}, Type::Boolean());

  ruleSet.Add(U"<", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"<", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U"<", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U"<", {Type::Float64(), Type::Float64()}, Type::Boolean());

  ruleSet.Add(U">=", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U">=", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U">=", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U">=", {Type::Float64(), Type::Float64()}, Type::Boolean());

  ruleSet.Add(U"<=", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"<=", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U"<=", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U"<=", {Type::Float64(), Type::Float64()}, Type::Boolean());

  ruleSet.Add(U"==", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"==", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U"==", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U"==", {Type::Float64(), Type::Float64()}, Type::Boolean());
  ruleSet.Add(U"==", {Type::Char(), Type::Char()}, Type::Boolean());

  ruleSet.Add(U"!=", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"!=", {Type::Int64(), Type::Int64()}, Type::Boolean());
  ruleSet.Add(U"!=", {Type::Float32(), Type::Float32()}, Type::Boolean());
  ruleSet.Add(U"!=", {Type::Float64(), Type::Float64()}, Type::Boolean());
  ruleSet.Add(U"!=", {Type::Char(), Type::Char()}, Type::Boolean());

  ruleSet.Add(U"+", {Type::Int32()}, Type::Int32());
  ruleSet.Add(U"+", {Type::Int64()}, Type::Int64());
  ruleSet.Add(U"+", {Type::Float32()}, Type::Float32());
  ruleSet.Add(U"+", {Type::Float64()}, Type::Float64());

  ruleSet.Add(U"-", {Type::Int32()}, Type::Int32());
  ruleSet.Add(U"-", {Type::Int64()}, Type::Int64());
  ruleSet.Add(U"-", {Type::Float32()}, Type::Float32());
  ruleSet.Add(U"-", {Type::Float64()}, Type::Float64());
}

TypePtr TypeChecker::VisitUnary(std::shared_ptr<UnaryExpression> node,
                                std::shared_ptr<SourceDocument> program,
                                Scope<TypePtr> *scope) {
  auto operand = VisitExpression(node->operand, program, scope);
  auto checkUnaryOp = [this, &node,
                       &operand](std::u32string opName) -> TypePtr {
    if (auto res = ruleSet.Match(opName, {operand})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->position,
                          Format(U"type mismatch: {}, operand: {}", opName,
                                 operand->ToString()));
    }
  };
  if (node->nodeType == ExpressionType::UnaryPlus) {
    return checkUnaryOp(U"+");
  } else if (node->nodeType == ExpressionType::UnaryMinus) {
    return checkUnaryOp(U"-");
  } else if (node->nodeType == ExpressionType::Convert) {
    TypePtr from = VisitExpression(node->operand, program, scope);
    TypePtr to = node->type;
    if (typeGraph.IsSubTypeof(from, to)) {
      return to;
    } else if (typeGraph.IsSuperTypeof(from, to)) {
      if (Type::IsPrimitive(from) && Type::IsPrimitive(to)) {
        // pass
      } else {
        node->nodeType = ExpressionType::DownCast;
      }
      return to;
    } else {
      throw TypeException(
          node->position,
          Format(U"cannot convert the object from type '{}' to '{}'",
                 from->ToString(), to->ToString()));
    }
  } else {
    throw NotImplementedException(U"not implemented unary operation");
  }
}

TypePtr TypeChecker::VisitBinary(std::shared_ptr<BinaryExpression> node,
                                 std::shared_ptr<SourceDocument> program,
                                 Scope<TypePtr> *scope) {
  auto left = VisitExpression(node->left, program, scope);
  auto right = VisitExpression(node->right, program, scope);
  auto checkBinaryOp = [this, &node, &left,
                        &right](std::u32string opName) -> TypePtr {
    if (auto res = ruleSet.Match(opName, {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(
          node->position,
          Format(U"binary type mismatch: {}, left: {}, right: {}", opName,
                 left->ToString(), right->ToString()));
    }
  };
  if (node->nodeType == ExpressionType::Add) {
    return checkBinaryOp(U"+");
  } else if (node->nodeType == ExpressionType::Subtract) {
    return checkBinaryOp(U"-");
  } else if (node->nodeType == ExpressionType::Multiply) {
    return checkBinaryOp(U"*");
  } else if (node->nodeType == ExpressionType::Divide) {
    return checkBinaryOp(U"/");
  } else if (node->nodeType == ExpressionType::GreaterThan) {
    return checkBinaryOp(U">");
  } else if (node->nodeType == ExpressionType::LessThan) {
    return checkBinaryOp(U"<");
  } else if (node->nodeType == ExpressionType::GreaterThanOrEqual) {
    return checkBinaryOp(U">=");
  } else if (node->nodeType == ExpressionType::LessThanOrEqual) {
    return checkBinaryOp(U"<=");
  } else if (node->nodeType == ExpressionType::Equal) {
    return checkBinaryOp(U"==");
  } else if (node->nodeType == ExpressionType::NotEqual) {
    return checkBinaryOp(U"!=");
  } else {
    throw NotImplementedException(U"not implemented binary operation");
  }
}

TypePtr TypeChecker::VisitBlock(std::shared_ptr<BlockExpression> node,
                                std::shared_ptr<SourceDocument> program,
                                Scope<TypePtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  for (const auto &exp : node->expressions) {
    VisitExpression(exp, program, scope);
  }
  return Attach(node, Type::Void());
}

TypePtr TypeChecker::Attach(ExpPtr node, TypePtr type) {
  node->type = type;
  return type;
}

TypePtr TypeChecker::VisitAssign(std::shared_ptr<BinaryExpression> node,
                                 std::shared_ptr<SourceDocument> program,
                                 Scope<TypePtr> *scope) {
  if (node->left->nodeType == ExpressionType::Parameter) {
    auto left = VisitExpression(node->left, program, scope);
    auto right = VisitExpression(node->right, program, scope);
    if (left->Equals(right)) {
      return Attach(node, Type::Void());
    } else if (typeGraph.IsSuperTypeof(left, right)) {
      // add type conversion
      if (Type::IsPrimitive(left) && Type::IsPrimitive(right)) {
        auto convertExp = std::make_shared<UnaryExpression>(
            node->position, ExpressionType::Convert, node->right);
        node->right = convertExp;
      }
      return Attach(node, Type::Void());
    } else {
      throw TypeException(
          node->position,
          Format(U"assignment type mismtach: =, left '{}', right '{}'",
                 left->ToString(), right->ToString()));
    }
  } else if (node->left->nodeType == ExpressionType::MemberAccess) {
    auto left = VisitExpression(node->left, program, scope);
    auto right = VisitExpression(node->right, program, scope);
    if (left->Equals(right)) {
      node->type = Type::Void();
      return Type::Void();
    } else {
      throw TypeException(node->position,
                          U"type mismtach: assignment to field");
    }
  } else {
    throw TypeException(node->position,
                        U"cannot assign to the left expression");
  }
}

TypePtr TypeChecker::VisitWhile(std::shared_ptr<WhileExpression> node,
                                std::shared_ptr<SourceDocument> program,
                                Scope<TypePtr> *scope) {
  TypePtr condition = VisitExpression(node->condition, program, scope);
  if (condition->typeCode == TypeCode::Boolean) {
    VisitExpression(node->body, program, scope);
    return Type::Void();
  } else {
    throw TypeException(node->position,
                        U"while condition must be boolean type");
  }
}

TypePtr TypeChecker::CheckType(SourcePosition position, TypePtr type,
                               std::shared_ptr<SourceDocument> program,
                               Scope<TypePtr> *scope) {
  if (type->typeCode == TypeCode::Unresolved) {
    auto unresolvedType = std::static_pointer_cast<UnresolvedType>(type);
    if (auto resultType = project.ResolveType(unresolvedType)) {
      return *resultType;
    } else if (program->packageRoute.route == unresolvedType->route) {
      if (auto resultType = scope->Get(unresolvedType->name)) {
        return resultType.value();
      } else {
        throw TypeException(position, Format(U"type '{}' not defined",
                                             unresolvedType->ToString()));
      }
    } else {
      throw TypeException(position, Format(U"type '{}' not defined",
                                           unresolvedType->ToString()));
    }
  } else if (type->typeCode == TypeCode::Function) {
    auto functionType = std::static_pointer_cast<FunctionType>(type);
    std::vector<TypePtr> parameters;
    std::transform(functionType->parameters.begin(),
                   functionType->parameters.end(),
                   std::back_inserter(parameters),
                   [this, position, &program, scope](TypePtr t) -> TypePtr {
                     return CheckType(position, t, program, scope);
                   });
    return std::make_shared<FunctionType>(
        CheckType(position, functionType->selfType, program, scope),
        functionType->name, parameters,
        CheckType(position, functionType->returnType, program, scope));
  } else if (type->typeCode == TypeCode::Array) {
    auto arrayType = std::static_pointer_cast<ArrayType>(type);
    return std::make_shared<ArrayType>(
        CheckType(position, arrayType->elementType, program, scope));
  } else {
    return type;
  }
}

void TypeChecker::CheckInterfaceConstraint(
    std::shared_ptr<ClassInfo> classInfo,
    std::shared_ptr<InterfaceInfo> interfaceInfo) {
  /*for (auto superClass : classInfo->superClasses)
  {
          if (superClass->typeCode == TypeCode::Interface)
          {
                  auto interfaceType =
  std::static_pointer_cast<InterfaceType>(superClass); auto interfaceInfo =
  project.GetInterface(interfaceType); if (interfaceInfo)
                  {
                          for (const auto& method :
  (*interfaceInfo)->methodMap.values)
                          {
                                  if
  (classInfo->methods.ContainsKey(method->name))
                                  {
                                          classInfo->virtualMethodTable[(*interfaceInfo)->name].push_back(method);
                                  }
                                  else
                                  {
                                          throw TypeException(method->position,
                                                  Format(U"interface constraint
  function '{}' not implemented", method->name));
                                  }
                          }
                  }
                  else
                  {
                          throw TypeException(classInfo->position,
                                  Format(U"undefined interface '{}'",
  interfaceType->ToString()));
                  }
          }
  }*/
}

TypePtr TypeChecker::VisitConstant(std::shared_ptr<ConstantExpression> node,
                                   std::shared_ptr<SourceDocument> program,
                                   Scope<TypePtr> *outerScope) {
  return node->type;
}

TypePtr TypeChecker::CheckClassInfo(std::shared_ptr<ClassInfo> info,
                                    std::shared_ptr<SourceDocument> program,
                                    Scope<TypePtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  for (auto &field : info->fields.values) {
    scope->Put(field->name, field->type);
  }
  for (auto &method : info->methods.values) {
    scope->Put(method->name, method->signature);
  }
  for (auto &field : info->fieldDefs.values) {
    CheckFieldDef(field, program, scope);
  }
  for (auto &method : info->methodDefs.values) {
    CheckMethodDef(method, program, scope);
  }
  return Type::Void();
}

TypePtr TypeChecker::CheckModuleInfo(std::shared_ptr<ModuleInfo> info,
                                     std::shared_ptr<SourceDocument> program,
                                     Scope<TypePtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  for (auto &field : info->fields.values) {
    scope->Put(field->name, field->type);
  }
  for (auto &method : info->methods.values) {
    scope->Put(method->name, method->signature);
  }
  for (auto &field : info->fields.values) {
    CheckFieldDef(field, program, scope);
  }
  for (auto &method : info->methods.values) {
    CheckMethodDef(method, program, scope);
  }
  return Type::Void();
}

TypePtr TypeChecker::CheckFieldDef(std::shared_ptr<FieldInfo> field,
                                   std::shared_ptr<SourceDocument> program,
                                   Scope<TypePtr> *scope) {
  if (field->value->nodeType == ExpressionType::Default) {
    scope->Put(field->name, field->type);
    return field->type;
  } else {
    auto type = VisitExpression(field->value, program, scope);
    if (!field->type->Equals(type)) {
      throw TypeException(field->position, U"field type mismatch");
    } else {
      scope->Put(field->name, field->type);
      return field->type;
    }
  }
}

TypePtr TypeChecker::CheckMethodDef(std::shared_ptr<MethodInfo> method,
                                    std::shared_ptr<SourceDocument> program,
                                    Scope<TypePtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  if (method->selfType->typeCode == TypeCode::Class) {
    scope->Put(U"this", method->selfType);
  }
  for (const auto &parameter : method->parameters) {
    scope->Put(parameter->name, parameter->type);
  }
  VisitExpression(method->body, program, scope);
  return method->returnType;
}

TypePtr
TypeChecker::VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                            std::shared_ptr<SourceDocument> program,
                            Scope<TypePtr> *scope) {
  if (auto type = scope->Get(parameter->name)) {
    std::cout << "parameter name: " << parameter->name
              << ", type: " << type.value()->ToString() << std::endl;
    return Attach(parameter, *type);
  } else {
    throw TypeException(parameter->position,
                        Format(U"parameter '{}' not defined", parameter->name));
  }
}

TypePtr TypeChecker::VisitReturn(std::shared_ptr<ReturnExpression> node,
                                 std::shared_ptr<SourceDocument> program,
                                 Scope<TypePtr> *scope) {
  TypePtr returnType = VisitExpression(node->value, program, scope);
  Attach(node, returnType);
  return returnType;
}

TypePtr
TypeChecker::VisitConditional(std::shared_ptr<ConditionalExpression> node,
                              std::shared_ptr<SourceDocument> program,
                              Scope<TypePtr> *scope) {
  auto condition = VisitExpression(node->condition, program, scope);
  auto ifTrue = VisitExpression(node->ifTrue, program, scope);
  auto ifFalse = VisitExpression(node->ifFalse, program, scope);
  if (condition->Equals(Type::Boolean())) {
    Attach(node, Type::Void());
    return Type::Void();
  } else {
    throw TypeException(node->condition->position,
                        U"condition type must be boolean");
  }
}

TypePtr TypeChecker::VisitDefault(std::shared_ptr<DefaultExpression> node,
                                  std::shared_ptr<SourceDocument> program,
                                  Scope<TypePtr> *outerScope) {
  return node->type;
}

TypePtr TypeChecker::VisitInvocation(std::shared_ptr<InvocationExpression> node,
                                     std::shared_ptr<SourceDocument> program,
                                     Scope<TypePtr> *scope) {
  auto exp = VisitExpression(node->expression, program, scope);
  // TO DO: named arguments
  std::vector<TypePtr> args(node->arguments.size());
  std::transform(node->arguments.begin(), node->arguments.end(), args.begin(),
                 [this, &program, &scope](const Argument &arg) -> TypePtr {
                   return VisitExpression(arg.value, program, scope);
                 });
  if (exp->typeCode == TypeCode::Function) {
    auto functionType = std::static_pointer_cast<FunctionType>(exp);
    if (functionType->Match(args)) {
      return Attach(node, functionType->returnType);
    } else {
      throw TypeException(node->position,
                          U"function call argument(s)' type(s) do not match");
    }
  } else if (exp->typeCode == TypeCode::Array) {
    auto arrayType = std::static_pointer_cast<ArrayType>(exp);
    if (args.size() == 1) {
      if (args.at(0)->typeCode == TypeCode::Int32) {
        return Attach(node, arrayType->elementType);
      } else {
        throw TypeException(node->position,
                            U"array index should be 'Int' type");
      }
    } else {
      throw TypeException(node->position,
                          U"array should be accessed by one index");
    }
  } else {
    throw TypeException(node->position, U"expression is not a function");
  }
}

TypePtr
TypeChecker::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                               std::shared_ptr<SourceDocument> program,
                               Scope<TypePtr> *scope) {
  TypePtr object = VisitExpression(node->object, program, scope);
  if (object->typeCode == TypeCode::Module) {
    auto moduleType = std::static_pointer_cast<ModuleType>(object);
    if (auto moduleInfo = project.GetModule(moduleType)) {
      if (moduleInfo.value()->fields.ContainsKey(node->field)) {
        auto &field = moduleInfo.value()->fields.GetValueByKey(node->field);
        return Attach(node, field->type);
      } else if (moduleInfo.value()->methods.ContainsKey(node->field)) {
        auto &method = moduleInfo.value()->methods.GetValueByKey(node->field);
        return Attach(node, method->signature);
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in module '{}'",
                                   node->field, moduleType->ToString()));
      }
    } else {
      throw TypeException(node->position,
                          Format(U"undefined module '{}'", object->ToString()));
    }
  } else if (object->typeCode == TypeCode::Class) {
    auto classType = std::static_pointer_cast<ClassType>(object);
    if (auto classInfo = project.GetClass(classType)) {
      if (classInfo.value()->fields.ContainsKey(node->field)) {
        auto &field = classInfo.value()->fields.GetValueByKey(node->field);
        return Attach(node, field->type);
      } else if (classInfo.value()->methods.ContainsKey(node->field)) {
        auto &method = classInfo.value()->methods.GetValueByKey(node->field);
        return Attach(node, method->signature);
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in class '{}'",
                                   node->field, classType->ToString()));
      }
    } else {
      throw TypeException(node->position,
                          Format(U"undefined class '{}'", object->ToString()));
    }
  } else if (object->typeCode == TypeCode::Interface) {
    auto interfaceType = std::static_pointer_cast<InterfaceType>(object);
    if (auto interfaceInfo = project.GetInterface(interfaceType)) {
      if (interfaceInfo.value()->allMethods.ContainsKey(node->field)) {
        auto &method =
            interfaceInfo.value()->methodDefs.GetValueByKey(node->field);
        return Attach(node, method->signature);
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in interface '{}'",
                                   node->field, interfaceType->ToString()));
      }
    } else {
      throw TypeException(node->position, Format(U"undefined interface '{}'",
                                                 interfaceType->ToString()));
    }
  } else if (object->typeCode == TypeCode::Array) {
    auto arrayType = std::static_pointer_cast<ArrayType>(object);
    if (node->field == U"Size" || node->field == U"Length") {
      return Attach(node, Type::Int32());
    } else {
      throw TypeException(
          node->position,
          Format(U"undefined field '{}' of array", node->field));
    }
  } else {
    throw TypeException(
        node->position,
        Format(U"object '{}' does not have any field", object->ToString()));
  }
}

TypePtr TypeChecker::VisitNewExpression(std::shared_ptr<NewExpression> node,
                                        std::shared_ptr<SourceDocument> program,
                                        Scope<TypePtr> *scope) {
  node->type = CheckType(node->position, node->type, program, scope);
  if (node->type->typeCode == TypeCode::Class) {
    auto newExpType = std::static_pointer_cast<ClassType>(node->type);
    if (auto classInfo = project.GetClass(newExpType)) {
      for (const auto &argument : node->arguments) {
        if (argument.name) {
          if (classInfo.value()->fields.ContainsKey(argument.name.value())) {
            auto &field =
                classInfo.value()->fields.GetValueByKey(argument.name.value());
            auto value = VisitExpression(argument.value, program, scope);
            if (field->type->Equals(value)) {
              // pass
            } else {
              throw TypeException(
                  node->position,
                  Format(U"field '{}' initialization type does not match",
                         *(argument.name)));
            }
          } else {
            throw TypeException(
                node->position,
                Format(U"field initialization name '{}' not found",
                       *(argument.name)));
          }
        } else {
          throw TypeException(node->position,
                              U"field initialization name not specified");
        }
      }
      auto name = FullQualifiedName(classInfo.value()->route)
                      .Concat(classInfo.value()->name);
      node->location =
          std::make_shared<TypeLocation>(LocationType::ClassName, name);
      return node->type;
    } else {
      throw TypeException(node->position,
                          Format(U"error new expression: undefined class '{}'",
                                 newExpType->ToString()));
    }
  } else {
    throw TypeException(node->position,
                        Format(U"new expression expects a class type, not '{}'",
                               node->type->ToString()));
  }
}

TypePtr
TypeChecker::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                                   std::shared_ptr<SourceDocument> program,
                                   Scope<TypePtr> *scope) {
  TypePtr value = VisitExpression(node->value, program, scope);
  auto &variable = node->variable;
  variable->type = CheckType(node->position, variable->type, program, scope);
  if (variable->type->typeCode == TypeCode::Unknown) {
    // var x = 10
    std::cout << "visit var def exp: " << value->ToString() << std::endl;
    variable->type = value;
    scope->Put(variable->name, variable->type);
    return Attach(node, value);
  } else if (variable->type->Equals(value)) {
    // var x: Int = 10
    scope->Put(variable->name, variable->type);
    return Attach(node, variable->type);
  } else if (typeGraph.IsSubTypeof(value, variable->type)) {
    if (Type::IsPrimitive(value) && Type::IsPrimitive(variable->type)) {
      auto convertExp = std::make_shared<UnaryExpression>(
          node->position, ExpressionType::Convert, node->value);
      convertExp->type = variable->type;
      node->value = convertExp;
    }
    scope->Put(variable->name, variable->type);
    return Attach(node, variable->type);
  } else {
    throw TypeException(node->position,
                        Format(U"variable initialization type mismatch, "
                               U"variable: '{}', initialization: '{}'",
                               node->variable->type->ToString(),
                               node->value->type->ToString()));
  }
}
void TypeChecker::CheckPackage(std::shared_ptr<Package> package,
                               Scope<TypePtr> *globalScope) {
  for (const auto &[_, classInfo] : package->classes) {
    auto classType =
        std::make_shared<ClassType>(classInfo->route, classInfo->name);
    classInfo->inheritanceChain = typeGraph.InheritanceChain(classType);
    classInfo->interfaceList = typeGraph.GetAllInterfaces(classType);
  }
  for (auto &[_, interfaceInfo] : package->interfaces) {
    auto interfaceType = std::make_shared<InterfaceType>(interfaceInfo->route,
                                                         interfaceInfo->name);
    interfaceInfo->allSuperInterfaces =
        typeGraph.GetAllSuperInterfaces(interfaceType);
    for (auto superInterfaceType : interfaceInfo->allSuperInterfaces) {
      if (auto superInterfaceInfo = project.GetInterface(superInterfaceType)) {
        for (auto method : (*superInterfaceInfo)->methodDefs) {
          interfaceInfo->allMethods.Add(method->name, method);
        }
      } else {
        throw CompilerException(interfaceInfo->position,
                                Format(U"missing interface {} for interface {}",
                                       superInterfaceType->ToString(),
                                       interfaceInfo->name));
      }
    }
  }
}

void TypeChecker::CheckProgramFile(std::shared_ptr<SourceDocument> program,
                                   Scope<TypePtr> *globalScope) {
  auto scope = scopeFactory->New(globalScope);
  unordered_set<FullQualifiedName> hideDefs;

  for (auto renameStatement : program->typeAliases) {
    auto name = FullQualifiedName(renameStatement.route)
                    .Concat(renameStatement.typeName);
    hideDefs.insert(name);
  }

  // fix import statements
  for (auto importStatement : program->importedPackages) {
    auto pkg = project.packages.at(importStatement.route);
    for (auto [_, classInfo] : pkg->classes) {
      if (auto name =
              FullQualifiedName(classInfo->route).Concat(classInfo->name);
          hideDefs.count(name) == 0) {
        auto classType =
            std::make_shared<ClassType>(classInfo->route, classInfo->name);
        scope->Put(classInfo->name, classType);
      }
    }
    for (auto [_, moduleInfo] : pkg->modules) {
      if (auto name =
              FullQualifiedName(moduleInfo->route).Concat(moduleInfo->name);
          hideDefs.count(name) == 0) {
        auto moduleType =
            std::make_shared<ModuleType>(moduleInfo->route, moduleInfo->name);
        scope->Put(moduleInfo->name, moduleType);
      }
    }
    for (auto [_, interfaceInfo] : pkg->interfaces) {
      if (auto name = FullQualifiedName(interfaceInfo->route)
                          .Concat(interfaceInfo->name);
          hideDefs.count(name) == 0) {
        auto interfaceType = std::make_shared<InterfaceType>(
            interfaceInfo->route, interfaceInfo->name);
        scope->Put(interfaceInfo->name, interfaceType);
      }
    }
  }
  // fix rename statements
  for (auto renameStatement : program->typeAliases) {
    auto unresolvedType = std::make_shared<UnresolvedType>(
        renameStatement.route, renameStatement.typeName);
    if (auto resolvedType = project.ResolveType(unresolvedType)) {
      scope->Put(renameStatement.alias, resolvedType.value());
      // TO DO: hide original name
    } else {
      throw TypeException(
          renameStatement.position,
          Format(U"missing type '{}' referred in the rename statement",
                 unresolvedType->ToString()));
    }
  }
  for (auto [_, classInfo] : program->classDefs) {
    auto classType =
        std::make_shared<ClassType>(classInfo->route, classInfo->name);
    scope->Put(classInfo->name, classType);
  }
  for (auto [_, moduleInfo] : program->moduleDefs) {
    auto moduleType =
        std::make_shared<ModuleType>(moduleInfo->route, moduleInfo->name);
    scope->Put(moduleInfo->name, moduleType);
  }
  for (auto [_, classInfo] : program->classDefs) {
    CheckClassInfo(classInfo, program, scope);
  }
  for (auto [_, moduleInfo] : program->moduleDefs) {
    CheckModuleInfo(moduleInfo, program, scope);
  }
}
void TypeChecker::CheckProject() {
  scopeFactory = ScopeFactory<TypePtr>::Create();
  auto globalScope = scopeFactory->New();
  ResolveSignatures(globalScope);
  InitializeTypeGraph(globalScope);
  for (auto &[_, pkg] : project.packages) {
    CheckPackage(pkg, globalScope);
  }
  for (auto &[_, program] : project.programs) {
    CheckProgramFile(program, globalScope);
  }
}
void TypeChecker::InitializeTypeGraph(Scope<TypePtr> *scope) {
  for (auto [_, program] : project.programs) {
    for (auto [_, classInfo] : program->classDefs) {
      for (auto &superClass : classInfo->superTypes) {
        auto classType =
            std::make_shared<ClassType>(classInfo->route, classInfo->name);
        superClass = CheckType(classInfo->position, superClass, program, scope);
        typeGraph.AddEdge(classType, superClass);
        cout << classType->ToString() << " -> " << superClass->ToString()
             << endl;
      }
    }
    for (auto [_, interfaceInfo] : program->interfaceDefs) {
      for (auto &superInterface : interfaceInfo->superInterfaces) {
        auto interfaceType = std::make_shared<InterfaceType>(
            interfaceInfo->route, interfaceInfo->name);
        superInterface =
            CheckType(interfaceInfo->position, superInterface, program, scope);
        typeGraph.AddEdge(interfaceType, superInterface);
        cout << interfaceType->ToString() << " -> "
             << superInterface->ToString() << endl;
      }
    }
  }
}
void TypeChecker::ResolveSignatures(Scope<TypePtr> *globalScope) {
  for (auto &[_, program] : project.programs) {
    auto scope = scopeFactory->New(globalScope);
    // fix import statements
    for (auto importStatement : program->importedPackages) {
      if (project.packages.count(importStatement.route)) {
        auto pkg = project.packages.at(importStatement.route);
        for (auto [_, classInfo] : pkg->classes) {
          auto classType =
              std::make_shared<ClassType>(classInfo->route, classInfo->name);
          scope->Put(classInfo->name, classType);
        }
        for (auto [_, moduleInfo] : pkg->modules) {
          auto moduleType =
              std::make_shared<ModuleType>(moduleInfo->route, moduleInfo->name);
          scope->Put(moduleInfo->name, moduleType);
        }
        for (auto [_, interfaceInfo] : pkg->interfaces) {
          auto interfaceType = std::make_shared<InterfaceType>(
              interfaceInfo->route, interfaceInfo->name);
          scope->Put(interfaceInfo->name, interfaceType);
        }
      } else {
        throw TypeException(importStatement.position,
                            Format(U"cannot find package '{}'",
                                   importStatement.route.ToString()));
      }
    }
    // fix rename statements
    for (auto renameStatement : program->typeAliases) {
      auto unresolvedType = std::make_shared<UnresolvedType>(
          renameStatement.route, renameStatement.typeName);
      if (auto resolvedType = project.ResolveType(unresolvedType)) {
        scope->Put(renameStatement.alias, resolvedType.value());
        // TO DO: hide original name
      } else {
        throw TypeException(
            renameStatement.position,
            Format(U"missing type '{}' referred in the rename statement",
                   unresolvedType->ToString()));
      }
    }
    for (auto [_, classInfo] : program->classDefs) {
      auto classType =
          std::make_shared<ClassType>(classInfo->route, classInfo->name);
      scope->Put(classInfo->name, classType);
    }
    for (auto [_, moduleInfo] : program->moduleDefs) {
      auto moduleType =
          std::make_shared<ModuleType>(moduleInfo->route, moduleInfo->name);
      scope->Put(moduleInfo->name, moduleType);
    }
    for (auto [_, classInfo] : program->classDefs) {
      for (auto &field : classInfo->fieldDefs) {
        ResolveFieldSignature(field, program, scope);
      }
      for (auto &method : classInfo->methodDefs) {
        ResolveMethodSignature(method, program, scope);
      }
    }
    for (auto [_, moduleInfo] : program->moduleDefs) {
      for (auto &field : moduleInfo->fields) {
        ResolveFieldSignature(field, program, scope);
      }
      for (auto &method : moduleInfo->methods) {
        ResolveMethodSignature(method, program, scope);
      }
    }
  }
}
void TypeChecker::ResolveMethodSignature(
    std::shared_ptr<MethodInfo> method, std::shared_ptr<SourceDocument> program,
    Scope<TypePtr> *scope) {
  method->signature =
      CheckType(method->position, method->signature, program, scope);
  for (auto &parameter : method->parameters) {
    parameter->type =
        CheckType(method->position, parameter->type, program, scope);
  }
  method->returnType =
      CheckType(method->position, method->returnType, program, scope);
  method->selfType =
      CheckType(method->position, method->selfType, program, scope);
}
void TypeChecker::ResolveFieldSignature(std::shared_ptr<FieldInfo> field,
                                        std::shared_ptr<SourceDocument> program,
                                        Scope<TypePtr> *scope) {
  field->type = CheckType(field->position, field->type, program, scope);
}
} // namespace cygni