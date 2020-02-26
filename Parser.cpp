#include "Parser.h"

#include "AST.h"
#include "util.h"

namespace SI {
namespace Interpreter {
using namespace SI::util;

Parser::Parser(const std::string &s) { lexer_.init(s); }

ASTNodePtr Parser::astInt(long num) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kInt);
  ast->setNumber(num);
  return ast;
};

ASTNodePtr Parser::astUnary(ASTNodeType type, ASTNodePtr operand) {
  auto ast = std::make_unique<ASTNode>(type);
  ast->setOperand(std::move(operand));
  return ast;
}

ASTNodePtr Parser::astBinOp(ASTNodeType type, ASTNodePtr left,
                            ASTNodePtr right) {
  auto ast = std::make_unique<ASTNode>(type);
  ast->setLeft(std::move(left));
  ast->setRight(std::move(right));
  return ast;
}

ASTNodePtr Parser::astBlock(std::vector<ASTNodePtr> decls,
                            ASTNodePtr compound) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kBlock);
  ast->setDecls(std::move(decls));
  ast->setCompound(std::move(compound));
  return ast;
}

ASTNodePtr Parser::astVardecl(const std::string &val) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kDeclaration);
  ast->setVar(val);
  return ast;
}

ASTNodePtr Parser::astProgram(ASTNodePtr block) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kProgram);
  ast->setBlock(std::move(block));
  return ast;
}

ASTNodePtr Parser::astVar(const std::string &id) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kVar);
  ast->setVar(id);
  return ast;
}

ASTNodePtr Parser::astAssignment(ASTNodePtr var, ASTNodePtr expr) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kAssignment);
  ast->setLeft(std::move(var));
  ast->setRight(std::move(expr));
  return ast;
}

ASTNodePtr Parser::astCompound(std::vector<ASTNodePtr> stmtlist) {
  auto ast = std::make_unique<ASTNode>(ASTNodeType::kCompound);
  ast->setCompounds(std::move(stmtlist));
  return ast;
}

ASTNodePtr Parser::readNumber(const Token &tok) const {
  SI_ASSERT(tok.type() == TokenType::kNumber);
  return astInt(std::stol(tok.val()));
}

void Parser::eatKeyword(TokenId expect) {
  auto tok = readToken();
  if (tok->type() != TokenType::kKeyword || tok->id() != expect) {
    std::cout << "text remaning : " << lexer_.remaning() << '\n';
    SI_ASSERT_MSG(0, "unexpect keyword , expect : " +
                         std::to_string(static_cast<std::size_t>(expect)) +
                         ", we but got : " +
                         std::to_string(static_cast<std::size_t>(tok->id())));
  }
}

std::string Parser::eatVar() {
  auto tok = readToken();
  if (!tok->isVar()) {
    SI_ASSERT_MSG(0, "current token's not var");
  }
  return tok->val();
}

ASTNodePtr Parser::factor() {
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
          SI_ASSERT_MSG(0, "bad token id");
      }
    }

    default:
      SI_ASSERT_MSG(0, "bad factor");
  }
  SI_ASSERT(0);
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
  SI_ASSERT(peekToken_.has_value());
  peekToken_ = std::nullopt;
}

bool Parser::next(ASTNodeType type) const {
  // if (type == ASTNodeType::kCompound) {
  //   auto tok = peekToken();
  //   return tok->isVar();
  // }

  // if (type == ASTNodeType::kDeclaration) {
  // }

  // SI_ASSERT_MSG(0, "unsupported more() type");
  // TODO : implement
  SI_ASSERT(0);
  return false;
}

ASTNodePtr Parser::term() {
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

ASTNodePtr Parser::expr() {
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

ASTNodePtr Parser::assignmentStatement() {
  auto var = readToken();

  if (var->type() != TokenType::kId) {
    SI_ASSERT_MSG(0, "bad assignmentStatement");
  }

  eatKeyword(TokenId::kAssign);
  // auto assign = readToken();
  // if (!assign->isKeyword(TokenId::kAssign)) {
  //   SI_ASSERT_MSG(0, "bad assignmentStatement");
  // }

  return astAssignment(astVar(var->val()), expr());
}

// variable_declaration : ID (COMMA ID)* COLON type_spec
// type_spec : INTEGER | REAL
std::vector<ASTNodePtr> Parser::variableDeclaration() {
  std::vector<ASTNodePtr> decls;
  auto val = eatVar();
  decls.push_back(astVardecl(val));

  auto tok = peekToken();
  while (tok->isKeyword(TokenId::kComma)) {
    eatKeyword(TokenId::kComma);
    decls.push_back(astVardecl(eatVar()));
    tok = peekToken();
  }

  eatKeyword(TokenId::kColon);
  typeSpec();

  return decls;
}

ASTNodePtr Parser::typeSpec() {
  eatKeyword(TokenId::kInteger);
  return nullptr;
}

// declarations : VAR (variable_declaration SEMI)+ | empty
std::vector<ASTNodePtr> Parser::declarations() {
  std::vector<ASTNodePtr> decls;
  auto tok = peekToken();
  if (!tok->isKeyword(TokenId::kVardecl)) {
    return decls;
  }
  eatKeyword(TokenId::kVardecl);

  auto decl = variableDeclaration();
  if (decl.empty()) {
    SI_ASSERT_MSG(0, "bad declaration");
  }

  do {
    eatKeyword(TokenId::kSemi);
    decls.insert(std::end(decls), std::make_move_iterator(decl.begin()),
                 std::make_move_iterator(decl.end()));
    decl = variableDeclaration();
  } while (!decl.empty());

  return decls;
}

// block : declarations compound_statement
ASTNodePtr Parser::block() {
  return astBlock(declarations(), compoundStatement());
}

// program : PROGRAM variable SEMI block DOT
ASTNodePtr Parser::program() {
  eatKeyword(TokenId::kProgram);
  auto tok = readToken();
  if (!tok->isVar()) {
    SI_ASSERT_MSG(0, "bad program");
  }

  // The program name, no use for now
  // auto var = astVar(tok->val());
  eatKeyword(TokenId::kSemi);
  auto p = astProgram(block());
  eatKeyword(TokenId::kDot);
  return p;
}

// compound_statement : BEGIN statement_list END
ASTNodePtr Parser::compoundStatement() {
  eatKeyword(TokenId::kBegin);
  auto compound = astCompound(statementList());
  // auto list = statementList();
  eatKeyword(TokenId::kEnd);
  return compound;
}

// statement_list : statement | statement SEMI statement_list
std::vector<ASTNodePtr> Parser::statementList() {
  std::vector<ASTNodePtr> list;
  list.push_back(statement());
  if (peekToken()->isKeyword(TokenId::kSemi)) {
    eatKeyword(TokenId::kSemi);
    SI::util::vectorExtends(list, statementList());
  }

  return list;
}

ASTNodePtr Parser::empty() {
  return std::make_unique<ASTNode>(ASTNodeType::kEmpty);
}

// statement : compound_statement | assignment_statement | empty
ASTNodePtr Parser::statement() {
  auto tok = peekToken();
  if (tok->isKeyword(TokenId::kBegin)) {
    // todo : use next(type)
    return compoundStatement();
  }

  if (tok->isVar()) {
    // todo : use next(type)
    return assignmentStatement();
  }

  return empty();
}

}  // namespace Interpreter
}  // namespace SI