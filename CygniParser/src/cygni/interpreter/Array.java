package cygni.interpreter;

public class Array implements Callable {
    public Object[] objects;

    public Array(Object[] objects) {
        this.objects = objects;
    }

    public Object invoke(Object[] arguments) {
        return objects[(Integer) arguments[0]];
    }

    public void write(int index, Object value) {
        objects[index] = value;
    }

    @Override
    public String toString() {
        String[] items = new String[objects.length];
        for (int i = 0; i < items.length; i++) {
            items[i] = objects[i].toString();
        }
        return "[" + String.join(", ", items) + "]";
    }
}
