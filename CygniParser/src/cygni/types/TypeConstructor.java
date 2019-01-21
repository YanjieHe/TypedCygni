package cygni.types;

import cygni.Scope;
import cygni.exceptions.TypeException;

import java.util.ArrayList;

public class TypeConstructor extends Type {
    public String name;
    public ArrayList<Type> types;

    public TypeConstructor(String name, ArrayList<Type> types) {
        this.name = name;
        this.types = types;
    }

    @Override
    public java.lang.String toString() {
        String[] items = new String[types.size()];
        for (int i = 0; i < types.size(); i++) {
            items[i] = types.get(i).toString();
        }
        return name + "[" + java.lang.String.join(", ", items) + "]";
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof TypeConstructor) {
            TypeConstructor other = (TypeConstructor) obj;
            if (name.equals(other.name) && types.size() == other.types.size()) {
                for (int i = 0; i < types.size(); i++) {
                    if (!types.get(i).equals(other.types.get(i))) {
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
        return name.hashCode();
    }

    @Override
    public Type substitute(Scope scope) {
        ArrayList<Type> result = new ArrayList<Type>();
        for (Type type : types) {
            result.add(type.substitute(scope));
        }
        return new TypeConstructor(name, result);
    }
}
