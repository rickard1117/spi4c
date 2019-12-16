#include "Parser.h"
#include <assert.h>

namespace SI {
namespace Interpreter {
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

} // namespace Interpreter
} // namespace SI