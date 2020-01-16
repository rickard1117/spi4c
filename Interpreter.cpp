#include "Interpreter.h"

#include "AST.h"
#include "NodeVisitor.h"
#include "Parser.h"

namespace SI {
namespace Interpreter {

int Interpreter::interpret(const std::string &formula) const {
  Parser parser{formula};
  std::unique_ptr<AST> tree = parser.expr();
  NodeVisitor visitor;
  return tree->accept(&visitor);
}

}  // namespace Interpreter
}  // namespace SI