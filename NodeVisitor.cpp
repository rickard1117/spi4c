#include "NodeVisitor.h"

#include "AST.h"

namespace SI {
namespace Interpreter {

int NodeVisitor::visit(const Token &t) { return t.value_; }

int NodeVisitor::visit(const Num &n) { return n.value_; }

int NodeVisitor::visit(const BinOp &op) {
  if (op.type_ == Token::kPlus) {
    return op.left_->accept(this) + op.right_->accept(this);
  } else if (op.type_ == Token::kMinus) {
    return op.left_->accept(this) - op.right_->accept(this);
  } else if (op.type_ == Token::kMul) {
    return op.left_->accept(this) * op.right_->accept(this);
  } else if (op.type_ == Token::kDiv) {
    return op.left_->accept(this) / op.right_->accept(this);
  } else {
    throw "bad visit";
  }
}

int NodeVisitor::visit(const UnaryOp &op) {
  if (op.type_ == Token::kPlus) {
    return op.child_->accept(this);
  } else if (op.type_ == Token::kMinus) {
    return op.child_->accept(this) * (-1);
  } else {
    throw "bad visit";
  }
}

int NodeVisitor::visit(const Var &var) { return varsTable_[var.id_]; }

int NodeVisitor::visit(const Assign &assign) {
  varsTable_[assign.var_->id_] = assign.expr_->accept(this);
  return 0;
}

int NodeVisitor::visit(const Compound &com) {
  for (auto &child : com.children_) {
    child->accept(this);
  }
  return 0;
}

int NodeVisitor::visit(const NoOp &op) { return 0; }

}  // namespace Interpreter
}  // namespace SI