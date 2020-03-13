#include "Parser.h"

#include "AST.h"
#include "Error.h"
#include "util.h"

namespace SI {
using namespace SI::util;

#define IN_PLACE_TYPE(name) \
  std::in_place_type_t<name> {}

Parser::Parser(const std::string &s) { lexer_.init(s); }

Ptr<ASTNode> Parser::astNumber(const std::string &num) {
  return std::make_unique<ASTNode>(ASTNodeType::kNumber, IN_PLACE_TYPE(Number),
                                   num);
};

Ptr<ASTNode> Parser::astUnary(UnaryOpType type, Ptr<ASTNode> operand) {
  return std::make_unique<ASTNode>(
      ASTNodeType::kUnaryOp, IN_PLACE_TYPE(UnaryOp), type, std::move(operand));
}

Ptr<ASTNode> Parser::astBinOp(BinaryOpType type, Ptr<ASTNode> left,
                              Ptr<ASTNode> right) {
  return std::make_unique<ASTNode>(ASTNodeType::kBinaryOp,
                                   IN_PLACE_TYPE(BinaryOp), type,
                                   std::move(left), std::move(right));
}

Ptr<ASTNode> Parser::astBlock(std::vector<Ptr<ASTNode>> decls,
                              Ptr<ASTNode> compound) {
  return std::make_unique<ASTNode>(ASTNodeType::kBlock, IN_PLACE_TYPE(Block),
                                   std::move(decls), std::move(compound));
}

Ptr<ASTNode> Parser::astDeclaration(const std::string &val,
                                    DeclarationType type,
                                    Ptr<ASTNode> proceDecl) {
  return std::make_unique<ASTNode>(ASTNodeType::kDeclaration,
                                   IN_PLACE_TYPE(Declaration), val, type,
                                   std::move(proceDecl));
}

Ptr<ASTNode> Parser::astProgram(const std::string &name, Ptr<ASTNode> block) {
  return std::make_unique<ASTNode>(
      ASTNodeType::kProgram, IN_PLACE_TYPE(Program), name, std::move(block));
}

Ptr<ASTNode> Parser::astVar(const std::string &id) {
  return std::make_unique<ASTNode>(ASTNodeType::kID, IN_PLACE_TYPE(ID), id);
}

Ptr<ASTNode> Parser::astAssignment(Ptr<ASTNode> var, Ptr<ASTNode> expr) {
  return std::make_unique<ASTNode>(ASTNodeType::kAssignment,
                                   IN_PLACE_TYPE(Assignment), std::move(var),
                                   std::move(expr));
}

Ptr<ASTNode> Parser::astCompound(std::vector<Ptr<ASTNode>> stmtlist) {
  return std::make_unique<ASTNode>(
      ASTNodeType::kCompound, IN_PLACE_TYPE(Compound), std::move(stmtlist));
}

Ptr<ASTNode> Parser::astEmpty() {
  return std::make_unique<ASTNode>(ASTNodeType::kEmpty, IN_PLACE_TYPE(Empty));
}

Ptr<ASTNode> Parser::astProcedureDecl(const std::string &name,
                                      std::vector<Ptr<ASTNode>> params,
                                      Ptr<ASTNode> block) {
  return std::make_unique<ASTNode>(ASTNodeType::kProcedureDecl,
                                   IN_PLACE_TYPE(ProcedureDecl), name,
                                   std::move(params), std::move(block));
}

Ptr<ASTNode> Parser::astParam(const std::string &val, DeclarationType type) {
  return std::make_unique<ASTNode>(ASTNodeType::kParam, IN_PLACE_TYPE(Param),
                                   val, type);
}

Ptr<ASTNode> Parser::readNumber(const Token &tok) const {
  return astNumber(tok.val());
}

void Parser::eatKeyword(TokenType expect) {
  auto tok = readToken();
  if (tok->type() != expect) {
    unexpectedError(*tok, std::to_string(static_cast<std::size_t>(expect)));
  }
}

std::string Parser::eatVar() {
  auto tok = readToken();
  if (!tok->isVar()) {
    unexpectedError(*tok, "variable");
  }
  return tok->val();
}

// factor : PLUS factor
//            | MINUS factor
//            | INTEGER_CONST
//            | REAL_CONST
//            | LPAREN expr RPAREN
//            | variable
Ptr<ASTNode> Parser::factor() {
  auto tok = readToken();
  switch (tok->type()) {
    case TokenType::kNumber:
      return readNumber(*tok);
    case TokenType::kPlus:
      return astUnary(UnaryOpType::kPlus, factor());
    case TokenType::kMinus:
      return astUnary(UnaryOpType::kMinus, factor());
    case TokenType::kLparent: {
      auto ast = expr();
      eatKeyword(TokenType::kRparent);
      return ast;
    }
    case TokenType::kVar:
      return astVar(tok->val());
    default:
      unexpectedError(*tok, "factor");
  }
  SI_ASSERT(0);
  return nullptr;
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

Ptr<ASTNode> Parser::term() {
  auto left = factor();

  const auto *tok = peekToken();

  for (;;) {
    if (tok->type() == TokenType::kStar) {
      eatToken();
      left = astBinOp(BinaryOpType::kMul, std::move(left), factor());
      tok = peekToken();
      continue;
    }
    if (tok->type() == TokenType::kSlash) {
      eatToken();
      left = astBinOp(BinaryOpType::kRealdIV, std::move(left), factor());
      tok = peekToken();
      continue;
    }
    if (tok->type() == TokenType::kIntDiv) {
      eatToken();
      left = astBinOp(BinaryOpType::kIntDiv, std::move(left), factor());
      tok = peekToken();
      continue;
    }
    break;
  }
  return left;
}

Ptr<ASTNode> Parser::expr() {
  auto left = term();

  const auto *tok = peekToken();

  for (;;) {
    if (tok->type() == TokenType::kPlus) {
      eatToken();
      left = astBinOp(BinaryOpType::kAdd, std::move(left), term());
      tok = peekToken();
      continue;
    }
    if (tok->type() == TokenType::kMinus) {
      eatToken();
      left = astBinOp(BinaryOpType::kSub, std::move(left), term());
      tok = peekToken();
      continue;
    }
    break;
  }
  return left;
}

Ptr<ASTNode> Parser::assignmentStatement() {
  auto var = readToken();

  if (var->type() != TokenType::kVar) {
    unexpectedError(*var, "variable");
  }
  eatKeyword(TokenType::kAssign);
  return astAssignment(astVar(var->val()), expr());
}

// variable_declaration : ID (COMMA ID)* COLON type_spec
// type_spec : INTEGER | REAL
std::vector<Ptr<ASTNode>> Parser::variableDeclaration() {
  std::vector<Ptr<ASTNode>> decls;
  auto val = eatVar();
  decls.push_back(astDeclaration(val));

  auto tok = peekToken();
  while (tok->type() == TokenType::kComma) {
    eatKeyword(TokenType::kComma);
    decls.push_back(astDeclaration(eatVar()));
    tok = peekToken();
  }

  eatKeyword(TokenType::kColon);
  auto t = typeSpec();
  for (auto &decl : decls) {
    decl->fetch<Declaration>().setType(t);
  }

  return decls;
}

DeclarationType Parser::typeSpec() {
  auto tok = readToken();
  if (tok->type() == TokenType::kInteger) {
    return DeclarationType::kInt;
  }

  if (tok->type() == TokenType::kReal) {
    return DeclarationType::kReal;
  }

  unexpectedError(*tok, "type_spec");
  SI_ASSERT(0);
  return DeclarationType::kNull;
}

// formal_parameters : ID (COMMA ID)* COLON type_spec
std::vector<Ptr<ASTNode>> Parser::formalParameters() {
  std::vector<Ptr<ASTNode>> decls;
  auto val = eatVar();
  decls.push_back(astParam(val));

  auto tok = peekToken();
  while (tok->type() == TokenType::kComma) {
    eatKeyword(TokenType::kComma);
    decls.push_back(astParam(eatVar()));
    tok = peekToken();
  }

  eatKeyword(TokenType::kColon);
  auto t = typeSpec();
  for (auto &decl : decls) {
    decl->fetch<Param>().setType(t);
  }

  return decls;
}

// formal_parameter_list : formal_parameters
//                | formal_parameters SEMI formal_parameter_list
std::vector<Ptr<ASTNode>> Parser::formalParameterList() {
  std::vector<Ptr<ASTNode>> paramList;
  vectorMoveExtends(paramList, formalParameters());
  auto tok = peekToken();
  while (tok->type() == TokenType::kSemi) {
    eatKeyword(TokenType::kSemi);
    vectorMoveExtends(paramList, formalParameterList());
    tok = peekToken();
  }

  return paramList;
}

// declarations : (VAR (variable_declaration SEMI)+)*
//         | (PROCEDURE ID (LPAREN formal_parameter_list RPAREN)? SEMI block
//         SEMI)* | empty
std::vector<Ptr<ASTNode>> Parser::declarations() {
  std::vector<Ptr<ASTNode>> decls;
  auto tok = peekToken();
  for (;;) {
    if (tok->type() == TokenType::kVardecl) {
      eatKeyword(TokenType::kVardecl);
      do {
        auto decl = variableDeclaration();
        decls.insert(std::end(decls), std::make_move_iterator(decl.begin()),
                     std::make_move_iterator(decl.end()));
        eatKeyword(TokenType::kSemi);
        tok = peekToken();
      } while (tok->isVar());
    }

    if (tok->type() == TokenType::kProcedure) {
      eatKeyword(TokenType::kProcedure);
      auto name = eatVar();
      std::vector<Ptr<ASTNode>> params;
      tok = peekToken();
      if (tok->type() == TokenType::kLparent) {
        eatKeyword(TokenType::kLparent);
        params = formalParameterList();
        eatKeyword(TokenType::kRparent);
      }
      eatKeyword(TokenType::kSemi);
      decls.push_back(
          astDeclaration(name, DeclarationType::kProcedure,
                         astProcedureDecl(name, std::move(params), block())));
      eatKeyword(TokenType::kSemi);
      tok = peekToken();
    }

    break;
  }
  return decls;
}

// block : declarations compound_statement
Ptr<ASTNode> Parser::block() {
  auto decls = declarations();
  return astBlock(std::move(decls), compoundStatement());
}

// program : PROGRAM variable SEMI block DOT
Ptr<ASTNode> Parser::program() {
  eatKeyword(TokenType::kProgram);
  auto tok = readToken();
  if (!tok->isVar()) {
    unexpectedError(*tok, "variable");
  }

  eatKeyword(TokenType::kSemi);
  auto p = astProgram(tok->val(), block());
  eatKeyword(TokenType::kDot);
  return p;
}

// compound_statement : BEGIN statement_list END
Ptr<ASTNode> Parser::compoundStatement() {
  eatKeyword(TokenType::kBegin);
  auto compound = astCompound(statementList());
  // auto list = statementList();
  eatKeyword(TokenType::kEnd);
  return compound;
}

// statement_list : statement | statement SEMI statement_list
std::vector<Ptr<ASTNode>> Parser::statementList() {
  std::vector<Ptr<ASTNode>> list;
  list.push_back(statement());
  if (peekToken()->type() == TokenType::kSemi) {
    eatKeyword(TokenType::kSemi);
    vectorMoveExtends(list, statementList());
  }

  return list;
}

Ptr<ASTNode> Parser::empty() { return astEmpty(); }

// statement : compound_statement | assignment_statement | empty
Ptr<ASTNode> Parser::statement() {
  auto tok = peekToken();
  if (tok->type() == TokenType::kBegin) {
    return compoundStatement();
  }

  if (tok->isVar()) {
    return assignmentStatement();
  }

  return empty();
}

void Parser::unexpectedError(const Token &tok,
                             const std::string &expect) const {
  throw InterpreterError(
      kErrorUnexpectedToken,
      " expect : " + expect + ", but got token : " + tok.toString());
}

}  // namespace SI