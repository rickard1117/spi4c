#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTableBuilder.h"
#include "gtest/gtest.h"

using namespace SI;

void CheckSymbolTable(const std::string &s) {
  Parser p{s};
  try {
    auto ast = p.program();
    SymbolTableBuilder visitor{std::make_shared<SymbolTable>()};
    visitor.visitProgram(ast->fetch<Program>());
  } catch (const InterpreterError &err) {
    std::cerr << err.what() << '\n';
    throw;
  }
}

TEST(TestError, UnrecognizedChar) {
  const std::string s = "#";
  Lexer l{s};
  ASSERT_THROW(l.getNextToken(), InterpreterError);
}

TEST(TestError, WrongTypeSpecToken) {
  const std::string s = R"(
    PROGRAM part1;
    VAR
      A1 : BEGIN;
    BEGIN 
      A1 := 1; 
    END.
  )";

  ASSERT_THROW(CheckSymbolTable(s), InterpreterError);
}

TEST(TestError, WrongProgramToken) {
  const std::string s = R"(
    PROGRAMwrong part1;
    VAR
      A1, b : INTEGER;
    BEGIN 
      A1 := 1; 
      b := A1 + 2;
    END.
  )";
  ASSERT_THROW(CheckSymbolTable(s), InterpreterError);
}

TEST(TestError, UndefineVar) {
  const std::string s = R"(
    PROGRAM part1;
    VAR
      A1 : INTEGER;
    BEGIN 
      A2 := 3;
    END.
  )";
  ASSERT_THROW(CheckSymbolTable(s), InterpreterError);
}

TEST(TestError, DupVarDefine) {
  const std::string s = R"(
    PROGRAM part1;
    VAR
      A1 : INTEGER;
      A1 : REAL;
    BEGIN 
      A1 := 3;
    END.
  )";
  ASSERT_THROW(CheckSymbolTable(s), InterpreterError);
}

// TEST(TestError, UndefineVar) {
//   const std::string s = R"(
//     PROGRAM part1;
//     VAR
//       a : INTEGER;
//     BEGIN
//       b := 3;
//     END.
//   )";
//   Parser p{s};
//   auto ast = p.program();
//   SymbolTableBuilder visitor{std::make_shared<SymbolTable>()};
//   try {
//     visitor.visitProgram(ast->fetch<Program>());
//   } catch (const InterpreterError &err) {
//     std::cerr << err.what() << '\n';
//   }

//   // ASSERT_THROW(p.program(), InterpreterError);
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}