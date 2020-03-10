#include "Lexer.h"

#include "Error.h"
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

void Lexer::advance() {
  if (current() == '\n') {
    currentLine_++;
    currentCol_ = 1;
  } else {
    currentCol_++;
  }
  idx_++;
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

  return std::make_unique<Token>(TokenType::kNumber, currentLine_, currentCol_,
                                 std::move(val));
}

std::unique_ptr<Token> Lexer::make_keyword(TokenType id) {
  return std::make_unique<Token>(id, currentLine_, currentCol_);
}

std::unique_ptr<Token> Lexer::read_rep(char expect, TokenType id,
                                       TokenType els) {
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

  auto tokid = Token::valToType(val);
  std::unique_ptr<Token> tok;
  if (tokid != TokenType::kNull) {
    tok = std::make_unique<Token>(tokid, currentLine_, currentCol_);
  } else {
    tok = std::make_unique<Token>(TokenType::kVar, currentLine_, currentCol_,
                                  std::move(val));
  }

  return tok;
}

void Lexer::skip_comment(char c) {
  while (c != '}') {
    c = readc();
  };
}

std::unique_ptr<Token> Lexer::getNextToken() {
  skipSpaces();
  auto c = readc();

  switch (c) {
    case '\0':
      return std::make_unique<Token>(TokenType::kEof, currentLine_,
                                     currentCol_);
    case '0' ... '9':
      return read_number(c);
    case ':':
      return read_rep('=', TokenType::kAssign, TokenType::kColon);
    case '+':
      return read_rep('+', TokenType::kInc, TokenType::kPlus);
    case '-':
      return read_rep('+', TokenType::kDec, TokenType::kMinus);
    case '*':
      return make_keyword(TokenType::kStar);
    case '/':
      return make_keyword(TokenType::kSlash);
    case '(':
      return make_keyword(TokenType::kLparent);
    case ')':
      return make_keyword(TokenType::kRparent);
    case '.':
      return make_keyword(TokenType::kDot);
    case ';':
      return make_keyword(TokenType::kSemi);
    case ',':
      return make_keyword(TokenType::kComma);
    case '{': {
      skip_comment(c);
      return getNextToken();
    }
    case 'a' ... 'z':
    case 'A' ... 'Z':
      return read_ident(c);
    default:
      throw InterpreterError(kErrorUnrecognized, c);
  }
  SI_ASSERT(0);
}

}  // namespace SI