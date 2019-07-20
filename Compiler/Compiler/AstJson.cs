using System;
using System.Collections.Generic;
using System.Linq;

namespace Compiler
{
    public class AstJson
    {
        public AstJson()
        {
        }
        public JsonObject Visit(Program program)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"path", new JsonValue(program.path)},
                {"modules", new JsonArray(program.modules.Select(VisitNode).ToList()) },
                {"classes", new JsonArray(program.classes.Select(VisitNode).ToList()) },
            });
        }
        JsonObject VisitNode(Ast node)
        {
            switch (node.kind)
            {
                case Kind.Add:
                case Kind.Subtract:
                case Kind.Multiply:
                case Kind.Divide:
                case Kind.Modulo:
                case Kind.GreaterThan:
                case Kind.LessThan:
                case Kind.GreaterThanOrEqual:
                case Kind.LessThanOrEqual:
                case Kind.Equal:
                case Kind.NotEqual:
                    return VisitBinary((Binary)node);
                case Kind.And:
                    throw new NotImplementedException();
                case Kind.Or:
                    throw new NotImplementedException();
                case Kind.Not:
                    throw new NotImplementedException();
                case Kind.UnaryPlus:
                case Kind.UnaryMinus:
                    return VisitUnary((Unary)node);
                case Kind.IfThen:
                    return VisitIfThen((IfThen)node);
                case Kind.IfElse:
                    return VisitIfElse((IfElse)node);
                case Kind.Constant:
                    return VisitConstant((Constant)node);
                case Kind.Block:
                    return VisitBlock((Block)node);
                case Kind.Name:
                    return VisitName((Name)node);
                case Kind.Return:
                    return VisitReturn((Return)node);
                case Kind.Var:
                    return VisitVar((Var)node);
                case Kind.Def:
                    return VisitDef((Def)node);
                case Kind.Assign:
                    return VisitBinary((Binary)node);
                case Kind.Call:
                    return VisitCall((Call)node);
                case Kind.While:
                    return VisitWhile((While)node);
                case Kind.DefClass:
                    return VisitClass((DefClass)node);
                case Kind.DefModule:
                    return VisitModule((DefModule)node);
                case Kind.TypeSpecifier:
                    return VisitType((TypeSpecifier)node);
                case Kind.MemberAccess:
                    return VisitMemberAccess((MemberAccess)node);
                default:
                    throw new NotSupportedException();
            }
        }

        JsonObject VisitBinary(Binary node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"left", VisitNode(node.left)},
                {"right", VisitNode(node.right)},
            });
        }

        JsonObject VisitUnary(Unary node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"operand", VisitNode(node.operand)}
            });
        }

        JsonObject VisitType(TypeSpecifier node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"name", new JsonValue(node.name)},
                {"parameters", new JsonArray(node.parameters.Select(VisitType).ToList())}
            });
        }

        JsonObject VisitDef(Def node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"name", new JsonValue(node.name)},
                {"body", VisitNode(node.body)},
                {"type", VisitType(node.type)}
            });
        }

        JsonObject VisitClass(DefClass node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"name", new JsonValue(node.name)},
                {"fields", new JsonArray(node.fields.Select(VisitNode).ToList())},
                {"methods", new JsonArray(node.methods.Select(VisitNode).ToList())}
            });
        }

        JsonObject VisitModule(DefModule node)
        {
            return new JsonMap(
     new Dictionary<string, JsonObject>()
     {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"name", new JsonValue(node.name)},
                {"fields", new JsonArray(node.fields.Select(VisitNode).ToList())},
                {"methods", new JsonArray(node.methods.Select(VisitNode).ToList())}
     });
        }

        JsonObject VisitCall(Call node)
        {
            return new JsonMap(
                 new Dictionary<string, JsonObject>()
                {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"function", VisitNode(node.function)},
                {"arguments", new JsonArray(node.arguments.Select(VisitNode).ToList())}
                });
        }

        JsonObject VisitIfThen(IfThen node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                { "condition", VisitNode(node.condition) },
                { "ifTrue",VisitNode(node.ifTrue) }
            });
        }

        JsonObject VisitIfElse(IfElse node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                { "condition", VisitNode(node.condition) },
                { "ifTrue",VisitNode(node.ifTrue) },
                { "ifFalse",VisitNode(node.ifFalse) }
            });
        }

        JsonObject VisitConstant(Constant node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"type", new JsonValue(node.value.GetType().ToString())},
                {"value", new JsonValue(node.value.ToString())}
            });
        }

        JsonObject VisitBlock(Block node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"expressions",new JsonArray( node.expressions.Select(VisitNode).ToList()) }
            });
        }

        JsonObject VisitName(Name node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"name", new JsonValue(node.name)}
            });
        }

        JsonObject VisitReturn(Return node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"value", VisitNode(node.value) }
            });
        }

        JsonObject VisitVar(Var node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"type", node.type == null?new JsonValue("<NULL>"):VisitType(node.type) },
                {"value", node.value == null?new JsonValue("<NULL>"):VisitNode(node.value) }
            });
        }

        JsonObject VisitWhile(While node)
        {
            return new JsonMap(
            new Dictionary<string, JsonObject>()
            {
                {"id", new JsonValue(node.id.ToString())},
                {"kind", new JsonValue(node.kind.ToString())},
                {"condition", VisitNode(node.condition)},
                {"body", VisitNode(node.body)}
            });
        }

        JsonObject VisitMemberAccess(MemberAccess node)
        {
            return new JsonMap(
                new Dictionary<string, JsonObject>
                {
                    { "id", new JsonValue(node.id.ToString())},
                    { "kind", new JsonValue(node.kind.ToString())},
                    { "expression", VisitNode(node.expression)},
                    { "member",new JsonValue(node.member.ToString())}
                }
            );
        }
    }
}
