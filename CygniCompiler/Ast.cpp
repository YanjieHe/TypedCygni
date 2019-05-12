#include "Ast.hpp"

Position::Position(int startLine, int startColumn, int endLine, int endColumn)
        : startLine{startLine}, startColumn{startColumn}, endLine{endLine}, endColumn{endColumn}
{

}

String Position::ToString()
{
    return String::ToString(startLine) + ", " + String::ToString(startColumn) + " -> " + String::ToString(endLine) +
           ", " + String::ToString(endColumn);
}

Ast::Ast(Kind kind, Position position)
        : kind{kind}, position{position}
{
    static int currentId = 0;
    currentId++;
    this->id = currentId;
}

IfThen::IfThen(Position position, Ptr<Ast> condition, Ptr<Ast> ifTrue)
        : Ast(Kind::IfThen, position), condition{condition}, ifTrue{ifTrue}
{

}

IfElse::IfElse(Position position, Ptr<Ast> condition, Ptr<Ast> ifTrue, Ptr<Ast> ifFalse)
        : Ast(Kind::IfElse, position), condition{condition}, ifTrue{ifTrue}, ifFalse{ifFalse}
{

}

Constant::Constant(Position position, Constant::ConstantType constantType,
                   String value)
        : Ast(Kind::Constant, position), constantType{constantType}, value{std::move(value)}
{

}

Block::Block(Position position, Vector<Ptr<Ast>> expressions)
        : Ast(Kind::Block, position), expressions{expressions}
{

}

Name::Name(Position position, String name)
        : Ast(Kind::Name, position), name{std::move(name)}
{

}

Return::Return(Position position, Ptr<Ast> value)
        : Ast(Kind::Return, position), value{value}
{

}

While::While(Position position, Ptr<Ast> condition, Ptr<Ast> body)
        : Ast(Kind::While, position), condition{condition}, body{body}
{

}

Call::Call(Position position, Ptr<Ast> function, Vector<Ptr<Ast>> arguments)
        : Ast(Kind::Call, position), function{function}, arguments{arguments}
{

}

Var::Var(Position position, String name, Optional<Ptr<Type>> type, Optional<Ptr<Ast>> value)
        : Ast(Kind::Var, position), name{std::move(name)}, type{std::move(type)}, value{std::move(value)},
          access{Access::Local}
{

}

Parameter::Parameter(String name, Ptr<Type> type)
        : name{name}, type{type}
{

}

Def::Def(Position position, String name, Vector<Parameter> parameters, Ptr<Type> type, Ptr<Ast> body)
        : Ast(Kind::Def, position), name{name}, parameters{parameters}, type{type}, body{body},
          access{Access::Local}
{

}

Ptr<Type> Def::MakeFunctionType(Vector<Parameter> parameters, Ptr<Type> returnType)
{
    Vector<Ptr<Type>> types;
    types.resize(parameters.size() + 1);
    std::transform(parameters.begin(), parameters.end(),
                   types.begin(), [](const Parameter &p)
                   {
                       return p.type;
                   });
    types.back() = std::move(returnType);
    return New<TypeList>("Function", types);
}

DefClass::DefClass(Position position, String name, Vector<Ptr<Var>> fields, Vector<Ptr<Def>> methods)
        : Ast(Kind::DefClass, position), name{name}, fields{fields}, methods{methods}
{

}

Program::Program(String path, Vector<Ptr<DefClass>> classes, Vector<Ptr<DefModule>> modules)
        : path{path}, classes{classes}, modules{modules}
{

}

String KindToString(Kind kind)
{
    switch (kind)
    {
        case Kind::Add:
            return "Add";
        case Kind::Subtract:
            return "Subtract";
        case Kind::Multiply:
            return "Multiply";
        case Kind::Divide:
            return "Divide";
        case Kind::Modulo:
            return "Modulo";
        case Kind::GreaterThan:
            return "GreaterThan";
        case Kind::LessThan:
            return "LessThan";
        case Kind::GreaterThanOrEqual:
            return "GreaterThanOrEqual";
        case Kind::LessThanOrEqual:
            return "LessThanOrEqual";
        case Kind::Equal:
            return "Equal";
        case Kind::NotEqual:
            return "NotEqual";
        case Kind::And:
            return "And";
        case Kind::Or:
            return "Or";
        case Kind::Not:
            return "Not";
        case Kind::UnaryPlus:
            return "UnaryPlus";
        case Kind::UnaryMinus:
            return "UnaryMinus";
        case Kind::IfThen:
            return "IfThen";
        case Kind::IfElse:
            return "IfElse";
        case Kind::Constant:
            return "Constant";
        case Kind::Block:
            return "Block";
        case Kind::Name:
            return "Name";
        case Kind::Return:
            return "Return";
        case Kind::Var:
            return "Var";
        case Kind::Def:
            return "Def";
        case Kind::Assign:
            return "Assign";
        case Kind::Call:
            return "Call";
        case Kind::While:
            return "While";
        case Kind::DefModule:
            return "DefModule";
        default:
        case Kind::DefClass:
            return "DefClass";
    }
}

DefModule::DefModule(Position position, String name, Vector<Ptr<Var>> fields, Vector<Ptr<Def>> methods)
        : Ast(Kind::DefModule, position), name{name}, fields{fields}, methods{methods}
{

}