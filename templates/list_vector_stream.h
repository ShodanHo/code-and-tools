template < template<class,class> class C, class T>
std::ostream& operator<<(std::ostream& os, const C<T, std::allocator<T> >& c)
{
  os << '[';
  for (auto itr = c.cbegin(); itr != c.cend(); ++itr) {
    if (itr != c.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}
