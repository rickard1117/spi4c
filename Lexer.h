#ifndef LEXER_H_
#define LEXER_H_

#include <string>

#include "AST.h"

namespace SI {
namespace Interpreter {
class Lexer {
 public:
  Lexer() : idx_(0) {}

  Lexer(const std::string &text) : text_(text), idx_(0) {}

  Token getNextToken();

  void init(const std::string &text) {
    text_ = text;
    idx_ = 0;
  }

 private:
  bool isNum(const char &c) { return '0' <= c && c <= '9'; }
  bool isLetter(const char &c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
  }
  void skipSpaces();
  bool parseNum(Token *);
  char current() const;
  void advance(int step = 1) { idx_ += step; }
  bool parseBegin();
  bool parseEnd();
  bool parseAssign(Token *);
  bool parseString(Token *);
  std::string parseVar();
  std::string text_;
  std::size_t idx_;
};

}  // namespace Interpreter
}  // namespace SI

#endif  // LEXER_H_