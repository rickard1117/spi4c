#include "NodeVisitor.h"

#include "AST.h"

namespace SI {
namespace Interpreter {

// Num NodeVisitor::visitBinOp(const AST &ast) {
//   switch (ast.type()) {
//     case TokenTypes::kPlus:
//     case TokenTypes::kMinus:
//     case TokenTypes::kMul:
//     case TokenTypes::kDiv:
//   }
// }

Num NodeVisitor::visit(const AST &ast) { 
  // return t.value_; 
  switch (ast.type()) {
    case TokenTypes::kPlus:
      return visit(ast.at(0)) + visit(ast.at(1));
    case TokenTypes::kMinus:
      return visit(ast.at(0)) - visit(ast.at(1));
    case TokenTypes::kMul:
      return visit(ast.at(0)) * visit(ast.at(1));
    case TokenTypes::kDiv:
      return visit(ast.at(0)) / visit(ast.at(1));
    case TokenTypes::kIngeter:
      return Num(ast.token_.intval());
    case TokenTypes::kReal:
      return Num(ast.token_.realval());
    case TokenTypes::k
  }

}

// int NodeVisitor::visit(const Num &n) { return n.value_; }

// int NodeVisitor::visit(const BinOp &op) {
//   if (op.type_ == TokenTypes::kPlus) {
//     return op.left_->accept(this) + op.right_->accept(this);
//   } else if (op.type_ == TokenTypes::kMinus) {
//     return op.left_->accept(this) - op.right_->accept(this);
//   } else if (op.type_ == TokenTypes::kMul) {
//     return op.left_->accept(this) * op.right_->accept(this);
//   } else if (op.type_ == TokenTypes::kDiv) {
//     return op.left_->accept(this) / op.right_->accept(this);
//   } else {
//     throw "bad visit";
//   }
// }

int NodeVisitor::visit(const UnaryOp &op) {
  if (op.type_ == TokenTypes::kPlus) {
    return op.child_->accept(this);
  } else if (op.type_ == TokenTypes::kMinus) {
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

int NodeVisitor::visit(const Block &b) { return 0; }

int NodeVisitor::visit(const VarDecl &vd) { return 0; }
}  // namespace Interpreter
}  // namespace SI