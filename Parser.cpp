#include "Parser.h"

#include <assert.h>

#include <iostream>

namespace SI {
namespace Interpreter {

void Parser::eat(const TokenTypes::Type &t) {
  if (currentToken_.type() != t) {
    std::cerr << "except " << t << " but got " << currentToken_.type() << '\n';
    throw "bad factor";
  }
  advance();
}

int Parser::caculate(int num, const Token &op, const Token &token) const {
  if (op.type() == TokenTypes::kPlus) {
    num += token.value();
  } else if (op.type() == TokenTypes::kMinus) {
    num -= token.value();
  } else if (op.type() == TokenTypes::kMul) {
    num *= token.value();
  } else if (op.type() == TokenTypes::kDiv) {
    num /= token.value();
  } else if (op.type() == TokenTypes::kEof) {
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
  if (currentToken_.type() == TokenTypes::kNum) {
    auto ast = std::make_unique<AST>(currentToken_);
    advance();
    return ast;
  }

  if (currentToken_.type() == TokenTypes::kLparent) {
    advance();
    auto ast = expr();
    eat(TokenTypes::kRparent);
    return ast;
  }

  if (currentToken_.type() == TokenTypes::kPlus ||
      currentToken_.type() == TokenTypes::kMinus) {
    auto type = currentToken_.type();
    advance();
    auto ast = std::make_unique<AST>(currentToken_);
    ast->add(factor());
    return ast;
  }

  if (currentToken_.type() == TokenTypes::kVar) {
    auto ast = std::make_unique<AST>(currentToken_)
    advance();
    return var;
  }

  throw "bad factor";
}

std::unique_ptr<AST> Parser::term() {
  auto left = factor();
  while (currentToken_.type() != TokenTypes::kEof) {
    if (currentToken_.type() != TokenTypes::kMul && currentToken_.type() != TokenTypes::kDiv) {
      break;
    }
    auto root = std::make_unique<AST>(currentToken_);
    eat(currentToken_.type());
    auto right = factor();
    root->add(std::move(left));
    root->add(std::move(right));
    left = std::move(root);

    // if (currentToken_.type() == TokenTypes::kMul) {
    //   advance();
    //   auto right = factor();
    //   left.reset(new BinOp(TokenTypes::kMul, std::move(left), std::move(right)));
    // } else if (currentToken_.type() == TokenTypes::kDiv) {
    //   advance();
    //   auto right = factor();
    //   left.reset(new BinOp(TokenTypes::kDiv, std::move(left), std::move(right)));
    // } else {
    //   break;
    // }
  }
  return left;
}

std::unique_ptr<AST> Parser::expr() {
  auto left = term();
  while (currentToken_.type() != TokenTypes::kEof) {
    if (currentToken_.type() != TokenTypes::kPlus && currentToken_.type() != TokenTypes::kMinus) {
      break;
    }
    auto root = std::make_unique<AST>(currentToken_);
    eat(currentToken_.type());
    auto right = factor();
    root->add(std::move(left));
    root->add(std::move(right));
    left = std::move(root);
  }
  return left;



  // while (currentToken_.type() != TokenTypes::kEof) {

  //   if (currentToken_.type() == TokenTypes::kPlus) {
  //     advance();
  //     auto right = term();
  //     left.reset(new BinOp(TokenTypes::kPlus, std::move(left), std::move(right)));
  //   } else if (currentToken_.type() == TokenTypes::kMinus) {
  //     advance();
  //     auto right = term();
  //     left.reset(new BinOp(TokenTypes::kMinus, std::move(left), std::move(right)));
  //   } else {
  //     break;
  //   }
  // }

  return left;
}

std::unique_ptr<AST> Parser::assignmentStatement() {
  if (currentToken_.type() != TokenTypes::kVar) {
    throw "bad factor";
  }

  // auto var = std::make_unique<Var>(currentToken_.varval());
  auto var = std::makd_unique<AST>(currentToken_);
  // advance();
  eat(currentToken_.type());
  auto assign = std::make_unique<AST>(currentToken_);
  eat(TokenTypes::kAssign);
  auto e = expr();
  assign->add(std::move(var));
  assign->add(std::move(e))
  return assign;
  // return std::make_unique<Assign>(std::move(var), std::move(e));
}

std::unique_ptr<AST> Parser::program() {
  eat(TokenTypes::kProgram);
  eat(TokenTypes::kVar);
  eat(TokenTypes::kSemi);
  auto com = compoundStatement();
  eat(TokenTypes::kDot);
  return com;
}

std::unique_ptr<AST> Parser::compoundStatement() {
  eat(TokenTypes::kBegin);
  auto list = statementList();
  eat(TokenTypes::kEnd);
  return list;
}

std::unique_ptr<AST> Parser::empty() { return std::make_unique<AST>(); }

std::unique_ptr<AST> Parser::statement() {
  if (currentToken_.type() == TokenTypes::kBegin) {
    return compoundStatement();
  }

  if (currentToken_.type() == TokenTypes::kVar) {
    return assignmentStatement();
  }

  return empty();
}

std::unique_ptr<AST> Parser::statementList() {
  auto com = Token(TyokenTypes::kCompound);
  auto list = std::make_unique<AST>(com);
  list->add(statement());
  while (currentToken_.type() == TokenTypes::kSemi) {
    advance();
    list->add(statement());
  }

  return list;
}

std::unique_ptr<AST> Parser::block() {
  eat(TokenTypes::kVardecl);
  // auto block = std::make_unique<Block>();
  auto block =  std::make_unique<AST>(TokenTypes::kBlock);
  block->add(variableDeclaration());
  eat(TokenTypes::kSemi);
  while (currentToken_.type() == TokenTypes::kVar) {
    block->add(variableDeclaration());
    eat(TokenTypes::kSemi);
  }
  block->add(compoundStatement());
  return block;
}

std::unique_ptr<AST> Parser::variableDeclaration() {
  assert(currentToken_.type() == TokenTypes::kVar);
  std::string id = currentToken_.varval();
  eat(TokenTypes::kVar);
  eat(TokenTypes::kColon);
  eat(TokenTypes::kInteger);
  return std::make_unique<AST>(id, VarDecl::kInteger);
}

}  // namespace Interpreter
}  // namespace SI