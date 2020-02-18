#include "NodeVisitor.h"

#include <assert.h>

#include "AST.h"
#include "util.h"

namespace SI {
namespace Interpreter {

using namespace SI::util;

void ASTNodeVisitor::visitCompound(const ASTNode &ast) {
  switch (ast.type_) {
    case ASTNodeType::kAssignment:
      visitAssignment(ast);
      return;
    case ASTNodeType::kCompound:
      visitCompound(ast);
      return;
    default:
      SI_ASSERT_MSG(0, "ast type not match!!!");
  }
}

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
  SI_ASSERT(root.type_ == ASTNodeType::kCompound);
  for (auto &ast : *root.compounds_) {
    visitCompound(*ast);
  }
}
}  // namespace Interpreter
}  // namespace SI