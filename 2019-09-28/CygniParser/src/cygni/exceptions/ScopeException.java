package cygni.exceptions;

public class ScopeException extends Exception {
    public String message;

    public ScopeException(String message) {
        this.message = message;
    }

    @Override
    public String getMessage() {
        return message;
    }
}
