// #include "Interpreter.h"
#include "Lexer.h"
#include "NodeVisitor.h"
#include "Parser.h"
#include "gtest/gtest.h"

using SI::Interpreter::AST;
using SI::Interpreter::Lexer;
using SI::Interpreter::NodeVisitor;
using SI::Interpreter::Parser;
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

TEST(Lexer, TestVarToken) {
  Lexer l{"aAbBcC := 3"};
  EXPECT_EQ(l.getNextToken().type(), Token::Type::kVar);
  EXPECT_EQ(l.getNextToken().type(), Token::Type::kAssign);
  EXPECT_EQ(l.getNextToken().type(), Token::Type::kNum);
}

TEST(Lexer, TestSemiToken) {
  Lexer l{";"};
  EXPECT_EQ(l.getNextToken().type(), Token::Type::kSemi);
}

static int parseArithmeticExpr(const std::string &formula) {
  Parser p{formula};
  auto ast = p.expr();
  NodeVisitor visitor;
  return ast->accept(&visitor);
}

TEST(TestParser, OneNumFormula) { ASSERT_EQ(parseArithmeticExpr("2"), 2); }

TEST(TestParser, CharWithSpaces) { ASSERT_EQ(parseArithmeticExpr("  2 "), 2); }

TEST(TestParser, TwoNumFormula) { ASSERT_EQ(parseArithmeticExpr(" 2 * 3"), 6); }

TEST(TestParser, MultiNumPlusMinusFormula) {
  ASSERT_EQ(parseArithmeticExpr(" 12 + 133 - 1 - 10"), 134);
}

TEST(TestParser, MultiMixedFormula) {
  ASSERT_EQ(parseArithmeticExpr(" 1 + 10 * 10 - 6 / 2 + 20"), 118);
}

TEST(TestParser, MixedWithParentFormula) {
  ASSERT_EQ(parseArithmeticExpr(" (1 + 2) * 3 + 4 / 2"), 11);
}

TEST(TestParser, ComplicatedFormula) {
  ASSERT_EQ(parseArithmeticExpr(
                "7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)"),
            10);
}

TEST(TestParser, MultiParents) {
  ASSERT_EQ(parseArithmeticExpr("7 + (((3 + 2)))"), 12);
}

TEST(TestParser, SimpleUnary) { ASSERT_EQ(parseArithmeticExpr("-3"), -3); }

static std::map<std::string, int> parseAssignmentStatement(
    const std::string &formula) {
  Parser p{formula};
  auto ast = p.assignmentStatement();
  NodeVisitor visitor;
  ast->accept(&visitor);
  return visitor.varsTable();
}

TEST(TestParser, SimpleAssignment) {
  auto table = parseAssignmentStatement("abc := 3");
  ASSERT_EQ(table["abc"], 3);
}

TEST(TestParser, MultiAssignCompoundStatement) {
  const std::string s = "BEGIN abc:=3; def := 4; ghi:= 5 END";
  Parser p{s};
  auto ast = p.compoundStatement();
  NodeVisitor visitor;
  ast->accept(&visitor);
  auto table = visitor.varsTable();

  ASSERT_EQ(table["abc"], 3);
  ASSERT_EQ(table["def"], 4);
  ASSERT_EQ(table["ghi"], 5);
}

TEST(TestParser, MultiCompoundStatement) {
  const std::string s =
      "BEGIN"
      "abc:=3;"
      "BEGIN"
      "xxx:=123;"
      "yyy:=234"
      "END;"
      "def := 4;"
      "ghi:= 5"
      "END.";
  Parser p{s};
  auto ast = p.compoundStatement();
  NodeVisitor visitor;
  ast->accept(&visitor);
  auto table = visitor.varsTable();

  ASSERT_EQ(table["abc"], 3);
  ASSERT_EQ(table["def"], 4);
  ASSERT_EQ(table["ghi"], 5);
  ASSERT_EQ(table["xxx"], 123);
  ASSERT_EQ(table["yyy"], 234);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}