package cygni.types;

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

    public TypeConstructor substitute(int startLine, int startCol, int endLine, int endCol, ArrayList<Type> arguments) throws TypeException {
        ArrayList<Type> result = new ArrayList<Type>();
        if (types.size() == arguments.size()) {
            for (int i = 0; i < arguments.size(); i++) {
                if (types.get(i) instanceof UnknownType) {
                    UnknownType unknownType = (UnknownType) types.get(i);
                    result.add(new KnownType(unknownType.name, unknownType, arguments.get(i)));
                } else {
                    throw new TypeException(startLine, startCol, endLine, endCol, "type argument(" + (i + 1) + ") do not mach");
                }
            }
            return new TypeConstructor(name, result);
        } else {
            throw new TypeException(startLine, startCol, endLine, endCol, "type arguments size does not match");
        }
    }
}
