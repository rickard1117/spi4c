#ifndef SI_TOKEN_H_
#define SI_TOKEN_H_

#include <optional>
#include <string>

#include "util.h"

namespace SI {

enum class TokenType {
  kId,
  kKeyword,
  kNumber,
  kString,
  kEof,
};

enum class TokenId {
  kNull,
  kVar,
#define op(name, _) name,
#define keyword(name, _) name,
#include "keyword.inc"
#undef keyword
#undef op
};

class Token : public SI::util::Noncopyable {
 public:
  Token(TokenType type, TokenId id) : type_(type), id_(id) {}
  Token(TokenType type, TokenId id, const std::string &val);

  static TokenId valtoid(const std::string &val);
  static std::string idtoval(TokenId id);

  const std::string val() const;

  TokenType type() const { return type_; }
  TokenId id() const { return id_; }
  bool isKeyword(TokenId id) const {
    return type_ == TokenType::kKeyword && id_ == id;
  }
  bool isVar() const { return type_ == TokenType::kId && id_ == TokenId::kVar; }
  bool isNumber() const { return type_ == TokenType::kNumber; }

 private:
  TokenType type_;
  TokenId id_;
  std::optional<std::string> val_;
};

}  // namespace SI

#endif  // SI_TOKEN_H_