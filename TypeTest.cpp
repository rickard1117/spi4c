#include "Type.h"
#include "gtest/gtest.h"

using namespace SI;

TEST(Type, EqualToInt) {
  GeneralArithVal ga = 5;
  ASSERT_EQ(ga, 5);
}

TEST(Type, EqualToDouble) {
  GeneralArithVal ga = 5.0;
  ASSERT_EQ(ga, 5.0);
}

TEST(Type, NEtoDouble) {
  GeneralArithVal ga = 5;
  ASSERT_NE(ga, 5.0);
}

TEST(Type, Arith) {
  GeneralArithVal i = 5;
  GeneralArithVal d = 3.3;

  ASSERT_EQ(i + d, 8.3);
  ASSERT_NE(i + d, 8);
  ASSERT_EQ(d + i, 8.3);
  ASSERT_NE(d + i, 8);

  ASSERT_EQ(i * d, 16.5);
  ASSERT_NE(i * d, 16);
  ASSERT_EQ(d * i, 16.5);
  ASSERT_NE(d * i, 16);

  GeneralArithVal two = 5;
  GeneralArithVal three = 3;
  ASSERT_EQ(two * three, 15);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}