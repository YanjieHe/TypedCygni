package cygni.interpreter;

import cygni.types.BoolType;
import cygni.types.DoubleType;
import cygni.types.IntType;
import cygni.types.Type;

public class BuiltinFunctions {
    public static Object add(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() + ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() + ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object subtract(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() - ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() - ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object multiply(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() * ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() * ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object divide(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() / ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() / ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object greaterThan(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() > ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() > ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object lessThan(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() < ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() < ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object greaterThanOrEqual(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() >= ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() >= ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object lessThanOrEqual(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() <= ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() <= ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object equal(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() == ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() == ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static Object notEqual(Object x, Object y) {
        if (x instanceof Integer && y instanceof Integer) {
            return ((Integer) x).intValue() != ((Integer) y).intValue();
        } else if (x instanceof Double && y instanceof Double) {
            return ((Double) x).doubleValue() != ((Double) y).doubleValue();
        } else {
            return null;
        }
    }

    public static <T, R> R[] convertArray(T[] input) {
        @SuppressWarnings("unchecked")
        R[] items = (R[]) new Object[input.length];
        for (int i = 0; i < input.length; i++) {
            items[i] = (R) input[i];
        }
        return items;
    }

    public static class MakeArray implements Callable {

        public MakeArray() {

        }


        @Override
        public Object invoke(Object[] arguments) {
            return new Array(new Object[(Integer) arguments[0]]);
        }
    }

}
