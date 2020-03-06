#include "SymbolTableBuilder.h"

#include "AST.h"
#include "SymbolTable.h"
#include "util.h"

namespace SI {

using namespace SI::util;

void SymbolTableBuilder::visitCompound(const Compound &com) {
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

void SymbolTableBuilder::visitProgram(const Program &prog) {
  visitBlock(prog.block_->fetch<Block>());
}

void SymbolTableBuilder::visitBlock(const Block &block) {
  for (const auto &decl : block.decls_) {
    visitDecl(decl->fetch<Declaration>());
  }
  visitCompound(block.compound_->fetch<Compound>());
}

void SymbolTableBuilder::visitDecl(const Declaration &decl) {
  auto kind = TypeKind::kNull;
  if (decl.type_ == DeclarationType::kInt) {
    kind = TypeKind::kInt;
  } else {
    kind = TypeKind::kReal;
  }
  table_->define(decl.var_, kind);
}

GeneralArithVal SymbolTableBuilder::visitBinOp(const BinaryOp &op) {
  visitArithExpr(*op.left_);
  visitArithExpr(*op.right_);
  return GeneralArithVal();
}

GeneralArithVal SymbolTableBuilder::visitUnaryOp(const UnaryOp &op) {
  visitArithExpr(*op.operand_);
  return GeneralArithVal();
}

GeneralArithVal SymbolTableBuilder::visitNumber(const Number &num) { return GeneralArithVal(); }

GeneralArithVal SymbolTableBuilder::visitVar(const Var &var) {
  const auto &name = var.id_;
  if (table_->lookup(name) == nullptr) {
    throw "cannot find symbol : " + name;
  }
  return GeneralArithVal();
}

GeneralArithVal SymbolTableBuilder::visitArithExpr(const ASTNode &ast) {
  switch (ast.type()) {
    case ASTNodeType::kBinaryOp:
      return visitBinOp(ast.fetch<BinaryOp>());
    case ASTNodeType::kNumber:
      break;
    case ASTNodeType::kUnaryOp:
      return visitUnaryOp(ast.fetch<UnaryOp>());
    case ASTNodeType::kVar:
      return visitVar(ast.fetch<Var>());
    default:
      SI_ASSERT_MSG(0,
                    "visitArithExpr type not match!!! " +
                        std::to_string(static_cast<std::size_t>(ast.type())));
  }
  return GeneralArithVal();
}

void SymbolTableBuilder::visitAssignment(const Assignment &assign) {
  visitVar(assign.var_->fetch<Var>());
  visitArithExpr(*assign.expr_);
}

}  // namespace SI