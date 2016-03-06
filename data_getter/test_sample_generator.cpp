#include "getters.h"
#include <iostream>
#include <list>
#include <array>
#include <cstring>
#include "util_templates.h"
#include "list_vector_stream.h"
#include "array_stream.h"
#include <gtest/gtest.h>

TEST(ConstLimitedGetter, equalLimitAndDestinationSize)
{
  int dst[10];
  memset(dst, 0, sizeof(dst));

  ConstLimitedGetter<int> cg(1, 10);

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);
  cg.restart();
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(10,1));

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(ConstLimitedGetter, limitSmallerThanDestinationSize)
{
  int dst[11];
  memset(dst, 0, sizeof(dst));

  ConstLimitedGetter<int> cg(1, 10);

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 10);
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,1,1,1,1, 1,1,1,1,1, 0}));
  cg.restart();
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 10);
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(ConstLimitedGetter, limitGreaterThanDestinationSize)
{
  ConstLimitedGetter<int> cg(1, 10);

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  int dst[4];
  memset(dst, 0, sizeof(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 2);
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(4,1));
  cg.restart();
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 2);
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == 0);

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(ConstUnlimitedGetter, multipleGets)
{
  ConstUnlimitedGetter<int> cg(1);

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  int dst[4];
  memset(dst, 0, sizeof(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(cg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(4,1));

  std::cout << HERE() << ' ' << STR(cg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(VariableLimitedGetter, limitGreaterThanDestinationSize)
{
  const std::vector<int> variable = { 1, 2, 3 };
  VariableLimitedGetter<int> vg(variable,10);

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  int dst[4];
  memset(dst, 0, sizeof(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  vg.restart();
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,3,1,2}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == 2);
  EXPECT_TRUE(std::list<int>(dst, dst + 2) == std::list<int>({3,1}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == 0);
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == 0);
  vg.restart();
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(VariableUnlimitedGetter, multipleGets)
{
  const std::vector<int> variable = { 1, 2, 3 };
  VariableUnlimitedGetter<int> vg(variable,10);

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  int dst[4];
  memset(dst, 0, sizeof(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  vg.restart();
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,3,1,2}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({3,1,1,2}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({3,1,2,3}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,3,1,2}));
  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({3,1,1,2}));

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(VariableUnlimitedGetter, largeGet)
{
  const std::vector<int> variable = { 1, 2, 3 };
  VariableUnlimitedGetter<int> vg(variable,5);

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  int dst[20];
  memset(dst, 0, sizeof(dst));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(vg.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,1,2}));

  std::cout << HERE() << ' ' << STR(vg) << '\n';

  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(ClgVlgCug, onBoundaries)
{
  ClgVlgCug<int> cvc(1,5, // clg
                     {1,2,3},10, // vlg
                     2); // cul

  int dst[10];
  memset(dst, 0, sizeof(dst));

  EXPECT_TRUE(cvc.get(dst,5) == 5);
  EXPECT_TRUE(std::list<int>(dst, dst + 5) == std::list<int>(5,1));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cvc.get(dst, 10) == 10);
  EXPECT_TRUE(std::list<int>(dst, dst + 10) == std::list<int>({1,2,3,1,2,3,1,2,3,1}));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';

  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(10,2));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(10,2));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(10,2));
  std::cout << HERE() << ' ' << std::list<int>(dst, dst + array_length(dst)) << '\n';
}

TEST(ClgVlgCug, notOnBoundariesSmall)
{
  ClgVlgCug<int> cvc(1,5, // clg
                     {1,2,3},10, // vlg
                     2); // cul

  int dst[4];
  memset(dst, 0, sizeof(dst));

  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(4,1));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,1,2,3}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,2,3,1}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,3,1,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2}));
}

TEST(ClgVlgCug, notOnBoundariesLarger)
{
  ClgVlgCug<int> cvc(1,5, // clg
                     {1,2,3},10, // vlg
                     2); // cul

  int dst[12];
  memset(dst, 0, sizeof(dst));

  EXPECT_TRUE(cvc.get(dst, 4) == 4);
  EXPECT_TRUE(std::list<int>(dst, dst + 4) == std::list<int>(4,1));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,1,2,3,1,2,3,1,2,3,1,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2,2,2,2,2,2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2,2,2,2,2,2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2,2,2,2,2,2,2,2,2}));
}

TEST(ClgVlgCug, notOnBoundariesLargerStill)
{
  ClgVlgCug<int> cvc(1,5, // clg
                     {1,2,3},10, // vlg
                     2); // cul

  int dst[12];
  memset(dst, 0, sizeof(dst));

  EXPECT_TRUE(cvc.get(dst, 7) == 7);
  EXPECT_TRUE(std::list<int>(dst, dst + 7) == std::list<int>({1,1,1,1,1,1,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({3,1,2,3,1,2,3,1,2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2,2,2,2,2,2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({2,2,2,2,2,2,2,2,2,2,2,2}));
}

TEST(ClgVlgCug, Largest)
{
  ClgVlgCug<int> cvc(1,5, // clg
                     {1,2,3},10, // vlg
                     2); // cul

  int dst[20];
  memset(dst, 0, sizeof(dst));

  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>({1,1,1,1,1,
                                                                              1,2,3,1,2,3,1,2,3,1,
                                                                              2,2,2,2,2}));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(array_length(dst),2));
  EXPECT_TRUE(cvc.get(dst, array_length(dst)) == array_length(dst));
  EXPECT_TRUE(std::list<int>(dst, dst + array_length(dst)) == std::list<int>(array_length(dst),2));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
