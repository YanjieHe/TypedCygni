package cygni.types;

import cygni.Scope;

public class TypeLeaf extends Type {
    public String name;

    public TypeLeaf(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return name;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof TypeLeaf) {
            TypeLeaf other = (TypeLeaf) obj;
            return name.equals(other.name);
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
        Type result = scope.lookUpType(name);
        if (result == null) {
            return this;
        } else {
            return result;
        }
    }
}
