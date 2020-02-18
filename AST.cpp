
#include "AST.h"

namespace SI {
namespace Interpreter {

void ASTNode::addStatement(std::unique_ptr<ASTNode> stmt) {
  if (stmt->empty()) {
    return;
  }
  if (compounds_ == nullptr) {
    compounds_ = std::make_unique<std::vector<std::unique_ptr<ASTNode>>>();
  }
  compounds_->push_back(std::move(stmt));
}
}  // namespace Interpreter
}  // namespace SI