package cygni;

import java.util.HashMap;

import cygni.types.Type;

public class Scope {
    public HashMap<String, HashMap<String, Object>> table;
    public Scope parent;

    public Scope() {
        this.parent = null;
        this.table = new HashMap<String, HashMap<String, Object>>();
    }

    public Scope(Scope parent) {
        this.parent = parent;
        this.table = new HashMap<String, HashMap<String, Object>>();
    }

    private void putLocal(String name, String key, Object value) {
        if (table.containsKey(name)) {
            HashMap<String, Object> map = table.get(name);
            map.put(key, value);
        } else {
            HashMap<String, Object> map = new HashMap<String, Object>();
            map.put(key, value);
            table.put(name, map);
        }
    }

    private Object lookupLocal(String name, String key) {
        if (table.containsKey(name)) {
            return table.get(name).getOrDefault(key, null);
        } else {
            return null;
        }
    }

    private Object lookup(String name, String key) {
        Object value = lookupLocal(name, key);
        if (value == null) {
            if (parent == null) {
                return null;
            } else {
                return parent.lookup(name, key);
            }
        } else {
            return value;
        }
    }

    public void putType(String name, Type type) {
        putLocal(name, "type", type);
    }

    public Type lookUpType(String name) {
        Object value = lookup(name, "type");
        if (value instanceof Type) {
            return (Type) value;
        } else {
            return null;
        }
    }

    public void putValue(String name, Object value) {
        putLocal(name, "value", value);
    }

    public Object lookUpValue(String name) {
        Object value = lookup(name, "value");
        return value;
    }

    public boolean putValueIfDefined(String name, Object value) {
        if (table.containsKey(name)) {
            HashMap<String, Object> map = table.get(name);
            if (map.containsKey("value")) {
                map.put("value", value);
                return true;
            } else {
                if (parent == null) {
                    return false;
                } else {
                    return parent.putValueIfDefined(name, value);
                }
            }
        } else {
            if (parent == null) {
                return false;
            } else {
                return parent.putValueIfDefined(name, value);
            }
        }
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        for (String key : table.keySet()) {
            builder.append("(");
            builder.append(key);
            builder.append(" -> ");
            builder.append(table.get(key).toString());
            builder.append(")");
        }
        builder.append("]");
        if (parent == null) {
            return builder.toString();
        } else {
            return parent.toString() + builder.toString();
        }
    }
}
