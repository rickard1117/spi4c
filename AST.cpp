
#include "AST.h"

#include "util.h"

namespace SI {
namespace Interpreter {

using namespace SI::util;

void ASTNode::addCompund(ASTNodePtr com) {
  if (com->empty()) {
    return;
  }
  if (compounds_ == nullptr) {
    compounds_ = std::make_unique<std::vector<ASTNodePtr>>();
  }
  compounds_->push_back(std::move(com));
}

void ASTNode::setDecls(std::vector<ASTNodePtr> &&decls) {
  declarations_ = std::move(decls);
}

void ASTNode::setCompound(ASTNodePtr com) {
  SI_ASSERT(type_ == ASTNodeType::kBlock);
  compounds_ = std::move(com->compounds_);
}

}  // namespace Interpreter
}  // namespace SI