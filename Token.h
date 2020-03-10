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
#define op(name, _) name,
#define keyword(name, _) name,
#include "keyword.inc"
#undef keyword
#undef op
  kEof,
};

class Token : public SI::util::Noncopyable {
 public:
  Token(TokenType type, std::size_t lineno, std::size_t colno)
      : type_(type), lineno_(lineno), colno_(colno) {}
  Token(TokenType type, std::size_t lineno, std::size_t colno,
        const std::string &val);

  static TokenType valToType(const std::string &val);

  const std::string val() const;
  const std::string toString() const;
  TokenType type() const { return type_; }
  bool isVar() const { return type_ == TokenType::kVar; }
  bool isNumber() const { return type_ == TokenType::kNumber; }

 private:
  TokenType type_;
  std::optional<std::string> val_;
  std::size_t lineno_;
  std::size_t colno_;
};

}  // namespace SI

#endif  // SI_TOKEN_H_