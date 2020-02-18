#ifndef SI_TOKEN_H_
#define SI_TOKEN_H_

#include <string>

namespace SI {
namespace Interpreter {

enum class TokenType {
  kId,
  kKeyword,
  kNumber,
  kString,
  kEof,
};

enum class TokenId {
  kNull,
#define op(name, _) name
  op(kPlus, "+"),
  op(kMinus, "-"),
  op(kStar, "*"),
  op(kSlash, "/"),
  op(kDec, "--"),
  op(kInc, "++"),
  op(kLparent, "("),
  op(kRparent, ")"),
  op(kBegin, "BEGIN"),
  op(kEnd, "END"),
  op(kDot, "."),
  op(kAssign, ":="),
  // op(kVar, "VAR"),
  op(kSemi, ";"),
  op(kProgram, "PROGRAM"),
  op(kVardecl, "VAR"),
  op(kColon, ":"),
#undef op
};

class Token {
 public:
  Token(TokenType type, TokenId id, std::string val = "")
      : type_(type), id_(id), val_(val) {}

  std::string &val() { return val_; }
  const std::string val() const { return val_; }
  TokenType type() const { return type_; }
  TokenId id() const { return id_; }
  bool isKeyword(TokenId id) const {
    return type_ == TokenType::kKeyword && id_ == id;
  }

 private:
  TokenType type_;
  TokenId id_;
  std::string val_;
};

}  // namespace Interpreter
}  // namespace SI

#endif  // SI_TOKEN_H_