package cygni.types;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;


public class Type {
    public String name;

    public static Type Int = new IntType();
    public static Type Float = new FloatType();
    public static Type Long = new LongType();
    public static Type Double = new DoubleType();
    public static Type Bool = new BoolType();
    public static Type Char = new CharType();
    public static Type String = new StringType();
    public static Type Unit = new UnitType();

    public Type(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return name;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Type) {
            Type other = (Type) obj;
            return name.equals(other.name);
        } else {
            return false;
        }
    }

    @Override
    public int hashCode() {
        return name.hashCode();
    }

    public static Type union(Type x, Type y) {
        if (x.equals(y)) {
            return x;
        } else {
            if (x instanceof IntType && y instanceof DoubleType) {
                return Type.Double;
            } else if (x instanceof DoubleType && y instanceof IntType) {
                return Type.Double;
            } else if (x instanceof UnionType && y instanceof UnionType) {
                return ((UnionType) x).union((UnionType) y);
            } else if (x instanceof UnionType && !(y instanceof UnionType)) {
                return ((UnionType) x).addType(y);
            } else if (!(x instanceof UnionType) && y instanceof UnionType) {
                return ((UnionType) y).addType(x);
            } else {
                HashSet<Type> types = new HashSet<Type>();
                types.add(x);
                types.add(y);
                return new UnionType(types);
            }
        }
    }

    private static HashMap<String, Type> basicTypes;

    static {
        basicTypes = new HashMap<String, Type>();
        basicTypes.put("Int", Type.Int);
        basicTypes.put("Float", Type.Float);
        basicTypes.put("Long", Type.Long);
        basicTypes.put("Double", Type.Double);
        basicTypes.put("Bool", Type.Bool);
        basicTypes.put("Char", Type.Char);
        basicTypes.put("String", Type.String);
        basicTypes.put("Unit", Type.Unit);
    }

    public static Type makeBasicType(String name) {
        return basicTypes.get(name);
    }

    public static Type makePrimitiveType(String name) {
        if (basicTypes.containsKey(name)) {
            return makeBasicType(name);
        } else {
            return new Type(name);
        }
    }

    public static Type makeType(String name, ArrayList<Type> types) {
        if (name.equals("U")) {
            HashSet<Type> set = new HashSet<Type>();
            for (Type type : types) {
                set.add(type);
            }
            return new UnionType(set);
        } else if (name.equals("Function")) {
            return FunctionType.fromTypeList(types);
        } else if (name.equals("Array")) {
            if (types.size() == 1) {
                return new ArrayType(types.get(0));
            } else {
                return null;
            }
        } else {
            // TO DO: assert types size == 1
            return makeBasicType(name);
        }
    }

}