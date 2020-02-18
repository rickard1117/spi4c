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
  Parser(const std::string &formula);
  std::unique_ptr<ASTNode> expr();
  std::unique_ptr<ASTNode> term();
  std::unique_ptr<ASTNode> factor();
  std::unique_ptr<ASTNode> assignmentStatement();
  std::unique_ptr<ASTNode> compoundStatement();
  std::unique_ptr<ASTNode> statementList();
  std::unique_ptr<ASTNode> program();
  std::unique_ptr<ASTNode> statement();
  std::unique_ptr<ASTNode> empty();
  // std::unique_ptr<ASTNode> block();
  // std::unique_ptr<ASTNode> variableDeclaration();

 private:
  std::unique_ptr<Token> readToken();
  const Token *peekToken();
  void eatToken();

  std::unique_ptr<ASTNode> readNumber(const Token &tok) const;

  std::unique_ptr<ASTNode> astInt(long num) const;
  std::unique_ptr<ASTNode> astUnary(ASTNodeType type,
                                    std::unique_ptr<ASTNode> operand) const;
  std::unique_ptr<ASTNode> astBinOp(ASTNodeType type,
                                    std::unique_ptr<ASTNode> left,
                                    std::unique_ptr<ASTNode> right) const;
  std::unique_ptr<ASTNode> astVar(const std::string &id) const;
  std::unique_ptr<ASTNode> astAssignment(std::unique_ptr<ASTNode> var,
                                         std::unique_ptr<ASTNode> expr) const;
  std::unique_ptr<ASTNode> astCompound() const;
  void eatKeyword(TokenId expect);
  Lexer lexer_;
  std::optional<std::unique_ptr<Token>> peekToken_;
};
}  // namespace Interpreter
}  // namespace SI
#endif  // PARSER_H_