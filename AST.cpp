
#include "AST.h"

#include "util.h"

namespace SI {
namespace Interpreter {

using namespace SI::util;

void ASTNode::addAssignment(ASTNodePtr com) {
  // if (com->empty()) {
  //   return;
  // }
  SI_ASSERT(com->type_ == ASTNodeType::kAssignment);
  if (assignments_ == nullptr) {
    assignments_ = std::make_unique<std::vector<ASTNodePtr>>();
  }
  assignments_->push_back(std::move(com));
}

void ASTNode::setDecls(std::vector<ASTNodePtr> &&decls) {
  declarations_ = std::move(decls);
}

void ASTNode::addCompoundOrAssignment(ASTNodePtr com) {
  // if (com->type_ == ASTNodeType::kAssignment) {
  //   addAssignment(std::move(com));
  //   return;
  // }
  //  if (com->type_ == ASTNodeType::kCompound) {
  //    addCommpound();
  //    return;
  // }
  //   SI_ASSERT(com->type_ == ASTNodeType::kEmpty);
}

void ASTNode::setCompound(ASTNodePtr com) {
  SI_ASSERT(type_ == ASTNodeType::kBlock);
  compound_ = std::move(com);
}

void ASTNode::setCompounds(std::vector<ASTNodePtr> coms) {
  SI_ASSERT(type_ == ASTNodeType::kCompound);
  compounds_ = std::move(coms);
}

void ASTNode::setNumber(long num) {
  SI_ASSERT(type_ == ASTNodeType::kInt);
  numval_ = num;
}

void ASTNode::setOperand(ASTNodePtr operand) {
  SI_ASSERT(type_ == ASTNodeType::kUnaryOpMinus ||
            type_ == ASTNodeType::kUnaryOpPlus);
  operand_ = std::move(operand);
}

void ASTNode::setLeft(ASTNodePtr left) {
  SI_ASSERT(type_ == ASTNodeType::kAdd || type_ == ASTNodeType::kSub ||
            type_ == ASTNodeType::kDiv || type_ == ASTNodeType::kMul ||
            type_ == ASTNodeType::kAssignment);
  left_ = std::move(left);
}

void ASTNode::setRight(ASTNodePtr right) {
  SI_ASSERT(type_ == ASTNodeType::kAdd || type_ == ASTNodeType::kSub ||
            type_ == ASTNodeType::kDiv || type_ == ASTNodeType::kMul ||
            type_ == ASTNodeType::kAssignment);
  right_ = std::move(right);
}

void ASTNode::setVar(const std::string &val) {
  SI_ASSERT(type_ == ASTNodeType::kVar);
  varId_ = val;
}

void ASTNode::setBlock(ASTNodePtr block) {
  SI_ASSERT(type_ == ASTNodeType::kProgram);
  block_ = std::move(block);
}

// bool ASTNode::empty() const { return empty_; }

}  // namespace Interpreter
}  // namespace SI