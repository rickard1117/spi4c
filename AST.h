#ifndef AST_H_
#define AST_H_

#include <memory>
#include <string>

namespace SI {
namespace Interpreter {

class NodeVisitor;
class AST {
  public:
    virtual int accept(const NodeVisitor &visitor) const = 0;
};

class Token : public AST {
  public:
    Token() : type_(kNull), value_(0) {}
    enum Type {
        kEof = 0,
        kNull,
        kSpace,
        kPlus,
        kMinus,
        kMul,
        kDiv,
        kLparent,
        kRparent,
        kNum,
        kBegin,
        kEnd,
        kDot,
        kAssign,
        kVar,
    };
    static const std::string BEGIN;
    static const std::string END;
    Type type() const { return type_; }
    int value() const { return value_; }
    bool operator==(const Token &t) {
        if (type_ != t.type_) {
            return false;
        }
        if (type_ == kNum) {
            return value_ == t.value_;
        }
        return true;
    }
    virtual int accept(const NodeVisitor &visitor) const override;

  private:
    friend class NodeVisitor;
    friend class Lexer;
    Type type_;
    int value_;
};

class Num : public AST {
  public:
    Num(int value) : value_(value) {}
    virtual int accept(const NodeVisitor &visitor) const override;

  private:
    friend class NodeVisitor;
    int value_;
};

class BinOp : public AST {
  public:
    BinOp(Token::Type type, std::unique_ptr<AST> left, std::unique_ptr<AST> right)
        : type_(type), left_(std::move(left)), right_(std::move(right)) {}
    virtual int accept(const NodeVisitor &visitor) const override;

  private:
    friend class NodeVisitor;
    Token::Type type_;
    std::unique_ptr<AST> left_;
    std::unique_ptr<AST> right_;
};

class UnaryOp : public AST {
  public:
    UnaryOp(Token::Type type, std::unique_ptr<AST> child) : type_(type), child_(std::move(child)) {}
    virtual int accept(const NodeVisitor &visitor) const override;

  private:
    friend class NodeVisitor;
    Token::Type type_;
    std::unique_ptr<AST> child_;
};

} // namespace Interpreter
} // namespace SI

#endif // AST_H_