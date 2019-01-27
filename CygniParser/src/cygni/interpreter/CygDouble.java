package cygni.interpreter;

public class CygDouble extends CygObject {
    private final double value;

    public CygDouble(double value) {
        this.value = value;
    }

    public double doubleValue() {
        return value;
    }

    @Override
    public String toString() {
        return Double.toString(value);
    }
}
