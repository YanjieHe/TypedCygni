#ifndef LOCATOR_HPP
#define LOCATOR_HPP

#include "Ast.hpp"
#include "Scope.hpp"

/*
 * Locator:
 *     **Location**: Ptr<Location>
 *     **Counter**, **Module**: Ptr<Vector<Ptr<DefModule>>>
 *     **Counter**, **Class**: Ptr<Vector<Ptr<DefClass>>>
 *     **Counter**, **Variable**: Ptr<Vector<Ptr<Var>>>
 *     **Counter**, **Function**: Ptr<Vector<Ptr<Def>>>
 *     **Scope**, **Constant**: Ptr<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Constant>>>>>
 *     **Scope**, **Parameter**: Ptr<Tuple<Ptr<Ast>, Ptr<Vector<Parameter>>>>
 */

class Locator {
 public:
  class ScopeCollection {
   public:
    LocationKind locationKind;
    Ptr<Scope<Location>> locationScope;
    Vector<Ptr<DefModule>> modules;
    Vector<Ptr<DefClass>> classes;
    Vector<Ptr<Var>> variables;
    Vector<Ptr<Def>> functions;
    Vector<Parameter> parameters;
    ScopeCollection()
        : locationKind{LocationKind::Global},
          locationScope(New<Scope<Location>>()) {}
    ScopeCollection(const ScopeCollection& parent, LocationKind locationKind)
        : locationKind{locationKind},
          locationScope(New<Scope<Location>>(parent.locationScope)) {}
  };

  class ConstantPool {
   public:
    Vector<Ptr<Constant>> constants;
    HashMap<int, int> constantNodes;
    ConstantPool();
    void Assign(Ptr<Constant> node) {
      int index = constants.size();
      constantNodes.insert({node->id, index});
      constants.push_back(node);
    }
  };

  ConstantPool constantPool;
  HashMap<int, Location> locations;
  void Locate(const Program& program, ScopeCollection& scopes) {
    RegisterModules(program.modules, scopes);
    RegisterClasses(program.classes, scopes);
    for (const auto& module : program.modules) {
      Locate(module, scopes);
    }
    for (const auto& _class : program.classes) {
      Locate(_class, scopes);
    }
  }

  void Locate(const Ptr<Ast>& node, ScopeCollection& scopes) {
    // std::cout << KindToString(node->kind) << std::endl;
    switch (node->kind) {
      case Kind::Add:
        LocateBinary(Cast<Add>(node), scopes);
        break;
      case Kind::Subtract:
        LocateBinary(Cast<Subtract>(node), scopes);
        break;
      case Kind::Multiply:
        LocateBinary(Cast<Multiply>(node), scopes);
        break;
      case Kind::Divide:
        LocateBinary(Cast<Divide>(node), scopes);
        break;
      case Kind::Modulo:
        LocateBinary(Cast<Modulo>(node), scopes);
        break;
      case Kind::GreaterThan:
        LocateBinary(Cast<GreaterThan>(node), scopes);
        break;
      case Kind::LessThan:
        LocateBinary(Cast<LessThan>(node), scopes);
        break;
      case Kind::GreaterThanOrEqual:
        LocateBinary(Cast<GreaterThanOrEqual>(node), scopes);
        break;
      case Kind::LessThanOrEqual:
        LocateBinary(Cast<LessThanOrEqual>(node), scopes);
        break;
      case Kind::Equal:
        LocateBinary(Cast<Equal>(node), scopes);
        break;
      case Kind::NotEqual:
        LocateBinary(Cast<NotEqual>(node), scopes);
        break;
      case Kind::And:
        LocateBinary(Cast<And>(node), scopes);
        break;
      case Kind::Or:
        LocateBinary(Cast<Or>(node), scopes);
        break;
      case Kind::Not:
        LocateUnary(Cast<Not>(node), scopes);
        break;
      case Kind::UnaryPlus:
        LocateUnary(Cast<UnaryPlus>(node), scopes);
        break;
      case Kind::UnaryMinus:
        LocateUnary(Cast<UnaryMinus>(node), scopes);
        break;
      case Kind::IfThen:
        LocateIfThen(Cast<IfThen>(node), scopes);
        break;
      case Kind::IfElse:
        LocateIfElse(Cast<IfElse>(node), scopes);
        break;
      case Kind::Constant:
        LocateConstant(Cast<Constant>(node), scopes);
        break;
      case Kind::Block:
        LocateBlock(Cast<Block>(node), scopes);
        break;
      case Kind::Name:
        LocateName(Cast<Name>(node), scopes);
        break;
      case Kind::Return:
        LocateReturn(Cast<Return>(node), scopes);
        break;
      case Kind::Var:
        LocateVar(Cast<Var>(node), scopes);
        break;
      case Kind::Def:
        LocateDef(Cast<Def>(node), scopes);
        break;
      case Kind::Assign:
        LocateAssign(Cast<Assign>(node), scopes);
        break;
      case Kind::Call:
        LocateCall(Cast<Call>(node), scopes);
        break;
      case Kind::While:
        break;
      case Kind::DefClass:
        LocateClass(Cast<DefClass>(node), scopes);
        break;
      case Kind::DefModule:
        LocateModule(Cast<DefModule>(node), scopes);
        break;
      case Kind::TypeExpr:
        throw NotImplementedException();
    }
  }

  template <typename BinaryAst>
  void LocateBinary(const Ptr<BinaryAst>& node, ScopeCollection& scopes) {
    Locate(node->left, scopes);
    Locate(node->right, scopes);
  }

  template <typename UnaryAst>
  void LocateUnary(const Ptr<UnaryAst>& node, ScopeCollection& scopes) {
    Locate(node->operand, scopes);
  }

  void LocateIfThen(const Ptr<IfThen>& node, ScopeCollection& scopes) {
    Locate(node->condition, scopes);
    Locate(node->ifTrue, scopes);
  }

  void LocateIfElse(const Ptr<IfElse>& node, ScopeCollection& scopes) {
    Locate(node->condition, scopes);
    Locate(node->ifTrue, scopes);
    Locate(node->ifFalse, scopes);
  }

  void LocateCall(const Ptr<Call>& node, ScopeCollection& scopes) {
    Locate(node->function, scopes);
    for (const auto& argument : node->arguments) {
      Locate(argument, scopes);
    }
  }

  static LocationKind AstKindToLocationKind(Kind kind) {
    switch (kind) {
      case Kind::Def:
        return LocationKind::Function;
      case Kind::DefClass:
        return LocationKind::Class;
      case Kind::DefModule:
        return LocationKind::Module;
      default:
        throw NotImplementedException();
    }
  }

  void LocateConstant(const Ptr<Constant>& node, ScopeCollection& scopes) {
    constantPool.Assign(node);
  }

  void LocateBlock(const Ptr<Block>& node, ScopeCollection& scopes) {
    for (const auto& exp : node->expressions) {
      Locate(exp, scopes);
    }
  }

  void LocateName(const Ptr<Name>& node, ScopeCollection& scopes) {
    auto location = scopes.locationScope->Lookup(node->name);
    if (location) {
      locations.insert({node->id, *location});  // TO DO: check
    } else {
      throw KeyNotFoundException();
    }
  }

  void LocateReturn(const Ptr<Return>& node, ScopeCollection& scopes) {
    Locate(node->value, scopes);
  }

  void LocateVar(const Ptr<Var>& node, ScopeCollection& scopes) {
    if (node->value) {
      Locate(*(node->value), scopes);
    }
    auto& variables = scopes.variables;
    int index = AddNode(variables, node);
    Location location{scopes.locationKind, index};
    locations.insert({node->id, location});
    scopes.locationScope->Put(node->name, location);
  }

  void LocateAssign(const Ptr<Assign>& node, ScopeCollection& scopes) {
    Locate(node->left, scopes);
    Locate(node->right, scopes);
  }

  void LocateDef(const Ptr<Def>& node, ScopeCollection& scopes) {
    auto& functions = scopes.functions;
    int index = AddNode(functions, node);
    Location location{scopes.locationKind, index};
    locations.insert({node->id, location});
    scopes.locationScope->Put(node->name, location);

    ScopeCollection newScopes(scopes, LocationKind::Function);
    for (const auto& parameter : node->parameters) {
      int parameterIndex = AddNode(newScopes.parameters, parameter);
      newScopes.locationScope->Put(
          parameter.name, Location{LocationKind::Function, parameterIndex});
    }
    Locate(node->body, newScopes);
  }

  void LocateModule(const Ptr<DefModule>& node, ScopeCollection& scopes) {
    ScopeCollection newScopes(scopes, LocationKind::Module);
    for (const auto& field : node->fields) {
      Locate(field, newScopes);
    }
    for (const auto& method : node->methods) {
      Locate(method, newScopes);
    }
  }

  void LocateClass(const Ptr<DefClass>& node, ScopeCollection& scopes) {
    ScopeCollection newScopes(scopes, LocationKind::Class);
    for (const auto& field : node->fields) {
      Locate(field, newScopes);
    }
    for (const auto& method : node->methods) {
      Locate(method, newScopes);
    }
  }

  template <typename AstElement>
  int AddNode(Vector<AstElement>& nodes, const AstElement& newNode) {
    int index = nodes.size();
    nodes.push_back(newNode);
    return index;
  }

  void RegisterClasses(const Vector<Ptr<DefClass>>& classes,
                       ScopeCollection& scopes) {
    auto& nodes = scopes.classes;
    for (const auto& _class : classes) {
      int index = AddNode(nodes, _class);
      Location location{scopes.locationKind, index};
      scopes.locationScope->Put(_class->name, location);
      ScopeCollection newScopes(scopes, LocationKind::Class);
      RegisterFields(_class->fields, newScopes);
      RegisterMethods(_class->methods, newScopes);
    }
  }

  void RegisterModules(const Vector<Ptr<DefModule>>& modules,
                       ScopeCollection& scopes) {
    auto& nodes = scopes.modules;
    for (const auto& module : modules) {
      int index = AddNode(nodes, module);
      Location location{scopes.locationKind, index};
      scopes.locationScope->Put(module->name, location);
      locations.insert({module->id, location});
      ScopeCollection newScopes(scopes, LocationKind::Module);

      RegisterFields(module->fields, newScopes);
      RegisterMethods(module->methods, newScopes);
    }
  }

  void RegisterFields(const Vector<Ptr<Var>>& fieldDefs,
                      ScopeCollection& scopes) {
    auto& nodes = scopes.variables;
    for (const auto& definition : fieldDefs) {
      int index = AddNode(nodes, definition);
      Location location{scopes.locationKind, index};
      locations.insert({definition->id, location});
      scopes.locationScope->Put(definition->name, location);
    }
  }

  void RegisterMethods(const Vector<Ptr<Def>>& functionDefs,
                       ScopeCollection& scopes) {
    auto& nodes = scopes.functions;
    for (const auto& definition : functionDefs) {
      int index = AddNode(nodes, definition);
      Location location{scopes.locationKind, index};
      locations.insert({definition->id, location});
      scopes.locationScope->Put(definition->name, location);
    }
  }
};

#endif  // LOCATOR_HPP
