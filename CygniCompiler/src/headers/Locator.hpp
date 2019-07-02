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
    Vector<Ptr<Constant>> constants;
    Vector<Parameter> parameters;
    ScopeCollection() = default;
  };

  HashMap<int, Location> locations;

  void Locate(const Program& program, const ScopeCollection& scopes) {
    RegisterModules(program.modules, scopes);
    RegisterClasses(program.classes, scopes);
    for (const auto& module : program.modules) {
      Locate(module, scope);
    }
    for (const auto& _class : program.classes) {
      Locate(_class, scope);
    }
  }

  void Locate(const Ptr<Ast>& node, const ScopeCollection& scopes) {
    std::cout << KindToString(node->kind) << std::endl;
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
        break;
      case Kind::Call:
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
  void LocateBinary(const Ptr<BinaryAst>& node, const ScopeCollection& scopes) {
    Locate(node->left, scope);
    Locate(node->right, scope);
  }

  template <typename UnaryAst>
  void LocateUnary(const Ptr<UnaryAst>& node, const ScopeCollection& scopes) {
    Locate(node->operand, scope);
  }

  void LocateIfThen(const Ptr<IfThen>& node, const ScopeCollection& scopes) {
    Locate(node->condition, scope);
    Locate(node->ifTrue, scope);
  }

  void LocateIfElse(const Ptr<IfElse>& node, const ScopeCollection& scopes) {
    Locate(node->condition, scope);
    Locate(node->ifTrue, scope);
    Locate(node->ifFalse, scope);
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

  void LocateConstant(const Ptr<Constant>& node,
                      const ScopeCollection& scopes) {
    auto constants = scopes.constants;
    int index = AddNode(constants, node);
    locations.insert({node->id, {scopes.locationKind, index}});
  }

  void LocateBlock(const Ptr<Block>& node, const ScopeCollection& scopes) {
    for (const auto& exp : node->expressions) {
      Locate(exp, scopes);
    }
  }

  void LocateName(const Ptr<Name>& node, const ScopeCollection& scopes) {
    auto location = scopes.locationScope->Lookup(node->name);
    locations.insert({node->id, location});
  }

  void LocateReturn(const Ptr<Return>& node, const ScopeCollection& scopes) {
    Locate(node->value, scopes);
  }

  void LocateVar(const Ptr<Var>& node, const ScopeCollection& scopes) {
    if (node->value) {
      Locate(*(node->value), scopes);
    }
    auto variables = scopes.variables;
    int index = AddNode(variables, node);
    Location location{scopes.locationKind, index};
    locations.insert({node->id, location});
    scopes.locationScope->Put(node->name, location);
  }

  void LocateDef(const Ptr<Def>& node, const ScopeCollection& scopes) {
    auto functions = scopes.functions;
    int index = AddNode(functions, node);
    Location location{scopes.locationKind, index};
    locations.insert({node->id, location});
    scopes.locationScope->Put(node->name, location);

    auto newScope = New<Scope>(scope);
    newScope->Put("**Scope**", "**Variable**",
                  New<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>>(
                      tuple(node, New<Vector<Ptr<Var>>>())));
    newScope->Put("**Scope**", "**Constant**",
                  New<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Constant>>>>>(
                      tuple(node, New<Vector<Ptr<Constant>>>())));
    Ptr<Vector<Parameter>> parameterList = New<Vector<Parameter>>();
    for (const auto& parameter : node->parameters) {
      AddNode(parameterList, parameter);
      newScope->Put(parameter.name, "Identifier",
                    New<Location>(Location{LocationKind::Function, index}));
    }
    newScope->Put("**Scope**", "**Parameter**",
                  New<Tuple<Ptr<Ast>, Ptr<Vector<Parameter>>>>(
                      tuple(node, parameterList)));
    Locate(node->body, newScope);
  }

  void LocateModule(const Ptr<DefModule>& node, const ScopeCollection& scopes) {
    scope->Put(
        "**Scope**", "**Variable**",
        tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>(node, New<Vector<Ptr<Var>>>()));
    scope->Put(
        "**Scope**", "**Function**",
        tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>(node, New<Vector<Ptr<Def>>>()));
    for (const auto& field : node->fields) {
      Locate(field, scope);
    }
    for (const auto& method : node->methods) {
      Locate(method, scope);
    }
  }

  void LocateClass(const Ptr<DefClass>& node, const ScopeCollection& scopes) {
    scope->Put(
        "**Scope**", "Variable",
        tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>(node, New<Vector<Ptr<Var>>>()));
    scope->Put(
        "**Scope**", "Function",
        tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>(node, New<Vector<Ptr<Def>>>()));
    for (const auto& field : node->fields) {
      Locate(field, scope);
    }
    for (const auto& method : node->methods) {
      Locate(method, scope);
    }
  }

  template <typename AstElement>
  int AddNode(Ptr<Vector<AstElement>>& nodes, const AstElement& newNode) {
    int index = nodes->size();
    nodes->push_back(newNode);
    return index;
  }

  void RegisterClasses(const Vector<Ptr<DefClass>>& classes, Ptr<Scope> scope) {
    auto result = scope->Lookup("**Counter**", "**Class**");
    if (result) {
      auto nodes = Cast<Vector<Ptr<DefClass>>>(*result);
      for (const auto& _class : classes) {
        int index = AddNode(nodes, _class);
        Location location{LocationKind::Global, index};
        scope->Put(_class->name, "**Location**", New<Location>(location));
        auto newScope = New<Scope>(scope);
        newScope->Put("**Counter**", "**Variable**", New<Vector<Ptr<Var>>>());
        newScope->Put("**Counter**", "**Function**", New<Vector<Ptr<Def>>>());
        RegisterFields(_class->fields, newScope, LocationKind::Class);
        RegisterMethods(_class->methods, newScope, LocationKind::Class);
      }
    } else {
      throw KeyNotFoundException();
    }
  }

  void RegisterModules(const Vector<Ptr<DefModule>>& modules,
                       const ScopeCollection& scopes) {
    auto nodes = scopes.modules;
    for (const auto& module : modules) {
      int index = AddNode(nodes, module);
      Location location{scopes.locationKind, index};
      scopes.locationScope->Put(module.name, location);
      locations.insert({module->id, location});
      auto newLocationScope = New<Scope<Location>>(scopes.locationScope);
      ScopeCollection newScopes;
      newScopes.locationKind = LocationKind::Module;
      newScopes.locationScope = newLocationScope;
      RegisterFields(module->fields, newScope);
      RegisterMethods(module->methods, newScope);
    }
  }

  void RegisterFields(const Vector<Ptr<Var>>& fieldDefs,
                      const ScopeCollection& scopes) {
    auto nodes = scopes.variables;
    for (const auto& definition : fieldDefs) {
      int index = AddNode(nodes, definition);
      Location location{scopes.locationKind, index};
      scopes.locationScope->Put(definition->name, location);
    }
  }

  void RegisterMethods(const Vector<Ptr<Def>>& functionDefs,
                       const ScopeCollection& scopes) {
    auto nodes = scopes.functions;
    for (const auto& definition : functionDefs) {
      int index = AddNode(nodes, definition);
      Location location{scopes.locationKind, index};
      scopes.locationScope->Put(definition->name, location);
    }
  }
};

#endif  // LOCATOR_HPP
