#include "Lexer.h"
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

int Lexer::parseNum() {
    std::size_t end = idx_ + 1;
    while (isNum(text_[end]) && end < text_.length()) {
        end++;
    }
    // FIXME : substr性能问题？
    auto result = std::stoi(text_.substr(idx_, end - idx_));
    idx_ = end;
    return result;
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

bool Lexer::parseAssign() {
    std::size_t end = idx_ + 2;
    if (text_.length() < end || !StringPiece(&text_[idx_], end).equal(":=")) {
        return false;
    }
    idx_ = end;
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

Token Lexer::getNextToken() {
    Token t;

    if (current() == ' ') {
        skipSpaces();
    }

    if (current() == '\0') {
        t.type_ = Token::Type::kEof;
        return t;
    }

    if (isNum(current())) {
        t.type_ = Token::Type::kNum;
        t.value_ = parseNum();
        return t;
    }

    if (current() == 'B') {
        if (parseBegin()) {
            t.type_ = Token::Type::kBegin;
            return t;
        }
        parseVar();
        t.type_ = Token::Type::kVar;
        return t;
    }

    if (current() == 'E') {
        if (parseEnd()) {
            t.type_ = Token::Type::kEnd;
            return t;
        }
        parseVar();
        t.type_ = Token::Type::kVar;
        return t;
    }

    if (isLetter(current())) {
        parseVar();
        t.type_ = Token::Type::kVar;
        return t;
    }

    if (current() == ':') {
        parseAssign();
        t.type_ = Token::Type::kAssign;
        return t;
    }

    if (current() == '+') {
        t.type_ = Token::Type::kPlus;
    } else if (current() == '-') {
        t.type_ = Token::Type::kMinus;
    } else if (current() == '*') {
        t.type_ = Token::Type::kMul;
    } else if (current() == '/') {
        t.type_ = Token::Type::kDiv;
    } else if (current() == '(') {
        t.type_ = Token::Type::kLparent;
    } else if (current() == ')') {
        t.type_ = Token::Type::kRparent;
    } else if (current() == '.') {
        t.type_ = Token::Type::kDot;
    } else {
        throw "bad token";
    }
    advance();
    return t;
}

} // namespace Interpreter
} // namespace SI