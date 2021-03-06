#include "Token.h"

#include <unordered_map>

#include "util.h"

namespace {
using SI::TokenType;

std::unordered_map<std::string, TokenType> val2id;
std::unordered_map<TokenType, std::string> id2val;
void mapinsert(TokenType type, const std::string &val) {
  val2id[val] = type;
  id2val[type] = val;
}

int init() {
#define op(name, str) mapinsert(TokenType::name, str);
#define keyword(name, str) mapinsert(TokenType::name, str);
#include "keyword.inc"
#undef keyword
#undef op
  return 0;
}

auto __ = init();

std::string idtoval(TokenType type) {
  auto it = id2val.find(type);
  if (it == id2val.cend()) {
    return "";
  }
  return it->second;
}

}  // namespace

namespace SI {

using namespace SI::util;

Token::Token(TokenType type, std::size_t lineno, std::size_t colno,
             const std::string &val)
    : type_(type), val_(val), lineno_(lineno), colno_(colno) {
  SI_ASSERT(val != "");
}

const std::string Token::val() const {
  if (isID() || isNumber()) {
    return val_.value();
  }
  return idtoval(type_);
}

TokenType Token::valToType(const std::string &val) {
  auto it = val2id.find(val);
  if (it == val2id.cend()) {
    return TokenType::kNull;
  }
  return it->second;
}

// bool Token::isKeyword(TokenType type) const {
//   auto index = static_cast<std::size_t>(type);
//   return index >= static_cast<std::size_t>(TokenType::kBegin) &&
//          index <= static_cast<std::size_t>(TokenType::kEnd);
// }

const std::string Token::toString() const {
  return "[type : " + std::to_string(static_cast<std::size_t>(type_)) +
         " line : " + std::to_string(lineno_) +
         " col : " + std::to_string(colno_) + " val : " + val() + "]";
}

}  // namespace SI