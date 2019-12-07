package cygni.exceptions;

public class RuntimeException extends Exception {
    public String message;

    public RuntimeException(String message) {
        this.message = message;
    }

    @Override
    public String getMessage() {
        return message;
    }
}
