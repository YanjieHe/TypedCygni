#ifndef POSITION_HPP
#define POSITION_HPP
#include "IJsonSerializable.hpp"
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;
using std::make_shared;

class Source: public IJsonSerializable {
public:
  typedef shared_ptr<Source> Ptr;

  string path;
  Source(string path) : path{path} {}

  Json ToJson() const override;
};

class Position : public IJsonSerializable {
public:
  Source::Ptr source;
  int startLine;
  int startCol;
  int endLine;
  int endCol;

  Position()
      : source{make_shared<Source>("<stdin>")}, startLine{-1}, startCol{-1},
        endLine{-1}, endCol{-1} {}
  Position(Source::Ptr source, int startLine, int startCol, int endLine,
           int endCol)
      : source{source}, startLine{startLine}, startCol{startCol},
        endLine{endLine}, endCol{endCol} {}

  Json ToJson() const override;
};

#endif // POSITION_HPP