package cygni.types;

import java.util.ArrayList;

import static java.lang.String.join;

public class Type {
    public String name;
    public ArrayList<Type> parameters;

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
        this.parameters = new ArrayList<Type>();
    }

    public Type(String name, ArrayList<Type> parameters) {
        this.name = name;
        this.parameters = parameters;
    }

    @Override
    public String toString() {
        if (parameters.size() == 0) {
            return name;
        } else {
            String[] items = new String[parameters.size()];
            for (int i = 0; i < items.length; i++) {
                items[i] = parameters.get(i).toString();
            }
            return name + "<" + join(", ", items) + ">";
        }
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Type) {
            Type other = (Type) obj;
            if (name.equals(other.name) && parameters.size() == other.parameters.size()) {
                for (int i = 0; i < parameters.size(); i++) {
                    if (!parameters.get(i).equals(other.parameters.get(i))) {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    @Override
    public int hashCode() {
        int x = name.hashCode();
        for (Type t : parameters) {
            x = x ^ t.hashCode();
        }
        return x;
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
                ArrayList<Type> types = new ArrayList<Type>();
                types.add(x);
                types.add(y);
                return new UnionType(types);
            }
        }
    }
}