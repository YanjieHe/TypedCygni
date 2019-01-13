package cygni.types;

import java.util.ArrayList;
import java.util.HashSet;

public class UnionType extends Type {
    public UnionType(ArrayList<Type> types) {
        super("U", types);
    }

    public UnionType union(UnionType other) {
        HashSet<Type> set = new HashSet<Type>();
        for (Type t : parameters) {
            set.add(t);
        }
        for (Type t : other.parameters) {
            set.add(t);
        }
        ArrayList<Type> types = new ArrayList<Type>();
        for (Type t : set) {
            types.add(t);
        }
        return new UnionType(types);
    }

    public UnionType addType(Type type) {
        HashSet<Type> set = new HashSet<Type>();
        for (Type t : parameters) {
            set.add(t);
        }
        set.add(type);
        ArrayList<Type> types = new ArrayList<Type>();
        for (Type t : set) {
            types.add(t);
        }
        return new UnionType(types);
    }
}
