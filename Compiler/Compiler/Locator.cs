using System;
using System.Collections.Generic;

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

    public class Locator
    {
        Dictionary<int, Location> locationMap;
        public Locator(Dictionary<int, Location> locationMap)
        {
            this.locationMap = locationMap;
        }

        void Locate(Ast node, Scope scope)
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
                case Kind.And:
                case Kind.Or:
                    LocateBinary((Binary)node, scope);
                    break;
                case Kind.Not:
                case Kind.UnaryPlus:
                case Kind.UnaryMinus:
                    LocateUnary((Unary)node, scope);
                    break;
                case Kind.IfThen:
                    LocateIfThen((IfThen)node, scope);
                    break;
                case Kind.IfElse:
                    LocateIfElse((IfElse)node, scope);
                    break;
                case Kind.Constant:
                    LocateConstant((Constant)node, scope);
                    break;
                case Kind.Block:
                    LocateBlock((Block)node, scope);
                    break;
                case Kind.Name:
                    LocateName((Name)node, scope);
                    break;
                case Kind.Return:
                    LocateReturn((Return)node, scope);
                    break;
                case Kind.Var:
                    break;
                case Kind.Def:
                    break;
                case Kind.Assign:
                    LocateBinary((Binary)node, scope);
                    break;
                case Kind.Call:
                    break;
                case Kind.While:
                    break;
                case Kind.DefClass:
                    break;
                case Kind.DefModule:
                    break;
                case Kind.TypeSpecifier:
                    break;
                default:
                    break;
            }
        }

        void LocateBinary(Binary node, Scope scope)
        {
            Locate(node.left, scope);
            Locate(node.right, scope);
        }

        void LocateUnary(Unary node, Scope scope)
        {
            Locate(node.operand, scope);
        }

        void LocateIfThen(IfThen node, Scope scope)
        {
            Locate(node.condition, scope);
            Locate(node.ifTrue, scope);
        }

        void LocateIfElse(IfElse node, Scope scope)
        {
            Locate(node.condition, scope);
            Locate(node.ifTrue, scope);
            Locate(node.ifFalse, scope);
        }

        void LocateConstant(Constant node, Scope scope)
        {
        }

        void LocateBlock(Block node, Scope scope)
        {
            foreach (var exp in node.expressions)
            {
                Locate(exp, scope);
            }
        }

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

        void LocateReturn(Return node, Scope scope)
        {
            Locate(node.value, scope);
        }

        void LocateVar(Var node, Scope scope)
        {
            if (node.value != null)
            {
                Locate(node.value, scope);
            }
        }
    }
}
