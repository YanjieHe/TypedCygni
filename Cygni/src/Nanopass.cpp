#include "Nanopass.hpp"
#include "Exception.hpp"
#include "Type.hpp"
#include "Visitor.hpp"
#include <functional>
#include <memory>
#include <stack>

using namespace std;

namespace cygni {
void PackageInfoCollectPass::CollectInfo(Project &project) {
  for (const auto &[_, program] : project.programs) {
    auto route = program->packageRoute.route;
    if (project.packages.count(route) == 0) {
      project.packages.insert({route, std::make_shared<Package>(route)});
    }
    auto pkg = project.packages[route];
    for (const auto &[name, classInfo] : program->classDefs) {
      pkg->classes.insert({name, classInfo});
    }
    for (const auto &[name, moduleInfo] : program->moduleDefs) {
      pkg->modules.insert({name, moduleInfo});
    }
    for (const auto &[name, interfaceInfo] : program->interfaceDefs) {
      pkg->interfaces.insert({name, interfaceInfo});
    }
  }
}
// ExpPtr TypeRenamePass::VisitVarDefExpression(
//     std::shared_ptr<VarDefExpression> node,
//     const Table<std::u32string, TypeAlias> &typeAliases) {
//   auto value = VisitExpression(node->value, typeAliases);
//   auto newNode = std::make_shared<VarDefExpression>(
//       node->position, node->variable, node->type, value);
//   newNode->variable->type = RenameType(newNode->variable->type, typeAliases);
//   return newNode;
// }
// ExpPtr TypeRenamePass::VisitNewExpression(
//     std::shared_ptr<NewExpression> node,
//     const Table<std::u32string, TypeAlias> &typeAliases) {
//   std::vector<Argument> arguments;
//   std::transform(node->arguments.begin(), node->arguments.end(),
//                  std::back_inserter(arguments),
//                  [this, &typeAliases](Argument arg) -> Argument {
//                    Argument newArg(VisitExpression(arg.value, typeAliases));
//                    newArg.index = arg.index;
//                    newArg.name = arg.name;
//                    return newArg;
//                  });
//   auto newNode =
//       std::make_shared<NewExpression>(node->position, node->type, arguments);
//   newNode->location = node->location;
//   newNode->type = RenameType(newNode->type, typeAliases);
//   return newNode;
// }
// void TypeRenamePass::RenameAll(Project &project) {
//   for (const auto &[_, program] : project.programs) {
//     const auto &typeAliases = program->typeAliases;
//     for (const auto &[_, classInfo] : program->classDefs) {
//       for (auto &superType : classInfo->superTypes) {
//         superType = RenameType(superType, typeAliases);
//       }
//       for (auto &field : classInfo->fieldDefs.values) {
//         RenameField(field, typeAliases);
//       }
//       for (auto &method : classInfo->methodDefs.values) {
//         RenameMethod(method, typeAliases);
//       }
//     }
//     for (const auto &[_, moduleInfo] : program->moduleDefs) {
//       for (auto &field : moduleInfo->fields.values) {
//         RenameField(field, typeAliases);
//       }
//       for (auto &method : moduleInfo->methods.values) {
//         RenameMethod(method, typeAliases);
//       }
//     }
//   }
// }
// void TypeRenamePass::RenameMethod(
//     std::shared_ptr<MethodInfo>method, const Table<std::u32string, TypeAlias>
//     &typeAliases) {
//   // std::function<bool(ExpPtr)> filter = [](ExpPtr node) -> bool
//   // {
//   // 	return node->nodeType == ExpressionType::VariableDefinition ||
//   // node->nodeType == ExpressionType::New;
//   // };

//   // TreeTraverser traverser(filter);
//   // std::vector<ExpPtr> nodeList;
//   // traverser.VisitExpression(method->body, nodeList);

//   // for (auto node : nodeList)
//   // {
//   // 	if (node->nodeType == ExpressionType::VariableDefinition)
//   // 	{
//   // 		auto varDef =
//   std::static_pointer_cast<VarDefExpression>(node);
//   // 		varDef->variable->type =
//   RenameType(varDef->variable->type,
//   // typeAliases);
//   // 	}
//   // 	else
//   // 	{
//   // 		// New
//   // 		auto newExp =
//   std::static_pointer_cast<NewExpression>(node);
//   // 		newExp->type = RenameType(newExp->type, typeAliases);
//   // 	}
//   // }

//   method->selfType = RenameType(method->selfType, typeAliases);
//   method->returnType = RenameType(method->returnType, typeAliases);
//   method->signature = RenameType(method->signature, typeAliases);
//   for (auto parameter : method->parameters) {
//     parameter->type = RenameType(parameter->type, typeAliases);
//   }
//   method->body = VisitExpression(method->body, typeAliases);
// }
// void TypeRenamePass::RenameField(
//     std::shared_ptr<FieldInfo>field, const Table<std::u32string, TypeAlias>
//     &typeAliases) {
//   field->type = RenameType(field->type, typeAliases);
// }
// TypePtr TypeRenamePass::RenameType(
//     TypePtr type, const Table<std::u32string, TypeAlias> &typeAliases) {
//   if (type->typeCode == TypeCode::Unresolved) {
//     auto unresolvedType = std::static_pointer_cast<UnresolvedType>(type);
//     if (typeAliases.ContainsKey(unresolvedType->name)) {
//       auto typeAlias = typeAliases.GetValueByKey(unresolvedType->name);
//       return std::make_shared<UnresolvedType>(typeAlias.route,
//                                               typeAlias.typeName);
//     } else {
//       std::cout << "cannot resolve type: " << unresolvedType->ToString()
//                 << std::endl;
//       return type;
//     }
//   } else if (type->typeCode == TypeCode::Function) {
//     auto functionType = std::static_pointer_cast<FunctionType>(type);
//     std::vector<TypePtr> parameters;
//     std::transform(
//         functionType->parameters.begin(), functionType->parameters.end(),
//         std::back_inserter(parameters),
//         [&](TypePtr t) -> TypePtr { return RenameType(t, typeAliases); });
//     return std::make_shared<FunctionType>(
//         RenameType(functionType->selfType, typeAliases), functionType->name,
//         parameters, RenameType(functionType->returnType, typeAliases));
//   } else if (type->typeCode == TypeCode::Array) {
//     auto arrayType = std::static_pointer_cast<ArrayType>(type);
//     return std::make_shared<ArrayType>(
//         RenameType(arrayType->elementType, typeAliases));
//   } else {
//     // TO DO: GENERIC TYPE
//     return type;
//   }
// }

void InheritanceTypeResolver::VisitProject(Project &project) {
  for (const auto &[_, pkg] : project.packages) {
    for (const auto &[_, classInfo] : pkg->classes) {
      VisitClass(project, classInfo);
    }
  }
}

void InheritanceTypeResolver::VisitClass(Project &project,
                                         std::shared_ptr<ClassInfo> classInfo) {
  for (auto &superType : classInfo->superTypes) {
    if (superType->typeCode == TypeCode::Unresolved) {
      auto unresolvedType = std::static_pointer_cast<UnresolvedType>(superType);
      if (auto typeInfo = project.ResolveType(unresolvedType)) {
        superType = *typeInfo;
      } else {
        throw TypeException(
            classInfo->position,
            Format(U"missing super type '{}'", superType->ToString()));
      }
    } else {
      throw TypeException(
          classInfo->position,
          Format(U"cannot inherit type '{}'", superType->ToString()));
    }
  }
}

void InheritanceProcessor::VisitProject(Project &project) {
  for (auto &[_, program] : project.programs) {
    for (auto &[_, classInfo] : program->classDefs) {
      for (size_t i = 0; i < classInfo->superTypes.size(); i++) {
        auto superType = classInfo->superTypes.at(i);
        if (superType->typeCode == TypeCode::Class) {
          if (i != 0) {
            throw TypeException(
                classInfo->position,
                U"the super class must be at the first of the extends list");
          }
        } else if (superType->typeCode == TypeCode::Interface) {
          // pass
        } else {
          throw TypeException(
              classInfo->position,
              Format(U"missing super class '{}", superType->ToString()));
        }
      }
    }
  }
  for (auto &[_, program] : project.programs) {
    for (auto &[_, classInfo] : program->classDefs) {
      VisitClass(project, classInfo);
    }
  }
}
void InheritanceProcessor::VisitClass(Project &project,
                                      std::shared_ptr<ClassInfo> classInfo) {
  std::stack<std::shared_ptr<FieldInfo>> fieldStack;
  std::stack<std::shared_ptr<MethodInfo>> methodStack;
  std::unordered_set<FullQualifiedName> typeSet;
  std::unordered_set<std::u32string> fieldNames;
  std::unordered_set<std::u32string> methodNames;

  auto originalClass = classInfo;
  bool done = false;
  while (!done) {
    if (typeSet.count(
            FullQualifiedName(classInfo->route).Concat(classInfo->name))) {
      throw TypeException(originalClass->position,
                          U"cycle detected in the inheritance chain");
    } else {
      typeSet.insert(
          FullQualifiedName(classInfo->route).Concat(classInfo->name));
    }
    int nFields = static_cast<int>(classInfo->fieldDefs.Size());
    for (int i = nFields - 1; i >= 0; i--) {
      auto &field = classInfo->fieldDefs.values[i];
      if (fieldNames.find(field->name) != fieldNames.end()) {
        throw TypeException(originalClass->position,
                            Format(U"duplicate field definition '{}' detected "
                                   U"in the inheritance chain",
                                   field->name));
      } else {
        fieldNames.insert(field->name);
        fieldStack.push(field);
      }
    }
    int nMethods = static_cast<int>(classInfo->methodDefs.Size());
    for (int i = nMethods - 1; i >= 0; i--) {
      auto &method = classInfo->methodDefs.values[i];
      if (methodNames.find(method->name) != methodNames.end()) {
        // override support
        throw TypeException(originalClass->position,
                            Format(U"duplicate method definition '{}' detected "
                                   U"in the inheritance chain",
                                   method->name));
      } else {
        methodNames.insert(method->name);
        methodStack.push(method);
      }
    }
    if (classInfo->superTypes.size() >= 1) {
      auto type = classInfo->superTypes.front();
      if (type->typeCode == TypeCode::Class) {
        if (auto superClassInfo =
                project.GetClass(std::static_pointer_cast<ClassType>(type))) {
          classInfo = *superClassInfo;
        } else {
          throw TypeException(classInfo->position,
                              Format(U"missing super class '{}' deifinition",
                                     type->ToString()));
        }
      } else {
        done = true;
      }
    } else {
      done = true;
    }
  }
  while (!(fieldStack.empty())) {
    auto field = fieldStack.top();
    originalClass->fields.Add(field->name, field);
    fieldStack.pop();
  }
  while (!(methodStack.empty())) {
    auto method = methodStack.top();
    originalClass->methods.Add(method->name, method);
    methodStack.pop();
  }
}
ExpPtr ArrayLengthPass::VisitMemberAccess(
    std::shared_ptr<MemberAccessExpression> node) {
  auto object = VisitExpression(node->object);
  if (object->type->typeCode == TypeCode::Array) {
    if (node->field == U"Size" || node->field == U"Length") {
      auto newNode = std::make_shared<UnaryExpression>(
          node->position, ExpressionType::ArrayLength, object);
      newNode->type = Type::Int32();
      return newNode;
    } else {
      throw TypeException(
          node->position,
          Format(U"not supported array field '{}'", node->field));
    }
  } else {
    auto newNode = std::make_shared<MemberAccessExpression>(
        node->position, object, node->field);
    newNode->type = node->type;
    return newNode;
  }
}
VirtualTableGenerator::VirtualTableGenerator(Project &project)
    : project{project}, typeGraph{project.typeGraph} {}

void VirtualTableGenerator::VisitClass(std::shared_ptr<ClassInfo> classInfo) {
  // traverse the type graph to get all the super classes and interfaces
  VirtualTable &virtualTable = classInfo->virtualTable;
  auto classType =
      std::make_shared<ClassType>(classInfo->route, classInfo->name);
  auto superTypes = typeGraph.GetAllSuperTypes(classType);
  for (auto superType : superTypes) {
    if (superType->typeCode == TypeCode::Class) {
      auto superClassType = std::static_pointer_cast<ClassType>(superType);
      auto className =
          FullQualifiedName(superClassType->route).Concat(superClassType->name);
      if (auto superClassInfo = project.GetClass(superClassType)) {
        VirtualMethods methodList;
        methodList.className = className;
        for (auto &method : superClassInfo.value()->methodDefs) {
          auto methodName = FullQualifiedName(superClassInfo.value()->route)
                                .Concat(superClassInfo.value()->name)
                                .Concat(method->name);
          methodList.methodNames.push_back(methodName);
        }
        virtualTable.push_back(methodList);
      } else {
        throw TypeException(classInfo->position,
                            Format(U"missing class type '{}' for inheritance",
                                   superType->ToString()));
      }
    } else if (superType->typeCode == TypeCode::Interface) {
      auto superInterfaceType =
          std::static_pointer_cast<InterfaceType>(superType);
      if (auto superInterfaceInfo = project.GetInterface(superInterfaceType)) {
        VirtualMethods methodList;
        methodList.className = FullQualifiedName(superInterfaceType->route)
                                   .Concat(superInterfaceType->name);
        for (auto &method : superInterfaceInfo.value()->allMethods) {
          if (classInfo->methods.ContainsKey(method->name)) {
            auto classMethod = classInfo->methods.GetValueByKey(method->name);
            auto selfClassType =
                std::static_pointer_cast<ClassType>(classMethod->selfType);
            auto methodName = FullQualifiedName(selfClassType->route)
                                  .Concat(selfClassType->name)
                                  .Concat(classMethod->name);
            methodList.methodNames.push_back(methodName);
          } else {
            throw TypeException(
                classInfo->position,
                Format(U"not implemented method '{}' for interface '{}'",
                       method->name, superType->ToString()));
          }
        }
        virtualTable.push_back(methodList);
      } else {
        throw TypeException(
            classInfo->position,
            Format(U"missing interface type '{}' for inheritance",
                   superType->ToString()));
      }
    } else {
      throw TypeException(
          classInfo->position,
          Format(U"error type '{}' for inheritance", superType->ToString()));
    }
  }
}
HandleThisPointerPass::HandleThisPointerPass(Project &project)
    : project{project} {}
void HandleThisPointerPass::VisitMethod(std::shared_ptr<MethodInfo> method) {
  currentMethod = method;
  method->body = VisitExpression(method->body);
}
ExpPtr HandleThisPointerPass::VisitParameter(
    std::shared_ptr<ParameterExpression> parameter) {
  if (parameter->location->type == LocationType::ClassField ||
      parameter->location->type == LocationType::ClassMethod) {
    auto thisVar = std::make_shared<ParameterExpression>(
        parameter->position, U"this", currentMethod->selfType);
    auto newNode = std::make_shared<MemberAccessExpression>(
        parameter->position, thisVar, parameter->name);
    newNode->type = parameter->type;
    newNode->location = parameter->location;
    thisVar->location = std::make_shared<ParameterLocation>(
        static_cast<int>(currentMethod->parameters.size()));
    return newNode;
  } else {
    return parameter;
  }
}
void ModuleFieldsInitializationPass::AddFieldsInitializer(
    std::shared_ptr<ModuleInfo> moduleInfo) {
  auto moduleType =
      std::make_shared<ModuleType>(moduleInfo->route, moduleInfo->name);
  auto initializerName = U"Initializer";
  if (moduleInfo->methods.ContainsKey(initializerName)) {
    auto initializer = moduleInfo->methods.GetValueByKey(initializerName);
    if (initializer->body->nodeType == ExpressionType::Block) {
      auto body = static_pointer_cast<BlockExpression>(initializer->body);
      ExpList expressions;
      for (auto field : moduleInfo->fields) {
        auto initField = make_shared<BinaryExpression>(
            field->position, ExpressionType::Assign,
            make_shared<ParameterExpression>(field->position, field->name,
                                             field->type),
            field->value);
        expressions.push_back(initField);
      }
      auto initFieldsBlock =
          make_shared<BlockExpression>(initializer->position, expressions);
      initializer->body = make_shared<BlockExpression>(
          initializer->body->position,
          std::vector<ExpPtr>{initFieldsBlock, initializer->body});
    } else {
      throw TypeException(initializer->position,
                          U"module initializer must be a block");
    }
  } else {
    ExpList expressions;
    for (auto field : moduleInfo->fields) {
      auto initField = make_shared<BinaryExpression>(
          field->position, ExpressionType::Assign,
          make_shared<ParameterExpression>(field->position, field->name,
                                           field->type),
          field->value);
      expressions.push_back(initField);
    }
    auto body = make_shared<BlockExpression>(moduleInfo->position, expressions);
    auto initializer = std::make_shared<MethodInfo>(
        moduleInfo->position, AccessModifier::Public, true, moduleType,
        Table<std::u32string, AnnotationInfo>(), initializerName,
        std::vector<std::shared_ptr<ParameterExpression>>(), Type::Void(),
        body);
    moduleInfo->methods.Add(initializer->name, initializer);
  }
}
void ModuleFieldsInitializationPass::VisitProject(Project &project) {
  for (auto [_, pkg] : project.packages) {
    for (auto [_, moduleInfo] : pkg->modules) {
      AddFieldsInitializer(moduleInfo);
    }
  }
}
void AddVoidReturnPass::AddVoidReturn(std::shared_ptr<MethodInfo> methodInfo) {
  if (methodInfo->returnType->typeCode == TypeCode::Void) {
    auto body = methodInfo->body;
    auto returnVoid = make_shared<ReturnExpression>(
        body->position,
        make_shared<ConstantExpression>(body->position, Type::Void(), U"void"));
    methodInfo->body =
        make_shared<BlockExpression>(body->position, ExpList{body, returnVoid});
  }
}
void AddVoidReturnPass::VisitProject(Project &project) {
  for (auto [_, pkg] : project.packages) {
    for (auto [_, classInfo] : pkg->classes) {
      for (auto methodInfo : classInfo->methodDefs) {
        AddVoidReturn(methodInfo);
      }
    }
    for (auto [_, moduleInfo] : pkg->modules) {
      for (auto methodInfo : moduleInfo->methods) {
        AddVoidReturn(methodInfo);
      }
    }
  }
}
} // namespace cygni