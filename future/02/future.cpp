// promise example
#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <unistd.h>

void
print_int (std::future<int>& fut)
{
  for (;;)
  {
    int x = fut.get ();
    std::cout << "value: " << x << '\n';
    if (x == 10)
      break;
  }
}

int
main ()
{
  std::promise<int> prom;                      // create promise

  std::future<int> fut = prom.get_future ();    // engagement with future

  std::thread th1 (print_int, std::ref (fut));  // send future to new thread

  sleep (2);
  for (auto i = 0; i <= 10; ++i)
  {
    prom.set_value (i);                         // fulfill promise
    sleep (2);
  }
  // (synchronizes with getting the future)
  th1.join ();
  return 0;
}
