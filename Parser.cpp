#include "Parser.h"

#include <assert.h>

#include <iostream>

namespace SI {
namespace Interpreter {

void Parser::eat(const Token::Type &t) {
  if (currentToken_.type() != t) {
    throw "bad factor";
  }
  advance();
}

int Parser::caculate(int num, const Token &op, const Token &token) const {
  if (op.type() == Token::kPlus) {
    num += token.value();
  } else if (op.type() == Token::kMinus) {
    num -= token.value();
  } else if (op.type() == Token::kMul) {
    num *= token.value();
  } else if (op.type() == Token::kDiv) {
    num /= token.value();
  } else if (op.type() == Token::kEof) {
    return num;
  } else {
    assert(0);
  }
  return num;
}

Parser::Parser(const std::string &formula) {
  lexer_.init(formula);
  advance();
}

std::unique_ptr<AST> Parser::factor() {
  if (currentToken_.type() == Token::kNum) {
    std::unique_ptr<AST> num(new Num(currentToken_.value()));
    advance();
    return num;
  }

  if (currentToken_.type() == Token::kLparent) {
    advance();
    auto ast = expr();
    eat(Token::kRparent);
    return ast;
  }

  if (currentToken_.type() == Token::kPlus ||
      currentToken_.type() == Token::kMinus) {
    auto type = currentToken_.type();
    advance();
    return std::unique_ptr<AST>(new UnaryOp(type, std::move(factor())));
  }
  throw "bad factor";
}

std::unique_ptr<AST> Parser::term() {
  auto left = factor();
  while (currentToken_.type() != Token::kEof) {
    if (currentToken_.type() == Token::kMul) {
      advance();
      auto right = factor();
      left.reset(new BinOp(Token::kMul, std::move(left), std::move(right)));
    } else if (currentToken_.type() == Token::kDiv) {
      advance();
      auto right = factor();
      left.reset(new BinOp(Token::kDiv, std::move(left), std::move(right)));
    } else {
      break;
    }
  }
  return left;
}

std::unique_ptr<AST> Parser::expr() {
  auto left = term();
  while (currentToken_.type() != Token::kEof) {
    if (currentToken_.type() == Token::kPlus) {
      advance();
      auto right = term();
      left.reset(new BinOp(Token::kPlus, std::move(left), std::move(right)));
    } else if (currentToken_.type() == Token::kMinus) {
      advance();
      auto right = term();
      left.reset(new BinOp(Token::kMinus, std::move(left), std::move(right)));
    } else {
      break;
    }
  }

  return left;
}

std::unique_ptr<AST> Parser::assignmentStatement() {
  if (currentToken_.type() != Token::kVar) {
    throw "bad factor";
  }

  auto var = std::unique_ptr<Var>(new Var(currentToken_.varval()));
  advance();
  eat(Token::kAssign);
  auto e = expr();
  return std::unique_ptr<AST>(new Assign(std::move(var), std::move(e)));
}

std::unique_ptr<AST> Parser::program() {
  auto com = compoundStatement();
  eat(Token::kDot);
  return com;
}

std::unique_ptr<AST> Parser::compoundStatement() {
  eat(Token::kBegin);
  auto list = statementList();
  eat(Token::kEnd);
  return list;
}

std::unique_ptr<AST> Parser::statementList() {
  auto list = std::unique_ptr<Compound>(new Compound());
  list->add(assignmentStatement());
  while (currentToken_.type() == Token::kSemi) {
    advance();
    list->add(assignmentStatement());
  }

  return list;
}

}  // namespace Interpreter
}  // namespace SI