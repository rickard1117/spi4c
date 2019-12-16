#ifndef PARSER_H_
#define PARSER_H_

#include "AST.h"
#include "Lexer.h"
#include <memory>
#include <string>

namespace SI {
namespace Interpreter {
class Parser {
  public:
    Parser(const std::string &formula);
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> term();
    std::unique_ptr<AST> factor();

  private:
    int caculate(int num, const Token &op, const Token &t) const;
    Lexer lexer_;
    Token currentToken_;
};
} // namespace Interpreter
} // namespace SI
#endif // PARSER_H_