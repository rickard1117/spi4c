#ifndef SI_H_
#define SI_H_
#include "util.h"
#include <assert.h>
#include <iostream>
#include <memory>
#include <string>

namespace SI {
namespace Interpreter {

class AST {
  public:
    virtual int visit() const = 0;
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

    virtual int visit() const override {
        assert(type_ == kNum);
        return value_;
    }

  private:
    friend class Lexer;
    Type type_;
    int value_;
};

class Num : public AST {
  public:
    Num(int value) : value_(value) {}
    virtual int visit() const override { return value_; }

  private:
    int value_;
};

class BinOp : public AST {
  public:
    BinOp(Token::Type type, std::unique_ptr<AST> left, std::unique_ptr<AST> right)
        : type_(type), left_(std::move(left)), right_(std::move(right)) {}
    virtual int visit() const override;

  private:
    Token::Type type_;
    std::unique_ptr<AST> left_;
    std::unique_ptr<AST> right_;
};

class UnaryOp : public AST {
  public:
    UnaryOp(Token::Type type, std::unique_ptr<AST> child) : type_(type), child_(std::move(child)) {}
    virtual int visit() const override;

  private:
    Token::Type type_;
    std::unique_ptr<AST> child_;
};

class Lexer {
  public:
    Lexer() : idx_(0) {}

    Lexer(const std::string &text) : text_(text), idx_(0) {}

    Token getNextToken();

    void init(const std::string &text) {
        text_ = text;
        idx_ = 0;
    }

  private:
    bool isNum(const char &c) { return '0' <= c && c <= '9'; }
    bool isLetter(const char &c) {return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');}
    void skipSpaces();
    int parseNum();
    char current() const;
    void advance();
    bool parseBegin();
    bool parseEnd();
    bool parseAssign();
    void parseVar();
    std::string text_;
    std::size_t idx_;
};

class Parser {
  public:
    Parser(const std::string &formula);
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> term();
    std::unique_ptr<AST> factor();

  private:
    int caculate(int num, const Token &op, const Token &t) const;
    Lexer lexer_;
    Token currentToken_;
};

class Interpreter {
  public:
    int interpret(const std::string &formula) const;

  private:
    int visit(const AST &tree) const { return tree.visit(); }
};
} // namespace Interpreter
} // namespace SI

#endif // SI_H_