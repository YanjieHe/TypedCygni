package cygni.types;

public class KnownType extends TypeLeaf {
    public UnknownType unknownType;
    public Type argument;

    public KnownType(String name, UnknownType unknownType, Type argument) {
        super(name);
        this.unknownType = unknownType;
        this.argument = argument;
    }
}
