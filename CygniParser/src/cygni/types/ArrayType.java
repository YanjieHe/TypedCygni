package cygni.types;

import cygni.Scope;

import java.util.ArrayList;

public class ArrayType extends TypeConstructor {
    public ArrayType(Type element) {
        super("Array", new ArrayList<Type>());
        types.add(element);
    }

    public ArrayType(ArrayList<Type> types) {
        super("Array", types);
    }

    @Override
    public Type substitute(Scope scope) {
        ArrayList<Type> result = new ArrayList<Type>();
        for (Type type : types) {
            result.add(type.substitute(scope));
        }
        return new ArrayType(result);
    }
}
