#include "Interpreter.h"

#include "AST.h"
#include "util.h"

namespace SI {

using namespace SI::util;

void Interpreter::visitCompound(const Compound &com) {
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

void Interpreter::visitProgram(const Program &prog) {
  visitBlock(prog.block_->fetch<Block>());
}

void Interpreter::visitBlock(const Block &block) {
  for (const auto &decl : block.decls_) {
    visitDecl(decl->fetch<Declaration>());
  }

  visitCompound(block.compound_->fetch<Compound>());
}

void Interpreter::visitProcedureDecl(const ProcedureDecl &decl) {}

void Interpreter::visitParam(const Param &param) {}

void Interpreter::visitDecl(const Declaration &decl) {
  if (decl.type_ == DeclarationType::kProcedure) {
    SI_ASSERT(decl.proceDecl_ != nullptr);
    visitProcedureDecl(decl.proceDecl_->fetch<ProcedureDecl>());
    return;
  }
  symbolTable_[decl.var_] = 0;
}

GeneralArithVal Interpreter::visitBinOp(const BinaryOp &op) {
  switch (op.type_) {
    case BinaryOpType::kAdd:
      return visitArithExpr(*op.left_) + visitArithExpr(*op.right_);
    case BinaryOpType::kSub:
      return visitArithExpr(*op.left_) - visitArithExpr(*op.right_);
    case BinaryOpType::kMul:
      return visitArithExpr(*op.left_) * visitArithExpr(*op.right_);
    case BinaryOpType::kIntDiv: {
      auto result = visitArithExpr(*op.left_) / visitArithExpr(*op.right_);
      return static_cast<int>(result.getreal());
    }
    case BinaryOpType::kRealdIV:
      return visitArithExpr(*op.left_).getreal() /
             visitArithExpr(*op.right_).getreal();
    default:
      SI_ASSERT(0);
  }
  return 0;
}

GeneralArithVal Interpreter::visitUnaryOp(const UnaryOp &op) {
  switch (op.type_) {
    case UnaryOpType::kMinus:
      return (-1) * visitArithExpr(*op.operand_);
    case UnaryOpType::kPlus:
      return visitArithExpr(*op.operand_);
    default:
      SI_ASSERT(0);
  }
  return 0;
}

GeneralArithVal Interpreter::visitNumber(const Number &num) {
  if (num.evalType_ == TypeKind::kInt) {
    return std::get<int>(num.num_);
  }
  if (num.evalType_ == TypeKind::kReal) {
    return std::get<double>(num.num_);
  }
  throw "Number type not match";
}

GeneralArithVal Interpreter::visitID(const ID &var) {
  auto itr = symbolTable_.find(var.name_);

  SI_ASSERT_MSG(itr != symbolTable_.cend(), "var not defined ! : " + var.name_);

  return symbolTable_[var.name_];
}

GeneralArithVal Interpreter::visitArithExpr(const ASTNode &ast) {
  switch (ast.type()) {
    case ASTNodeType::kBinaryOp:
      return visitBinOp(ast.fetch<BinaryOp>());
    case ASTNodeType::kNumber:
      return visitNumber(ast.fetch<Number>());
    case ASTNodeType::kUnaryOp:
      return visitUnaryOp(ast.fetch<UnaryOp>());
    case ASTNodeType::kID:
      return visitID(ast.fetch<ID>());
    default:
      SI_ASSERT_MSG(0,
                    "visitArithExpr type not match!!! " +
                        std::to_string(static_cast<std::size_t>(ast.type())));
  }
  return 0;
}

void Interpreter::visitAssignment(const Assignment &ast) {
  const auto &id = ast.var_->fetch<ID>().name_;
  if (symbolTable_.find(id) == symbolTable_.cend()) {
    SI_ASSERT_MSG(0, "var not defined ! : " + id);
  }
  symbolTable_[id] = visitArithExpr(*ast.expr_);
}

void Interpreter::visitProcedureCall(const ProcedureCall &call) {}

}  // namespace SI