package cygni.types;

import java.util.ArrayList;

public class FunctionType extends Type {
    public FunctionType(ArrayList<Type> parametersType, Type returnType) {
        super("Function");
        for (int i = 0; i < parametersType.size(); i++) {
            this.parameters.add(parametersType.get(i));
        }
        this.parameters.add(returnType);
    }
}
