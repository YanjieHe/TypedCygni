#include "Parser.hpp"

Parser::Parser(String path, Vector<Token> tokens)
    : path{std::move(path)}, tokens{std::move(tokens)}, offset{0} {}

Program Parser::ParseProgram() {
  Vector<Ptr<DefClass>> classes;
  Vector<Ptr<DefModule>> modules;
  while (!IsEof()) {
    if (Look().tag == Tag::Class) {
      classes.push_back(ParseDefClass());
    } else {
      modules.push_back(ParseDefModule());
    }
  }
  return Program(path, classes, modules);
}

Ptr<Ast> Parser::Statement() {
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

Ptr<Ast> Parser::ParseAssign() {
  const Token& start = Look();
  Ptr<Ast> x = ParseOr();
  if (Look().tag == Tag::Assign) {
    Token t = Match(Tag::Assign);
    Ptr<Ast> y = ParseOr();
    return New<Binary>(GetPos(start), Kind::Assign, x, y);
  } else {
    return x;
  }
}

Ptr<Ast> Parser::ParseOr() {
  const Token& start = Look();
  Ptr<Ast> x = ParseAnd();
  while (Look().tag == Tag::Or) {
    Token t = Match(Tag::Or);
    Ptr<Ast> y = ParseAnd();
    x = New<Binary>(GetPos(start), Kind::Or, x, y);
  }
  return x;
}

Ptr<Ast> Parser::ParseAnd() {
  const Token& start = Look();
  Ptr<Ast> x = ParseEquality();
  while (Look().tag == Tag::And) {
    Token t = Match(Tag::And);
    Ptr<Ast> y = ParseEquality();
    x = New<Binary>(GetPos(start), Kind::And, x, y);
  }
  return x;
}

Ptr<Ast> Parser::ParseEquality() {
  const Token& start = Look();
  Ptr<Ast> x = ParseRelation();
  while (Look().tag == Tag::Equal || Look().tag == Tag::NotEqual) {
    Token t = Look();
    Move();
    Ptr<Ast> y = ParseRelation();
    if (t.tag == Tag::Equal) {
      x = New<Binary>(GetPos(start), Kind::Equal, x, y);
    } else {
      x = New<Binary>(GetPos(start), Kind::NotEqual, x, y);
    }
  }
  return x;
}

Ptr<Ast> Parser::ParseRelation() {
  const Token& start = Look();
  Ptr<Ast> x = ParseExpr();
  if (Look().tag == Tag::GreaterThan || Look().tag == Tag::LessThan ||
      Look().tag == Tag::GreaterThanOrEqual ||
      Look().tag == Tag::LessThanOrEqual) {
    Token t = Look();
    Move();
    Ptr<Ast> y = ParseExpr();
    if (t.tag == Tag::GreaterThan) {
      return New<Binary>(GetPos(start), Kind::GreaterThan, x, y);
    } else if (t.tag == Tag::LessThan) {
      return New<Binary>(GetPos(start), Kind::LessThan, x, y);
    } else if (t.tag == Tag::GreaterThanOrEqual) {
      return New<Binary>(GetPos(start), Kind::GreaterThanOrEqual, x, y);
    } else {
      return New<Binary>(GetPos(start), Kind::LessThanOrEqual, x, y);
    }
  } else {
    return x;
  }
}

Ptr<Ast> Parser::ParseExpr() {
  const Token& start = Look();
  Ptr<Ast> x = ParseTerm();
  while (Look().tag == Tag::Add || Look().tag == Tag::Subtract) {
    Token t = Look();
    Move();
    Ptr<Ast> y = ParseTerm();
    if (t.tag == Tag::Add) {
      x = New<Binary>(GetPos(start), Kind::Add, x, y);
    } else {
      x = New<Binary>(GetPos(start), Kind::Subtract, x, y);
    }
  }
  return x;
}

Ptr<Ast> Parser::ParseTerm() {
  const Token& start = Look();
  Ptr<Ast> x = ParseUnary();
  while (Look().tag == Tag::Multiply || Look().tag == Tag::Divide) {
    Token t = Look();
    Move();
    Ptr<Ast> y = ParseUnary();
    if (t.tag == Tag::Multiply) {
      x = New<Binary>(GetPos(start), Kind::Multiply, x, y);
    } else {
      x = New<Binary>(GetPos(start), Kind::Divide, x, y);
    }
  }
  return x;
}

Ptr<Ast> Parser::ParseUnary() {
  const Token& start = Look();
  if (Look().tag == Tag::Add) {
    Move();
    Ptr<Ast> x = ParseUnary();
    return New<Binary>(GetPos(start), Kind::UnaryPlus, x);
  } else if (Look().tag == Tag::Subtract) {
    Move();
    Ptr<Ast> x = ParseUnary();
    return New<Binary>(GetPos(start), Kind::UnaryMinus, x);
  } else if (Look().tag == Tag::Not) {
    Move();
    Ptr<Ast> x = ParseUnary();
    return New<Binary>(GetPos(start), Kind::Not, x);
  } else {
    return ParsePostfix();
  }
}

Ptr<Ast> Parser::ParsePostfix() {
  Ptr<Ast> x = ParseFactor();
  while (Look().tag == Tag::LeftParenthesis || Look().tag == Tag::LeftBracket) {
    const Token& start = Look();

    if (Look().tag == Tag::LeftParenthesis) {
      Vector<Ptr<Ast>> arguments;
      Match(Tag::LeftParenthesis);
      if (Look().tag == Tag::RightParenthesis) {
        Match(Tag::RightParenthesis);
        x = New<Call>(GetPos(start), x, arguments);
      } else {
        arguments.push_back(ParseOr());
        while (!IsEof() && Look().tag != Tag::RightParenthesis) {
          Match(Tag::Comma);
          arguments.push_back(ParseOr());
        }
        Match(Tag::RightParenthesis);
        x = New<Call>(GetPos(start), x, arguments);
      }
    } else if (Look().tag == Tag::LeftBracket) {
      // TO DO
    }
  }
  return x;
}

Ptr<Ast> Parser::ParseFactor() {
  if (Look().tag == Tag::LeftParenthesis) {
    Move();
    Ptr<Ast> x = ParseOr();
    Match(Tag::RightParenthesis);
    return x;
  } else if (Look().tag == Tag::Integer) {
    String v = Look().text;
    const Token& start = Look();
    Move();
    return New<Constant>(GetPos(start), Constant::ConstantType::Int32Type, v);
  } else if (Look().tag == Tag::Float) {
    String v = Look().text;
    const Token& start = Look();
    Move();
    return New<Constant>(GetPos(start), Constant::ConstantType::FloatType, v);
  } else if (Look().tag == Tag::String) {
    String v = Look().text;
    const Token& start = Look();
    Move();
    return New<Constant>(GetPos(start), Constant::ConstantType::StringType, v);
  } else if (Look().tag == Tag::True) {
    const Token& start = Look();
    Move();
    Ptr<Ast> x = New<Constant>(GetPos(start),
                               Constant::ConstantType::BooleanType, "true");
    return x;
  } else if (Look().tag == Tag::False) {
    const Token& start = Look();
    Move();
    Ptr<Ast> x = New<Constant>(GetPos(start),
                               Constant::ConstantType::BooleanType, "false");
    return x;
  } else if (Look().tag == Tag::Identifier) {
    String name = Look().text;
    const Token& start = Look();
    Move();
    Ptr<Ast> x = New<Name>(GetPos(start), name);
    return x;
  } else {
    throw ParserException(Look().line, Look().column, "factor");
  }
}

Ptr<Ast> Parser::ParseBlock() {
  const Token& start = Look();
  Match(Tag::LeftBrace);
  Vector<Ptr<Ast>> expressions;
  while (!IsEof() && Look().tag != Tag::RightBrace) {
    expressions.push_back(Statement());
  }
  Match(Tag::RightBrace);
  return New<Block>(GetPos(start), expressions);
}

Ptr<Ast> Parser::IfStatement() {
  const Token& start = Look();
  Match(Tag::If);
  Ptr<Ast> condition = ParseOr();
  Ptr<Ast> ifTrue = ParseBlock();
  if (Look().tag == Tag::Else) {
    Match(Tag::Else);
    if (Look().tag == Tag::If) {
      Ptr<Ast> chunk = IfStatement();
      return New<IfElse>(GetPos(start), condition, ifTrue, chunk);
    } else {
      Ptr<Ast> chunk = ParseBlock();
      return New<IfElse>(GetPos(start), condition, ifTrue, chunk);
    }
  } else {
    return New<IfThen>(GetPos(start), condition, ifTrue);
  }
}

Ptr<Var> Parser::ParseVar() {
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
    if (result) {
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