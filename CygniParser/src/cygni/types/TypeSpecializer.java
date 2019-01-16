package cygni.types;

import java.util.ArrayList;
import java.util.HashMap;

public class TypeSpecializer {
    public static Type specialize(Type type, HashMap<String, Type> map) {
        if (type instanceof ArrayType) {
            return specializeArray((ArrayType) type, map);
        } else if (type instanceof FunctionType) {
            return specializeFunction((FunctionType) type, map);
        } else if (type instanceof TypeParameter) {
            return specializeTypeParameter((TypeParameter) type, map);
        } else {
            // basic type
            return type;
        }
    }

    private static Type specializeArray(ArrayType type, HashMap<String, Type> map) {
        return new ArrayType(specialize(type.element, map));
    }

    private static Type specializeFunction(FunctionType type, HashMap<String, Type> map) {
        ArrayList<Type> parametersType = new ArrayList<Type>();
        for (int i = 0; i < type.parametersType.size(); i++) {
            parametersType.add(specialize(type.parametersType.get(i), map));
        }
        return new FunctionType(parametersType, specialize(type.returnType, map));
    }

    private static Type specializeTypeParameter(TypeParameter type, HashMap<String, Type> map) {
        return map.get(type.name);
    }

    public static Type specializeAll(AllType type, ArrayList<Type> types) {
        HashMap<String, Type> map = new HashMap<String, Type>();
        for (int i = 0; i < type.parameters.size(); i++) {
            map.put(type.parameters.get(i).name, types.get(i));
        }
        return specialize(type, map);
    }
}
