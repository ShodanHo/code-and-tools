template < template < class, std::size_t > class C, class T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const C<T, N>& c)
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


