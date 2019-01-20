package cygni.util;

import java.util.ArrayList;
import java.util.HashMap;

public class JsonUtil {
    public static final String leftQuote = "\"";
    public static final String rightQuote = "\"";
    public static final String comma = ", ";

    public static String toString(Object object) {
        if (object instanceof HashMap) {
            return toString((HashMap<String, Object>) object);
        } else if (object instanceof ArrayList) {
            return toString((ArrayList<Object>) object);
        } else {
            return leftQuote + object.toString() + rightQuote;
        }
    }

    public static String toString(HashMap<String, Object> map) {
        StringBuilder builder = new StringBuilder();
        builder.append("{");
        boolean first = true;
        for (String key : map.keySet()) {
            if (first) {
                first = false;
            } else {
                builder.append(comma);
            }
            builder.append(leftQuote + key + rightQuote + ": ");
            builder.append(toString(map.get(key)));
        }
        builder.append("}");
        return builder.toString();
    }

    public static String toString(ArrayList<Object> list) {
        StringBuilder builder = new StringBuilder();
        builder.append("[");
        if (list.size() == 0) {
            builder.append("]");
        } else {
            builder.append(toString(list.get(0)));
            for (int i = 1; i < list.size(); i++) {
                builder.append(comma);
                builder.append(toString(list.get(i)));
            }
            builder.append("]");
        }
        return builder.toString();
    }
}