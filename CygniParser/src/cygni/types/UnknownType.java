package cygni.types;

public class UnknownType extends TypeLeaf {
    public Type lowerBound;
    public Type upperBound;

    public UnknownType(String name) {
        super(name);
        this.lowerBound = Type.Nothing;
        this.upperBound = Type.Any;
    }

    public UnknownType(String name, Type lowerBound, Type upperBound) {
        super(name);
        this.lowerBound = lowerBound;
        this.upperBound = upperBound;
    }
}
