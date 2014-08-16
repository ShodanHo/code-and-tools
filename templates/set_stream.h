template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
{
  os << '[';
  for (auto itr = s.cbegin(); itr != s.cend(); ++itr) {
    if (itr != s.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}
