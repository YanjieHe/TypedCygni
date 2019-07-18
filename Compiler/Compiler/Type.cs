using System;
using System.Collections.Generic;
using System.Linq;
namespace Compiler
{
    public enum TypeCode
    {
        VOID,
        ANY,
        INT,
        FLOAT,
        LONG,
        DOUBLE,
        CHAR,
        STRING,
        BOOL,
        ARRAY,
        FUNCTION,
        MODULE,
        CLASS,
        OBJECT
    };
    public abstract class Type
    {
        public abstract TypeCode GetTypeCode();

        public abstract bool Equals(Type other);

        public static Type ANY = new AnyType();
        public static Type VOID = new VoidType();
        public static Type INT = new IntType();
        public static Type FLOAT = new FloatType();
        public static Type LONG = new LongType();
        public static Type DOUBLE = new DoubleType();
        public static Type CHAR = new CharType();
        public static Type STRING = new StringType();
        public static Type BOOL = new BoolType();
    };

    public class VoidType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.VOID; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.VOID;
        }

        public override String ToString() { return "Void"; }
    }

    public class AnyType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.ANY; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.ANY;
        }

        public override String ToString() { return "Any"; }
    }

    public class IntType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.INT; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.INT;
        }

        public override String ToString() { return "Int"; }
    }

    public class FloatType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.FLOAT; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.FLOAT;
        }

        public override String ToString() { return "FLOAT"; }
    };

    public class LongType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.LONG; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.LONG;
        }

        public override String ToString() { return "Long"; }
    }

    public class DoubleType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.DOUBLE; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.DOUBLE;
        }

        public override String ToString() { return "Double"; }
    }

    public class CharType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.CHAR; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.CHAR;
        }

        public override String ToString() { return "Char"; }
    }

    class StringType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.STRING; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.STRING;
        }

        public override String ToString() { return "String"; }
    }

    public class BoolType : Type
    {
        public override TypeCode GetTypeCode() { return TypeCode.BOOL; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.BOOL;
        }

        public override String ToString() { return "Bool"; }
    }

    public class ArrayType : Type
    {
        public Type element;

        public ArrayType(Type element)
        { this.element = element; }

        public override TypeCode GetTypeCode() { return TypeCode.ARRAY; }

        public override bool Equals(Type other)
        {
            if (other.GetTypeCode() == TypeCode.ARRAY)
            {
                var array = (ArrayType)(other);
                return element.Equals(array.element);
            }
            else
            {
                return false;
            }
        }

        public override String ToString() { return "Array[" + element.ToString() + "]"; }
    }

    public class FunctionType : Type
    {
        public List<Type> parameters;
        public Type returnType;

        public FunctionType(List<Type> parameters, Type returnType)
        {
            this.parameters = parameters;
            this.returnType = returnType;
        }
        public override TypeCode GetTypeCode() { return TypeCode.FUNCTION; }

        public override bool Equals(Type other)
        {
            if (other.GetTypeCode() == TypeCode.FUNCTION)
            {
                var function = (FunctionType)(other);
                if (parameters.Count == function.parameters.Count)
                {
                    for (int i = 0; i < parameters.Count; i++)
                    {
                        var x = parameters[i];
                        var y = function.parameters[i];
                        if (!x.Equals(y))
                        {
                            return false;
                        }
                    }
                    return returnType.Equals(function.returnType);
                }
                else
                {
                    return false;
                }

            }
            else
            {
                return false;
            }
        }

        public override String ToString()
        {
            var items = parameters.Select(p => p.ToString()).ToList();
            items.Add(returnType.ToString());
            return "Function[" + String.Join(", ", items) + "]";
        }
    };

    public class ModuleType : Type
    {
        public String name;
        public DefModule definition;
        public ModuleType(String name, DefModule definition)
        {
            this.name = name;
            this.definition = definition;
        }
        public override TypeCode GetTypeCode() { return TypeCode.MODULE; }

        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.MODULE &&
                   name == ((ModuleType)(other)).name;
        }

        public override String ToString() { return name; }
    };

    public class ClassType : Type
    {
        public String name;
        public DefClass definition;
        public ClassType(String name, DefClass definition)
        {
            this.name = name;
            this.definition = definition;
        }
        public override TypeCode GetTypeCode() { return TypeCode.CLASS; }
        public override bool Equals(Type other)
        {
            return other.GetTypeCode() == TypeCode.CLASS &&
                   name == ((ClassType)(other)).name;
        }

        public override String ToString() { return name; }
    }

    public class ObjectType : Type
    {
        public ClassType classType;
        public ObjectType(ClassType classType)
        {
            this.classType = classType;
        }
        public override TypeCode GetTypeCode() { return TypeCode.OBJECT; }
        public override bool Equals(Type other)
        {
            return classType.Equals(other);
        }

        public override String ToString() { return classType.ToString(); }
    }

}
