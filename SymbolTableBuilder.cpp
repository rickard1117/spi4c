#include "SymbolTableBuilder.h"

#include "AST.h"
#include "Error.h"
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
  currentTable_ = std::make_shared<ScopedSymbolTable>(prog.name_, nullptr);
  visitBlock(prog.block_->fetch<Block>());
}

void SymbolTableBuilder::visitBlock(const Block &block) {
  for (const auto &decl : block.decls_) {
    visitDecl(decl->fetch<Declaration>());
  }
  visitCompound(block.compound_->fetch<Compound>());
}

void SymbolTableBuilder::enterNewScope(const std::string &name) {
  currentTable_ = std::make_shared<ScopedSymbolTable>(name, currentTable_);
}

void SymbolTableBuilder::exitScope() {
  // SI_ASSERT(currentTable_ != nullptr);
  currentTable_ = currentTable_->upperScope();
}

void SymbolTableBuilder::visitParam(const Param &param) {
  auto kind = TypeKind::kNull;
  if (param.type_ == DeclarationType::kInt) {
    kind = TypeKind::kInt;
  } else {
    SI_ASSERT(param.type_ == DeclarationType::kReal);
    kind = TypeKind::kReal;
  }
  if (!currentTable_->define(param.var_, kind)) {
    throw InterpreterError(kErrorDupID, param.var_);
  }
}

void SymbolTableBuilder::visitProcedureDecl(const ProcedureDecl &decl) {
  // new scope
  enterNewScope(decl.name_);
  for (auto &p : decl.params_) {
    visitParam(p->fetch<Param>());
  }
  visitBlock(decl.block_->fetch<Block>());
  exitScope();
}

void SymbolTableBuilder::visitDecl(const Declaration &decl) {
  if (decl.type_ == DeclarationType::kProcedure) {
    visitProcedureDecl(decl.proceDecl_->fetch<ProcedureDecl>());
    return;
  }
  auto kind = TypeKind::kNull;
  if (decl.type_ == DeclarationType::kInt) {
    kind = TypeKind::kInt;
  } else {
    SI_ASSERT(decl.type_ == DeclarationType::kReal);
    kind = TypeKind::kReal;
  }
  if (!currentTable_->define(decl.var_, kind)) {
    throw InterpreterError(kErrorDupID, decl.var_);
  }
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

GeneralArithVal SymbolTableBuilder::visitNumber(const Number &num) {
  return GeneralArithVal();
}

GeneralArithVal SymbolTableBuilder::visitVar(const Var &var) {
  const std::string &name = var.id_;
  if (currentTable_->lookup(name) == nullptr) {
    throw InterpreterError(kErrorIDNotFound, name);
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