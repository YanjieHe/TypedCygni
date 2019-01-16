package cygni.types;

public class ArrayType extends Type {
    public Type element;

    public ArrayType(Type element) {
        super("Array");
        this.element = element;
    }

    @Override
    public java.lang.String toString() {
        return name + "[" + element.toString() + "]";
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ArrayType) {
            ArrayType other = (ArrayType) obj;
            return this.element.equals(other.element);
        } else {
            return false;
        }
    }
}
