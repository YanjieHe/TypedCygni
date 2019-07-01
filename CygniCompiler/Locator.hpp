#ifndef LOCATOR_HPP
#define LOCATOR_HPP

#include "Ast.hpp"
#include "Scope.hpp"
#include "Exception.hpp"
#include "Location.hpp"

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

template<typename T>
class NodeInfo
{
public:
    Ptr<Ast> node;
    Vector<T> values;

    NodeInfo()
    {

    }

    NodeInfo(Ptr<Ast> node, Vector<T> values)
            : node{node}, values{values}
    {

    }
};

class Locator
{
public:
    HashMap<int, Location> locations;

    void Locate(const Program &program, const Ptr<Scope> &scope)
    {
        scope->Put("**Counter**", "**Module**", New<Vector<Ptr<DefModule>>>());
        scope->Put("**Counter**", "**Class**", New<Vector<Ptr<DefClass>>>());
        RegisterModules(program.modules, scope);
        RegisterClasses(program.classes, scope);
        for (const auto &module:program.modules)
        {
            Locate(module, scope);
        }
        for (const auto &_class: program.classes)
        {
            Locate(_class, scope);
        }
    }

    void Locate(const Ptr<Ast> &node, const Ptr<Scope> &scope)
    {
        std::cout << KindToString(node->kind) << std::endl;
        switch (node->kind)
        {
            case Kind::Add:
                LocateBinary(Cast<Add>(node), scope);
                break;
            case Kind::Subtract:
                LocateBinary(Cast<Subtract>(node), scope);
                break;
            case Kind::Multiply:
                LocateBinary(Cast<Multiply>(node), scope);
                break;
            case Kind::Divide:
                LocateBinary(Cast<Divide>(node), scope);
                break;
            case Kind::Modulo:
                LocateBinary(Cast<Modulo>(node), scope);
                break;
            case Kind::GreaterThan:
                LocateBinary(Cast<GreaterThan>(node), scope);
                break;
            case Kind::LessThan:
                LocateBinary(Cast<LessThan>(node), scope);
                break;
            case Kind::GreaterThanOrEqual:
                LocateBinary(Cast<GreaterThanOrEqual>(node), scope);
                break;
            case Kind::LessThanOrEqual:
                LocateBinary(Cast<LessThanOrEqual>(node), scope);
                break;
            case Kind::Equal:
                LocateBinary(Cast<Equal>(node), scope);
                break;
            case Kind::NotEqual:
                LocateBinary(Cast<NotEqual>(node), scope);
                break;
            case Kind::And:
                LocateBinary(Cast<And>(node), scope);
                break;
            case Kind::Or:
                LocateBinary(Cast<Or>(node), scope);
                break;
            case Kind::Not:
                LocateUnary(Cast<Not>(node), scope);
                break;
            case Kind::UnaryPlus:
                LocateUnary(Cast<UnaryPlus>(node), scope);
                break;
            case Kind::UnaryMinus:
                LocateUnary(Cast<UnaryMinus>(node), scope);
                break;
            case Kind::IfThen:
                LocateIfThen(Cast<IfThen>(node), scope);
                break;
            case Kind::IfElse:
                LocateIfElse(Cast<IfElse>(node), scope);
                break;
            case Kind::Constant:
                LocateConstant(Cast<Constant>(node), scope);
                break;
            case Kind::Block:
                LocateBlock(Cast<Block>(node), scope);
                break;
            case Kind::Name:
                LocateName(Cast<Name>(node), scope);
                break;
            case Kind::Return:
                LocateReturn(Cast<Return>(node), scope);
                break;
            case Kind::Var:
                LocateVar(Cast<Var>(node), scope);
                break;
            case Kind::Def:
                LocateDef(Cast<Def>(node), scope);
                break;
            case Kind::Assign:
                break;
            case Kind::Call:
                break;
            case Kind::While:
                break;
            case Kind::DefClass:
                LocateClass(Cast<DefClass>(node), scope);
                break;
            case Kind::DefModule:
                LocateModule(Cast<DefModule>(node), scope);
                break;
            case Kind::TypeExpr:
                throw NotImplementedException();
        }
    }

    template<typename BinaryAst>
    void LocateBinary(const Ptr<BinaryAst> &node, const Ptr<Scope> &scope)
    {
        Locate(node->left, scope);
        Locate(node->right, scope);
    }

    template<typename UnaryAst>
    void LocateUnary(const Ptr<UnaryAst> &node, const Ptr<Scope> &scope)
    {
        Locate(node->operand, scope);
    }

    void LocateIfThen(const Ptr<IfThen> &node, const Ptr<Scope> &scope)
    {
        Locate(node->condition, scope);
        Locate(node->ifTrue, scope);
    }

    void LocateIfElse(const Ptr<IfElse> &node, const Ptr<Scope> &scope)
    {
        Locate(node->condition, scope);
        Locate(node->ifTrue, scope);
        Locate(node->ifFalse, scope);
    }

    static LocationKind AstKindToLocationKind(Kind kind)
    {
        switch (kind)
        {
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

    void LocateConstant(const Ptr<Constant> &node, const Ptr<Scope> &scope)
    {
        auto result = scope->Lookup("**Scope**", "**Constant**");
        if (result)
        {
            Ptr<Vector<Ptr<Constant>>> constants;
            Ptr<Ast> scopeNode;
            tie(scopeNode, constants) = *Cast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Constant>>>>>(*result);
            int index = AddNode(constants, node);
            locations.insert({node->id, {AstKindToLocationKind(scopeNode->kind), index}});
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateBlock(const Ptr<Block> &node, const Ptr<Scope> &scope)
    {
        for (const auto &exp: node->expressions)
        {
            Locate(exp, scope);
        }
    }

    void LocateName(const Ptr<Name> &node, const Ptr<Scope> &scope)
    {
        auto result = scope->Lookup(node->name, "**Location**");
        if (result)
        {
            Ptr<Location> location = Cast<Location>(*result);
            locations.insert({node->id, *location});
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateReturn(const Ptr<Return> &node, const Ptr<Scope> &scope)
    {
        Locate(node->value, scope);
    }

    void LocateVar(const Ptr<Var> &node, const Ptr<Scope> &scope)
    {
        if (node->value)
        {
            Locate(*(node->value), scope);
        }
        auto result = scope->Lookup("**Scope**", "**Variable**");
        if (result)
        {
            Ptr<Vector<Ptr<Var>>> variables;
            Ptr<Ast> scopeNode;
            tie(scopeNode, variables) = *Cast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>>(*result);
            int index = AddNode(variables, node);
            Location location{AstKindToLocationKind(scopeNode->kind), index};
            locations.insert({node->id, location});
            scope->Put(node->name, "Identifier", New<Location>(location));
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateDef(const Ptr<Def> &node, const Ptr<Scope> &scope)
    {
        auto result = scope->Lookup("**Scope**", "**Function**");
        if (result)
        {
            Ptr<Vector<Ptr<Def>>> functions;
            Ptr<Ast> scopeNode;
            tie(scopeNode, functions) = *Cast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>>(*result);
            int index = AddNode(functions, node);
            Location location{AstKindToLocationKind(scopeNode->kind), index};
            locations.insert({node->id, location});
            scope->Put(node->name, "Identifier", New<Location>(location));

            auto newScope = New<Scope>(scope);
            newScope->Put("**Scope**", "**Variable**",
                          New<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>>(tuple(node, New<Vector<Ptr<Var>>>())));
            newScope->Put("**Scope**", "**Constant**",
                          New<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Constant>>>>>(tuple(node, New<Vector<Ptr<Constant>>>())));
            Ptr<Vector<Parameter>> parameterList = New<Vector<Parameter>>();
            for (const auto &parameter: node->parameters)
            {
                AddNode(parameterList, parameter);
                newScope->Put(parameter.name, "Identifier", New<Location>(Location{LocationKind::Function, index}));
            }
            newScope->Put("**Scope**", "**Parameter**",
                          New<Tuple<Ptr<Ast>, Ptr<Vector<Parameter>>>>(tuple(node, parameterList)));
            Locate(node->body, newScope);
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateModule(const Ptr<DefModule> &node, const Ptr<Scope> &scope)
    {
        scope->Put("**Scope**", "**Variable**", tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>(node, New<Vector<Ptr<Var>>>()));
        scope->Put("**Scope**", "**Function**", tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>(node, New<Vector<Ptr<Def>>>()));
        for (const auto &field: node->fields)
        {
            Locate(field, scope);
        }
        for (const auto &method: node->methods)
        {
            Locate(method, scope);
        }
    }

    void LocateClass(const Ptr<DefClass> &node, const Ptr<Scope> &scope)
    {
        scope->Put("**Scope**", "Variable", tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>(node, New<Vector<Ptr<Var>>>()));
        scope->Put("**Scope**", "Function", tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>(node, New<Vector<Ptr<Def>>>()));
        for (const auto &field: node->fields)
        {
            Locate(field, scope);
        }
        for (const auto &method: node->methods)
        {
            Locate(method, scope);
        }
    }

    template<typename AstElement>
    int AddNode(Ptr<Vector<AstElement>> &nodes, const AstElement &newNode)
    {
        int index = nodes->size();
        nodes->push_back(newNode);
        return index;
    }

    void RegisterClasses(const Vector<Ptr<DefClass>> &classes, Ptr<Scope> scope)
    {
        auto result = scope->Lookup("**Counter**", "**Class**");
        if (result)
        {
            auto nodes = Cast<Vector<Ptr<DefClass>>>(*result);
            for (const auto &_class: classes)
            {
                int index = AddNode(nodes, _class);
                Location location{LocationKind::Global, index};
                scope->Put(_class->name, "**Location**", New<Location>(location));
                auto newScope = New<Scope>(scope);
                newScope->Put("**Counter**", "**Variable**", New<Vector<Ptr<Var>>>());
                newScope->Put("**Counter**", "**Function**", New<Vector<Ptr<Def>>>());
                RegisterFields(_class->fields, newScope, LocationKind::Class);
                RegisterMethods(_class->methods, newScope, LocationKind::Class);
            }
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void RegisterModules(const Vector<Ptr<DefModule>> &modules, const Ptr<Scope> &scope)
    {
        auto result = scope->Lookup("**Counter**", "**Module**");
        if (result)
        {
            auto nodes = Cast<Vector<Ptr<DefModule>>>(*result);
            for (const auto &module: modules)
            {
                int index = AddNode(nodes, module);
                Location location{LocationKind::Global, index};
                scope->Put(module->name, "**Location**", New<Location>(location));
                locations.insert({module->id, location});
                auto newScope = New<Scope>(scope);
                newScope->Put("**Counter**", "**Variable**", New<Vector<Ptr<Var>>>());
                newScope->Put("**Counter**", "**Function**", New<Vector<Ptr<Def>>>());
                RegisterFields(module->fields, newScope, LocationKind::Module);
                RegisterMethods(module->methods, newScope, LocationKind::Module);
            }
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void RegisterFields(const Vector<Ptr<Var>> &fieldDefs, const Ptr<Scope> &scope, LocationKind locationKind)
    {
        auto result = scope->Lookup("**Counter**", "**Variable**");
        if (result)
        {
            auto nodes = Cast<Vector<Ptr<Var>>>(*result);
            for (const auto &definition: fieldDefs)
            {
                int index = AddNode(nodes, definition);
                Location location{locationKind, index};
                scope->Put(definition->name, "**Location**", New<Location>(location));
            }
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void RegisterMethods(const Vector<Ptr<Def>> &functionDefs, const Ptr<Scope> &scope, LocationKind locationKind)
    {
        auto result = scope->Lookup("Counter", "FunctionDefinitions");
        if (result)
        {
            auto nodes = Cast<Vector<Ptr<Def>>>(*result);
            for (const auto &definition: functionDefs)
            {
                int index = AddNode(nodes, definition);
                Location location{locationKind, index};
                scope->Put(definition->name, "**Location**", New<Location>(location));
            }
        }
        else
        {
            throw KeyNotFoundException();
        }
    }
};


#endif //LOCATOR_HPP
