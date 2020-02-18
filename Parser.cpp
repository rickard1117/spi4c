#include "Parser.h"

#include <assert.h>

#include "AST.h"

namespace SI {
namespace Interpreter {

Parser::Parser(const std::string &formula) { lexer_.init(formula); }

std::unique_ptr<ASTNode> Parser::astInt(long num) const {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kInt);
  ast->setLong(num);
  return ast;
};

std::unique_ptr<ASTNode> Parser::readNumber(const Token &tok) const {
  assert(tok.type() == TokenType::kNumber);
  return astInt(std::stol(tok.val()));
}

std::unique_ptr<ASTNode> Parser::astUnary(
    ASTNodeType type, std::unique_ptr<ASTNode> operand) const {
  auto ast = std::make_unique<ASTNode>(type);
  ast->setOperand(std::move(operand));
  return ast;
}

void Parser::eatKeyword(TokenId expect) {
  auto tok = readToken();
  if (tok->type() != TokenType::kKeyword || tok->id() != expect) {
    throw "unexpect keyword";
  }
}

std::unique_ptr<ASTNode> Parser::factor() {
  auto tok = readToken();
  switch (tok->type()) {
    case TokenType::kNumber:
      return readNumber(*tok);
    case TokenType::kKeyword: {
      switch (tok->id()) {
        case TokenId::kPlus:
          return astUnary(ASTNodeType::kUnaryOpPlus, factor());
        case TokenId::kMinus:
          return astUnary(ASTNodeType::kUnaryOpMinus, factor());
        case TokenId::kLparent: {
          auto ast = expr();
          eatKeyword(TokenId::kRparent);
          return ast;
        }
        default:
          throw "bad token id";
      }
    }

    default:
      throw "bad factor";
  }
  assert(0);
}

std::unique_ptr<Token> Parser::readToken() {
  if (peekToken_.has_value()) {
    auto tok = std::move(peekToken_.value());
    peekToken_ = std::nullopt;
    return tok;
  }
  return lexer_.getNextToken();
}

const Token *Parser::peekToken() {
  if (!peekToken_.has_value()) {
    peekToken_ = lexer_.getNextToken();
  }
  return peekToken_.value().get();
}

void Parser::eatToken() {
  assert(peekToken_.has_value());
  peekToken_ = std::nullopt;
}

std::unique_ptr<ASTNode> Parser::astBinOp(
    ASTNodeType type, std::unique_ptr<ASTNode> left,
    std::unique_ptr<ASTNode> right) const {
  auto ast = std::make_unique<ASTNode>(type);
  ast->setLeft(std::move(left));
  ast->setRight(std::move(right));
  return ast;
}

std::unique_ptr<ASTNode> Parser::term() {
  auto left = factor();

  const auto *tok = peekToken();

  while (tok->type() == TokenType::kKeyword) {
    if (tok->id() == TokenId::kStar) {
      eatToken();
      left = astBinOp(ASTNodeType::kMul, std::move(left), factor());
      tok = peekToken();
      continue;
    }
    if (tok->id() == TokenId::kSlash) {
      eatToken();
      left = astBinOp(ASTNodeType::kDiv, std::move(left), factor());
      tok = peekToken();
      continue;
    }
    break;
  }
  return left;
}

std::unique_ptr<ASTNode> Parser::expr() {
  auto left = term();

  const auto *tok = peekToken();

  while (tok->type() == TokenType::kKeyword) {
    if (tok->id() == TokenId::kPlus) {
      eatToken();
      left = astBinOp(ASTNodeType::kAdd, std::move(left), term());
      tok = peekToken();
      continue;
    }
    if (tok->id() == TokenId::kMinus) {
      eatToken();
      left = astBinOp(ASTNodeType::kSub, std::move(left), term());
      tok = peekToken();
      continue;
    }
    break;
  }
  return left;
}

std::unique_ptr<ASTNode> Parser::astVar(const std::string &id) const {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kVar);
  ast->setVar(id);
  return ast;
}

std::unique_ptr<ASTNode> Parser::astAssignment(
    std::unique_ptr<ASTNode> var, std::unique_ptr<ASTNode> expr) const {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kAssignment);
  ast->setLeft(std::move(var));
  ast->setRight(std::move(expr));
  return ast;
}

std::unique_ptr<ASTNode> Parser::assignmentStatement() {
  auto var = readToken();

  if (var->type() != TokenType::kId) {
    throw "bad assignmentStatement";
  }

  auto assign = readToken();
  if (!assign->isKeyword(TokenId::kAssign)) {
    throw "bad assignmentStatement";
  }

  return astAssignment(astVar(var->val()), expr());
}

std::unique_ptr<ASTNode> Parser::program() {
  auto com = compoundStatement();
  eatKeyword(TokenId::kDot);
  return com;
}

std::unique_ptr<ASTNode> Parser::compoundStatement() {
  eatKeyword(TokenId::kBegin);
  auto list = statementList();
  eatKeyword(TokenId::kEnd);
  return list;
}

std::unique_ptr<ASTNode> Parser::astCompound() const {
  return std::make_unique<ASTNode>(ASTNodeType::kCompound);
}

std::unique_ptr<ASTNode> Parser::statementList() {
  auto stmt = statement();
  const auto *tok = peekToken();
  if (!tok->isKeyword(TokenId::kSemi)) {
    return stmt;
  }

  auto list = astCompound();
  list->addStatement(std::move(stmt));
  while (tok->isKeyword(TokenId::kSemi)) {
    eatToken();
    list->addStatement(statement());
    tok = peekToken();
  }

  return list;
}

std::unique_ptr<ASTNode> Parser::empty() {
  return std::make_unique<ASTNode>(ASTNodeType::kEmpty);
}

std::unique_ptr<ASTNode> Parser::statement() {
  auto tok = peekToken();
  if (tok->isKeyword(TokenId::kBegin)) {
    return compoundStatement();
  }

  if (tok->isKeyword(TokenId::kAssign)) {
    return assignmentStatement();
  }

  return empty();
}

// std::unique_ptr<AST> Parser::block() {
//   eat(TokenTypes::kVardecl);
//   // auto block = std::make_unique<Block>();
//   auto block = std::make_unique<AST>(TokenTypes::kBlock);
//   block->add(variableDeclaration());
//   eat(TokenTypes::kSemi);
//   while (currentToken_.type() == TokenTypes::kVar) {
//     block->add(variableDeclaration());
//     eat(TokenTypes::kSemi);
//   }
//   block->add(compoundStatement());
//   return block;
// }

// std::unique_ptr<AST> Parser::variableDeclaration() {
//   assert(currentToken_.type() == TokenTypes::kVar);
//   std::string id = currentToken_.varval();
//   eat(TokenTypes::kVar);
//   eat(TokenTypes::kColon);
//   eat(TokenTypes::kInteger);
//   return std::make_unique<AST>(id, VarDecl::kInteger);
// }

}  // namespace Interpreter
}  // namespace SI