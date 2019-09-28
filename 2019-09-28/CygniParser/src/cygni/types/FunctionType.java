package cygni.types;

import cygni.Scope;
import cygni.exceptions.TypeException;

import java.util.ArrayList;

public class FunctionType extends TypeConstructor {
    public ArrayList<UnknownType> unknownTypes;

    public FunctionType(ArrayList<Type> types) {
        super("Function", types);
        this.unknownTypes = new ArrayList<UnknownType>();
    }

    public FunctionType(ArrayList<Type> types, ArrayList<UnknownType> unknownTypes) {
        super("Function", types);
        this.unknownTypes = unknownTypes;
    }

    public boolean matchArguments(ArrayList<Type> argumentTypes) {
        return match(argumentTypes, getReturnType());
    }

    public boolean match(ArrayList<Type> parameterTypes, Type returnType) {
        if (parameterTypes.size() + 1 == types.size()) {
            for (int i = 0; i < parameterTypes.size(); i++) {
                if (!types.get(i).equals(parameterTypes.get(i))) {
                    return false;
                }
            }
            return types.get(types.size() - 1).equals(returnType);
        } else {
            return false;
        }
    }

    public Type getReturnType() {
        return types.get(types.size() - 1);
    }

    public static FunctionType create(ArrayList<Type> parameterTypes, Type returnType) {
        ArrayList<Type> types = new ArrayList<Type>();
        for (Type type : parameterTypes) {
            types.add(type);
        }
        types.add(returnType);
        return new FunctionType(types);
    }

    public static FunctionType create(ArrayList<Type> parameterTypes, Type returnType, ArrayList<UnknownType> unknownTypes) {
        ArrayList<Type> types = new ArrayList<Type>();
        for (Type type : parameterTypes) {
            types.add(type);
        }
        types.add(returnType);
        return new FunctionType(types, unknownTypes);
    }

    @Override
    public Type substitute(Scope scope) {
        ArrayList<Type> result = new ArrayList<Type>();
        for (Type type : types) {
            result.add(type.substitute(scope));
        }
        return new FunctionType(result, unknownTypes);
    }
}
