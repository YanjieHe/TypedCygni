package cygni.types;

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

}
