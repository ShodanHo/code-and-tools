#include <thread>
#include <mutex>
#include "mutex_cond_collections.h"
#include <unistd.h> // sleep
#include <iostream>

#define HERE() __FUNCTION__ << ':' << __LINE__
#define STR(x) #x << '=' << x

static conditioned_list<int> l;

static void producer()
{
  std::cout << HERE() << '\n';
  for (auto i = 0; i < 10; ++i) {
    l.push_back(i);
    sleep(1);
  }
  for (auto i = 0; i < 10; ++i) {
    l.push_back(-1);
  }
}

static void consumer(int n)
{
  for (;;) {
    //l.cond_wait();
    auto ret = l.cond_wait_for(std::chrono::seconds(1));
    if (ret == std::cv_status::timeout) {
      std::cout << HERE() << ':' << STR(n) << " timeout\n";
    }
    if (l.size() > 0) {
      int x = l.front_value();
      l.pop_front();
      std::cout << HERE() << ':' << STR(n) << ' ' << STR(x) << '\n';
      if (x == -1) break;
    }
  }
}

int main()
{
  std::thread x[10];
  for (int i=0; i<10; ++i) {
    x[i] = std::thread(consumer, i);
  }

  std::thread p(producer);

  for (int i=0; i<10; ++i) {
    x[i].join();
    std::cout << "thread " << i << " terminated\n";
  }
  p.join();
  std::cout << "thread p terminated\n";
  
  return 0;
}
