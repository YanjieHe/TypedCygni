#include "Visitor.hpp"
#include <algorithm>
#include <stack>

using namespace std;

namespace cygni {

json AstToJsonSerialization::VisitSourceLocation(SourcePosition location) {
  json obj;
  obj["startLine"] = location.startLine;
  obj["startCol"] = location.startCol;
  obj["endLine"] = location.endLine;
  obj["endCol"] = location.endCol;
  return obj;
}

json AstToJsonSerialization::VisitUnary(std::shared_ptr<UnaryExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["operand"] = VisitExpression(node->operand);
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
  for (const auto &field : info->fieldDefs.values) {
    std::string name = UTF32ToUTF8(field->name);
    fieldsJson[name] = VisitFieldDef(field);
  }
  obj["fields"] = fieldsJson;

  json methodsJson(std::unordered_map<std::string, json>{});
  for (const auto &method : info->methodDefs.values) {
    std::string name = UTF32ToUTF8(method->name);
    methodsJson[name] = VisitMethodDef(method);
  }
  obj["methods"] = methodsJson;

  obj["virtualTable"] = VisitVirtualTable(info->virtualTable);
  return obj;
}

json AstToJsonSerialization::VisitModuleInfo(std::shared_ptr<ModuleInfo> info) {
  json obj;
  obj["nodeType"] = "module";
  obj["name"] = UTF32ToUTF8(info->name);

  json fieldsJson(std::unordered_map<std::string, json>{});
  for (const auto &field : info->fields.values) {
    std::string name = UTF32ToUTF8(field->name);
    fieldsJson[name] = VisitFieldDef(field);
  }
  obj["fields"] = fieldsJson;

  json methodsJson(std::unordered_map<std::string, json>{});
  for (const auto &method : info->methods.values) {
    std::string name = UTF32ToUTF8(method->name);
    methodsJson[name] = VisitMethodDef(method);
  }
  obj["methods"] = methodsJson;
  return obj;
}

json AstToJsonSerialization::VisitInterfaceInfo(
    std::shared_ptr<InterfaceInfo> info) {
  json obj;
  obj["nodeType"] = "module";
  obj["name"] = UTF32ToUTF8(info->name);

  json methodsJson(std::unordered_map<std::string, json>{});
  for (const auto &method : info->methodDefs.values) {
    std::string name = UTF32ToUTF8(method->name);
    methodsJson[name] = VisitMethodDef(method);
  }
  obj["methods"] = methodsJson;
  return obj;
}

json AstToJsonSerialization::VisitFieldDef(
    const std::shared_ptr<FieldInfo> field) {
  json obj;
  obj["isStatic"] = field->isStatic;
  obj["position"] = VisitSourceLocation(field->position);
  obj["accessModifier"] =
      UTF32ToUTF8(Enum<AccessModifier>::ToString(field->modifier));
  obj["annotations"] = VisitAnnotationList(field->annotations);
  obj["name"] = UTF32ToUTF8(field->name);
  obj["type"] = UTF32ToUTF8(field->type->ToString());
  obj["value"] = VisitExpression(field->value);
  return obj;
}

json AstToJsonSerialization::VisitMethodDef(
    const std::shared_ptr<MethodInfo> method) {
  json obj;

  obj["isStatic"] = method->isStatic;
  obj["position"] = VisitSourceLocation(method->position);
  obj["accessModifier"] =
      UTF32ToUTF8(Enum<AccessModifier>::ToString(method->modifier));
  obj["annotations"] = VisitAnnotationList(method->annotations);
  obj["name"] = UTF32ToUTF8(method->name);

  std::vector<json> parametersJson;
  for (const auto &parameter : method->parameters) {
    parametersJson.push_back(VisitParameter(parameter));
  }
  obj["parameters"] = parametersJson;
  obj["body"] = VisitExpression(method->body);
  obj["type"] = UTF32ToUTF8(method->signature->ToString());

  std::vector<json> localVarsJson;
  for (const auto &localVar : method->localVariables) {
    localVarsJson.push_back(VisitVarDefExpression(localVar));
  }
  obj["localVariables"] = localVarsJson;
  return obj;
}

json AstToJsonSerialization::VisitParameter(
    std::shared_ptr<ParameterExpression> parameter) {
  json obj;
  obj["name"] = UTF32ToUTF8(parameter->name);
  AttachNodeInformation(obj, parameter);
  obj["location"] = VisitLocation(parameter->location);
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
  obj["arguments"] = VisitArgumentList(node->arguments);
  obj["parameterLocation"] = VisitLocation(node->location);
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

json AstToJsonSerialization::VisitWhile(std::shared_ptr<WhileExpression> node) {
  json obj;
  AttachNodeInformation(obj, node);
  obj["condition"] = VisitExpression(node->condition);
  obj["body"] = VisitExpression(node->body);
  return obj;
}

json AstToJsonSerialization::VisitAssign(
    std::shared_ptr<BinaryExpression> node) {
  return VisitBinary(node);
}

void AstToJsonSerialization::AttachNodeInformation(json &obj, ExpPtr node) {
  obj["id"] = node->id;
  obj["position"] = VisitSourceLocation(node->position);
  obj["nodeType"] = UTF32ToUTF8(Enum<ExpressionType>::ToString(node->nodeType));
  obj["type"] = UTF32ToUTF8(node->type->ToString());
}

json AstToJsonSerialization::VisitPackage(std::shared_ptr<Package> package) {
  json obj;
  json classesJson(std::unordered_map<std::string, json>{});
  for (const auto &[_, classInfo] : package->classes) {
    std::string name = UTF32ToUTF8(classInfo->name);
    classesJson[name] = VisitClassInfo(classInfo);
  }
  json moduleJson(std::unordered_map<std::string, json>{});
  for (const auto &[_, moduleInfo] : package->modules) {
    std::string name = UTF32ToUTF8(moduleInfo->name);
    moduleJson[name] = VisitModuleInfo(moduleInfo);
  }
  obj["route"] = UTF32ToUTF8(package->route.ToString());
  obj["classes"] = classesJson;
  obj["modules"] = moduleJson;
  return obj;
}

json AstToJsonSerialization::VisitProject(Project &project) {
  json obj;
  for (auto [_, pkg] : project.packages) {
    obj[UTF32ToUTF8(pkg->route.ToString())] = VisitPackage(pkg);
  }
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
    const Table<std::u32string, AnnotationInfo> &annotations) {
  json annotationList(std::unordered_map<std::string, json>{});
  for (const auto &annotation : annotations.values) {
    json obj;
    std::string name = UTF32ToUTF8(annotation.name);
    obj["position"] = VisitSourceLocation(annotation.position);
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

json AstToJsonSerialization::VisitLocation(LocationPtr location) {
  json obj;
  if (location) {
    obj["type"] = UTF32ToUTF8(Enum<LocationType>::ToString(location->type));
  }
  return obj;
}

json AstToJsonSerialization::VisitVirtualTable(
    const VirtualTable &virtualTable) {
  std::vector<json> objList;
  for (const auto &item : virtualTable) {
    json obj;
    std::vector<json> methods;
    for (auto methodName : item.methodNames) {
      json methodNameObj;
      methodNameObj["methodName"] = UTF32ToUTF8(methodName.ToString());
      methods.push_back(methodNameObj);
    }
    obj["methods"] = methods;
    objList.push_back(obj);
  }
  return objList;
}

TreeTraverser::TreeTraverser(std::function<bool(ExpPtr)> filter)
    : filter{filter} {}

void TreeTraverser::VisitConstant(std::shared_ptr<ConstantExpression> node,
                                  std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
}

void TreeTraverser::VisitParameter(std::shared_ptr<ParameterExpression> node,
                                   std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
}

void TreeTraverser::VisitAssign(std::shared_ptr<BinaryExpression> node,
                                std::vector<ExpPtr> &nodeList) {
  VisitBinary(node, nodeList);
}

void TreeTraverser::VisitDefault(std::shared_ptr<DefaultExpression> node,
                                 std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
}

void TreeTraverser::VisitWhile(std::shared_ptr<WhileExpression> node,
                               std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->condition, nodeList);
  VisitExpression(node->body, nodeList);
}

void TreeTraverser::VisitUnary(std::shared_ptr<UnaryExpression> node,
                               std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->operand, nodeList);
}
void TreeTraverser::VisitBinary(std::shared_ptr<BinaryExpression> node,
                                std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->left, nodeList);
  VisitExpression(node->right, nodeList);
}
void TreeTraverser::VisitBlock(std::shared_ptr<BlockExpression> node,
                               std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  for (auto exp : node->expressions) {
    VisitExpression(exp, nodeList);
  }
}
void TreeTraverser::VisitReturn(std::shared_ptr<ReturnExpression> node,
                                std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->value, nodeList);
}
void TreeTraverser::VisitConditional(
    std::shared_ptr<ConditionalExpression> node,
    std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->condition, nodeList);
  VisitExpression(node->ifTrue, nodeList);
  VisitExpression(node->ifFalse, nodeList);
}
void TreeTraverser::VisitInvocation(std::shared_ptr<InvocationExpression> node,
                                    std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->expression, nodeList);
  for (auto arg : node->arguments) {
    VisitExpression(arg.value, nodeList);
  }
}
void TreeTraverser::VisitMemberAccess(
    std::shared_ptr<MemberAccessExpression> node,
    std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->object, nodeList);
}
void TreeTraverser::VisitNewExpression(std::shared_ptr<NewExpression> node,
                                       std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  for (auto arg : node->arguments) {
    VisitExpression(arg.value, nodeList);
  }
}
void TreeTraverser::VisitVarDefExpression(
    std::shared_ptr<VarDefExpression> node, std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->value, nodeList);
}
void TreeTraverser::VisitWhileExpression(std::shared_ptr<WhileExpression> node,
                                         std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(node, nodeList);
  VisitExpression(node->condition, nodeList);
  VisitExpression(node->body, nodeList);
}

void TreeTraverser::VisitField(const std::shared_ptr<FieldInfo> field,
                               std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(field->value, nodeList);
  VisitExpression(field->value, nodeList);
}

void TreeTraverser::VisitMethod(const std::shared_ptr<MethodInfo> method,
                                std::vector<ExpPtr> &nodeList) {
  AddIfNeeded(method->body, nodeList);
  VisitExpression(method->body, nodeList);
}

void TreeTraverser::VisitClass(std::shared_ptr<ClassInfo> classInfo,
                               std::vector<ExpPtr> &nodeList) {
  for (const auto &field : classInfo->fieldDefs) {
    VisitField(field, nodeList);
  }
  for (const auto &method : classInfo->methodDefs) {
    VisitMethod(method, nodeList);
  }
}

void TreeTraverser::VisitModule(std::shared_ptr<ModuleInfo> moduleInfo,
                                std::vector<ExpPtr> &nodeList) {
  for (const auto &field : moduleInfo->fields) {
    VisitField(field, nodeList);
  }
  for (const auto &method : moduleInfo->methods) {
    VisitMethod(method, nodeList);
  }
}

void TreeTraverser::AddIfNeeded(ExpPtr node, std::vector<ExpPtr> &nodeList) {
  if (filter(node)) {
    nodeList.push_back(node);
  }
}

void LocalVariableCollector::VisitMethodDef(
    std::shared_ptr<MethodInfo> method) {
  std::function<bool(ExpPtr)> filter = [](ExpPtr node) {
    return node->nodeType == ExpressionType::VariableDefinition;
  };
  TreeTraverser traverser(filter);
  std::vector<ExpPtr> nodeList;
  traverser.VisitExpression(method->body, nodeList);
  int offset = 0;
  for (auto parameter : method->parameters) {
    parameter->location = std::make_shared<ParameterLocation>(offset);
    offset++;
  }
  if (method->selfType->typeCode == TypeCode::Class) {
    offset++;
  }
  for (auto node : nodeList) {
    auto varDef = std::static_pointer_cast<VarDefExpression>(node);
    varDef->variable->location = std::make_shared<ParameterLocation>(offset);
    method->localVariables.push_back(varDef);
    offset++;
  }
}
void LocalVariableCollector::VisitPackage(std::shared_ptr<Package> package) {
  for (auto &[_, classInfo] : package->classes) {
    for (auto &methodInfo : classInfo->methodDefs) {
      VisitMethodDef(methodInfo);
    }
  }
  for (auto &[_, moduleInfo] : package->modules) {
    for (auto &methodInfo : moduleInfo->methods) {
      VisitMethodDef(methodInfo);
    }
  }
}
void LocalVariableCollector::VisitProject(Project &project) {
  for (auto [_, package] : project.packages) {
    VisitPackage(package);
  }
}
VariableLocator::VariableLocator(Project &project) : project{project} {}
void VariableLocator::VisitConstant(std::shared_ptr<ConstantExpression> node,
                                    Scope<LocationPtr> *scope) {}
void VariableLocator::VisitDefault(std::shared_ptr<DefaultExpression> node,
                                   Scope<LocationPtr> *scope) {}
void VariableLocator::VisitAssign(std::shared_ptr<BinaryExpression> node,
                                  Scope<LocationPtr> *scope) {
  VisitBinary(node, scope);
}
void VariableLocator::VisitWhile(std::shared_ptr<WhileExpression> node,
                                 Scope<LocationPtr> *scope) {
  VisitExpression(node->condition, scope);
  VisitExpression(node->body, scope);
}
void VariableLocator::VisitBlock(std::shared_ptr<BlockExpression> node,
                                 Scope<LocationPtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  for (auto exp : node->expressions) {
    VisitExpression(exp, scope);
  }
}
void VariableLocator::VisitUnary(std::shared_ptr<UnaryExpression> node,
                                 Scope<LocationPtr> *scope) {
  VisitExpression(node->operand, scope);
}
void VariableLocator::VisitBinary(std::shared_ptr<BinaryExpression> node,
                                  Scope<LocationPtr> *scope) {
  VisitExpression(node->left, scope);
  VisitExpression(node->right, scope);
}
void VariableLocator::VisitClassInfo(std::shared_ptr<ClassInfo> info,
                                     Scope<LocationPtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  int offset = 0;
  for (auto &field : info->fields) {
    auto name =
        FullQualifiedName(info->route).Concat(info->name).Concat(field->name);
    scope->Put(field->name, std::make_shared<MemberLocation>(
                                LocationType::ClassField, name, offset));
    field->index = offset;
    offset++;
  }
  offset = 0;
  for (auto &method : info->methods) {
    auto selfClassType = std::static_pointer_cast<ClassType>(method->selfType);
    auto name = FullQualifiedName(selfClassType->route)
                    .Concat(selfClassType->name)
                    .Concat(method->name);
    scope->Put(method->name, std::make_shared<MemberLocation>(
                                 LocationType::ClassMethod, name, offset));
  }
  for (auto &method : info->methodDefs) {
    VisitMethodDef(method, scope);
  }
}
void VariableLocator::VisitModuleInfo(std::shared_ptr<ModuleInfo> info,
                                      Scope<LocationPtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  int offset = 0;
  for (auto &field : info->fields) {
    auto name =
        FullQualifiedName(info->route).Concat(info->name).Concat(field->name);
    scope->Put(field->name, std::make_shared<MemberLocation>(
                                LocationType::ModuleField, name, offset));
    field->index = offset;
    offset++;
  }
  offset = 0;
  for (auto &method : info->methods) {
    auto name =
        FullQualifiedName(info->route).Concat(info->name).Concat(method->name);
    scope->Put(method->name, std::make_shared<MemberLocation>(
                                 LocationType::ModuleMethod, name, offset));
  }
  for (const auto &method : info->methods.values) {
    VisitMethodDef(method, scope);
  }
}
void VariableLocator::VisitMethodDef(const std::shared_ptr<MethodInfo> method,
                                     Scope<LocationPtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
  if (method->selfType->typeCode == TypeCode::Class) {
    // put 'this' at the last position of parameters
    scope->Put(U"this", std::make_shared<ParameterLocation>(
                            static_cast<int>(method->parameters.size())));
  }
  for (auto parameter : method->parameters) {
    auto location = parameter->location;
    scope->Put(parameter->name, location);
  }
  VisitExpression(method->body, scope);
}
void VariableLocator::VisitParameter(
    std::shared_ptr<ParameterExpression> parameter, Scope<LocationPtr> *scope) {
  if (auto value = scope->Get(parameter->name)) {
    parameter->location = *value;
  } else {
    throw TypeException(
        parameter->position,
        Format(U"parameter '{}' is not defined", parameter->name));
  }
}
void VariableLocator::VisitReturn(std::shared_ptr<ReturnExpression> node,
                                  Scope<LocationPtr> *scope) {
  VisitExpression(node->value, scope);
}
void VariableLocator::VisitConditional(
    std::shared_ptr<ConditionalExpression> node, Scope<LocationPtr> *scope) {
  VisitExpression(node->condition, scope);
  VisitExpression(node->ifTrue, scope);
  VisitExpression(node->ifFalse, scope);
}
void VariableLocator::VisitInvocation(
    std::shared_ptr<InvocationExpression> node, Scope<LocationPtr> *scope) {
  VisitExpression(node->expression, scope);
  for (auto arg : node->arguments) {
    VisitExpression(arg.value, scope);
  }
}
void VariableLocator::VisitMemberAccess(
    std::shared_ptr<MemberAccessExpression> node, Scope<LocationPtr> *scope) {
  VisitExpression(node->object, scope);
  TypePtr object = node->object->type;
  if (object->typeCode == TypeCode::Module) {
    auto moduleType = std::static_pointer_cast<ModuleType>(object);

    if (auto res = project.GetModule(moduleType)) {
      const auto &moduleInfo = *res;
      if (moduleInfo->fields.ContainsKey(node->field)) {
        auto &field = moduleInfo->fields.GetValueByKey(node->field);
        auto name = FullQualifiedName(moduleInfo->route)
                        .Concat(moduleInfo->name)
                        .Concat(field->name);
        node->location = std::make_shared<MemberLocation>(
            LocationType::ModuleField, name, *field->index);
      } else if (moduleInfo->methods.ContainsKey(node->field)) {
        auto &method = moduleInfo->methods.GetValueByKey(node->field);
        auto name = FullQualifiedName(moduleInfo->route)
                        .Concat(moduleInfo->name)
                        .Concat(method->name);
        int methodIndex = moduleInfo->methods.GetIndexByKey(node->field);
        node->location = std::make_shared<MemberLocation>(
            LocationType::ModuleMethod, name, methodIndex);
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in module '{}'",
                                   node->field, moduleType->name));
      }
    } else {
      throw TypeException(node->position, Format(U"undefined module '{}'",
                                                 moduleType->ToString()));
    }
  } else if (object->typeCode == TypeCode::Class) {
    auto classType = std::static_pointer_cast<ClassType>(object);
    if (auto res = project.GetClass(classType)) {
      const auto &classInfo = *res;
      if (classInfo->fields.ContainsKey(node->field)) {
        auto &field = classInfo->fields.GetValueByKey(node->field);
        auto name = FullQualifiedName(classInfo->route)
                        .Concat(classInfo->name)
                        .Concat(field->name);
        node->location = std::make_shared<MemberLocation>(
            LocationType::ClassField, name, *field->index);
      } else if (classInfo->methods.ContainsKey(node->field)) {
        auto &method = classInfo->methods.GetValueByKey(node->field);
        auto selfClassType =
            std::static_pointer_cast<ClassType>(method->selfType);
        auto name = FullQualifiedName(selfClassType->route)
                        .Concat(selfClassType->name)
                        .Concat(method->name);
        if (auto selfClassInfo = project.GetClass(selfClassType)) {
          int methodIndex =
              selfClassInfo.value()->methodDefs.GetIndexByKey(node->field);
          node->location = std::make_shared<MemberLocation>(
              LocationType::ClassMethod, name, methodIndex);
        } else {
          throw TypeException(
              node->position,
              Format(U"undefined class '{}'", selfClassType->ToString()));
        }
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in class '{}'",
                                   node->field, classType->name));
      }
    } else {
      throw TypeException(node->position,
                          Format(U"undefined class '{}'", classType->name));
    }
  } else if (object->typeCode == TypeCode::Interface) {
    auto interfaceType = std::static_pointer_cast<InterfaceType>(object);
    if (auto res = project.GetInterface(interfaceType)) {
      const auto &interfaceInfo = *res;
      if (interfaceInfo->allMethods.ContainsKey(node->field)) {
        auto &method = interfaceInfo->allMethods.GetValueByKey(node->field);
        auto name = FullQualifiedName(interfaceInfo->route)
                        .Concat(interfaceInfo->name)
                        .Concat(method->name);
        int methodIndex = interfaceInfo->allMethods.GetIndexByKey(node->field);
        node->location = std::make_shared<MemberLocation>(
            LocationType::InterfaceMethod, name, methodIndex);
      } else {
        throw TypeException(node->position,
                            Format(U"undefined field '{}' in class '{}'",
                                   node->field, interfaceInfo->name));
      }
    } else {
      throw TypeException(node->position, Format(U"undefined interface '{}'",
                                                 interfaceType->name));
    }
  } else {
    throw TypeException(
        node->position,
        Format(U"object '{}' does not have any field", object->ToString()));
  }
}
void VariableLocator::VisitNewExpression(std::shared_ptr<NewExpression> node,
                                         Scope<LocationPtr> *scope) {
  if (node->type->typeCode == TypeCode::Class) {
    auto classType = std::static_pointer_cast<ClassType>(node->type);
    if (auto res = project.GetClass(classType)) {
      const auto &classInfo = *res;
      for (auto &arg : node->arguments) {
        if (arg.name) {
          if (classInfo->fields.ContainsKey(*arg.name)) {
            int index = classInfo->fields.GetIndexByKey(*arg.name);
            arg.index = index;
            VisitExpression(arg.value, scope);
          } else {
            throw TypeException(
                node->position,
                Format(U"missing field name '{}' in the new statement",
                       *arg.name));
          }
        } else {
          throw TypeException(
              node->position,
              Format(U"missing field name '{}' in the new statement",
                     *arg.name));
        }
      }
    } else {
      throw TypeException(node->position, Format(U"undefined class '{}'",
                                                 classType->ToString()));
    }
  } else {
    throw TypeException(node->position,
                        U"wrong new statement: type is not a class");
  }
}
void VariableLocator::VisitVarDefExpression(
    std::shared_ptr<VarDefExpression> node, Scope<LocationPtr> *scope) {
  scope->Put(node->variable->name, node->variable->location);
  VisitExpression(node->value, scope);
}
void VariableLocator::VisitWhileExpression(
    std::shared_ptr<WhileExpression> node, Scope<LocationPtr> *scope) {
  VisitExpression(node->condition, scope);
  VisitExpression(node->body, scope);
}

void VariableLocator::VisitPackage(std::shared_ptr<Package> package,
                                   Scope<LocationPtr> *globalScope) {
  auto scope = scopeFactory->New(globalScope);
  for (auto [_, moduleInfo] : package->modules) {
    auto name = FullQualifiedName(moduleInfo->route).Concat(moduleInfo->name);
    scope->Put(moduleInfo->name,
               std::make_shared<TypeLocation>(LocationType::ModuleName, name));
  }
  for (auto [_, classInfo] : package->classes) {
    VisitClassInfo(classInfo, scope);
  }
  for (auto [_, moduleInfo] : package->modules) {
    VisitModuleInfo(moduleInfo, scope);
  }
}
void VariableLocator::VisitProgramFile(std::shared_ptr<SourceDocument> program,
                                       Scope<LocationPtr> *outerScope) {
  auto scope = scopeFactory->New(outerScope);
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
        auto classLoc =
            make_shared<TypeLocation>(LocationType::ClassName, name);
        scope->Put(classInfo->name, classLoc);
      }
    }
    for (auto [_, moduleInfo] : pkg->modules) {
      if (auto name =
              FullQualifiedName(moduleInfo->route).Concat(moduleInfo->name);
          hideDefs.count(name) == 0) {
        auto moduleLoc =
            make_shared<TypeLocation>(LocationType::ModuleName, name);
        scope->Put(moduleInfo->name, moduleLoc);
      }
    }
    for (auto [_, interfaceInfo] : pkg->interfaces) {
      if (auto name = FullQualifiedName(interfaceInfo->route)
                          .Concat(interfaceInfo->name);
          hideDefs.count(name) == 0) {
        auto interfaceLoc =
            make_shared<TypeLocation>(LocationType::InterfaceName, name);
        scope->Put(interfaceInfo->name, interfaceLoc);
      }
    }
  }
  // fix rename statements
  for (auto renameStatement : program->typeAliases) {
    auto unresolvedType = std::make_shared<UnresolvedType>(
        renameStatement.route, renameStatement.typeName);
    if (auto resolvedType = project.ResolveType(unresolvedType)) {
      if (resolvedType.value()->typeCode == TypeCode::Class) {
        auto classType = static_pointer_cast<ClassType>(resolvedType.value());
        auto name = FullQualifiedName(classType->route).Concat(classType->name);
        auto classLoc =
            make_shared<TypeLocation>(LocationType::ClassName, name);
        scope->Put(renameStatement.alias, classLoc);
      } else if (resolvedType.value()->typeCode == TypeCode::Module) {
        auto moduleType = static_pointer_cast<ModuleType>(resolvedType.value());
        auto name =
            FullQualifiedName(moduleType->route).Concat(moduleType->name);
        auto moduleLoc =
            make_shared<TypeLocation>(LocationType::ModuleName, name);
        scope->Put(renameStatement.alias, moduleLoc);
      } else if (resolvedType.value()->typeCode == TypeCode::Interface) {
        auto interfaceType =
            static_pointer_cast<InterfaceType>(resolvedType.value());
        auto name =
            FullQualifiedName(interfaceType->route).Concat(interfaceType->name);
        auto interfaceLoc =
            make_shared<TypeLocation>(LocationType::InterfaceName, name);
        scope->Put(renameStatement.alias, interfaceLoc);
      } else {
        throw TypeException(
            renameStatement.position,
            U"resolved type is not a class, module or interface");
      }
    } else {
      throw TypeException(
          renameStatement.position,
          Format(U"missing type '{}' referred in the rename statement",
                 unresolvedType->ToString()));
    }
  }
  for (auto [_, classInfo] : program->classDefs) {
    auto name = FullQualifiedName(classInfo->route).Concat(classInfo->name);
    auto classLoc =
        std::make_shared<TypeLocation>(LocationType::ClassName, name);
    scope->Put(classInfo->name, classLoc);
  }
  for (auto [_, moduleInfo] : program->moduleDefs) {
    auto name = FullQualifiedName(moduleInfo->route).Concat(moduleInfo->name);
    auto moduleLoc =
        std::make_shared<TypeLocation>(LocationType::ModuleName, name);
    scope->Put(moduleInfo->name, moduleLoc);
  }
  for (auto [_, classInfo] : program->classDefs) {
    VisitClassInfo(classInfo, scope);
  }
  for (auto [_, moduleInfo] : program->moduleDefs) {
    VisitModuleInfo(moduleInfo, scope);
  }
}
void VariableLocator::VisitProject() {
  scopeFactory = ScopeFactory<LocationPtr>::Create();
  auto scope = scopeFactory->New();
  for (auto [_, program] : project.programs) {
    VisitProgramFile(program, scope);
  }
}
void ConstantCollector::VisitMethodDef(std::shared_ptr<MethodInfo> method,
                                       ConstantSet &constantSet) {
  std::function<bool(ExpPtr)> filter = [](ExpPtr node) {
    return node->nodeType == ExpressionType::Constant ||
           node->nodeType == ExpressionType::MemberAccess ||
           node->nodeType == ExpressionType::UpCast ||
           node->nodeType == ExpressionType::DownCast ||
           node->nodeType == ExpressionType::New ||
           node->nodeType == ExpressionType::Parameter;
  };
  TreeTraverser traverser(filter);
  std::vector<ExpPtr> nodeList;
  traverser.VisitExpression(method->body, nodeList);
  for (auto exp : nodeList) {
    if (exp->nodeType == ExpressionType::Constant) {
      auto node = std::static_pointer_cast<ConstantExpression>(exp);
      if (node->type->typeCode == TypeCode::Boolean) {
        // pass
      } else {
        // TO DO: small integers, 0.0 and 1.0
        switch (node->type->typeCode) {
        case TypeCode::Int32: {
          constantSet[ConstantKind::CONSTANT_FLAG_I32].insert(node->constant);
          break;
        }
        case TypeCode::Int64: {
          constantSet[ConstantKind::CONSTANT_FLAG_I64].insert(node->constant);
          break;
        }
        case TypeCode::Float32: {
          constantSet[ConstantKind::CONSTANT_FLAG_F32].insert(node->constant);
          break;
        }
        case TypeCode::Float64: {
          constantSet[ConstantKind::CONSTANT_FLAG_F64].insert(node->constant);
          break;
        }
        case TypeCode::Boolean: {
          // TO DO
          constantSet[ConstantKind::CONSTANT_FLAG_BOOLEAN].insert(
              node->constant);
          break;
        }
        case TypeCode::Char: {
          // TO DO
          constantSet[ConstantKind::CONSTANT_FLAG_CHAR].insert(node->constant);
          break;
        }
        case TypeCode::String: {
          constantSet[ConstantKind::CONSTANT_FLAG_STRING].insert(
              node->constant);
          constantSet[ConstantKind::CONSTANT_FLAG_METHOD].insert(
              U"Predef.String.New");
          break;
        }
        case TypeCode::Void: {
          break;
        }
        default: {
          throw TypeException(
              node->position,
              Format(U"not supported type '{}' for constant expression",
                     node->type->ToString()));
        }
        }
      }
    } else if (exp->nodeType == ExpressionType::MemberAccess) {
      auto node = std::static_pointer_cast<MemberAccessExpression>(exp);
      if (node->location->type == LocationType::ModuleField) {
        auto loc = std::static_pointer_cast<MemberLocation>(node->location);
        constantSet[ConstantKind::CONSTANT_FLAG_STATIC_VAR].insert(
            loc->name.ToString());
      } else if (node->location->type == LocationType::ModuleMethod) {
        auto loc = std::static_pointer_cast<MemberLocation>(node->location);
        constantSet[ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION].insert(
            loc->name.ToString());
      } else if (node->location->type == LocationType::ClassMethod ||
                 node->location->type == LocationType::InterfaceMethod) {
        auto loc = std::static_pointer_cast<MemberLocation>(node->location);
        constantSet[ConstantKind::CONSTANT_FLAG_METHOD].insert(
            loc->name.ToString());
      }
    } else if (exp->nodeType == ExpressionType::UpCast ||
               exp->nodeType == ExpressionType::DownCast) {
      auto node = std::static_pointer_cast<UnaryExpression>(exp);
      constantSet[ConstantKind::CONSTANT_FLAG_CLASS].insert(
          node->operand->type->ToString());
      constantSet[ConstantKind::CONSTANT_FLAG_CLASS].insert(
          node->type->ToString());
    } else if (exp->nodeType == ExpressionType::New) {
      auto node = std::static_pointer_cast<NewExpression>(exp);
      constantSet[ConstantKind::CONSTANT_FLAG_CLASS].insert(
          node->type->ToString());
    } else if (exp->nodeType == ExpressionType::Parameter) {
      auto node = std::static_pointer_cast<ParameterExpression>(exp);
      if (node->location->type == LocationType::ModuleField) {
        auto loc = std::static_pointer_cast<MemberLocation>(node->location);
        constantSet[ConstantKind::CONSTANT_FLAG_STATIC_VAR].insert(
            loc->name.ToString());
      } else if (node->location->type == LocationType::ModuleMethod) {
        auto loc = std::static_pointer_cast<MemberLocation>(node->location);
        constantSet[ConstantKind::CONSTANT_FLAG_STATIC_FUNCTION].insert(
            loc->name.ToString());
      }
    }
  }
}
void ConstantCollector::VisitPackage(std::shared_ptr<Package> package) {
  for (auto &[_, classInfo] : package->classes) {
    ConstantSet constantSet;
    for (auto &method : classInfo->methodDefs.values) {
      VisitMethodDef(method, constantSet);
    }
    int index = 0;
    for (auto pair : constantSet) {
      ConstantKind kind = pair.first;
      for (std::u32string constVal : pair.second) {
        classInfo->constantMap[kind].insert({constVal, index});
        index++;
      }
    }
  }
  for (auto &[_, moduleInfo] : package->modules) {
    ConstantSet constantSet;
    for (auto &method : moduleInfo->methods.values) {
      VisitMethodDef(method, constantSet);
    }
    int index = 0;
    for (auto pair : constantSet) {
      ConstantKind kind = pair.first;
      for (std::u32string constVal : pair.second) {
        moduleInfo->constantMap[kind].insert({constVal, index});
        index++;
      }
    }
  }
}
void ConstantCollector::VisitProject(Project &project) {
  for (const auto &[_, package] : project.packages) {
    VisitPackage(package);
  }
}
// void PackageImporter::ImportPackages(Project & project)
// {
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ClassInfo>>> classMap;
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ModuleInfo>>> moduleMap;
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<InterfaceInfo>>> interfaceMap; 	for (auto pkg :
// project.packages)
// 	{
// 		std::unordered_set<PackageRoute> visited;
// 		CollectInfo(project, classMap, moduleMap, interfaceMap,
// pkg->route, pkg, visited);
// 	}
// 	for (auto pkg : project.packages)
// 	{
// 		if (classMap.find(pkg->route) != classMap.end())
// 		{
// 			for (auto classInfo : classMap.at(pkg->route))
// 			{
// 				pkg->classes.Add(classInfo->name, classInfo);
// 			}
// 		}
// 		if (moduleMap.find(pkg->route) != moduleMap.end())
// 		{
// 			for (auto moduleInfo : moduleMap.at(pkg->route))
// 			{
// 				pkg->modules.Add(moduleInfo->name, moduleInfo);
// 			}
// 		}
// 		if (interfaceMap.find(pkg->route) != interfaceMap.end())
// 		{
// 			for (auto interfaceInfo : interfaceMap.at(pkg->route))
// 			{
// 				pkg->interfaces.Add(interfaceInfo->name,
// interfaceInfo);
// 			}
// 		}
// 	}
// }
// void PackageImporter::CollectInfo(Project& project,
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ClassInfo>>>& classMap,
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<ModuleInfo>>>& moduleMap,
// 	std::unordered_map<PackageRoute,
// std::vector<std::shared_ptr<InterfaceInfo>>>& interfaceMap, 	PackageRoute
// currentRoute, 	std::shared_ptr<Package> pkg,
// std::unordered_set<PackageRoute>& visited)
// {
// 	if (visited.find(pkg->route) != visited.end())
// 	{
// 		// already visited the package, pass
// 	}
// 	else
// 	{
// 		visited.insert(pkg->route);

// 		for (const auto& classInfo : pkg->classDefs)
// 		{
// 			classMap[currentRoute].push_back(classInfo);
// 		}
// 		for (const auto& moduleInfo : pkg->moduleDefs)
// 		{
// 			moduleMap[currentRoute].push_back(moduleInfo);
// 		}
// 		for (const auto& interfaceInfo : pkg->interfaceDefs)
// 		{
// 			interfaceMap[currentRoute].push_back(interfaceInfo);
// 		}

// 		for (auto importedPkg : pkg->importedPackages)
// 		{
// 			if (project.packages.ContainsKey(importedPkg.route))
// 			{
// 				std::shared_ptr<Package> next =
// project.packages.GetValueByKey(importedPkg.route);
// CollectInfo(project, classMap, moduleMap, interfaceMap, currentRoute, next,
// visited);
// 			}
// 			else
// 			{
// 				throw TypeException(importedPkg.position,
// 					Format(U"missing pakcage: {}",
// importedPkg.route.ToString()));
// 			}
// 		}
// 	}
// }
} // namespace cygni
