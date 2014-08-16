#include <iostream>
#include <sstream>

#define TOSTR(x) #x << '=' << x.toString()

struct slistelem {
  char *data;
  slistelem *next;
  std::string toString() const;
};

std::string slistelem::toString() const
{
  std::ostringstream oss;
  oss << "(data=" << static_cast<void*>(data) << ",next=" << next << ")";
  return oss.str();
}

class slist { // singly linked list
private:
  slistelem *head; // head

public:
  slist() : head(0) {}
  ~slist() { release(); }

  void prepend(char *c);

  std::string toString() const;

private:
  void release();
};

void slist::prepend(char *data)
{
  slistelem *link = new slistelem;
  link->data = data;
  link->next = head;
  head = link;
}

std::string
slist::toString() const
{
  std::ostringstream oss;
  oss << "(head=" << head;

  slistelem *head = this->head;
  while (head != 0) {
    oss << ",element " << static_cast<void*>(head) << ':'
	<< head->toString();
    head = head->next;
  }
  oss << ')';
  return oss.str();
}

void slist::release()
{
  while (head != 0)
  {
    slistelem *link = head;
    delete [] link->data;
    head = link->next;
  }
}

int main()
{
  {
    slist x;

    std::cout << TOSTR(x) << std::endl;

    char *ptr;
    ptr = new char[100];
    x.prepend(ptr);
    std::cout << TOSTR(x) << std::endl;

    ptr = new char[200];
    x.prepend(ptr);
    std::cout << TOSTR(x) << std::endl;
    
    x.prepend(0);
    std::cout << TOSTR(x) << std::endl;
  }

  return 0;
}
