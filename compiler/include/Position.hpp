#ifndef POSITION_HPP
#define POSITION_HPP
#include "IJsonSerializable.hpp"

class Position : public IJsonSerializable {
public:
  int startLine;
  int startCol;
  int endLine;
  int endCol;

  Position() : startLine{-1}, startCol{-1}, endLine{-1}, endCol{-1} {}
  Position(int startLine, int startCol, int endLine, int endCol)
      : startLine{startLine}, startCol{startCol}, endLine{endLine},
        endCol{endCol} {}

  Json ToJson() const override;
};

#endif // POSITION_HPP