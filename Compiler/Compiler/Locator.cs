using System;
using System.Collections.Generic;
using System.Linq;
namespace Compiler
{
    public class LocationException : Exception
    {
        public Position position;
        public LocationException(Position position, String message) : base(message)
        {
            this.position = position;
        }
    }
    public enum LocationKind
    {
        Program,
        Module,
        Class,
        Function
    }
    public class Location
    {
        public LocationKind kind;
        public int offset;
        public Location(LocationKind kind, int offset)
        {
            this.kind = kind;
            this.offset = offset;
        }

        public override string ToString()
        {
            return String.Format("[{0}, {1}]", kind, offset);
        }
    }
    public class LocationRegister
    {
        Dictionary<int, Scope> scopeMap;
        public LocationRegister(Dictionary<int, Scope> scopeMap)
        {
            this.scopeMap = scopeMap;
        }

        public void Register(Program program, Scope scope)
        {
            for (int i = 0; i < program.modules.Count; i++)
            {
                var module = program.modules[i];
                scope.Insert(module.name, "LOCATION", new Location(LocationKind.Program, i));
            }
            for (int i = 0; i < program.classes.Count; i++)
            {
                var _class = program.classes[i];
                scope.Insert(_class.name, "LOCATION", new Location(LocationKind.Program, i));
            }
            program.modules.ForEach(m => RegisterModule(m, scope));
            program.classes.ForEach(c => RegisterClass(c, scope));
        }

        void RegisterModule(DefModule module, Scope parent)
        {
            Scope scope = new Scope(parent);
            for (int i = 0; i < module.methods.Count; i++)
            {
                var function = module.methods[i];
                scope.Insert(function.name, "LOCATION", new Location(LocationKind.Module, i));
            }
            for (int i = 0; i < module.fields.Count; i++)
            {
                var variable = module.fields[i];
                scope.Insert(variable.name, "LOCATION", new Location(LocationKind.Module, i));
            }
            scopeMap.Add(module.id, scope);
        }

        void RegisterClass(DefClass _class, Scope parent)
        {
            Scope scope = new Scope(parent);
            for (int i = 0; i < _class.methods.Count; i++)
            {
                var function = _class.methods[i];
                scope.Insert(function.name, "LOCATION", new Location(LocationKind.Class, i));
            }
            for (int i = 0; i < _class.fields.Count; i++)
            {
                var variable = _class.fields[i];
                scope.Insert(variable.name, "LOCATION", new Location(LocationKind.Class, i));
            }
            scopeMap.Add(_class.id, scope);
        }
    }
    public static class AstVisitor
    {
        public static IEnumerable<Ast> Visit(Ast node)
        {
            yield return node;
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
                case Kind.And:
                case Kind.Or:
                    foreach (var item in VisitBinary((Binary)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Not:
                case Kind.UnaryPlus:
                case Kind.UnaryMinus:
                    foreach (var item in VisitUnary((Unary)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.IfThen:
                    foreach (var item in VisitIfThen((IfThen)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.IfElse:
                    foreach (var item in VisitIfElse((IfElse)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Constant:
                    break;
                case Kind.Block:
                    foreach (var item in VisitBlock((Block)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Name:
                    break;
                case Kind.Return:
                    foreach (var item in VisitReturn((Return)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Var:
                    foreach (var item in VisitVar((Var)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Def:
                    foreach (var item in VisitDef((Def)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Assign:
                    foreach (var item in VisitBinary((Binary)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.Call:
                    foreach (var item in VisitCall((Call)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.While:
                    foreach (var item in VisitCall((Call)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.DefClass:
                    foreach (var item in VisitDefClass((DefClass)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.DefModule:
                    foreach (var item in VisitDefModule((DefModule)node))
                    {
                        yield return item;
                    }
                    break;
                case Kind.TypeSpecifier:
                    foreach (var item in VisitTypeSpecifier((TypeSpecifier)node))
                    {
                        yield return item;
                    }
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        static IEnumerable<Ast> VisitBinary(Binary node)
        {
            return Visit(node.left).Concat(Visit(node.right));
        }

        static IEnumerable<Ast> VisitUnary(Unary node)
        {
            return Visit(node.operand);
        }
        static IEnumerable<Ast> VisitIfThen(IfThen node)
        {
            return Visit(node.condition).Concat(Visit(node.ifTrue));
        }
        static IEnumerable<Ast> VisitIfElse(IfElse node)
        {
            return Visit(node.condition).Concat(Visit(node.ifTrue)).Concat(Visit(node.ifFalse));
        }
        static IEnumerable<Ast> VisitBlock(Block node)
        {
            foreach (var exp in node.expressions)
            {
                foreach (var item in Visit(exp))
                {
                    yield return item;
                }
            }
        }
        static IEnumerable<Ast> VisitReturn(Return node)
        {
            return Visit(node.value);
        }
        static IEnumerable<Ast> VisitVar(Var node)
        {
            if (node.type != null)
            {
                foreach (var item in Visit(node.type))
                {
                    yield return item;
                }
            }
            if (node.value != null)
            {
                foreach (var item in Visit(node.value))
                {
                    yield return item;
                }
            }
        }
        static IEnumerable<Ast> VisitDef(Def node)
        {
            return Visit(node.type).Concat(Visit(node.body));
        }
        static IEnumerable<Ast> VisitCall(Call node)
        {
            foreach (var item in Visit(node.function))
            {
                yield return item;
            }
            foreach (var arg in node.arguments)
            {
                foreach (var item in Visit(arg))
                {
                    yield return item;
                }
            }
        }
        static IEnumerable<Ast> VisitWhile(While node)
        {
            return Visit(node.condition).Concat(Visit(node.body));
        }
        static IEnumerable<Ast> VisitDefModule(DefModule node)
        {
            foreach (var variable in node.fields)
            {
                foreach (var item in Visit(variable))
                {
                    yield return item;
                }
            }
            foreach (var function in node.methods)
            {
                foreach (var item in Visit(function))
                {
                    yield return item;
                }
            }
        }
        static IEnumerable<Ast> VisitDefClass(DefClass node)
        {
            foreach (var variable in node.fields)
            {
                foreach (var item in Visit(variable))
                {
                    yield return item;
                }
            }
            foreach (var function in node.methods)
            {
                foreach (var item in Visit(function))
                {
                    yield return item;
                }
            }
        }
        static IEnumerable<Ast> VisitTypeSpecifier(TypeSpecifier node)
        {
            foreach (var p in node.parameters)
            {
                foreach (var item in Visit(p))
                {
                    yield return item;
                }
            }
        }
    }


    public class Locator
    {
        Dictionary<int, Scope> scopeMap;
        Dictionary<int, Location> locationMap;
        Dictionary<int, List<Object>> constantPoolMap;
        public Locator(Dictionary<int, Scope> scopeMap, Dictionary<int, Location> locationMap, Dictionary<int, List<Object>> constantPoolMap)
        {
            this.scopeMap = scopeMap;
            this.locationMap = locationMap;
            this.constantPoolMap = constantPoolMap;
        }

        public void Locate(Program program, Scope parent)
        {
            foreach (var module in program.modules)
            {
                Scope scope = scopeMap[module.id].Clone();
                scope.ChangeParent(parent);
                foreach (var function in module.methods)
                {
                    LocateDef(function, scope);
                }
                var constants = LocateConstants(module, LocationKind.Module);
                constantPoolMap.Add(module.id, constants);
            }
            foreach (var _class in program.classes)
            {
                Scope scope = scopeMap[_class.id].Clone();
                scope.ChangeParent(parent);
                foreach (var function in _class.methods)
                {
                    LocateDef(function, scope);
                }
                var constants = LocateConstants(_class, LocationKind.Class);
                constantPoolMap.Add(_class.id, constants);
            }
        }

        List<Object> LocateConstants(Ast tree, LocationKind kind)
        {
            var constants = AstVisitor.Visit(tree)
            .Where(node => node.kind == Kind.Constant)
            .Cast<Constant>().ToList();
            for (int i = 0; i < constants.Count; i++)
            {
                locationMap.Add(constants[i].id, new Location(kind, i));
            }
            return constants.Select(constant => constant.value).ToList();
        }
        //void Locate(Ast node, Scope scope)
        //{
        //    switch (node.kind)
        //    {
        //        case Kind.Add:
        //        case Kind.Subtract:
        //        case Kind.Multiply:
        //        case Kind.Divide:
        //        case Kind.Modulo:
        //        case Kind.GreaterThan:
        //        case Kind.LessThan:
        //        case Kind.GreaterThanOrEqual:
        //        case Kind.LessThanOrEqual:
        //        case Kind.Equal:
        //        case Kind.NotEqual:
        //        case Kind.And:
        //        case Kind.Or:
        //            LocateBinary((Binary)node, scope);
        //            break;
        //        case Kind.Not:
        //        case Kind.UnaryPlus:
        //        case Kind.UnaryMinus:
        //            LocateUnary((Unary)node, scope);
        //            break;
        //        case Kind.IfThen:
        //            LocateIfThen((IfThen)node, scope);
        //            break;
        //        case Kind.IfElse:
        //            LocateIfElse((IfElse)node, scope);
        //            break;
        //        case Kind.Constant:
        //            LocateConstant((Constant)node, scope);
        //            break;
        //        case Kind.Block:
        //            LocateBlock((Block)node, scope);
        //            break;
        //        case Kind.Name:
        //            LocateName((Name)node, scope);
        //            break;
        //        case Kind.Return:
        //            LocateReturn((Return)node, scope);
        //            break;
        //        case Kind.Var:
        //            break;
        //        case Kind.Def:
        //            break;
        //        case Kind.Assign:
        //            LocateBinary((Binary)node, scope);
        //            break;
        //        case Kind.Call:
        //            break;
        //        case Kind.While:
        //            break;
        //        case Kind.DefClass:
        //            break;
        //        case Kind.DefModule:
        //            break;
        //        case Kind.TypeSpecifier:
        //            break;
        //        default:
        //            break;
        //    }
        //}

        //void LocateBinary(Binary node, Scope scope)
        //{
        //    Locate(node.left, scope);
        //    Locate(node.right, scope);
        //}

        //void LocateUnary(Unary node, Scope scope)
        //{
        //    Locate(node.operand, scope);
        //}

        //void LocateIfThen(IfThen node, Scope scope)
        //{
        //    Locate(node.condition, scope);
        //    Locate(node.ifTrue, scope);
        //}

        //void LocateIfElse(IfElse node, Scope scope)
        //{
        //    Locate(node.condition, scope);
        //    Locate(node.ifTrue, scope);
        //    Locate(node.ifFalse, scope);
        //}

        //void LocateConstant(Constant node, Scope scope)
        //{
        //}

        //void LocateBlock(Block node, Scope scope)
        //{
        //    foreach (var exp in node.expressions)
        //    {
        //        Locate(exp, scope);
        //    }
        //}

        void LocateName(Name node, Scope scope)
        {
            Object result = scope.Lookup(node.name, "LOCATION");
            if (result != null)
            {
                Location location = (Location)result;
                locationMap.Add(node.id, location);
            }
            else
            {
                throw new LocationException(node.position, "name not defined");
            }
        }

        //void LocateReturn(Return node, Scope scope)
        //{
        //    Locate(node.value, scope);
        //}

        void LocateVar(Var node, Scope scope)
        {
            Object result = scope.Lookup(node.name, "LOCATION");
            if (result != null)
            {
                Location location = (Location)result;
                locationMap.Add(node.id, location);
            }
            else
            {
                throw new LocationException(node.position, "name not defined");
            }
        }

        void LocateDef(Def node, Scope parent)
        {
            Scope scope = new Scope(parent);
            for (int i = 0; i < node.parameters.Count; i++)
            {
                scope.Insert(node.parameters[i].name, "LOCATION", new Location(LocationKind.Function, i));
            }

            var locals = AstVisitor.Visit(node.body)
                .Where(n => n.kind == Kind.Var)
                .Cast<Var>().ToList();

            for (int i = 0; i < locals.Count; i++)
            {
                scope.Insert(locals[i].name, "LOCATION", new Location(LocationKind.Function, node.parameters.Count + i));
            }

            foreach (var name in AstVisitor.Visit(node.body)
                .Where(n => n.kind == Kind.Name)
                .Cast<Name>())
            {
                LocateName(name, scope);
            }
            foreach (var variable in AstVisitor.Visit(node.body)
                .Where(n => n.kind == Kind.Var)
                .Cast<Var>())
            {
                LocateVar(variable, scope);
            }
        }
    }
}

