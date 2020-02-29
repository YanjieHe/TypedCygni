#include "Visitor.hpp"
#include "Exception.hpp"
#include <algorithm>
using std::cout;
using std::endl;

namespace cygni {

json AstToJsonSerialization::VisitSourceLocation(SourceLocation location) {
  json obj;
  obj["startLine"] = location.startLine;
  obj["startCol"] = location.startCol;
  obj["endLine"] = location.endLine;
  obj["endCol"] = location.endCol;
  return obj;
}

json AstToJsonSerialization::VisitBinary(
    std::shared_ptr<BinaryExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["left"] = VisitExpression(node->left);
  obj["right"] = VisitExpression(node->right);
  return obj;
}

json AstToJsonSerialization::VisitConstant(
    std::shared_ptr<ConstantExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["constant"] = UTF32ToUTF8(node->constant);
  obj["type"] = UTF32ToUTF8(node->type->ToString());
  return obj;
}

json AstToJsonSerialization::VisitClassInfo(std::shared_ptr<ClassInfo> info) {
  json obj;
  obj["nodeType"] = "class";
  obj["name"] = UTF32ToUTF8(info->name);

  json fieldsJson(std::unordered_map<std::string, json>{});
  for (const auto &field : info->fields.values) {
    std::string name = UTF32ToUTF8(field.name);
    fieldsJson[name] = VisitFieldDef(field);
  }
  obj["fields"] = fieldsJson;

  json methodsJson(std::unordered_map<std::string, json>{});
  for (const auto &method : info->methods.values) {
    std::string name = UTF32ToUTF8(method.name);
    methodsJson[name] = VisitMethodDef(method);
  }
  obj["methods"] = methodsJson;
  return obj;
}

json AstToJsonSerialization::VisitModuleInfo(std::shared_ptr<ModuleInfo> info) {
  json obj;
  obj["nodeType"] = "module";

  obj["name"] = UTF32ToUTF8(info->name);

  json fieldsJson(std::unordered_map<std::string, json>{});
  for (const auto &field : info->fields.values) {
    std::string name = UTF32ToUTF8(field.name);
    fieldsJson[name] = VisitFieldDef(field);
  }
  obj["fields"] = fieldsJson;

  json methodsJson(std::unordered_map<std::string, json>{});
  for (const auto &method : info->methods.values) {
    std::string name = UTF32ToUTF8(method.name);
    methodsJson[name] = VisitMethodDef(method);
  }
  obj["methods"] = methodsJson;
  return obj;
}

json AstToJsonSerialization::VisitFieldDef(const FieldDef &field) {
  json obj;
  obj["isStatic"] = field.isStatic;
  obj["location"] = VisitSourceLocation(field.location);
  obj["accessModifier"] =
      UTF32ToUTF8(Enum<AccessModifier>::ToString(field.modifier));
  obj["annotations"] = VisitAnnotationList(field.annotations);
  obj["name"] = UTF32ToUTF8(field.name);
  obj["type"] = UTF32ToUTF8(field.type->ToString());
  obj["value"] = VisitExpression(field.value);
  return obj;
}

json AstToJsonSerialization::VisitMethodDef(const MethodDef &method) {
  json obj;

  obj["isStatic"] = method.isStatic;
  obj["location"] = VisitSourceLocation(method.location);
  obj["accessModifier"] =
      UTF32ToUTF8(Enum<AccessModifier>::ToString(method.modifier));
  obj["annotations"] = VisitAnnotationList(method.annotations);
  obj["name"] = UTF32ToUTF8(method.name);

  std::vector<json> parametersJson;
  for (const auto &parameter : method.parameters) {
    parametersJson.push_back(VisitParameter(parameter));
  }
  obj["parameters"] = parametersJson;
  obj["body"] = VisitExpression(method.body);
  obj["type"] = UTF32ToUTF8(method.signature->ToString());

  return obj;
}

json AstToJsonSerialization::VisitParameter(
    std::shared_ptr<ParameterExpression> parameter) {
  json obj;
  obj["name"] = UTF32ToUTF8(parameter->name);
  AttachNodeInformation(obj, parameter);
  return obj;
}

json AstToJsonSerialization::VisitBlock(std::shared_ptr<BlockExpression> node) {
  json obj;
  obj["expressions"] = VisitExpressionList(node->expressions);
  AttachNodeInformation(obj, node);
  return obj;
}

json AstToJsonSerialization::VisitReturn(
    std::shared_ptr<ReturnExpression> node) {
  json obj;
  obj["value"] = VisitExpression(node->value);
  AttachNodeInformation(obj, node);
  return obj;
}

json AstToJsonSerialization::VisitConditional(
    std::shared_ptr<ConditionalExpression> node) {
  json obj;
  obj["condition"] = VisitExpression(node->condition);
  obj["ifTrue"] = VisitExpression(node->ifTrue);
  obj["ifFalse"] = VisitExpression(node->ifFalse);
  AttachNodeInformation(obj, node);
  return obj;
}

json AstToJsonSerialization::VisitDefault(
    std::shared_ptr<DefaultExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  return obj;
}

json AstToJsonSerialization::VisitInvocation(
    std::shared_ptr<InvocationExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["expression"] = VisitExpression(node->expression);
  obj["arguments"] = VisitArgumentList(node->arguments);
  return obj;
}

json AstToJsonSerialization::VisitMemberAccess(
    std::shared_ptr<MemberAccessExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["object"] = VisitExpression(node->object);
  obj["fieldName"] = UTF32ToUTF8(node->field);
  return obj;
}

json AstToJsonSerialization::VisitNewExpression(
    std::shared_ptr<NewExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["class"] = UTF32ToUTF8(node->name);
  obj["arguments"] = VisitArgumentList(node->arguments);
  return obj;
}

json AstToJsonSerialization::VisitVarDefExpression(
    std::shared_ptr<VarDefExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["variable"] = VisitExpression(node->variable);
  obj["value"] = VisitExpression(node->value);
  return obj;
}

void AstToJsonSerialization::AttachNodeInformation(json &obj, ExpPtr node) {
  obj["id"] = node->id;
  obj["location"] = VisitSourceLocation(node->location);
  obj["nodeType"] = UTF32ToUTF8(Enum<ExpressionType>::ToString(node->nodeType));
  obj["type"] = UTF32ToUTF8(node->type->ToString());
}

json AstToJsonSerialization::VisitExpression(ExpPtr node) {
  switch (node->nodeType) {
  case ExpressionType::Add:
  case ExpressionType::Subtract:
  case ExpressionType::Multiply:
  case ExpressionType::Divide:
  case ExpressionType::GreaterThan:
  case ExpressionType::LessThan:
  case ExpressionType::GreaterThanOrEqual:
  case ExpressionType::LessThanOrEqual:
  case ExpressionType::Equal:
  case ExpressionType::NotEqual:
  case ExpressionType::Assign:
    return VisitBinary(std::static_pointer_cast<BinaryExpression>(node));
  case ExpressionType::Constant:
    return VisitConstant(std::static_pointer_cast<ConstantExpression>(node));
  case ExpressionType::Block:
    return VisitBlock(std::static_pointer_cast<BlockExpression>(node));
  case ExpressionType::Return:
    return VisitReturn(std::static_pointer_cast<ReturnExpression>(node));
  case ExpressionType::Parameter:
    return VisitParameter(std::static_pointer_cast<ParameterExpression>(node));
  case ExpressionType::Conditional:
    return VisitConditional(
        std::static_pointer_cast<ConditionalExpression>(node));
  case ExpressionType::Default:
    return VisitDefault(std::static_pointer_cast<DefaultExpression>(node));
  case ExpressionType::Invoke:
    return VisitInvocation(
        std::static_pointer_cast<InvocationExpression>(node));
  case ExpressionType::MemberAccess:
    return VisitMemberAccess(
        std::static_pointer_cast<MemberAccessExpression>(node));
  case ExpressionType::New:
    return VisitNewExpression(std::static_pointer_cast<NewExpression>(node));
  case ExpressionType::VariableDefinition:
    return VisitVarDefExpression(
        std::static_pointer_cast<VarDefExpression>(node));
  default:
    throw NotImplementedException();
  }
}

json AstToJsonSerialization::VisitProgram(const Program &program) {
  json obj;
  json classesJson(std::unordered_map<std::string, json>{});
  for (const auto &info : program.classes.values) {
    std::string name = UTF32ToUTF8(info->name);
    classesJson[name] = VisitClassInfo(info);
  }
  json moduleJson(std::unordered_map<std::string, json>{});
  for (const auto &info : program.modules.values) {
    std::string name = UTF32ToUTF8(info->name);
    moduleJson[name] = VisitModuleInfo(info);
  }
  obj["packageName"] = UTF32ToUTF8(program.packageName);
  obj["classes"] = classesJson;
  obj["modules"] = moduleJson;
  return obj;
}

std::vector<json> AstToJsonSerialization::VisitArgumentList(
    const std::vector<Argument> &arguments) {
  std::vector<json> argumentsJson;
  for (const auto &argument : arguments) {
    json arg;
    if (argument.name) {
      arg["name"] = UTF32ToUTF8(*(argument.name));
      arg["value"] = VisitExpression(argument.value);
    } else {
      arg["name"] = "";
      arg["value"] = VisitExpression(argument.value);
    }
    argumentsJson.push_back(arg);
  }
  return argumentsJson;
}

json AstToJsonSerialization::VisitAnnotationList(
    const std::vector<AnnotationInfo> &annotations) {
  json annotationList(std::unordered_map<std::string, json>{});
  for (const auto &annotation : annotations) {
    json obj;
    std::string name = UTF32ToUTF8(annotation.name);
    obj["location"] = VisitSourceLocation(annotation.location);
    obj["name"] = name;
    obj["arguments"] = VisitArgumentList(annotation.arguments);
    annotationList[name] = obj;
  }
  return annotationList;
}

std::vector<json> AstToJsonSerialization::VisitExpressionList(
    const std::vector<ExpPtr> &expressions) {
  std::vector<json> expressionList;
  for (const auto &exp : expressions) {
    expressionList.push_back(VisitExpression(exp));
  }
  return expressionList;
}

// void LocalVariableCollector::VisitProgram(Program& program) {
// 	for (auto& info : program.classes.values) {
// 		for (auto& method : info->methods.values) {
// 			VisitMethodDef(method);
// 		}
// 	}
// }

// void LocalVariableCollector::VisitMethodDef(MethodDef& method) {
// 	// TO DO
// }

// void LocalVariableCollector::
// VisitBlockExpression(std::shared_ptr<BlockExpression> node) {

// }

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
    if (rule.parameters.size() == parameters.size()) {
      int n = rule.parameters.size();
      for (int i = 0; i < n; i++) {
        if (!(rule.parameters[i]->Equals(parameters[i]))) {
          return false;
        }
      }
      return true;
    } else {
      return false;
    }
  };
  if (rules.find(functionName) != rules.end()) {
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

TypeChecker::TypeChecker(const Program &program) : program{program} {

  ruleSet.Add(U"+", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"+", {Type::Int64(), Type::Int64()}, Type::Int64());

  ruleSet.Add(U"-", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"-", {Type::Int64(), Type::Int64()}, Type::Int64());

  ruleSet.Add(U"*", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"*", {Type::Int64(), Type::Int64()}, Type::Int64());

  ruleSet.Add(U"/", {Type::Int32(), Type::Int32()}, Type::Int32());
  ruleSet.Add(U"/", {Type::Int64(), Type::Int64()}, Type::Int64());

  ruleSet.Add(U"==", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"==", {Type::Int64(), Type::Int64()}, Type::Boolean());

  ruleSet.Add(U"!=", {Type::Int32(), Type::Int32()}, Type::Boolean());
  ruleSet.Add(U"!=", {Type::Int64(), Type::Int64()}, Type::Boolean());
}

TypePtr TypeChecker::VisitBinary(std::shared_ptr<BinaryExpression> node,
                                 ScopePtr scope) {
  auto left = VisitExpression(node->left, scope);
  auto right = VisitExpression(node->right, scope);
  if (node->nodeType == ExpressionType::Add) {
    if (auto res = ruleSet.Match(U"+", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: +");
    }
  } else if (node->nodeType == ExpressionType::Subtract) {
    if (auto res = ruleSet.Match(U"-", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: -");
    }
  } else if (node->nodeType == ExpressionType::Multiply) {
    if (auto res = ruleSet.Match(U"*", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: *");
    }
  } else if (node->nodeType == ExpressionType::Divide) {
    if (auto res = ruleSet.Match(U"+", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: /");
    }
  } else if (node->nodeType == ExpressionType::Equal) {
    if (auto res = ruleSet.Match(U"==", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: ==");
    }
  } else if (node->nodeType == ExpressionType::NotEqual) {
    if (auto res = ruleSet.Match(U"!=", {left, right})) {
      return Attach(node, *res);
    } else {
      throw TypeException(node->location, U"type mismatch: !=");
    }
  } else if (node->nodeType == ExpressionType::Assign) {
    if (left->Equals(right)) {
      return Attach(node, left);
    } else {
      throw TypeException(node->location, U"type mismtach: =");
    }
  } else {
    throw NotImplementedException();
  }
}

TypePtr TypeChecker::VisitBlock(std::shared_ptr<BlockExpression> node,
                                ScopePtr scope) {
  for (const auto &exp : node->expressions) {
    VisitExpression(exp, scope);
  }
  Attach(node, Type::Void());
  return Type::Void();
}

TypePtr TypeChecker::Attach(ExpPtr node, TypePtr type) {
  node->type = type;
  return type;
}

TypePtr TypeChecker::VisitExpression(ExpPtr node, ScopePtr scope) {
  //	std::cout << "msg: "
  //			  << cygni::UTF32ToUTF8(
  //					 Enum<ExpressionType>::ToString(node->nodeType))
  //			  << std::endl;
  switch (node->nodeType) {
  case ExpressionType::Add:
  case ExpressionType::Subtract:
  case ExpressionType::Multiply:
  case ExpressionType::Divide:
  case ExpressionType::Equal:
  case ExpressionType::NotEqual:
  case ExpressionType::Assign:
    return VisitBinary(std::static_pointer_cast<BinaryExpression>(node), scope);
  case ExpressionType::Block:
    return VisitBlock(std::static_pointer_cast<BlockExpression>(node), scope);
  case ExpressionType::Constant:
    return VisitConstant(std::static_pointer_cast<ConstantExpression>(node));
  case ExpressionType::Parameter:
    return VisitParameter(std::static_pointer_cast<ParameterExpression>(node),
                          scope);
  case ExpressionType::Conditional:
    return VisitConditional(
        std::static_pointer_cast<ConditionalExpression>(node), scope);
  case ExpressionType::Default:
    return VisitDefault(std::static_pointer_cast<DefaultExpression>(node));
  case ExpressionType::Invoke:
    return VisitInvocation(std::static_pointer_cast<InvocationExpression>(node),
                           scope);
  case ExpressionType::Return:
    return VisitReturn(std::static_pointer_cast<ReturnExpression>(node), scope);
  case ExpressionType::MemberAccess:
    return VisitMemberAccess(
        std::static_pointer_cast<MemberAccessExpression>(node), scope);
  case ExpressionType::New:
    return VisitNewExpression(std::static_pointer_cast<NewExpression>(node),
                              scope);
  case ExpressionType::VariableDefinition:
    return VisitVarDefExpression(
        std::static_pointer_cast<VarDefExpression>(node), scope);
  default:
    throw NotImplementedException();
  }
}

TypePtr TypeChecker::VisitConstant(std::shared_ptr<ConstantExpression> node) {
  return node->type;
}

TypePtr TypeChecker::VisitClassInfo(std::shared_ptr<ClassInfo> info,
                                    ScopePtr outerScope) {
  ScopePtr scope{outerScope};

  for (const auto &field : info->fields.values) {
    VisitFieldDef(field, scope);
  }
  for (const auto &method : info->methods.values) {
    scope->Put(method.name, method.signature);
  }
  for (const auto &method : info->methods.values) {
    VisitMethodDef(method, scope);
  }
  return Type::Void();
}

TypePtr TypeChecker::VisitModuleInfo(std::shared_ptr<ModuleInfo> info,
                                     ScopePtr outerScope) {
  ScopePtr scope{outerScope};

  for (const auto &field : info->fields.values) {
    VisitFieldDef(field, scope);
  }
  for (const auto &method : info->methods.values) {
    scope->Put(method.name, method.signature);
  }
  for (const auto &method : info->methods.values) {
    VisitMethodDef(method, scope);
  }
  return Type::Void();
}

TypePtr TypeChecker::VisitFieldDef(const FieldDef &field, ScopePtr scope) {
  if (field.value->nodeType == ExpressionType::Default) {
    scope->Put(field.name, field.type);
    return field.type;
  } else {
    auto type = VisitExpression(field.value, scope);
    if (!field.type->Equals(type)) {
      throw TypeException(field.location, U"field type mismatch");
    } else {
      scope->Put(field.name, field.type);
      return field.type;
    }
  }
}

TypePtr TypeChecker::VisitMethodDef(const MethodDef &method,
                                    ScopePtr outerScope) {
  cout << "visit method: " << UTF32ToUTF8(method.name) << endl;
  ScopePtr scope{outerScope};
  for (const auto &parameter : method.parameters) {
    scope->Put(parameter->name, parameter->type);
  }
  VisitExpression(method.body, scope);
  return method.returnType;
}

TypePtr
TypeChecker::VisitParameter(std::shared_ptr<ParameterExpression> parameter,
                            ScopePtr scope) {
  auto result = scope->Get(parameter->name);
  if (result) {
    auto type = std::any_cast<TypePtr>(*result);
    Attach(parameter, type);
    return type;
  } else {
    throw TypeException(parameter->location, U"parameter name not found");
  }
}

TypePtr TypeChecker::VisitReturn(std::shared_ptr<ReturnExpression> node,
                                 ScopePtr scope) {
  TypePtr returnType = VisitExpression(node->value, scope);
  Attach(node, returnType);
  return returnType;
}

TypePtr
TypeChecker::VisitConditional(std::shared_ptr<ConditionalExpression> node,
                              ScopePtr scope) {
  auto condition = VisitExpression(node->condition, scope);
  auto ifTrue = VisitExpression(node->ifTrue, scope);
  auto ifFalse = VisitExpression(node->ifFalse, scope);
  if (condition->Equals(Type::Boolean())) {
    Attach(node, Type::Void());
    return Type::Void();
  } else {
    throw TypeException(node->condition->location,
                        U"condition type must be boolean");
  }
}

TypePtr TypeChecker::VisitDefault(std::shared_ptr<DefaultExpression> node) {
  return node->type;
}

TypePtr TypeChecker::VisitInvocation(std::shared_ptr<InvocationExpression> node,
                                     ScopePtr scope) {
  // TO DO: named arguments
  auto exp = VisitExpression(node->expression, scope);
  std::vector<TypePtr> args(node->arguments.size());
  std::transform(node->arguments.begin(), node->arguments.end(), args.begin(),
                 [this, &scope](const Argument &arg) -> TypePtr {
                   return VisitExpression(arg.value, scope);
                 });
  if (exp->typeCode == TypeCode::Function) {
    auto functionType = std::static_pointer_cast<FunctionType>(exp);
    if (functionType->Match(args)) {
      Attach(node, functionType->returnType);
      return functionType->returnType;
    } else {
      throw TypeException(node->location,
                          U"function call arguments type do not match");
    }
  } else {
    throw TypeException(node->location, U"expression is not a function");
  }
}

TypePtr
TypeChecker::VisitMemberAccess(std::shared_ptr<MemberAccessExpression> node,
                               ScopePtr scope) {
  TypePtr object = VisitExpression(node->object, scope);
  if (object->typeCode == TypeCode::Module) {
    auto moduleType = std::static_pointer_cast<ModuleType>(object);
    const auto &moduleInfo = program.modules.GetValueByKey(moduleType->name);
    if (moduleInfo->fields.ContainsKey(node->field)) {
      auto field = moduleInfo->fields.GetValueByKey(node->field);
      node->type = field.type;
      return field.type;
    } else if (moduleInfo->methods.ContainsKey(node->field)) {
      auto method = moduleInfo->methods.GetValueByKey(node->field);
      node->type = method.signature;
      return method.signature;
    } else {
      throw TypeException(node->location, U"undefined field");
    }
  } else if (object->typeCode == TypeCode::Class) {
    std::shared_ptr<ClassType> classType =
        std::static_pointer_cast<ClassType>(object);
    const auto &classInfo = program.classes.GetValueByKey(classType->name);
    if (classInfo->fields.ContainsKey(node->field)) {
      auto field = classInfo->fields.GetValueByKey(node->field);
      node->type = field.type;
      return field.type;
    } else if (classInfo->methods.ContainsKey(node->field)) {
      auto method = classInfo->methods.GetValueByKey(node->field);
      node->type = method.signature;
    } else {
      throw TypeException(node->location, U"undefined field");
    }
  } else {
    throw TypeException(node->location, U"object does not have any field");
  }
}

TypePtr TypeChecker::VisitNewExpression(std::shared_ptr<NewExpression> node,
                                        ScopePtr scope) {
  // TO DO: arguments
  if (program.classes.ContainsKey(node->name)) {
    auto type = std::make_shared<ClassType>(node->name);
    node->type = type;
    return type;
  } else {
    throw TypeException(node->location,
                        U"error new expression: undefined class");
  }
}

TypePtr
TypeChecker::VisitVarDefExpression(std::shared_ptr<VarDefExpression> node,
                                   ScopePtr scope) {
  TypePtr value = VisitExpression(node->value, scope);
  //  cout << UTF32ToUTF8(value->ToString()) << endl;
  //  cout << UTF32ToUTF8(node->type->ToString()) << endl;
  if (node->type->Equals(value)) {
    scope->Put(node->variable->name, node->type);
    return node->type;
  } else {
    throw TypeException(node->location,
                        U"variable initialization type mismatch");
  }
}

void TypeChecker::VisitProgram(ScopePtr scope) {
  for (const auto &module : program.modules.values) {
    TypePtr moduleType = std::make_shared<ModuleType>(module->name);
    scope->Put(module->name, moduleType);
  }
  for (const auto &_class : program.classes.values) {
    VisitClassInfo(_class, scope);
  }
  for (const auto &module : program.modules.values) {
    VisitModuleInfo(module, scope);
  }
}
} // namespace cygni
