
#include "AST.h"
#include "NodeVisitor.h"

namespace SI {
namespace Interpreter {

int Token::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

int Num::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

int BinOp::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

int UnaryOp::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

int Var::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

int Assign::accept(NodeVisitor *visitor) const { return visitor->visit(*this); }

} // namespace Interpreter
} // namespace SI