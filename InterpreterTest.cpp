#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "SymbolTableBuilder.h"
#include "gtest/gtest.h"

using namespace SI;

static GeneralArithVal parseArithmeticExpr(const std::string &formula) {
  Parser p{formula};
  auto ast = p.expr();
  Interpreter visitor;
  return visitor.visitArithExpr(*ast);
}

TEST(TestParser, OneNumFormula) { ASSERT_EQ(parseArithmeticExpr("2"), 2); }

TEST(TestParser, CharWithSpaces) { ASSERT_EQ(parseArithmeticExpr("  2 "), 2); }

TEST(TestParser, TwoNumFormula) { ASSERT_EQ(parseArithmeticExpr(" 2 * 3"), 6); }

TEST(TestParser, MultiNumPlusMinusFormula) {
  ASSERT_EQ(parseArithmeticExpr(" 12 + 133 - 1 - 10"), 134);
}

TEST(TestParser, MultiMixedFormula) {
  ASSERT_EQ(parseArithmeticExpr(" 1 + 10 * 10 - 6 / 2 + 20"), 118.0);
}

TEST(TestParser, MixedWithParentFormula) {
  ASSERT_EQ(parseArithmeticExpr(" (1 + 2) * 3 + 4 / 2"), 11.0);
}

TEST(TestParser, ComplicatedFormula) {
  ASSERT_EQ(parseArithmeticExpr(
                "7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)"),
            10.0);
}

TEST(TestParser, MultiParents) {
  ASSERT_EQ(parseArithmeticExpr("7 + (((3 + 2)))"), 12);
}

TEST(TestParser, SimpleUnary) { ASSERT_EQ(parseArithmeticExpr("-3"), -3); }

// static std::map<std::string, int> parseAssignmentStatement(
//     const std::string &formula) {
//   Parser p{formula};
//   auto ast = p.assignmentStatement();
//   Interpreter visitor;
//   visitor.visitAssignment(ast->fetch<Assignment>());
//   return visitor.symbols();
// }

// TEST(TestParser, SimpleAssignment) {
//   auto table = parseAssignmentStatement("abc := 3");
//   ASSERT_EQ(table["abc"], 3);
// }

static const std::map<std::string, GeneralArithVal> ParserProgram(
    const std::string &s) {
  Parser p{s};
  auto ast = p.program();
  Interpreter visitor;
  visitor.visitProgram(ast->fetch<Program>());
  return visitor.symbols();
}

// TEST(TestParser, MultiAssignCompoundStatementProgram) {
//   const std::string s = "PROGRAM part3;BEGIN abc:=3; def := 4; ghi:= 5 END.";
//   auto table = ParserProgram(s);

//   ASSERT_EQ(table["abc"], 3);
//   ASSERT_EQ(table["def"], 4);
//   ASSERT_EQ(table["ghi"], 5);
// }

TEST(TestParser, UseDefinedVar) {
  const std::string s = R"(
    PROGRAM part1;
    VAR
      A1, b : INTEGER;
    BEGIN 
      A1 := 1; 
      b := A1 + 2;
    END.
  )";
  auto table = ParserProgram(s);
  ASSERT_EQ(table["A1"], 1);
  ASSERT_EQ(table["b"], 3);
}

TEST(TestParser, TestRealArith) {
  const std::string s = R"(
    PROGRAM part2;
    VAR
      a     : INTEGER;
      b,c   : REAL;
    BEGIN
      a := 3;
      b := 3 + 4.2;
      c := 1 + b * a / 2 + 1;
    END.
  )";

  auto table = ParserProgram(s);

  ASSERT_EQ(table["a"], 3);
  ASSERT_EQ(table["b"], 7.2);
  ASSERT_EQ(table["c"], 12.8);
}

TEST(Lexer, TestComment) {
  const std::string s = R"(
    PROGRAM part2;
    VAR
      number     : INTEGER; { a number decl}
      real       : REAL;
    BEGIN
      {this is a comment}
      number := 5 DIV 2;
      real := 5 / 2;
    END.
  )";
  auto table = ParserProgram(s);

  ASSERT_EQ(table["number"], 2);
  ASSERT_EQ(table["real"], 2.5);
}

TEST(TestParser, IntegerAndRealDiv) {
  const std::string s = R"(
    PROGRAM part2;
    VAR
      number     : INTEGER;
      real       : REAL;
    BEGIN
      number := 5 DIV 2;
      real := 5 / 2;
    END.
  )";

  auto table = ParserProgram(s);
  // auto num = table["number"];

  ASSERT_EQ(table["number"], 2);
  ASSERT_EQ(table["real"], 2.5);
}

TEST(TestParser, DeclAndCompound) {
  const std::string s = R"(
    PROGRAM part2;
    VAR
      number     : INTEGER;
      a, b, c, x : INTEGER;
    BEGIN
      number := 3;
      BEGIN
        c := 0;
        x := c + 3;
      END;
      a := 4;
    END.
  )";

  auto table = ParserProgram(s);

  ASSERT_EQ(table["number"], 3);
  ASSERT_EQ(table["a"], 4);
  ASSERT_EQ(table["c"], 0);
  ASSERT_EQ(table["x"], 3);
}

TEST(TestParser, TestSimpleProcedureDefine) {
  const std::string s = R"(
    PROGRAM Part12;
    VAR
      a : INTEGER;

    PROCEDURE P1;
    VAR
      a : REAL;
      k : INTEGER;

      PROCEDURE P2;
      VAR
          a, z : INTEGER;
      BEGIN {P2}
          z := 777;
      END;  {P2}

    BEGIN {P1}
    END;  {P1}

    BEGIN {Part12}
      a := 10;
    END.  {Part12}
  )";

  ASSERT_NO_THROW(ParserProgram(s));
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