#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Token.h"
#include "util.h"

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
  kProgram,
};
// class ASTProgram;
// class ASTBlock;
// class ASTCompound;
// class ASTAssignment;
// class ASTDeclaration;
// class ASTVar;
// class ASTType;
// class ASTNumber;
// class ASTUnaryOp;

// class ASTBase : public SI::util::Noncopyable {
//  public:
//   virtual ~ASTBase() = default;
//   virtual void accept(const class BaseVisitor &v) = 0;
// };

// class ASTProgram : public ASTBase {
//  public:
//   ASTProgram(Ptr<ASTBlock> block) : block_(std::move(block)) {}
//   void accept(const class BaseVisitor &v) override;

//  private:
//   Ptr<ASTBlock> block_;
// };

// class ASTBlock : public ASTBase {
//  public:
//   ASTBlock(std::vector<Ptr<ASTDeclaration>> &&decls, Ptr<ASTCompound>
//   compound)
//       : decls_(decls), compound_(std::move(compound)) {}
//   void accept(const class BaseVisitor &v) override;

//  private:
//   std::vector<Ptr<ASTDeclaration>> decls_;
//   Ptr<ASTCompound> compound_;
// };

// class ASTVar : public ASTBase {
//  public:
//   ASTVar(const std::string &id) : id_(id) {}
//   void accept(const class BaseVisitor &v) override;

//  private:
//   std::string id_;
// };

// class ASTDeclaration : public ASTBase {
//  public:
//   ASTDeclaration(Ptr<ASTVar> var, Ptr<ASTType> type)
//       : var_(std::move(var)), type_(std::move(type)) {}
//   void accept(const class BaseVisitor &v) override;

//  private:
//   Ptr<ASTVar> var_;
//   Ptr<ASTType> type_;
// };

// class ASTCompound : public ASTBase {
//  public:
//   ASTCompound() = default;
//   void addCompound();
//   void accept(const class BaseVisitor &v) override;

//  private:
//   std::vector<Ptr<ASTAssignment>> ass_;
// };

// class ASTRValueExpr : public ASTBase {
// public:
//   void accept(const class BaseVisitor &v) override;
// };

// class ASTBinOp : public ASTBase {
//  public:
//   ASTBinOp() = delete;
//   ASTBinOp()
//   void accept(const class BaseVisitor &v) override;

//  private:
//   Ptr<ASTVar> var_;
//   std::variant<Ptr<ASTBinOp>, Ptr<ASTVar>, Ptr<ASTUnaryOp>, Ptr<ASTNumber>>
//       expr_;
// };

// class ASTUnaryOp : public ASTBase {
//   public
// }

// class ASTNumber : public ASTBase {
//  public:
//   ASTNumber(long num) : num_(num) {}
//   ASTNumber(double num) : num_(num) {}
//   void accept(const class BaseVisitor &v) override;

//  private:
//   std::variant<long, double> num_;
// };

class ASTNode;
using ASTNodePtr = std::unique_ptr<ASTNode>;
class ASTNode {
 public:
  ASTNode(ASTNodeType type) : type_(type) {}

  void setNumber(long num);
  void setOperand(ASTNodePtr operand);
  void setLeft(ASTNodePtr left);
  void setRight(ASTNodePtr right);
  void setVar(const std::string &val);
  void setBlock(ASTNodePtr block);
  // bool empty() const;
  void addAssignment(ASTNodePtr com);
  void addCompoundOrAssignment(ASTNodePtr com);
  void setDecls(std::vector<ASTNodePtr> &&decls);
  void setCompound(ASTNodePtr com);
  void setCompounds(std::vector<ASTNodePtr> coms);

 private:
  friend class ASTNodeVisitor;
  ASTNodeType type_;

  // empty
  // bool empty_;

  // variable
  std::string varId_;

  // number
  std::variant<long, double> numval_;

  // binary op  or assignment
  ASTNodePtr left_;
  ASTNodePtr right_;

  // unary op
  ASTNodePtr operand_;

  // program
  ASTNodePtr block_;

  // compound
  std::unique_ptr<std::vector<ASTNodePtr>> assignments_;

  // block
  std::vector<ASTNodePtr> declarations_;

  // block
  ASTNodePtr compound_;

  // compound
  std::vector<ASTNodePtr> compounds_;
};

// using ASTNodePtr = std::unique_ptr<ASTNode>;

}  // namespace Interpreter
}  // namespace SI

#endif  // AST_H_