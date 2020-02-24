#include "Token.h"

#include <unordered_map>

namespace {
using SI::Interpreter::TokenId;

std::unordered_map<std::string, TokenId> keywords;
void keywordsInsert(TokenId id, const std::string &val) { keywords[val] = id; }

int init() {
  // using SI::Interpreter::TokenId;
#define op(name, str) keywordsInsert(TokenId::name, str);
#define keyword(name, str) keywordsInsert(TokenId::name, str);
#include "keyword.inc"
#undef keyword
#undef op
  return 0;
}

auto __ = init();

}  // namespace

namespace SI {
namespace Interpreter {

TokenId Token::keyword2id(const std::string &id) {
  auto it = keywords.find(id);
  if (it == keywords.cend()) {
    return TokenId::kNull;
  }
  return it->second;
}

}  // namespace Interpreter
}  // namespace SI