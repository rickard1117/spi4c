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
#define AST_TYPE(DO, DELIMITER)                                          \
  DO(Program)                                                            \
  DELIMITER DO(Block) DELIMITER DO(Compound) DELIMITER DO(Assignment)    \
      DELIMITER DO(Number) DELIMITER DO(UnaryOp) DELIMITER DO(BinaryOp)  \
          DELIMITER DO(Declaration) DELIMITER DO(ID) DELIMITER DO(Empty) \
              DELIMITER DO(ProcedureDecl) DELIMITER DO(Param)

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
  std::variant<int, double> num_;
};

class Program : public Noncopyable {
 public:
  Program(const std::string &name, Ptr<ASTNode> block)
      : name_(name), block_(std::move(block)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string name_;
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

enum class BinaryOpType { kAdd, kSub, kMul, kIntDiv, kRealdIV };
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

class ID : public Noncopyable {
 public:
  ID(const std::string &val) : name_(val) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string name_;
};

enum class DeclarationType { kNull, kInt, kReal, kProcedure };
class Declaration : public Noncopyable {
 public:
  Declaration(const std::string &var,
              DeclarationType type = DeclarationType::kNull,
              Ptr<ASTNode> proceDecl = nullptr)
      : var_(var), type_(type), proceDecl_(std::move(proceDecl)) {}
  void setType(DeclarationType type) { type_ = type; }

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string var_;
  DeclarationType type_;
  Ptr<ASTNode> proceDecl_;
};

class Param : public Noncopyable {
 public:
  Param(const std::string &var, DeclarationType type = DeclarationType::kNull)
      : var_(var), type_(type) {}
  void setType(DeclarationType type) { type_ = type; }

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string var_;
  DeclarationType type_;
};

class ProcedureDecl : public Noncopyable {
 public:
  ProcedureDecl(const std::string &name, std::vector<Ptr<ASTNode>> params,
                Ptr<ASTNode> block)
      : name_(name), params_(std::move(params)), block_(std::move(block)) {}

 private:
  friend class Interpreter;
  friend class SymbolTableBuilder;
  const std::string name_;
  std::vector<Ptr<ASTNode>> params_;
  Ptr<ASTNode> block_;
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