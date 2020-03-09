#ifndef SI_TOKEN_H_
#define SI_TOKEN_H_

#include <optional>
#include <string>

#include "util.h"

namespace SI {

enum class TokenType {
  kNull = 0,
  kVar,
  kNumber,
// kString,
#define op(name, _) name,
#define keyword(name, _) name,
#include "keyword.inc"
#undef keyword
#undef op
  kEof,
};

// enum class TokenId {
//   kNull,
//   kVar,
// #define op(name, _) name,
// #define keyword(name, _) name,
// #include "keyword.inc"
// #undef keyword
// #undef op
// };

class Token : public SI::util::Noncopyable {
 public:
  Token(TokenType type) : type_(type) {}
  Token(TokenType type, const std::string &val);

  static TokenType valToType(const std::string &val);
  // static std::string idtoval(TokenId id);

  const std::string val() const;

  TokenType type() const { return type_; }
  // TokenId id() const { return id_; }
  // bool isKeyword(TokenType type) const;
  bool isVar() const { return type_ == TokenType::kVar; }
  bool isNumber() const { return type_ == TokenType::kNumber; }

 private:
  TokenType type_;
  std::optional<std::string> val_;
};

}  // namespace SI

#endif  // SI_TOKEN_H_