#ifndef PARSER_H_
#define PARSER_H_

#include <memory>
#include <optional>
#include <string>

#include "AST.h"
#include "Lexer.h"
#include "Token.h"

namespace SI {
namespace Interpreter {
class Parser {
 public:
  Parser(const std::string &s);

  // expr : term ((PLUS | MINUS) term)*
  ASTNodePtr expr();

  // term : factor ((MUL | INTEGER_DIV | FLOAT_DIV) factor)*
  ASTNodePtr term();

  // factor : PLUS factor
  //          | MINUS factor
  //          | INTEGER_CONST
  //          | REAL_CONST
  //          | LPAREN expr RPAREN
  //          | variable
  ASTNodePtr factor();

  // assignment_statement : variable ASSIGN expr
  ASTNodePtr assignmentStatement();

  // compound_statement : BEGIN statement_list END
  ASTNodePtr compoundStatement();

  // statement_list : statement
  //                  | statement SEMI statement_list
  std::vector<ASTNodePtr> statementList();

  // program : PROGRAM variable SEMI block DOT
  ASTNodePtr program();

  // statement : compound_statement
  //             | assignment_statement
  //             | empty
  ASTNodePtr statement();

  // empty
  ASTNodePtr empty();

  // block : declarations compound_statement
  ASTNodePtr block();

  // declarations : VAR (variable_declaration SEMI)+
  //                | empty
  std::vector<ASTNodePtr> declarations();

  // variable_declaration : ID (COMMA ID)* COLON type_spec
  std::vector<ASTNodePtr> variableDeclaration();

  ASTNodePtr typeSpec();

 private:
  std::unique_ptr<Token> readToken();
  const Token *peekToken();
  void eatToken();
  // check if the next parsed node is of the specified type
  bool next(ASTNodeType type) const;
  ASTNodePtr readNumber(const Token &tok) const;

  // type : kInt
  // field : numval_
  // child : none
  static ASTNodePtr astInt(long num);

  // type : kUnaryOpPlus | kUnaryOpMinus
  // field : operand_
  // children : kUnary | kInt
  static ASTNodePtr astUnary(ASTNodeType type, ASTNodePtr operand);

  // type : kAdd | kSub | kMul | kDiv
  // field : left_ right_
  // children : kInt | kUnary | kAdd | kSub | kMul | kDiv
  static ASTNodePtr astBinOp(ASTNodeType type, ASTNodePtr left,
                             ASTNodePtr right);

  // type : kVar
  // field : varId_
  // children : none
  static ASTNodePtr astVar(const std::string &id);

  // type : kAssignment
  // field : left_ right_
  // children : left_(kVar) right_(kUnary | kInt | kAdd | kSub | kMul | kDiv)
  static ASTNodePtr astAssignment(ASTNodePtr var, ASTNodePtr expr);

  // type : kCompound
  // field : compounds_
  // children : kAssignment
  static ASTNodePtr astCompound(std::vector<ASTNodePtr> stmtlist);

  // type : kBlock
  // field : declarations_ compounds_
  // children : kDeclaration
  static ASTNodePtr astBlock(std::vector<ASTNodePtr> decls,
                             ASTNodePtr compound);

  // type : kDeclaration
  // field : left_ right_
  // children : left(kVar) right(kType)
  static ASTNodePtr astVardecl(const std::string &val);

  // type : kProgram
  // field ; block_
  // children : block
  static ASTNodePtr astProgram(ASTNodePtr block);

  void eatKeyword(TokenId expect);
  std::string eatVar();

  Lexer lexer_;
  std::optional<std::unique_ptr<Token>> peekToken_;
};
}  // namespace Interpreter
}  // namespace SI
#endif  // PARSER_H_