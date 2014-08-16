#include <iostream>
#include <sstream>

#define TOSTR(x) #x << '=' << x.toString()

template <typename T>
struct list_element {
  list_element(T _data = 0, list_element* _next = 0) :
    data(_data), next(_next) {}

  T data;
  list_element *next;
  std::string toString() const;
};

template <typename T>
std::string list_element<T>::toString() const
{
  std::ostringstream oss;
  oss << "(data=" << static_cast<void*>(data) << ",next=" << next << ")";
  return oss.str();
}

template <>
std::string list_element<int>::toString() const
{
  std::ostringstream oss;
  oss << "(data=" << data << ",next=" << next << ")";
  return oss.str();
}

template <typename T>
class cursor_list {
  list_element<T>* head;
  list_element<T>* cursor;

public:
  cursor_list() : head(nullptr), cursor(nullptr) {}
  cursor_list(const cursor_list<T>&);
  ~cursor_list() { release(); }

  void begin() { cursor = head; }
  void next() { if (cursor != 0) cursor = cursor->next; }

  void push_front(T data);
  void push_back(T data);
  char *get_cursor() { return cursor->data; }

  std::string toString() const;

private:
  void release();
};

template <typename T>
cursor_list<T>::cursor_list(const cursor_list<T>& cl)
  : head(nullptr), cursor(nullptr)
{
  for (list_element<T>* ptr = cl.head; ptr != nullptr; ptr = ptr->next)
  {
    push_back(ptr->data);
  }
  cursor = head;
}

template <typename T>
void cursor_list<T>::push_front(T data)
{
  list_element<T> *link = new list_element<T>(data, head);
  if (head == nullptr)
    cursor = link;
  head = link;
}

template <typename T>
void cursor_list<T>::push_back(T data)
{
  list_element<T> *link = new list_element<T>(data, nullptr);
  if (head == nullptr) {
    head = cursor = link;
  } else {
    for (list_element<T>* ptr = head; ; ptr = ptr->next)
    {
      if (ptr->next == nullptr) {
	ptr->next = link;
	return;
      }
    }
  }
}

template <typename T>
std::string
cursor_list<T>::toString() const
{
  std::ostringstream oss;
  oss << "(head=" << head;

  list_element<T> *head = this->head;
  while (head != 0) {
    oss << ",element " << static_cast<void*>(head) << ':'
	<< head->toString();
    head = head->next;
  }
  oss << ",cursor=" << static_cast<void*>(cursor);
  oss << ')';
  return oss.str();
}

template <typename T>
void cursor_list<T>::release()
{
  while (head != 0)
  {
    list_element<T> *link = head;
    head = link->next;
    delete [] link->data;
    delete link;
  }
}

template <>
void cursor_list<int>::release()
{
  while (head != 0)
  {
    list_element<int> *link = head;
    head = link->next;
    delete link;
  }
}

int main()
{
  {
    cursor_list<char *> x;
    std::cout << TOSTR(x) << std::endl;

    char *ptr;
    ptr = new char[100];
    x.push_front(ptr);
    std::cout << TOSTR(x) << std::endl;

    ptr = new char[200];
    x.push_front(ptr);
    std::cout << TOSTR(x) << std::endl;
    
    x.push_front(0);
    std::cout << TOSTR(x) << std::endl;

    x.begin();
    std::cout << TOSTR(x) << std::endl;

    x.next();
    std::cout << TOSTR(x) << std::endl;
  }

  {
    cursor_list<int> x;
    std::cout << TOSTR(x) << std::endl;
    x.push_front(0);
    x.push_front(1);
    x.push_front(2);
    std::cout << TOSTR(x) << std::endl;
  }

  {
    cursor_list<int> y;
    std::cout << TOSTR(y) << std::endl;
    y.push_back(0);
    y.push_back(1);
    y.push_back(2);
    std::cout << TOSTR(y) << std::endl;
  }

  {
    cursor_list<int> a;
    a.push_front(0);
    a.push_front(1);
    a.push_front(2);
    std::cout << TOSTR(a) << std::endl;

    cursor_list<int> b(a);
    std::cout << TOSTR(b) << std::endl;
  }
  return 0;
}
