#include "Lexer.h"
#include "assert.h"
#include "util.h"

namespace SI {
namespace Interpreter {

using SI::util::StringPiece;

char Lexer::current() const {
    if (text_.empty() || idx_ >= text_.length()) {
        return '\0';
    }
    return text_[idx_];
}

void Lexer::skipSpaces() {
    while (current() == ' ') {
        advance();
    }
}

bool Lexer::parseNum(Token *t) {
    assert(isNum(current()));
    std::size_t end = idx_ + 1;
    while (isNum(text_[end]) && end < text_.length()) {
        end++;
    }
    // FIXME : substr性能问题？
    t->type_ = Token::Type::kNum;
    t->value_ = std::stoi(text_.substr(idx_, end - idx_));
    idx_ = end;
    return true;
}

bool Lexer::parseBegin() {
    std::size_t end = idx_ + 5;
    if (text_.length() < end || !StringPiece(&text_[idx_], end).equal("BEGIN")) {
        return false;
    }
    idx_ = end;
    return true;
}

bool Lexer::parseEnd() {
    std::size_t end = idx_ + 3;
    if (text_.length() < end || !StringPiece(&text_[idx_], end).equal("END")) {
        return false;
    }
    idx_ = end;
    return true;
}

bool Lexer::parseAssign(Token *t) {
    std::size_t end = idx_ + 2;
    if (text_.length() < end || !StringPiece(&text_[idx_], end).equal(":=")) {
        return false;
    }
    idx_ = end;
    t->type_ = Token::Type::kAssign;
    return true;
}

std::string Lexer::parseVar() {
    std::size_t end = idx_;
    while (end < text_.length() && isLetter(text_[end])) {
        end++;
    }
    auto var = text_.substr(idx_, end - idx_);
    idx_ = end;
    return var;
}

bool Lexer::parseString(Token *t) {
    assert(isLetter(current()));
    if (StringPiece(&text_[idx_]).equal("BEGIN")) {
        advance(5);
        t->type_ = Token::Type::kBegin;
        return true;
    } else if (StringPiece(&text_[idx_]).equal("END")) {
        advance(3);
        t->type_ = Token::Type::kEnd;
        return true;
    }

    std::size_t end = idx_;
    while (end < text_.length() && isLetter(text_[end])) {
        end++;
    }
    t->type_ = Token::Type::kVar;
    t->varval_ = text_.substr(idx_, end - idx_);
    idx_ = end;
    return true;
}

Token Lexer::getNextToken() {
    Token t;

    skipSpaces();

    if (current() == '\0') {
        t.type_ = Token::Type::kEof;
        return t;
    }

    switch (current()) {
    case '\0':
        t.type_ = Token::Type::kEof;
        return t;
    case '0' ... '9':
        if (parseNum(&t)) {
            return t;
        }
    case ':':
        if (parseAssign(&t)) {
            return t;
        }
    case '+':
        t.type_ = Token::Type::kPlus;
        advance();
        return t;
    case '-':
        t.type_ = Token::Type::kMinus;
        advance();
        return t;
    case '*':
        t.type_ = Token::Type::kMul;
        advance();
        return t;
    case '/':
        t.type_ = Token::Type::kDiv;
        advance();
        return t;
    case '(':
        t.type_ = Token::Type::kLparent;
        advance();
        return t;
    case ')':
        t.type_ = Token::Type::kRparent;
        advance();
        return t;
    case '.':
        t.type_ = Token::Type::kDot;
        advance();
        return t;
    case 'a' ... 'z':
    case 'A' ... 'Z':
        if (parseString(&t)) {
            return t;
        }
    default:
        throw "bad token";
    }
    assert(0);
    return t;
}

} // namespace Interpreter
} // namespace SI