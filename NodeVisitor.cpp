#include "NodeVisitor.h"
#include "AST.h"

namespace SI {
namespace Interpreter {

int NodeVisitor::visit(const Token &t) const { return t.value_; }

int NodeVisitor::visit(const Num &n) const { return n.value_; }

int NodeVisitor::visit(const BinOp &op) const {
    if (op.type_ == Token::kPlus) {
        return op.left_->accept(*this) + op.right_->accept(*this);
        // return visit(op.left_) + visit(op.right_);
    } else if (op.type_ == Token::kMinus) {
        return op.left_->accept(*this) - op.right_->accept(*this);
    } else if (op.type_ == Token::kMul) {
        return op.left_->accept(*this) * op.right_->accept(*this);
    } else if (op.type_ == Token::kDiv) {
        return op.left_->accept(*this) / op.right_->accept(*this);
    } else {
        throw "bad visit";
    }
}
int NodeVisitor::visit(const UnaryOp &op) const {
    if (op.type_ == Token::kPlus) {
        return op.child_->accept(*this);
    } else if (op.type_ == Token::kMinus) {
        return op.child_->accept(*this) * (-1);
    } else {
        throw "bad visit";
    }
}

} // namespace Interpreter
} // namespace SI