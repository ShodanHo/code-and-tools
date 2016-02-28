template <class T_t>
class list_mtx_t
{
private:
  mtx_t mtx;
  std::list<T_t> l;
public:
  list_mtx_t(): mtx(), l() {}
  size_t size() {
    size_t ret; { lock_t lock(mtx); ret = l.size(); } return ret;
  }
  void push_back(const T_t &T) { lock_t lock(mtx); l.push_back(T); }
  T_t &front() {
    T_t ret;
    {
      lock_t lock(mtx);
      ret = l.front();
    }
    return *new T_t(ret);
  }
  void pop_front() { lock_t lock(mtx); l.pop_front(); }
};

