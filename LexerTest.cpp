#include "Lexer.h"
#include "gtest/gtest.h"

using namespace SI;

TEST(Lexer, TestSingleEOF) {
  Lexer l{""};
  ASSERT_EQ(l.getNextToken()->type(), TokenType::kEof);
  ASSERT_EQ(l.getNextToken()->type(), TokenType::kEof);
}

TEST(Lexer, TestSingleSpace) {
  Lexer l{" "};
  ASSERT_EQ(l.getNextToken()->type(), TokenType::kEof);
}

TEST(Lexer, TestOneCharInteger) {
  Lexer l{"2"};
  auto t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kNumber);
  ASSERT_EQ(t->val(), "2");
}

TEST(Lexer, TestMultiCharInteger) {
  Lexer l{"223"};
  auto t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kNumber);
  ASSERT_EQ(t->val(), "223");
}

// TEST(Lexer, TestReal) {
//   Lexer l{"223.223"};
//   auto t = l.getNextToken();
//   ASSERT_EQ(t->type(), TokenType::kNumber);
//   ASSERT_EQ(t->val(), "223.223");
// }

TEST(Lexer, TestRealNum) {
  Lexer l{"0.456 + 2.335 - 1.012"};
  auto t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kNumber);
  ASSERT_EQ(t->val(), "0.456");

  t = l.getNextToken();
  ASSERT_TRUE(t->isKeyword(TokenId::kPlus));

  t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kNumber);
  ASSERT_EQ(t->val(), "2.335");

  t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kKeyword);
  ASSERT_TRUE(t->isKeyword(TokenId::kMinus));

  t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kNumber);
  ASSERT_EQ(t->val(), "1.012");
}

// TEST(Lexer, TestBadReal) {
//   Lexer l{"123."};
//   auto t = l.getNextToken();
//   ASSERT_EQ(t->type(), TokenType::kNumber);
//   ASSERT_EQ(t->val(), "123");

//   t = l.getNextToken();
//   ASSERT_EQ(t->type(), TokenType::kKeyword);
//   ASSERT_EQ(t->id(), TokenId::kDot);
// }

TEST(Lexer, TestSingleOp) {
  Lexer l{"*"};
  auto t = l.getNextToken();
  ASSERT_EQ(t->type(), TokenType::kKeyword);
  ASSERT_EQ(t->id(), TokenId::kStar);
}

// TEST(Lexer, TestBadFormula) {
//   Lexer l{"$"};
//   ASSERT_ANY_THROW(l.getNextToken());
// }

TEST(Lexer, TestComplicatedFormula) {
  Lexer l{"3 * ( 123 + 456) / 20 + 1 "};

  auto t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "3");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kStar);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kLparent);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "123");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kPlus);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "456");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kRparent);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kSlash);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "20");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kPlus);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "1");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kEof);
}

TEST(Lexer, TestDotToken) {
  Lexer l{"."};
  auto t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kDot);
}

TEST(Lexer, TestAssignToken) {
  Lexer l{"::="};

  auto t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kColon);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kAssign);
}

TEST(Lexer, TestVarToken) {
  Lexer l{"aAbBcC := 3"};

  auto t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kId);
  EXPECT_EQ(t->val(), "aAbBcC");

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kAssign);

  t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kNumber);
  EXPECT_EQ(t->val(), "3");
}

TEST(Lexer, TestSemiToken) {
  Lexer l{";"};
  auto t = l.getNextToken();
  EXPECT_EQ(t->type(), TokenType::kKeyword);
  EXPECT_EQ(t->id(), TokenId::kSemi);
}

TEST(Token, Testidtoval) {
  Token t{TokenType::kId, TokenId::kVar, "abc123"};
  EXPECT_EQ(t.isVar(), true);
  EXPECT_EQ(t.val(), "abc123");

  Token t2{TokenType::kKeyword, TokenId::kBegin};
  EXPECT_EQ(t2.val(), "BEGIN");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}