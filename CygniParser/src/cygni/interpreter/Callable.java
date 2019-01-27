package cygni.interpreter;

import cygni.exceptions.RuntimeException;

public interface Callable {
    CygObject invoke(CygObject[] arguments) throws RuntimeException;
}
