#include "Parser.hpp"
using std::vector;
using std::unordered_map;
namespace cygni {
Parser::Parser(std::vector<Token> tokens, std::shared_ptr<SourceDocument> document)
    :tokens{tokens}, document{document}{
    }


Program Parser::ParseProgram() {
    std::unordered_map<std::u32string, std::shared_ptr<ClassInfo>> classes;
  while (!IsEof()) {
    if (Look().tag == Tag::Class) {
        ParseDefClass();
    } else {
        ParseDefModule();
    }
  }
  std::u32string path;
  return Program(path, classes);
}

ExpPtr Parser::Statement() {
  switch (Look().tag) {
    case Tag::If:
      return IfStatement();
    case Tag::Var:
      return ParseVar();
    case Tag::Def:
      return ParseDef();
    case Tag::Return:
      return ParseReturn();
    case Tag::While:
      return ParseWhile();
    default:
      return ParseAssign();
  }
}

ExpPtr Parser::ParseAssign() {
  const Token& start = Look();
  auto x = ParseOr();
  if (Look().tag == Tag::Assign) {
    Token t = Match(Tag::Assign);
    auto y = ParseOr();
    return std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Assign, x, y);
  } else {
    return x;
  }
}

ExpPtr Parser::ParseOr() {
  const Token& start = Look();
  auto x = ParseAnd();
  while (Look().tag == Tag::Or) {
    Token t = Match(Tag::Or);
    auto y = ParseAnd();
    x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Or, x, y);
  }
  return x;
}

ExpPtr Parser::ParseAnd() {
  const Token& start = Look();
  auto x = ParseEquality();
  while (Look().tag == Tag::And) {
    Token t = Match(Tag::And);
    auto y = ParseEquality();
    x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::And, x, y);
  }
  return x;
}

ExpPtr Parser::ParseEquality() {
  const Token& start = Look();
  auto x = ParseRelation();
  while (Look().tag == Tag::Equal || Look().tag == Tag::NotEqual) {
    Token t = Look();
    Advance();
    auto y = ParseRelation();
    if (t.tag == Tag::Equal) {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Equal, x, y);
    } else {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::NotEqual, x, y);
    }
  }
  return x;
}

ExpPtr Parser::ParseRelation() {
  const Token& start = Look();
  auto x = ParseExpr();
  if (Look().tag == Tag::GreaterThan || Look().tag == Tag::LessThan ||
      Look().tag == Tag::GreaterThanOrEqual ||
      Look().tag == Tag::LessThanOrEqual) {
    Token t = Look();
    Advance();
    auto y = ParseExpr();
    if (t.tag == Tag::GreaterThan) {
      return std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::GreaterThan, x, y);
    } else if (t.tag == Tag::LessThan) {
      return std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::LessThan, x, y);
    } else if (t.tag == Tag::GreaterThanOrEqual) {
      return std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::GreaterThanOrEqual, x, y);
    } else {
      return std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::LessThanOrEqual, x, y);
    }
  } else {
    return x;
  }
}

ExpPtr Parser::ParseExpr() {
  const Token& start = Look();
  auto x = ParseTerm();
  while (Look().tag == Tag::Add || Look().tag == Tag::Subtract) {
    Token t = Look();
    Advance();
    auto y = ParseTerm();
    if (t.tag == Tag::Add) {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Add, x, y);
    } else {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Subtract, x, y);
    }
  }
  return x;
}

ExpPtr Parser::ParseTerm() {
  const Token& start = Look();
  auto x = ParseUnary();
  while (Look().tag == Tag::Multiply || Look().tag == Tag::Divide) {
    Token t = Look();
    Advance();
    auto y = ParseUnary();
    if (t.tag == Tag::Multiply) {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Multiply, x, y);
    } else {
      x = std::make_shared<BinaryExpression>(GetLoc(start), ExpressionType::Divide, x, y);
    }
  }
  return x;
}

ExpPtr Parser::ParseUnary() {
  const Token& start = Look();
  if (Look().tag == Tag::Add) {
    Advance();
    auto x = ParseUnary();
    return std::make_shared<UnaryExpression>(GetLoc(start), ExpressionType::UnaryPlus, x);
  } else if (Look().tag == Tag::Subtract) {
    Advance();
    auto x = ParseUnary();
    return std::make_shared<UnaryExpression>(GetLoc(start), ExpressionType::UnaryMinus, x);
  } else if (Look().tag == Tag::Not) {
    Advance();
    auto x = ParseUnary();
    return std::make_shared<UnaryExpression>(GetLoc(start), ExpressionType::Not, x);
  } else {
    return ParsePostfix();
  }
}

ExpPtr Parser::ParsePostfix() {
  auto x = ParseFactor();
  while (Look().tag == Tag::LeftParenthesis || Look().tag == Tag::LeftBracket) {
    const Token& start = Look();

    if (Look().tag == Tag::LeftParenthesis) {
    vector<ExpPtr> arguments;
      Match(Tag::LeftParenthesis);
      if (Look().tag == Tag::RightParenthesis) {
        Match(Tag::RightParenthesis);
        x = std::make_shared<InvocationExpression>(GetLoc(start), x, arguments);
      } else {
        arguments.push_back(ParseOr());
        while (!IsEof() && Look().tag != Tag::RightParenthesis) {
          Match(Tag::Comma);
          arguments.push_back(ParseOr());
        }
        Match(Tag::RightParenthesis);
        x = std::make_shared<InvocationExpression>(GetLoc(start), x, arguments);
      }
    } else if (Look().tag == Tag::LeftBracket) {
      // TO DO
    }
  }
  return x;
}

ExpPtr Parser::ParseFactor() {
  if (Look().tag == Tag::LeftParenthesis) {
    Advance();
    ExpPtr x = ParseOr();
    Match(Tag::RightParenthesis);
    return x;
  } else if (Look().tag == Tag::Integer) {
    std::u32string v = Look().text;
    const Token& start = Look();
    Advance();
    return std::make_shared<ConstantExpression>(GetLoc(start), std::make_shared<Int32Type>(), v);
  } else if (Look().tag == Tag::Float) {
    std::u32string v = Look().text;
    const Token& start = Look();
    Advance();
    return std::make_shared<ConstantExpression>(GetLoc(start), std::make_shared<DoubleType>(), v);
  } else if (Look().tag == Tag::String) {
    std::u32string v = Look().text;
    const Token& start = Look();
    Advance();
    return std::make_shared<ConstantExpression>(GetLoc(start), std::make_shared<StringType>(), v);
  } else if (Look().tag == Tag::True) {
    const Token& start = Look();
    Advance();
    return std::make_shared<ConstantExpression>(GetLoc(start), std::make_shared<BooleanType>(), U"true");
  } else if (Look().tag == Tag::False) {
    const Token& start = Look();
    Advance();
    return std::make_shared<ConstantExpression>(GetLoc(start), std::make_shared<BooleanType>(), U"false");
  } else if (Look().tag == Tag::Identifier) {
    std::u32string name = Look().text;
    const Token& start = Look();
    Advance();
    return std::make_shared<ParameterExpression>(GetLoc(start), name, std::make_shared<UnknownType>());
  } else {
    throw ParserException(Look().line, Look().column, U"factor");
  }
}

ExpPtr Parser::ParseBlock() {
  const Token& start = Look();
  Match(Tag::LeftBrace);
  vector<ExpPtr> expressions;
  while (!IsEof() && Look().tag != Tag::RightBrace) {
    expressions.push_back(Statement());
  }
  Match(Tag::RightBrace);
  return std::make_shared<BlockExpression>(GetLoc(start), expressions);
}

ExpPtr Parser::IfStatement() {
  const Token& start = Look();
  Match(Tag::If);
  ExpPtr condition = ParseOr();
  ExpPtr ifTrue = ParseBlock();
  if (Look().tag == Tag::Else) {
    Match(Tag::Else);
    if (Look().tag == Tag::If) {
      auto chunk = IfStatement();
      return std::make_shared<ConditionalExpression>(GetLoc(start), condition, ifTrue, chunk);
    } else {
      auto chunk = ParseBlock();
      return std::make_shared<ConditionalExpression>(GetLoc(start), condition, ifTrue, chunk);
    }
  } else {
      auto empty = std::make_shared<DefaultExpression>(GetLoc(Look()));
      return std::make_shared<ConditionalExpression>(GetLoc(start), condition, ifTrue, empty);
  }
}

ExpPtr Parser::ParseVar() {
  const Token& start = Look();
  Match(Tag::Var);
  Token t = Match(Tag::Identifier);
  if (Look().tag == Tag::Colon) {
    Match(Tag::Colon);
    auto type = Optional<Ptr<TypeExpression>>(ParseType());
    if (Look().tag == Tag::Assign) {
      Match(Tag::Assign);
      auto value = Optional<Ptr<Ast>>(ParseOr());
      return New<Var>(GetPos(start), t.text, type, value);
    } else {
      return New<Var>(GetPos(start), t.text, type, Optional<Ptr<Ast>>());
    }
  } else {
    Match(Tag::Assign);
    auto value = Optional<Ptr<Ast>>(ParseOr());
    return New<Var>(GetPos(start), t.text, Optional<Ptr<TypeExpression>>(),
                    value);
  }
}

Ptr<Var> Parser::ParseVarDeclaration() {
  const Token& start = Look();
  Match(Tag::Var);
  Token t = Match(Tag::Identifier);
  Match(Tag::Colon);
  auto type = Optional<Ptr<TypeExpression>>(ParseType());
  return New<Var>(GetPos(start), t.text, type, Optional<Ptr<Ast>>());
}

Ptr<Def> Parser::ParseDef() {
  const Token& start = Look();
  Match(Tag::Def);
  String name = Match(Tag::Identifier).text;

  Match(Tag::LeftParenthesis);
  Vector<Parameter> parameters;
  if (Look().tag != Tag::RightParenthesis) {
    parameters.push_back(ParseParameter());
    while (!IsEof() && Look().tag != Tag::RightParenthesis) {
      Match(Tag::Comma);
      parameters.push_back(ParseParameter());
    }
  }
  Match(Tag::RightParenthesis);
  Match(Tag::Colon);
  auto returnType = ParseType();
  Ptr<Ast> body = ParseBlock();
  return New<Def>(GetPos(start), name, parameters,
                  Def::MakeFunctionType(GetPos(start), parameters, returnType),
                  body);
}

Parameter Parser::ParseParameter() {
  String name = Match(Tag::Identifier).text;
  Match(Tag::Colon);
  auto type = ParseType();
  return Parameter(name, type);
}

Ptr<TypeExpression> Parser::ParseType() {
  String name = Match(Tag::Identifier).text;
  if (Look().tag == Tag::LeftBracket) {
    Match(Tag::LeftBracket);
    const auto& token = Look();
    Vector<Ptr<TypeExpression>> types = ParseTypeArguments();
    auto result = New<TypeExpression>(GetPos(token), name, types);
    if (types.empty()) {
      throw ParserException(Look().line, Look().column, "type error");
    } else {
      return result;
    }
  } else {
    return New<TypeExpression>(GetPos(Look()), name);
  }
}

Ptr<Ast> Parser::ParseReturn() {
  const Token& start = Look();
  Match(Tag::Return);
  Ptr<Ast> value = ParseOr();
  return New<Return>(GetPos(start), value);
}

Vector<Ptr<TypeExpression>> Parser::ParseTypeArguments() {
  Match(Tag::LeftBracket);
  Vector<Ptr<TypeExpression>> types;
  types.push_back(ParseType());
  while (!IsEof() && Look().tag != Tag::RightBracket) {
    Match(Tag::Comma);
    types.push_back(ParseType());
  }
  Match(Tag::RightBracket);
  return types;
}

Ptr<While> Parser::ParseWhile() {
  const Token& start = Look();
  Token token = Match(Tag::While);
  Ptr<Ast> condition = ParseOr();
  Ptr<Ast> body = ParseBlock();
  return New<While>(GetPos(start), condition, body);
}

Ptr<DefClass> Parser::ParseDefClass() {
  const Token& start = Look();
  Match(Tag::Class);
  String name = Match(Tag::Identifier).text;
  Vector<Ptr<Var>> fields;
  Vector<Ptr<Def>> methods;
  Match(Tag::LeftBrace);
  while (!IsEof() && Look().tag != Tag::RightBrace) {
    Access access = ParseAccess();

    if (Look().tag == Tag::Var) {
      // ParseVar field: Type
      auto field = ParseVarDeclaration();
      field->access = access;
      fields.push_back(field);
    } else if (Look().tag == Tag::Def) {
      // def method(args..) { }
      auto method = ParseDef();
      method->access = access;
      methods.push_back(method);
    } else {
      throw ParserException(Look().line, Look().column, "unexpected token");
    }
  }
  Match(Tag::RightBrace);
  return New<DefClass>(GetPos(start), name, fields, methods);
}

Ptr<DefModule> Parser::ParseDefModule() {
  const Token& start = Look();
  Match(Tag::Module);
  String name = Match(Tag::Identifier).text;
  Vector<Ptr<Var>> fields;
  Vector<Ptr<Def>> methods;
  Match(Tag::LeftBrace);
  while (!IsEof() && Look().tag != Tag::RightBrace) {
    Access access = ParseAccess();
    if (Look().tag == Tag::Var) {
      // ParseVar field: Type
      auto field = ParseVarDeclaration();
      field->access = access;
      fields.push_back(field);
    } else if (Look().tag == Tag::Def) {
      // def method(args..) { }
      auto method = ParseDef();
      method->access = access;
      methods.push_back(method);
    } else {
      throw ParserException(Look().line, Look().column, "unexpected token");
    }
  }
  Match(Tag::RightBrace);
  return New<DefModule>(GetPos(start), name, fields, methods);
}

Access Parser::ParseAccess() {
  if (Look().tag == Tag::Private) {
    Match(Tag::Private);
    return Access::Private;
  } else if (Look().tag == Tag::Protected) {
    Match(Tag::Protected);
    return Access::Protected;
  } else {
    return Access::Public;
  }
}
} // namespace cygni
