package cygni.ast;

import cygni.types.Type;

public class Parameter {
    public String name;
    public Type type;

    public Parameter(String name, Type type) {
        this.name = name;
        this.type = type;
    }

    @Override
    public String toString() {
        return name + ": " + type.toString();
    }
}
