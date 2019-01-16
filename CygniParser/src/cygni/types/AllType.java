package cygni.types;

import java.util.ArrayList;

public class AllType extends Type {
    public ArrayList<TypeParameter> parameters;
    public Type innerType;

    public AllType(ArrayList<TypeParameter> parameters, Type innerType) {
        super("All");
        this.parameters = parameters;
        this.innerType = innerType;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof AllType) {
            AllType other = (AllType) obj;
            if (parameters.size() == other.parameters.size()) {
                for (int i = 0; i < parameters.size(); i++) {
                    if (!parameters.get(i).equals(other.parameters.get(i))) {
                        return false;
                    }
                }
                return innerType.equals(other.innerType);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}
