#ifndef PARSER_H_
#define PARSER_H_

#include <memory>
#include <string>

#include "AST.h"
#include "Lexer.h"

namespace SI {
namespace Interpreter {
class Parser {
 public:
  Parser(const std::string &formula);
  std::unique_ptr<AST> expr();
  std::unique_ptr<AST> term();
  std::unique_ptr<AST> factor();
  std::unique_ptr<AST> assignmentStatement();
  std::unique_ptr<AST> compoundStatement();
  std::unique_ptr<AST> statementList();
  std::unique_ptr<AST> program();
  std::unique_ptr<AST> statement();
  std::unique_ptr<AST> empty();

 private:
  void eat(const Token::Type &t);
  int caculate(int num, const Token &op, const Token &t) const;
  void advance() { currentToken_ = lexer_.getNextToken(); }
  Lexer lexer_;
  Token currentToken_;
};
}  // namespace Interpreter
}  // namespace SI
#endif  // PARSER_H_