#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

#include <map>

namespace SI {
namespace Interpreter {

class NodeVisitor {
 public:
  int visit(const class Token &);
  int visit(const class Num &);
  int visit(const class BinOp &);
  int visit(const class UnaryOp &);
  int visit(const class Var &);
  int visit(const class Assign &);
  int visit(const class Compound &);
  int visit(const class NoOp &);
  int visit(const class Block &);
  int visit(const class VarDecl &);
  const std::map<std::string, int> &varsTable() const { return varsTable_; }

 private:
  std::string currentVar_;
  std::map<std::string, int> varsTable_;
};
}  // namespace Interpreter
}  // namespace SI

#endif  // NODE_VISITOR_H__