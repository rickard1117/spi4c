
#include "AST.h"
#include "NodeVisitor.h"

namespace SI {
namespace Interpreter {

int Token::accept(const NodeVisitor &visitor) const { return visitor.visit(*this); }

int Num::accept(const NodeVisitor &visitor) const { return visitor.visit(*this); }

int BinOp::accept(const NodeVisitor &visitor) const { return visitor.visit(*this); }

int UnaryOp::accept(const NodeVisitor &visitor) const { return visitor.visit(*this); }

} // namespace Interpreter
} // namespace SI