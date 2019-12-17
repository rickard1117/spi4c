#ifndef NODE_VISITOR_H__
#define NODE_VISITOR_H__

namespace SI {
namespace Interpreter {

class Token;
class Num;
class BinOp;
class UnaryOp;

class NodeVisitor {
  public:
    int visit(const Token &) const;
    int visit(const Num &) const;
    int visit(const BinOp &) const;
    int visit(const UnaryOp &) const;

  private:
    int result;
};
} // namespace Interpreter
} // namespace SI

#endif // NODE_VISITOR_H__