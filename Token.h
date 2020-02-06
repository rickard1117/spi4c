#ifndef SI_TOKEN_H_
#define SI_TOKEN_H_

#include <string>

namespace SI {
namespace Interpreter {

enum class TokenTypes {
  kEof,
  kNull,
  kPlus,     // +
  kMinus,    // -
  kStar,      // *
  kSlash,      // /
  kLparent,  // (
  kRparent,  // )
  kBegin,    // BEGIN
  kEnd,      // END
  kDot,      // .
  kAssign,   // :=
  kVar,      // VAR
  kId,
  kSemi,     // ;
  kProgram,  // PROGRAM
  kVardecl,  // VAR
  kColon,    // :
  kInteger,
  kReal,
  kCompound,
  kBlock,
};

class Token {
 public:
//   Token() : type_(TokenTypes::kNull),intval_(0),realval(0.0) {}
  Token(TokenTypes type = TokenTypes::kNull):type_(type),intval_(0),realval(0.0){}
//   Token(TokenTypes type, const std::string &val) : type_(type),intval_(0),realval(0.0), strval_(val) {}

  Token(TokenTypes type, int val):type_(type), intval_(val), realval_(0.0) {
    assert(token.type_ == TokenTypes::kInteger);
  }

  Token(TokenTypes type, double real):type_(type),intval_(0),realval(real) {
    assert(token.type_ == TokenTypes::kReal);
  }

  TokenTypes type() const {
      return type_;
  }
  int intval() const {
      return intval_;
  }

  double realval() const {
      return realval_;
  }

  const std::string &strval() const {
      return strval_;
  }
 private:
  friend class Lexer;
  friend class ASTNode;
  TokenTypes type_;
  int intval_;
  double realval_;
  const std::string strval_;
};

// template <typename T = void *>
// class Token {
//  public:
//   Token(const std::string &name, const T &val = nullptr)
//       : val_(val), name_(name) {}
//   T val() const { return val_; }
//   std::string name() const { return name_; }

//  private:
//   const T val_;  // token的內容,可以用于词法分析匹配。
//   const std::string name_;
// };

}  // namespace Interpreter
}  // namespace SI

#endif  // SI_TOKEN_H_