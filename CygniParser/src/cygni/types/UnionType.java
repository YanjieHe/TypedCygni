package cygni.types;

import java.util.HashSet;

import static java.lang.String.join;

public class UnionType {
    public HashSet<Type> types;

    public UnionType(HashSet<Type> types) {
//        super("U");
        this.types = types;
    }

    public UnionType union(UnionType other) {
        HashSet<Type> set = new HashSet<Type>();
        for (Type t : types) {
            set.add(t);
        }
        for (Type t : other.types) {
            set.add(t);
        }
        return new UnionType(set);
    }

    public UnionType addType(Type type) {
        HashSet<Type> set = new HashSet<Type>();
        for (Type t : types) {
            set.add(t);
        }
        set.add(type);
        return new UnionType(set);
    }

//    @Override
//    public java.lang.String toString() {
//        String[] items = new String[types.size()];
//        int i = 0;
//        for (Type type : types) {
//            items[i] = type.toString();
//            i++;
//        }
//        return name + "[" + join(", ", items) + "]";
//    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof UnionType) {
            UnionType other = (UnionType) obj;
            if (types.size() == other.types.size()) {
                for (Type type : types) {
                    if (!other.types.contains(type)) {
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
}
