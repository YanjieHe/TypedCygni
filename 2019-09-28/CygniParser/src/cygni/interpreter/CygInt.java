package cygni.interpreter;

public class CygInt extends CygObject {
    private final int value;

    public CygInt(int value) {
        this.value = value;
    }

    public int intValue() {
        return value;
    }

    @Override
    public String toString() {
        return Integer.toString(value);
    }
}
