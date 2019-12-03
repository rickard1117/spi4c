#include "si.h"
#include "gtest/gtest.h"

using SI::Interpreter::AST;
using SI::Interpreter::Interpreter;
using SI::Interpreter::Lexer;
using SI::Interpreter::Token;

TEST(Lexer, TestSingleEOF) {
    Lexer l{""};
    auto t = l.getNextToken();
    ASSERT_EQ(t.type(), Token::Type::kEof);
}

TEST(Lexer, TestSingleSpace) {
    Lexer l{" "};
    ASSERT_EQ(l.getNextToken().type(), Token::Type::kEof);
}

TEST(Lexer, TestOneCharNum) {
    Lexer l{"2"};
    auto t = l.getNextToken();
    ASSERT_EQ(t.type(), Token::Type::kNum);
    ASSERT_EQ(t.value(), 2);
}

TEST(Lexer, TestMultiCharNum) {
    Lexer l{"223"};
    auto t = l.getNextToken();
    ASSERT_EQ(t.type(), Token::Type::kNum);
    ASSERT_EQ(t.value(), 223);
}

TEST(Lexer, TestSingleOp) {
    Lexer l{"*"};
    ASSERT_EQ(l.getNextToken().type(), Token::Type::kMul);
    ASSERT_EQ(l.getNextToken().type(), Token::Type::kEof);
}

TEST(Lexer, TestBadFormula) {
    Lexer l{"$"};
    ASSERT_ANY_THROW(l.getNextToken());
}

TEST(Lexer, TestComplicatedFormula) {
    Lexer l{"3 * ( 123 + 456) / 20 + 1 "};

    auto t = l.getNextToken();
    EXPECT_EQ(t.type(), Token::Type::kNum);
    EXPECT_EQ(t.value(), 3);
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kMul);
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kLparent);

    t = l.getNextToken();
    EXPECT_EQ(t.type(), Token::Type::kNum);
    EXPECT_EQ(t.value(), 123);

    EXPECT_EQ(l.getNextToken().type(), Token::Type::kPlus);

    t = l.getNextToken();
    EXPECT_EQ(t.type(), Token::Type::kNum);
    EXPECT_EQ(t.value(), 456);

    EXPECT_EQ(l.getNextToken().type(), Token::Type::kRparent);
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kDiv);

    t = l.getNextToken();
    EXPECT_EQ(t.type(), Token::Type::kNum);
    EXPECT_EQ(t.value(), 20);

    EXPECT_EQ(l.getNextToken().type(), Token::Type::kPlus);

    t = l.getNextToken();
    EXPECT_EQ(t.type(), Token::Type::kNum);
    EXPECT_EQ(t.value(), 1);
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kEof);
}

TEST(Lexer, TestBeginToken) {
    Lexer l{"BEGIN"};
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kBegin);
}

TEST(Lexer, TestDotToken) {
    Lexer l{"."};
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kDot);
}

TEST(Lexer, TestAssignToken) {
    Lexer l{":="};
    EXPECT_EQ(l.getNextToken().type(), Token::Type::kAssign);
}

class TestInterpreter : public ::testing::Test {
  public:
    Interpreter i;
};

TEST_F(TestInterpreter, OneNumFormula) { ASSERT_EQ(i.interpret("2"), 2); }

TEST_F(TestInterpreter, CharWithSpaces) { ASSERT_EQ(i.interpret("  2 "), 2); }

TEST_F(TestInterpreter, TwoNumFormula) { ASSERT_EQ(i.interpret(" 2 * 3"), 6); }

TEST_F(TestInterpreter, MultiNumPlusMinusFormula) {
    ASSERT_EQ(i.interpret(" 12 + 133 - 1 - 10"), 134);
}

TEST_F(TestInterpreter, MultiMixedFormula) {
    ASSERT_EQ(i.interpret(" 1 + 10 * 10 - 6 / 2 + 20"), 118);
}

TEST_F(TestInterpreter, MixedWithParentFormula) {
    ASSERT_EQ(i.interpret(" (1 + 2) * 3 + 4 / 2"), 11);
}

TEST_F(TestInterpreter, ComplicatedFormula) {
    ASSERT_EQ(i.interpret("7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)"), 10);
}

TEST_F(TestInterpreter, MultiParents) { ASSERT_EQ(i.interpret("7 + (((3 + 2)))"), 12); }

TEST_F(TestInterpreter, SimpleUnary) { ASSERT_EQ(i.interpret("-3"), -3); }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}