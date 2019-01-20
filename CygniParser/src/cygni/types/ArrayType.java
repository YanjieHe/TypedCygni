package cygni.types;

import java.util.ArrayList;

public class ArrayType extends TypeConstructor {
    public ArrayType(Type element) {
        super("Array", new ArrayList<Type>());
        types.add(element);
    }

    public ArrayType(ArrayList<Type> types) {
        super("Array", types);
    }
}
