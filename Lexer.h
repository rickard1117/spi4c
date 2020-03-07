#ifndef LEXER_H_
#define LEXER_H_

#include <memory>
#include <string>

#include "Token.h"
// #include "AST.h"

namespace SI {
class Lexer {
 public:
  Lexer() : idx_(0) {}

  Lexer(const std::string &text) : text_(text), idx_(0) {}

  std::unique_ptr<Token> getNextToken();

  void init(const std::string &text) {
    text_ = text;
    idx_ = 0;
  }

  std::string remaning() const { return text_.substr(idx_); }

 private:
  std::unique_ptr<Token> read_number(char c);
  std::unique_ptr<Token> read_ident(char c);
  std::unique_ptr<Token> read_rep(char expect, TokenId id, TokenId els);
  std::unique_ptr<Token> make_keyword(TokenId id);
  void skip_comment(char c);
  void skipSpaces();
  char readc();
  char current() const;
  bool next(char expect);
  void advance(int step = 1) { idx_ += step; }
  void back(int step = 1);

  std::string text_;
  std::size_t idx_;
};

}  // namespace SI

#endif  // LEXER_H_