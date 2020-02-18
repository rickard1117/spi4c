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
  kInt = 0,       // 整形
  kReal,          // 浮点
  kAdd,           // 加法
  kSub,           // 减法
  kMul,           // 乘法
  kDiv,           // 除法
  kBinOp,         // 二元运算符
  kUnaryOpPlus,   // 一元运算符
  kUnaryOpMinus,  //
  kVar,
  kEmpty,
  kCompound,
  kAssignment,
};

class ASTNode {
 public:
  ASTNode(ASTNodeType type)
      : type_(type), empty_(type == ASTNodeType::kEmpty) {}

  void setLong(long num) { numval_ = num; }
  void setOperand(std::unique_ptr<ASTNode> operand) {
    operand_ = std::move(operand);
  }
  void setLeft(std::unique_ptr<ASTNode> left) { left_ = std::move(left); }
  void setRight(std::unique_ptr<ASTNode> right) { right_ = std::move(right); }
  void setVar(const std::string &val) { varId_ = val; }
  bool empty() const { return empty_; }
  void addStatement(std::unique_ptr<ASTNode> stmt);

 private:
  friend class ASTNodeVisitor;
  ASTNodeType type_;

  // empty
  bool empty_;

  // variable
  std::string varId_;

  // int or real
  std::variant<long, double> numval_;

  // binary op  or assignment
  std::unique_ptr<ASTNode> left_;
  std::unique_ptr<ASTNode> right_;

  // unary op
  std::unique_ptr<ASTNode> operand_;

  // compound
  std::unique_ptr<std::vector<std::unique_ptr<ASTNode>>> compounds_;
};
}  // namespace Interpreter
}  // namespace SI

#endif  // AST_H_