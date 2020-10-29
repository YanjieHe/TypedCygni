#ifndef POSITION_HPP
#define POSITION_HPP
#include "IJsonSerializable.hpp"

class Position : public IJsonSerializable {
public:
  int startLine;
  int startCol;
  int endLine;
  int endCol;

  Position() : startLine{0}, startCol{0}, endLine{endLine}, endCol{endCol} {}
  Position(int startLine, int startCol, int endLine, int endCol)
      : startLine{startLine}, startCol{startCol}, endLine{endLine},
        endCol{endCol} {}

  Json ToJson() const override;
};

#endif // POSITION_HPP