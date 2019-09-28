package cygni.interpreter;

public class BuiltinFunctions {
    public static CygObject add(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygInt(((CygInt) x).intValue() + ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygDouble(((CygDouble) x).doubleValue() + ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject subtract(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygInt(((CygInt) x).intValue() - ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygDouble(((CygDouble) x).doubleValue() - ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject multiply(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygInt(((CygInt) x).intValue() * ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygDouble(((CygDouble) x).doubleValue() * ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject divide(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygInt(((CygInt) x).intValue() / ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygDouble(((CygDouble) x).doubleValue() / ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject greaterThan(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() > ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() > ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject lessThan(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() < ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() < ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject greaterThanOrEqual(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() >= ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() >= ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject lessThanOrEqual(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() <= ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() <= ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject equal(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() == ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() == ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static CygObject notEqual(CygObject x, CygObject y) {
        if (x instanceof CygInt && y instanceof CygInt) {
            return new CygBool(((CygInt) x).intValue() != ((CygInt) y).intValue());
        } else if (x instanceof CygDouble && y instanceof CygDouble) {
            return new CygBool(((CygDouble) x).doubleValue() != ((CygDouble) y).doubleValue());
        } else {
            return null;
        }
    }

    public static <T, R> R[] convertArray(T[] input) {
        @SuppressWarnings("unchecked")
        R[] items = (R[]) new CygObject[input.length];
        for (int i = 0; i < input.length; i++) {
            items[i] = (R) input[i];
        }
        return items;
    }

    public static class MakeArray extends CygObject implements Callable {

        public MakeArray() {

        }


        @Override
        public CygObject invoke(CygObject[] arguments) {
            return new CygArray(new CygObject[((CygInt) arguments[0]).intValue()]);
        }
    }

}
