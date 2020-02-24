#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Token.h"

namespace SI {
namespace Interpreter {

enum class ASTNodeType {
  kInt = 0,
  kReal,
  kAdd,
  kSub,
  kMul,
  kDiv,
  kUnaryOpPlus,
  kUnaryOpMinus,
  kVar,
  kEmpty,
  kCompound,
  kAssignment,
  kBlock,
  kDeclaration,
};

class ASTNode;
using ASTNodePtr = std::unique_ptr<ASTNode>;
class ASTNode {
 public:
  ASTNode(ASTNodeType type) : type_(type) {}

  void setNumber(long num) { numval_ = num; }
  void setOperand(ASTNodePtr operand) { operand_ = std::move(operand); }
  void setLeft(ASTNodePtr left) { left_ = std::move(left); }
  void setRight(ASTNodePtr right) { right_ = std::move(right); }
  void setVar(const std::string &val) { varId_ = val; }
  bool empty() const { return empty_; }
  void addCompund(ASTNodePtr com);
  // void addDecl(ASTNodePtr decls);
  void setDecls(std::vector<ASTNodePtr> &&decls);
  void setCompound(ASTNodePtr com);
  // enum Type {

  // };

 private:
  friend class ASTNodeVisitor;
  ASTNodeType type_;

  // std::variant<int, AstAssignment, AstBinaryOp, AstBlock, AstCompound,
  // AstDeclration,
  //              AstEmpty, AstNumber, AstUnaryOp, AstVariable>
  //     imp_;
  // empty
  bool empty_;

  // variable
  std::string varId_;

  // number
  std::variant<long, double> numval_;

  // binary op  or assignment
  ASTNodePtr left_;
  ASTNodePtr right_;

  // unary op
  ASTNodePtr operand_;

  // compounds
  std::unique_ptr<std::vector<ASTNodePtr>> compounds_;

  // declarations
  std::vector<ASTNodePtr> declarations_;
};

// using ASTNodePtr = std::unique_ptr<ASTNode>;

}  // namespace Interpreter
}  // namespace SI

#endif  // AST_H_