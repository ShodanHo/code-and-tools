#include <iostream>
#include <gtest/gtest.h>
#include <thread>
#include "mutexedConditionedLists.h"

TEST(ConditionedList, PushCondWait)
{
  conditioned_list<int> l;
  EXPECT_EQ(l.size (), 0);

  std::thread s ([&l]()
  {
    for (int i = 0; i < 10; ++i)
    l.push_back (i);
  });

  while (l.size () < 10)
    std::this_thread::yield ();

  EXPECT_EQ(l.size (), 10);

  auto getFromCondList = [&l]()
  {
    l.cond_wait ();
    auto ret = l.front_value ();
    l.pop_front ();
    return ret;
  };

  for (int i = 0; i < 10; ++i)
  {
    auto ret = getFromCondList ();
    EXPECT_EQ(ret, i);
  }
  EXPECT_EQ(l.size (), 0);

  s.join ();
}

TEST(DualConditionedLists, PushCondWait)
{
  dual_conditioned_lists<int, long> dl;
  auto int_producer = [](dual_conditioned_lists<int, long> *dl)
  {
    for (int i = 0; i < 10; ++i)
      dl->push_back (i);
  };
  auto long_producer = [] (dual_conditioned_lists<int, long>* dl)
  {
    for (long l = 0; l < 10; ++l)
      dl->push_back (l);
  };

  std::thread ip = std::thread (int_producer, &dl);
  std::thread lp = std::thread (long_producer, &dl);

  bool int_producer_term = false;
  bool long_producer_term = false;
  int t1Count = 0;
  int t2Count = 0;
  while (!int_producer_term || !long_producer_term)
  {
    dl.cond_wait ();
    if (dl.sizeT1 () > 0)
    {
      int t1 = dl.front_valueT1 ();
      dl.pop_frontT1 ();
      EXPECT_EQ(t1, t1Count++);
      if (t1 == 9)
        int_producer_term = true;
    }
    if (dl.sizeT2 () > 0)
    {
      long t2 = dl.front_valueT2 ();
      dl.pop_frontT2 ();
      EXPECT_EQ(t2, t2Count++);
      if (t2 == 9)
        long_producer_term = true;
    }
  }
  ip.join ();
  lp.join ();
}

TEST(DualConditionedListsOrdered, PushSizeAndPop)
{
  dual_conditioned_lists_ordered<int, std::string> int_and_strings_list;

  int_and_strings_list.push_back (1);
  int_and_strings_list.push_back ("Hello");
  int_and_strings_list.push_back ("world");
  int_and_strings_list.push_back (0);

  EXPECT_EQ(int_and_strings_list.size (), 4);
  EXPECT_EQ((dual_conditioned_lists_ordered<int, std::string>::typeType::typeT1),
            int_and_strings_list.front_type ());
  {
    auto val = int_and_strings_list.front_valueT1 ();
    int_and_strings_list.pop_front ();
    EXPECT_EQ(val, 1);
  }

  EXPECT_EQ(int_and_strings_list.size (), 3);
  EXPECT_EQ((dual_conditioned_lists_ordered<int, std::string>::typeType::typeT2),
            int_and_strings_list.front_type ());
  {
    auto val = int_and_strings_list.front_valueT2 ();
    int_and_strings_list.pop_front ();
    EXPECT_EQ(val, "Hello");
  }

  EXPECT_EQ(int_and_strings_list.size (), 2);
  EXPECT_EQ((dual_conditioned_lists_ordered<int, std::string>::typeType::typeT2),
            int_and_strings_list.front_type ());
  {
    auto val = int_and_strings_list.front_valueT2 ();
    int_and_strings_list.pop_front ();
    EXPECT_EQ(val, "world");
  }

  EXPECT_EQ(int_and_strings_list.size (), 1);
  EXPECT_EQ((dual_conditioned_lists_ordered<int, std::string>::typeType::typeT1),
            int_and_strings_list.front_type ());
  {
    auto val = int_and_strings_list.front_valueT1 ();
    int_and_strings_list.pop_front ();
    EXPECT_EQ(val, 0);
  }

  EXPECT_EQ(int_and_strings_list.size (), 0);
}

TEST(DualConditionedListsOrdered, Asynchronously)
{
  auto seq_int_producer = [] (dual_conditioned_lists_ordered<std::string, int>* sl, std::list<int>* il)
  {
    for (auto it = il->begin(); it != il->end(); ++it)
    {
      sl->push_back (*it);

      do {
        std::this_thread::yield ();
      } while (sl->size() > 0);
    }
  };
  auto seq_string_producer = [] (dual_conditioned_lists_ordered<std::string, int>* sl, std::list<std::string>* il)
  {
    for (auto it = il->begin(); it != il->end(); ++it)
    {
      sl->push_back (*it);

      do {
        std::this_thread::yield ();
      } while (sl->size() > 0);
    }
  };

  dual_conditioned_lists_ordered<std::string, int> strings_and_ints_list;

  std::list<std::string> string_inputs{"Hello","world","quit"};
  std::thread sp = std::thread (seq_string_producer, &strings_and_ints_list, &string_inputs);

  std::list<int> int_inputs{1,2,3,-1};
  std::thread ip = std::thread (seq_int_producer, &strings_and_ints_list, &int_inputs);

  std::list<std::string> string_outputs;
  std::list<int> int_outputs;
  bool string_producer_term = false;
  bool int_producer_term = false;
  while (!string_producer_term || !int_producer_term)
  {
    strings_and_ints_list.cond_wait ();
    if (strings_and_ints_list.front_type ()
        == dual_conditioned_lists_ordered<std::string, int>::typeType::typeT1)
    {
      auto val = strings_and_ints_list.front_valueT1 ();
      strings_and_ints_list.pop_front ();
      string_outputs.push_back(val);
      if (val == "quit")
        string_producer_term = true;
    }
    if (strings_and_ints_list.front_type ()
        == dual_conditioned_lists_ordered<std::string, int>::typeType::typeT2)
    {
      auto val = strings_and_ints_list.front_valueT2 ();
      strings_and_ints_list.pop_front ();
      int_outputs.push_back(val);
      if (val == -1)
        int_producer_term = true;
    }
    std::this_thread::yield ();
  }
  EXPECT_EQ(string_inputs, string_outputs);
  EXPECT_EQ(int_inputs, int_outputs);
  sp.join ();
  ip.join ();
}

int
main (int argc, char **argv)
{
  testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}
