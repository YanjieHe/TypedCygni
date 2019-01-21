package cygni.lexer;

import java.util.HashMap;

public class Token {
    public int line;
    public int col;
    public Tag tag;
    public Object value;

    private static HashMap<String, Tag> keywords;

    static {
        keywords = new HashMap<String, Tag>();
        keywords.put("if", Tag.If);
        keywords.put("else", Tag.Else);
        keywords.put("var", Tag.Var);
        keywords.put("def", Tag.Def);
        keywords.put("return", Tag.Return);
        keywords.put("true", Tag.True);
        keywords.put("false", Tag.False);
        keywords.put("while", Tag.While);
        keywords.put("class", Tag.Class);
        keywords.put("private", Tag.Private);
    }

    public Token(int line, int col, Tag tag, Object value) {
        this.line = line;
        this.col = col;
        this.tag = tag;
        this.value = value;
        if (this.tag == Tag.Identifier) {
            String text = (String) this.value;
            if (keywords.containsKey(text)) {
                this.tag = keywords.get(text);
            }
        }
    }

    @Override
    public String toString() {
        if (value instanceof String) {
            return "t(Tag." + tag + ", \"" + value.toString() + "\")";
        } else {
            return "t(Tag." + tag + ", " + value.toString() + ")";
        }
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Token) {
            Token t = (Token) o;
            return tag == t.tag & value.equals(t.value);
        } else {
            return false;
        }
    }
}
