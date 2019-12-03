
#include "si.h"
#include <assert.h>
namespace SI {
namespace Interpreter {
static inline bool isNum(const char &c) { return '0' <= c && c <= '9'; }

char Lexer::current() const {
    if (text_.empty() || idx_ >= text_.length()) {
        return '\0';
    }
    return text_[idx_];
}

void Lexer::advance() { idx_++; }

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
    auto result = std::stoi(text_.substr(idx_, end - idx_));
    idx_ = end;
    return result;
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
    } else {
        throw "bad token";
    }
    advance();
    return t;
}

int BinOp::visit() const {
    if (type_ == Token::kPlus) {
        return left_->visit() + right_->visit();
    } else if (type_ == Token::kMinus) {
        return left_->visit() - right_->visit();
    } else if (type_ == Token::kMul) {
        return left_->visit() * right_->visit();
    } else if (type_ == Token::kDiv) {
        return left_->visit() / right_->visit();
    } else {
        throw "bad visit";
    }
}

int UnaryOp::visit() const {
    if (type_ == Token::kPlus) {
        return child_->visit();
    } else if (type_ == Token::kMinus) {
        return child_->visit() * (-1);
    } else {
        throw "bad visit";
    }
}

int Parser::caculate(int num, const Token &op, const Token &token) const {
    if (op.type() == Token::kPlus) {
        num += token.value();
    } else if (op.type() == Token::kMinus) {
        num -= token.value();
    } else if (op.type() == Token::kMul) {
        num *= token.value();
    } else if (op.type() == Token::kDiv) {
        num /= token.value();
    } else if (op.type() == Token::kEof) {
        return num;
    } else {
        assert(0);
    }
    return num;
}

Parser::Parser(const std::string &formula) {
    lexer_.init(formula);
    currentToken_ = lexer_.getNextToken();
}

std::unique_ptr<AST> Parser::factor() {
    if (currentToken_.type() == Token::kNum) {
        std::unique_ptr<AST> num(new Num(currentToken_.value()));
        currentToken_ = lexer_.getNextToken();
        return num;
    }

    if (currentToken_.type() == Token::kLparent) {
        currentToken_ = lexer_.getNextToken();
        auto ast = expr();
        if (currentToken_.type() != Token::kRparent) {
            throw "bad factor";
        }
        currentToken_ = lexer_.getNextToken();
        return ast;
    }

    if (currentToken_.type() == Token::kPlus || currentToken_.type() == Token::kMinus) {
        auto type = currentToken_.type();
        currentToken_ = lexer_.getNextToken();
        return std::unique_ptr<AST>(new UnaryOp(type, std::move(factor())));
    }
    throw "bad factor";
}

std::unique_ptr<AST> Parser::term() {
    auto left = factor();
    while (currentToken_.type() != Token::kEof) {
        if (currentToken_.type() == Token::kMul) {
            currentToken_ = lexer_.getNextToken();
            auto right = factor();
            left.reset(new BinOp(Token::kMul, std::move(left), std::move(right)));
        } else if (currentToken_.type() == Token::kDiv) {
            currentToken_ = lexer_.getNextToken();
            auto right = factor();
            left.reset(new BinOp(Token::kDiv, std::move(left), std::move(right)));
        } else {
            break;
        }
    }
    return left;
}

std::unique_ptr<AST> Parser::expr() {
    auto left = term();
    while (currentToken_.type() != Token::kEof) {
        if (currentToken_.type() == Token::kPlus) {
            currentToken_ = lexer_.getNextToken();
            auto right = term();
            left.reset(new BinOp(Token::kPlus, std::move(left), std::move(right)));
        } else if (currentToken_.type() == Token::kMinus) {
            currentToken_ = lexer_.getNextToken();
            auto right = term();
            left.reset(new BinOp(Token::kMinus, std::move(left), std::move(right)));
        } else {
            break;
        }
    }

    return left;
}

int Interpreter::interpret(const std::string &formula) const {
    Parser parser{formula};
    std::unique_ptr<AST> tree = parser.expr();
    return visit(*tree);
}

} // namespace Interpreter
} // namespace SI
