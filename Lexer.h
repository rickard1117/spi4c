#ifndef LEXER_H_
#define LEXER_H_

#include "AST.h"
#include <string>

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
    bool isLetter(const char &c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }
    void skipSpaces();
    int parseNum();
    char current() const;
    void advance() { idx_++; }
    bool parseBegin();
    bool parseEnd();
    bool parseAssign();
    std::string parseVar();
    std::string text_;
    std::size_t idx_;
};

} // namespace Interpreter
} // namespace SI

#endif // LEXER_H_