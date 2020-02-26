#include "NodeVisitor.h"

#include "AST.h"
#include "util.h"

namespace SI {
namespace Interpreter {

using namespace SI::util;

void ASTNodeVisitor::visitCompound(const ASTNode &ast) {
  for (const auto &com : ast.compounds_) {
    if (com->type_ == ASTNodeType::kAssignment) {
      visitAssignment(*com);
    } else if (com->type_ == ASTNodeType::kCompound) {
      visitCompound(*com);
    } else if (com->type_ == ASTNodeType::kEmpty) {
      continue;
    } else {
      SI_ASSERT_MSG(0,
                    "compound children type not match!!! " +
                        std::to_string(static_cast<std::size_t>(com->type_)));
    }
  }
}

void ASTNodeVisitor::visitProgram(const ASTNode &prog) {
  SI_ASSERT(prog.type_ == ASTNodeType::kProgram);
  visitBlock(*prog.block_);
}

void ASTNodeVisitor::visitBlock(const ASTNode &block) {
  for (const auto &decl : block.declarations_) {
    visitDecl(*decl);
  }

  visitCompound(*block.compound_);
}

void ASTNodeVisitor::visitDecl(const ASTNode &decl) {}

int ASTNodeVisitor::visitArith(const ASTNode &ast) {
  switch (ast.type_) {
    case ASTNodeType::kInt:
      return std::get<long>(ast.numval_);
    case ASTNodeType::kAdd:
      return visitArith(*ast.left_) + visitArith(*ast.right_);
    case ASTNodeType::kSub:
      return visitArith(*ast.left_) - visitArith(*ast.right_);
    case ASTNodeType::kMul:
      return visitArith(*ast.left_) * visitArith(*ast.right_);
    case ASTNodeType::kDiv:
      return visitArith(*ast.left_) / visitArith(*ast.right_);
    case ASTNodeType::kUnaryOpMinus:
      return -visitArith(*ast.operand_);
    case ASTNodeType::kUnaryOpPlus:
      return visitArith(*ast.operand_);
    default:
      SI_ASSERT_MSG(0, "ast type not match!!!");
  }
  return 0;
}

void ASTNodeVisitor::visitAssignment(const ASTNode &ast) {
  SI_ASSERT(ast.type_ == ASTNodeType::kAssignment);
  SI_ASSERT(ast.left_->type_ == ASTNodeType::kVar);
  symbolTable_[ast.left_->varId_] = visitArith(*ast.right_);
}

void ASTNodeVisitor::visit(const ASTNode &root) {
  // SI_ASSERT(root.type_ == ASTNodeType::kCompound);
  // for (auto &ast : *root.compounds_) {
  //   visitCompound(*ast);
  // }
}
}  // namespace Interpreter
}  // namespace SI