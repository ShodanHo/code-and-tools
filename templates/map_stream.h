template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T,U>& p)
{
  os << '{' << p.first << ":" << p.second << '}';
  return os;
}

template < template<class,class,class,class> class C,
	   class Key, class T,
	   class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const C<Key, T, Compare, Alloc>& c)
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
