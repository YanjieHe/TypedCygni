package cygni.interpreter;

public class CygBool extends CygObject {
    private final boolean value;

    public CygBool(boolean value) {
        this.value = value;
    }

    public boolean booleanValue() {
        return value;
    }

    @Override
    public String toString() {
        return Boolean.toString(value);
    }
}
