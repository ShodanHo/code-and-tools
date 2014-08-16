#include <stdio.h>
#include <thread>

void hello()
{
  printf("World!!\n");
}

int main()
{
  printf("Hello... ");
  std::thread thread1(hello); //create new thread.
  thread1.join(); //wait for "thread1" to finish
}
