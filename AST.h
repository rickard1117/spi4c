#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>
#include <vector>
#include "Token.h"

namespace SI {
namespace Interpreter {

enum class ASTNodeType {
  kInt = 0, // 整形
  kReal,   // 浮点
  kAdd,  // 加法
  kSub,  // 减法
  kMul, // 乘法
  kDiv, // 除法
  kBinOp, // 二元运算符
  kUnaryOp, // 一元运算符
  k
};

class AST {
public:
  AST(TokenTypes type): token_(type) {}
  explicit AST(Token token)：token_(token) {}

  // ASTNode(Token token, int val)：token_(token), intval_(val), realval_(0.0) {
  //   assert(token.type_ == TokenTypes::kInteger);
  // }

  // ASTNode(Token token, double real)：token_(token),intval_(0),realval(real) {
  //   assert(token.type_ == TokenTypes::kReal);
  // }
  
  TokenTypes type() const {
    return token_.type();
  }
  void add(std::unique_ptr<AST> node) {
    children_.push_back(std::move(node));
  }

  const AST &at(std::size_t idx) {
    return children_.at(idx);
  }

  // void setInt(int val) {
  //   assert(token.type_ == TokenTypes::kInteger);
  //   intval_ = val;
  // }
  // void setReal(double val) {
  //   assert(token.type_ == TokenTypes::kReal);
  //   realval_ = val;
  // }
  
private:
  Token token_;
  ASTNodeType type_;
  // ASTNodeType type_;
  // int intval_;
  // double realval_;
  std::vector<std::unique_ptr<AST>> children_;
};

// class Integer : public AST {
// private:
//   int i_;
// };



// class NodeVisitor;
// class AST {
//  public:
//   virtual ~AST() = default;
//   virtual int accept(NodeVisitor *visitor) const = 0;
// };

// class Token : public AST {
//  public:
//   Token() : type_(kNull), value_(0) {}
//   enum Type {
//     kEof = 0,
//     kNull = 1,
//     kSpace = 2,
//     kPlus = 3,
//     kMinus = 4,
//     kMul = 5,
//     kDiv = 6,
//     kLparent = 7,
//     kRparent = 8,
//     kNum = 9,
//     kBegin = 10,
//     kEnd = 11,
//     kDot = 12,     // .
//     kAssign = 13,  // :=
//     kVar = 14,
//     kSemi = 15,     // ;
//     kProgram = 16,  // PROGRAM
//     kVardecl = 17,  // VAR
//     kColon = 18,    // :
//     kInteger = 19,  // INTEGER
//   };
//   Type type() const { return type_; }
//   int value() const { return value_; }
//   const std::string &varval() { return varval_; }
//   bool operator==(const Token &t) {
//     if (type_ != t.type_) {
//       return false;
//     }
//     if (type_ == kNum) {
//       return value_ == t.value_;
//     }
//     return true;
//   }
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   friend class Lexer;
//   Type type_;
//   int value_;
//   std::string varval_;
// };

// class Num : public AST {
//  public:
//   Num(int value) : value_(value) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   int value_;
// };

// class BinOp : public AST {
//  public:
//   BinOp(Token::Type type, std::unique_ptr<AST> left, std::unique_ptr<AST> right)
//       : type_(type), left_(std::move(left)), right_(std::move(right)) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   Token::Type type_;
//   std::unique_ptr<AST> left_;
//   std::unique_ptr<AST> right_;
// };

// class UnaryOp : public AST {
//  public:
//   UnaryOp(Token::Type type, std::unique_ptr<AST> child)
//       : type_(type), child_(std::move(child)) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   Token::Type type_;
//   std::unique_ptr<AST> child_;
// };

// class Var : public AST {
//  public:
//   Var(const std::string &id) : id_(id) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   const std::string id_;
// };

// class Assign : public AST {
//  public:
//   Assign(std::unique_ptr<Var> var, std::unique_ptr<AST> expr)
//       : var_(std::move(var)), expr_(std::move(expr)) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   std::unique_ptr<Var> var_;
//   std::unique_ptr<AST> expr_;
// };

// class Compound : public AST {
//  public:
//   void add(std::unique_ptr<AST> compound) {
//     children_.push_back(std::move(compound));
//   }
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   friend class NodeVisitor;
//   std::vector<std::unique_ptr<AST>> children_;
// };

// class NoOp : public AST {
//  public:
//   virtual int accept(NodeVisitor *visitor) const override;
// };

// class VarDecl : public AST {
//  public:
//   enum Type {
//     kInteger = 0,
//   };
//   VarDecl(const std::string &id, VarDecl::Type type) : id_(id), type_(type) {}
//   virtual int accept(NodeVisitor *visitor) const override;

//  private:
//   std::string id_;
//   Type type_;
// };

// class Block : public AST {
//  public:
//   virtual int accept(NodeVisitor *visitor) const override;
//   void add(std::unique_ptr<AST> compound) {
//     children_.push_back(std::move(compound));
//   }

//  private:
//   friend class NodeVisitor;
//   std::vector<std::unique_ptr<AST>> children_;
// };
}  // namespace Interpreter
}  // namespace SI

#endif  // AST_H_