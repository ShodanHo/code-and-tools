#include <iostream>
#include <list>
#include "list_vector_stream.h"
#define STR(x) #x  << '=' << x

class MultAddCoder
{
  const int m_mult;
  const int m_add;
public:
  MultAddCoder(int mult, int add) : m_mult(mult), m_add(add) {}

  int encode(int a) const { return m_mult * a + m_add; }
  int decode(int a) const { return (a - m_add) / m_mult; }

  std::list<int> encode(const std::list<int>&) const;
  std::list<int> decode(const std::list<int>&) const;
};

std::list<int>
MultAddCoder::encode(const std::list<int>& msg) const
{
  std::list<int> ret;
  for (std::list<int>::const_iterator it = msg.cbegin();
       it != msg.cend();
       ++it)
  {
    ret.push_back(encode(*it));
  }
  return ret;
}

std::list<int>
MultAddCoder::decode(const std::list<int>& msg) const
{
  std::list<int> ret;
  for (std::list<int>::const_iterator it = msg.cbegin();
       it != msg.cend();
       ++it)
  {
    ret.push_back(decode(*it));
  }
  return ret;
}

int main()
{
  std::cout << MultAddCoder(3,5).encode(65) << std::endl;
  std::cout << MultAddCoder(3,5).decode(200) << std::endl;

  std::list<int> message;
  message.push_back(65);
  message.push_back(115);
  std::cout << STR(message) << std::endl;

  std::list<int> encoded_message = MultAddCoder(3,5).encode(message);
  std::cout << STR(encoded_message) << std::endl;

  std::list<int> decoded_message = MultAddCoder(3,5).decode(encoded_message);
  std::cout << STR(decoded_message) << std::endl;

  {
    MultAddCoder a(3,5), b(4, 8), c(2, 6), d(7, 9);
    std::list<int> encoded_message = 
      d.encode(c.encode(b.encode(a.encode(message))));

    std::cout << STR(encoded_message) << std::endl;

    std::list<int> decoded_message =
      a.decode(b.decode(c.decode(d.decode(encoded_message))));

    std::cout << STR(decoded_message) << std::endl;
  }
  return 0;
}
