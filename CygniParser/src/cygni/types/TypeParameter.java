package cygni.types;

public class TypeParameter extends Type {
    // TO DO: constraints
    public TypeParameter(String name) {
        super(name);
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof TypeParameter) {
            return name.equals(((TypeParameter) obj).name);
        } else {
            return false;
        }
    }
}
