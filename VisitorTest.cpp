#include "Lexer.h"
#include "NodeVisitor.h"
#include "Parser.h"
#include "gtest/gtest.h"

using namespace SI::Interpreter;

static int parseArithmeticExpr(const std::string &formula) {
  Parser p{formula};
  auto ast = p.expr();
  ASTNodeVisitor visitor;
  return visitor.visitArithExpr(*ast);
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
  ASTNodeVisitor visitor;
  visitor.visitAssignment(ast->fetch<Assignment>());
  return visitor.symbols();
}

TEST(TestParser, SimpleAssignment) {
  auto table = parseAssignmentStatement("abc := 3");
  ASSERT_EQ(table["abc"], 3);
}

std::map<std::string, int> ParserProgram(const std::string &s) {
  Parser p{s};
  auto ast = p.program();
  ASTNodeVisitor visitor;
  visitor.visitProgram(ast->fetch<Program>());
  return visitor.symbols();
}

TEST(TestParser, MultiAssignCompoundStatementProgram) {
  const std::string s = "PROGRAM part3;BEGIN abc:=3; def := 4; ghi:= 5 END.";
  auto table = ParserProgram(s);

  ASSERT_EQ(table["abc"], 3);
  ASSERT_EQ(table["def"], 4);
  ASSERT_EQ(table["ghi"], 5);
}

// TEST(TestParser, UseDefinedVar) {
//   const std::string s = "PROGRAM part1;BEGIN A1 := 1; b := A1 + 2 END.";
//   auto table = ParserProgram(s);
//   ASSERT_EQ(table["A1"], 1);
//   ASSERT_EQ(table["b"], 3);
// }



TEST(TestParser, MultiCompoundStatementProgram) {
  const std::string s = R"(
    PROGRAM part2;
    BEGIN
      abc:=3;
        BEGIN
          xxx:=123;
          yyy:=234
        END;;
      def := 4;
      ghi:= 5;;
    END.
  )";

  auto table = ParserProgram(s);

  ASSERT_EQ(table["abc"], 3);
  ASSERT_EQ(table["def"], 4);
  ASSERT_EQ(table["ghi"], 5);
  ASSERT_EQ(table["xxx"], 123);
  ASSERT_EQ(table["yyy"], 234);
}

// TEST(TestParser, TestOneIntVarDeclarationsBlock) {
//   const std::string s = "VAR number : INTEGER;";
//   Parser p{s};
//   auto ast = p.variableDeclaration();
//   NodeVisitor visitor;
//   ast->accept(&visitor);
//   // auto table visitor.varsTable();
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}