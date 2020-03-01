#include "NodeVisitor.h"

#include "AST.h"
#include "util.h"

namespace SI {
namespace Interpreter {

using namespace SI::util;

void ASTNodeVisitor::visitCompound(const Compound &com) {
  for (const auto &ast : com.children_) {
    switch (ast->type()) {
      case ASTNodeType::kAssignment:
        visitAssignment(ast->fetch<Assignment>());
        continue;
      case ASTNodeType::kCompound:
        visitCompound(ast->fetch<Compound>());
        continue;
      case ASTNodeType::kEmpty:
        continue;
      default:
        SI_ASSERT_MSG(
            0, "compound children type not match!!! " +
                   std::to_string(static_cast<std::size_t>(ast->type())));
    }
  }
}

void ASTNodeVisitor::visitProgram(const Program &prog) {
  visitBlock(prog.block_->fetch<Block>());
}

void ASTNodeVisitor::visitBlock(const Block &block) {
  for (const auto &decl : block.decls_) {
    visitDecl(decl->fetch<Declaration>());
  }

  visitCompound(block.compound_->fetch<Compound>());
}

void ASTNodeVisitor::visitDecl(const Declaration &decl) {
  symbolTable_[decl.var_] = 0;
}

int ASTNodeVisitor::visitBinOp(const BinaryOp &op) {
  switch (op.type_) {
    case BinaryOpType::kAdd:
      return visitArithExpr(*op.left_) + visitArithExpr(*op.right_);
    case BinaryOpType::kSub:
      return visitArithExpr(*op.left_) - visitArithExpr(*op.right_);
    case BinaryOpType::kMul:
      return visitArithExpr(*op.left_) * visitArithExpr(*op.right_);
    case BinaryOpType::kDiv:
      return visitArithExpr(*op.left_) / visitArithExpr(*op.right_);
    default:
      SI_ASSERT(0);
  }
}

int ASTNodeVisitor::visitUnaryOp(const UnaryOp &op) {
  switch (op.type_) {
    case UnaryOpType::kMinus:
      return (-1) * visitArithExpr(*op.operand_);
    case UnaryOpType::kPlus:
      return visitArithExpr(*op.operand_);
    default:
      SI_ASSERT(0);
  }
}

int ASTNodeVisitor::visitNumber(const Number &num) {
  return std::stoi(num.val_);
}

int ASTNodeVisitor::visitVar(const Var &var) {
  auto itr = symbolTable_.find(var.id_);
  if (itr == symbolTable_.cend()) {
    SI_ASSERT_MSG(0, "var not defined ! : " + var.id_);
  }
  return symbolTable_[var.id_];
}

int ASTNodeVisitor::visitArithExpr(const ASTNode &ast) {
  switch (ast.type()) {
    case ASTNodeType::kBinaryOp:
      return visitBinOp(ast.fetch<BinaryOp>());
    case ASTNodeType::kNumber:
      return visitNumber(ast.fetch<Number>());
    case ASTNodeType::kUnaryOp:
      return visitUnaryOp(ast.fetch<UnaryOp>());
    case ASTNodeType::kVar:
      return visitVar(ast.fetch<Var>());
    default:
      SI_ASSERT_MSG(0,
                    "visitArithExpr type not match!!! " +
                        std::to_string(static_cast<std::size_t>(ast.type())));
  }
}

void ASTNodeVisitor::visitAssignment(const Assignment &ast) {
  const auto &id = ast.var_->fetch<Var>().id_;
  if(symbolTable_.find(id) == symbolTable_.cend()) {
    SI_ASSERT_MSG(0, "var not defined ! : " + id);
  }
  symbolTable_[id] = visitArithExpr(*ast.expr_);
}

}  // namespace Interpreter
}  // namespace SI