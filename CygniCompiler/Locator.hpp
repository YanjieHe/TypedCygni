#ifndef LOCATOR_HPP
#define LOCATOR_HPP

#include "Ast.hpp"
#include "Scope.hpp"
#include "Exception.hpp"

enum class LocationKind
{
    Global,
    Class,
    Module,
    Function
};

class Location
{
public:
    LocationKind locationKind;
    int index;

    Location() = default;

    Location(LocationKind locationKind, int index)
            : locationKind{locationKind}, index{index}
    {

    }

    String ToString() const
    {
        switch (locationKind)
        {
            case LocationKind::Global:
                return String("Global, ") + String::ToString(index);
            case LocationKind::Class:
                return String("Class, ") + String::ToString(index);
            case LocationKind::Module:
                return String("Module, ") + String::ToString(index);
            case LocationKind::Function:
                return String("Function, ") + String::ToString(index);
            default:
                throw NotImplementedException();
        }
    }
};

class Locator
{
public:
    HashMap<int, Location> locations;

    void Locate(const Program &program, const Ptr<Scope> &scope)
    {
        scope->Put("Counter", "Module", New<Vector<Ptr<DefModule>>>());
        scope->Put("Counter", "Class", New<Vector<Ptr<DefClass>>>());
        std::cout << "register modules" << std::endl;
        RegisterModules(program.modules, scope);
        std::cout << "register classes" << std::endl;
        RegisterClasses(program.classes, scope);
        std::cout << "locate modules" << std::endl;
        for (const auto &module:program.modules)
        {
            Locate(module, scope);
        }
        std::cout << "locate classes" << std::endl;
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
            default:
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
        Optional<Any> result = scope->Lookup("**Scope**", "Constant");
        if (result)
        {
            Ptr<Vector<Ptr<Constant>>> constants;
            Ptr<Ast> scopeNode;
            tie(scopeNode, constants) = (*result).AnyCast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Constant>>>>>();
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
        Optional<Any> result = scope->Lookup(node->name, "Identifier");
        if (result)
        {
            Location location = (*result).AnyCast<Location>();
            locations.insert({node->id, location});
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
        Optional<Any> result = scope->Lookup("**Scope**", "Variable");
        if (result)
        {
            Ptr<Vector<Ptr<Var>>> variables;
            Ptr<Ast> scopeNode;
            tie(scopeNode, variables) = (*result).AnyCast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>>();
            int index = AddNode(variables, node);
            Location location{AstKindToLocationKind(scopeNode->kind), index};
            locations.insert({node->id, location});
            scope->Put(node->name, "Identifier", location);
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateDef(const Ptr<Def> &node, const Ptr<Scope> &scope)
    {
        Optional<Any> result = scope->Lookup("**Scope**", "Function");
        if (result)
        {
            Ptr<Vector<Ptr<Def>>> functions;
            Ptr<Ast> scopeNode;
            tie(scopeNode, functions) = (*result).AnyCast<Tuple<Ptr<Ast>, Ptr<Vector<Ptr<Def>>>>>();
            int index = AddNode(functions, node);
            Location location{AstKindToLocationKind(scopeNode->kind), index};
            locations.insert({node->id, location});
            scope->Put(node->name, "Identifier", location);

            auto newScope = New<Scope>(scope);
            newScope->Put("**Scope**", "Variable", tuple<Ptr<Ast>, Ptr<Vector<Ptr<Var>>>>(node, {}));
            Ptr<Vector<Parameter>> parameterList = New<Vector<Parameter>>();
            for (const auto &parameter: node->parameters)
            {
                AddNode(parameterList, parameter);
                newScope->Put(parameter.name, "Identifier", Location{LocationKind::Function, index});
            }
            newScope->Put("**Scope**", "Parameter", tuple(node, parameterList));
            Locate(node->body, newScope);
        }
        else
        {
            throw KeyNotFoundException();
        }
    }

    void LocateModule(const Ptr<DefModule> &node, const Ptr<Scope> &scope)
    {
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
        auto result = scope->Lookup("Counter", "Class");
        if (result)
        {
            auto &nodes = (*result).AnyCast<Ptr<Vector<Ptr<DefClass>>>>();
            for (const auto &_class: classes)
            {
                int index = AddNode(nodes, _class);
                Location location{LocationKind::Global, index};
                scope->Put(_class->name, "Identifier", location);
                auto newScope = New<Scope>(scope);
                newScope->Put("Counter", "FieldDefinitions", New<Vector<Ptr<Var>>>());
                newScope->Put("Counter", "FunctionDefinitions", New<Vector<Ptr<Def>>>());
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
        auto result = scope->Lookup("Counter", "Module");
        if (result)
        {
            std::cout << "successfully found module" << std::endl;
            auto nodes = (*result).AnyCast<Ptr<Vector<Ptr<DefModule>>>>();
            std::cout << "register modules" << std::endl;
            for (const auto &module: modules)
            {
                int index = AddNode(nodes, module);
                std::cout << "index = " << index << std::endl;
                Location location{LocationKind::Global, index};
                scope->Put(module->name, "Identifier", location);
                std::cout << "module name = " << module->name << std::endl;
                locations.insert({module->id, location});
                std::cout << "record location: " << location.ToString() << std::endl;
                auto newScope = New<Scope>(scope);
                newScope->Put("Counter", "FieldDefinitions", New<Vector<Ptr<Var>>>());
                newScope->Put("Counter", "FunctionDefinitions", New<Vector<Ptr<Def>>>());
                std::cout << "register fields" << std::endl;
                RegisterFields(module->fields, newScope, LocationKind::Module);
                std::cout << "register methods" << std::endl;
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
        auto result = scope->Lookup("Counter", "FieldDefinitions");
        if (result)
        {
            auto &nodes = (*result).AnyCast<Ptr<Vector<Ptr<Var>>>>();
            for (const auto &definition: fieldDefs)
            {
                int index = AddNode(nodes, definition);
                Location location{locationKind, index};
                scope->Put(definition->name, "Identifier", location);
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
            auto &nodes = (*result).AnyCast<Ptr<Vector<Ptr<Def>>>>();
            for (const auto &definition: functionDefs)
            {
                int index = AddNode(nodes, definition);
                Location location{locationKind, index};
                scope->Put(definition->name, "Identifier", location);
            }
        }
        else
        {
            throw KeyNotFoundException();
        }
    }
};


#endif //LOCATOR_HPP
