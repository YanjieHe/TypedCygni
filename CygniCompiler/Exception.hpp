#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include "String.hpp"
#include "Predef.hpp"

class Exception : std::exception
{
protected:
    String message;
    HashMap<String, String> data;
public:
    Exception() = default;

    explicit Exception(String message) : message{std::move(message)}
    {

    }

    const String &Message()
    {
        return message;
    }

    const HashMap<String, String> &Data()
    {
        return data;
    }

    Exception &AddInfo(const String &key, const String &value)
    {
        data.insert({key, value});
        return *this;
    }

};

class LexicalException : public Exception
{
public:
    int line;
    int column;

    LexicalException(int line, int column, String message)
            : Exception(std::move(message)), line{line}, column{column}
    {
        AddInfo("line", String::ToString(line));
        AddInfo("column", String::ToString(column));
    }
};

class ParserException : public Exception
{
public:
    int line;
    int column;

    ParserException(int line, int column, String message)
            : Exception(std::move(message)), line{line}, column{column}
    {
        AddInfo("line", String::ToString(line));
        AddInfo("column", String::ToString(column));
    }
};

class NotSupportedException : public Exception
{
public:
    NotSupportedException() : Exception("Specified method is not supported.")
    {

    }

    explicit NotSupportedException(String message) : Exception(std::move(message))
    {

    }
};

class NotImplementedException : public Exception
{
public:
    NotImplementedException() : Exception("The method or operation is not implemented.")
    {

    }

    explicit NotImplementedException(String message) : Exception(std::move(message))
    {

    }
};

class ArgumentException : public Exception
{
public:
    ArgumentException() : Exception("ArgumentException")
    {

    }

    explicit ArgumentException(String message) : Exception(std::move(message))
    {

    }
};

class FileNotFoundException : public Exception
{
public:
    String path;

    FileNotFoundException() : Exception("Unable to find the specified file.")
    {

    }

    explicit FileNotFoundException(String path)
            : Exception("Unable to find the specified file."), path{std::move(path)}
    {
        AddInfo("path", path);
    }
};

class KeyNotFoundException : public Exception
{
public:
    KeyNotFoundException() : Exception("The given key was not present in the dictionary.")
    {

    }
};

#endif //EXCEPTION_HPP
