#include "Lexer.h"

#include "Token.h"
#include "util.h"

namespace SI {
using namespace SI::util;

char Lexer::current() const {
  if (text_.empty() || idx_ >= text_.length()) {
    return '\0';
  }
  return text_[idx_];
}

void Lexer::skipSpaces() {
  while (isblank(current()) || '\n' == current()) {
    advance();
  }
}

void Lexer::back(int step) {
  idx_ -= step;
  SI_ASSERT(idx_ >= 0);
}

bool Lexer::next(char expect) {
  auto c = readc();
  if (c == expect) {
    return true;
  }
  back();
  return false;
}

std::unique_ptr<Token> Lexer::read_number(char c) {
  SI_ASSERT(isdigit(c));
  std::string val;
  auto dotflag = false;
  char last = c;
  for (;; val.push_back(c), last = c, c = readc()) {
    if (isdigit(c)) {
      continue;
    }
    if (c == '.' && !dotflag) {
      dotflag = true;
      continue;
    }
    break;
  }
  back();
  if (last == '.') {
    back();
  }

  return std::make_unique<Token>(TokenType::kNumber, TokenId::kNull,
                                 std::move(val));
}

std::unique_ptr<Token> Lexer::make_keyword(TokenId id) {
  return std::make_unique<Token>(TokenType::kKeyword, id);
}

std::unique_ptr<Token> Lexer::read_rep(char expect, TokenId id, TokenId els) {
  return make_keyword(next(expect) ? id : els);
}

char Lexer::readc() {
  auto c = current();
  advance();
  return c;
}

std::unique_ptr<Token> Lexer::read_ident(char c) {
  SI_ASSERT(isalpha(c));

  std::string val;
  while (isalpha(c) || isdigit(c) || c == '_') {
    val.push_back(c);
    c = readc();
  }
  back();

  auto tokid = Token::keyword2id(val);
  std::unique_ptr<Token> tok;
  if (tokid != TokenId::kNull) {
    tok = std::make_unique<Token>(TokenType::kKeyword, tokid, std::move(val));
  } else {
    tok =
        std::make_unique<Token>(TokenType::kId, TokenId::kVar, std::move(val));
  }

  return tok;
}

void Lexer::skip_comment(char c) {
  while (c != '}') {
    c = readc();
  };
  // back();
}

std::unique_ptr<Token> Lexer::getNextToken() {
  skipSpaces();
  auto c = readc();

  switch (c) {
    case '\0':
      return std::make_unique<Token>(TokenType::kEof, TokenId::kNull);
    case '0' ... '9':
      return read_number(c);
    case ':':
      return read_rep('=', TokenId::kAssign, TokenId::kColon);
    case '+':
      return read_rep('+', TokenId::kInc, TokenId::kPlus);
    case '-':
      return read_rep('+', TokenId::kDec, TokenId::kMinus);
    case '*':
      return make_keyword(TokenId::kStar);
    case '/':
      return make_keyword(TokenId::kSlash);
    case '(':
      return make_keyword(TokenId::kLparent);
    case ')':
      return make_keyword(TokenId::kRparent);
    case '.':
      return make_keyword(TokenId::kDot);
    case ';':
      return make_keyword(TokenId::kSemi);
    case ',':
      return make_keyword(TokenId::kComma);
    case '{': {
      skip_comment(c);
      return getNextToken();
    }
    case 'a' ... 'z':
    case 'A' ... 'Z':
      return read_ident(c);
    default:
      SI_ASSERT_MSG(0, "bad token" + std::to_string(c));
  }
  SI_ASSERT(0);
}

}  // namespace SI