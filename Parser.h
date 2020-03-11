#ifndef PARSER_H_
#define PARSER_H_

#include <memory>
#include <optional>
#include <string>

#include "AST.h"
#include "Lexer.h"
#include "Token.h"

namespace SI {
class Parser {
 public:
  Parser(const std::string &s);
  Ptr<ASTNode> expr();
  Ptr<ASTNode> term();
  Ptr<ASTNode> factor();
  Ptr<ASTNode> assignmentStatement();
  Ptr<ASTNode> compoundStatement();
  std::vector<Ptr<ASTNode>> statementList();
  Ptr<ASTNode> program();
  Ptr<ASTNode> statement();
  Ptr<ASTNode> empty();
  Ptr<ASTNode> block();
  std::vector<Ptr<ASTNode>> formalParameters();
  std::vector<Ptr<ASTNode>> formalParameterList();
  std::vector<Ptr<ASTNode>> declarations();
  std::vector<Ptr<ASTNode>> variableDeclaration();
  DeclarationType typeSpec();

 private:
  void unexpectedError(const Token &tok, const std::string &expect) const;
  std::unique_ptr<Token> readToken();
  const Token *peekToken();
  void eatToken();
  Ptr<ASTNode> readNumber(const Token &tok) const;

  static Ptr<ASTNode> astNumber(const std::string &num);
  static Ptr<ASTNode> astUnary(UnaryOpType type, Ptr<ASTNode> operand);
  static Ptr<ASTNode> astBinOp(BinaryOpType type, Ptr<ASTNode> left,
                               Ptr<ASTNode> right);
  static Ptr<ASTNode> astVar(const std::string &id);
  static Ptr<ASTNode> astAssignment(Ptr<ASTNode> var, Ptr<ASTNode> expr);
  static Ptr<ASTNode> astCompound(std::vector<Ptr<ASTNode>> stmtlist);
  static Ptr<ASTNode> astBlock(std::vector<Ptr<ASTNode>> decls,
                               Ptr<ASTNode> compound);
  static Ptr<ASTNode> astDeclaration(
      const std::string &val, DeclarationType type = DeclarationType::kNull,
      Ptr<ASTNode> proceDecl = nullptr);
  static Ptr<ASTNode> astProgram(Ptr<ASTNode> block);
  static Ptr<ASTNode> astEmpty();
  static Ptr<ASTNode> astProcedureDecl(const std::string &name,
                                       std::vector<Ptr<ASTNode>> params,
                                       Ptr<ASTNode> block);
  static Ptr<ASTNode> astParam(const std::string &val,
                               DeclarationType type = DeclarationType::kNull);

  void eatKeyword(TokenType expect);
  std::string eatVar();

  Lexer lexer_;
  std::optional<std::unique_ptr<Token>> peekToken_;
};
}  // namespace SI
#endif  // PARSER_H_