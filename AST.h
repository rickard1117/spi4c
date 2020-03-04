#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Token.h"
#include "Type.h"
#include "util.h"

namespace SI {

using SI::util::Noncopyable;
using SI::util::Ptr;

#define COMMA ,
#define SEMI ;
#define AST_TYPE(DO, DELIMITER)                                         \
  DO(Program)                                                           \
  DELIMITER DO(Block) DELIMITER DO(Compound) DELIMITER DO(Assignment)   \
      DELIMITER DO(Number) DELIMITER DO(UnaryOp) DELIMITER DO(BinaryOp) \
          DELIMITER DO(Declaration) DELIMITER DO(Var) DELIMITER DO(Empty)

#define CLASS_DECL(name) class name
#define MAKE_ENUM(name) k##name
#define STD_VARIANT(name) name
AST_TYPE(CLASS_DECL, SEMI);

class ASTNode;
class Empty : public Noncopyable {
 public:
  Empty() = default;
};

class Number : public Noncopyable {
 public:
  Number(const std::string &val);

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  Type evalType_;
  Type promoteType_;
  std::variant<int, float> num_;
};

class Program : public Noncopyable {
 public:
  Program(Ptr<ASTNode> block) : block_(std::move(block)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  Ptr<ASTNode> block_;
};

class Block : public Noncopyable {
 public:
  Block(std::vector<Ptr<ASTNode>> decls, Ptr<ASTNode> compound)
      : decls_(std::move(decls)), compound_(std::move(compound)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  std::vector<Ptr<ASTNode>> decls_;
  Ptr<ASTNode> compound_;
};

class Compound : public Noncopyable {
 public:
  Compound(std::vector<Ptr<ASTNode>> children)
      : children_(std::move(children)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  std::vector<Ptr<ASTNode>> children_;
};

class Assignment : public Noncopyable {
 public:
  Assignment(Ptr<ASTNode> var, Ptr<ASTNode> expr)
      : var_(std::move(var)), expr_(std::move(expr)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  Ptr<ASTNode> var_;
  Ptr<ASTNode> expr_;
};

enum class UnaryOpType { kPlus, kMinus };
class UnaryOp : public Noncopyable {
 public:
  UnaryOp(UnaryOpType type, Ptr<ASTNode> operand)
      : type_(type), operand_(std::move(operand)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  UnaryOpType type_;
  Ptr<ASTNode> operand_;
};

enum class BinaryOpType { kAdd, kSub, kMul, kDiv };
class BinaryOp : public Noncopyable {
 public:
  BinaryOp(BinaryOpType type, Ptr<ASTNode> left, Ptr<ASTNode> right)
      : type_(type), left_(std::move(left)), right_(std::move(right)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  BinaryOpType type_;
  Ptr<ASTNode> left_;
  Ptr<ASTNode> right_;
};

class Var : public Noncopyable {
 public:
  Var(const std::string &val) : id_(val) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string id_;
};

enum class DeclarationType { kInt, kReal };
class Declaration : public Noncopyable {
 public:
  Declaration(const std::string &var, DeclarationType type)
      : var_(var), type_(type) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string var_;
  const DeclarationType type_;
};

enum class ASTNodeType { AST_TYPE(MAKE_ENUM, COMMA) };

class ASTNode {
 public:
  template <class T, class... Args>
  ASTNode(ASTNodeType type, std::in_place_type_t<T> t, Args &&... args)
      : type_(type), v_(t, std::forward<Args>(args)...) {}

  ASTNodeType type() const { return type_; }

  template <class T>
  const T &fetch() const {
    return std::get<T>(v_);
  }

  template <class T>
  T &fetch() {
    return std::get<T>(v_);
  }

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const ASTNodeType type_;
  std::variant<AST_TYPE(STD_VARIANT, COMMA)> v_;
};

}  // namespace SI

#endif  // AST_H_