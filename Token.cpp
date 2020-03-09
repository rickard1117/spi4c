#include "Token.h"

#include <unordered_map>

#include "util.h"

namespace {
using SI::TokenId;

std::unordered_map<std::string, TokenId> val2id;
std::unordered_map<TokenId, std::string> id2val;
void mapinsert(TokenId id, const std::string &val) {
  val2id[val] = id;
  id2val[id] = val;
}

int init() {
#define op(name, str) mapinsert(TokenId::name, str);
#define keyword(name, str) mapinsert(TokenId::name, str);
#include "keyword.inc"
#undef keyword
#undef op
  return 0;
}

auto __ = init();

}  // namespace

namespace SI {

using namespace SI::util;

Token::Token(TokenType type, TokenId id, const std::string &val)
    : type_(type), id_(id), val_(val) {
  SI_ASSERT(val != "");
}

const std::string Token::val() const {
  if (isVar() || isNumber()) {
    return val_.value();
  }
  return Token::idtoval(id_);
}

TokenId Token::valtoid(const std::string &id) {
  auto it = val2id.find(id);
  if (it == val2id.cend()) {
    return TokenId::kNull;
  }
  return it->second;
}

std::string Token::idtoval(TokenId id) {
  auto it = id2val.find(id);
  if (it == id2val.cend()) {
    return "";
  }
  return it->second;
}

}  // namespace SI