#include "gtest/gtest.h"
#include "interface.h"

TEST(abc,ghi)
{
  ASSERT_EQ(0,0);
  //ASSERT_EQ(1,2);
  //EXPECT_EQ(1,2);
  ASSERT_EQ(1,1);
}

TEST(abc,def)
{
  EXPECT_EQ(0,0);
  //EXPECT_EQ(1,2);
  //EXPECT_EQ(1,2);
  EXPECT_EQ(1,1);
}

TEST(abc,jkl)
{
  EXPECT_EQ(add(1,2),3);
}

