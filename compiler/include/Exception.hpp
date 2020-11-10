#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include "Position.hpp"

using std::string;

class Error {
public:
  Position pos;
  string message;
  Error(Position pos, string message) : pos{pos}, message{message} {}
};

class SyntaxError {
public:
  Position pos;
  string message;
  SyntaxError() : pos(), message() {}
  SyntaxError(Position pos, string message) : pos{pos}, message{message} {}
  string ToString() const {
    return pos.source->path + ":" + std::to_string(pos.startLine + 1) + ":" +
           std::to_string(pos.startCol + 1) + ": " + message;
  }
};

class TypeError : public Error {
public:
  TypeError(Position pos, string message) : Error(pos, message) {}
};

#endif // EXCEPTION_HPP