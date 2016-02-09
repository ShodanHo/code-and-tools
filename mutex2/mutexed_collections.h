#ifndef __MUTEXED_COLLECTIONS_H__
#define __MUTEXED_COLLECTIONS_H__

#include <mutex>          // std::mutex, std::lock
#include <list>

struct lock_t
{
  std::mutex* mMtx;
  lock_t(std::mutex* mtx) : mMtx(mtx) { mMtx->lock(); }
  virtual ~lock_t() { mMtx->unlock(); }
};

template <typename X>
class mutexed_list
{
  mutable std::mutex mMtx;
  std::list<X> mList;
public:
  mutexed_list() : mMtx(), mList() {}
  void push_back(const X& x)
  {
    lock_t take(&mMtx);
    mList.push_back(x);
  }
  typename std::list<X*>::size_type size() const noexcept
  {
    lock_t take(&mMtx);
    return mList.size();
  }
  X front_value() // not reference front(); or const_reference front() const;
  {
    lock_t take(&mMtx);
    X ret = mList.front();
    return ret;
  }
  void pop_front()
  {
    lock_t take(&mMtx);
    mList.pop_front();
  }
};

#endif // __MUTEXED_COLLECTIONS_H__
