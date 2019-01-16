package cygni.types;

import java.util.ArrayList;

public class FunctionType extends Type {
    ArrayList<Type> parametersType;
    Type returnType;

    public FunctionType(ArrayList<Type> parametersType, Type returnType) {
        super("Function");
        this.parametersType = parametersType;
        this.returnType = returnType;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof FunctionType) {
            FunctionType other = (FunctionType) obj;
            if (name.equals(other.name) && parametersType.size() == other.parametersType.size()) {
                for (int i = 0; i < parametersType.size(); i++) {
                    if (!parametersType.get(i).equals(other.parametersType.get(i))) {
                        return false;
                    }
                }
                return returnType.equals(other.returnType);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    @Override
    public java.lang.String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append(name);
        builder.append("[");
        for (Type type : parametersType) {
            builder.append(type.toString());
            builder.append(", ");
        }
        builder.append(returnType.toString());
        builder.append("]");
        return builder.toString();
    }

    public static FunctionType fromTypeList(ArrayList<Type> types) {
        ArrayList<Type> parametersType = new ArrayList<Type>();
        for (int i = 0; i < types.size() - 1; i++) {
            parametersType.add(types.get(i));
        }
        return new FunctionType(parametersType, types.get(types.size() - 1));
    }
}
