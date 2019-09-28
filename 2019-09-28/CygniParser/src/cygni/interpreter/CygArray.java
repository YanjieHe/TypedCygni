package cygni.interpreter;

public class CygArray extends CygObject implements Callable {
    public CygObject[] objects;

    public CygArray(CygObject[] objects) {
        this.objects = objects;
    }

    @Override
    public CygObject invoke(CygObject[] arguments) throws RuntimeException {
        return objects[((CygInt) arguments[0]).intValue()];
    }

    public void write(int index, CygObject value) {
        objects[index] = value;
    }

    @Override
    public String toString() {
        String[] items = new String[objects.length];
        for (int i = 0; i < items.length; i++) {
            if (objects[i] == null) {
                items[i] = "";
            } else {
                items[i] = objects[i].toString();
            }
        }
        return "[" + String.join(", ", items) + "]";
    }
}
