#ifndef __MUTEX_COND_COLLECTIONS_H__
#define __MUTEX_COND_COLLECTIONS_H__

#include <condition_variable>
#include <list>

template <typename X>
class conditioned_list
{
  mutable std::mutex mtx; // mutable so that size() can be const
  std::condition_variable cv;
  std::list<X> mList;
public:
  conditioned_list() : mtx(), cv(), mList() {}

  void push_back(const X& x)
  {
    std::unique_lock<std::mutex> lck(mtx);
    mList.push_back(x);
    cv.notify_one();
  }

  typename std::list<X*>::size_type size() const noexcept
  {
    std::unique_lock<std::mutex> lck(mtx);
    return mList.size();
  }

  X front_value() // not reference front(); or const_reference front() const;
  {
    std::unique_lock<std::mutex> lck(mtx);
    X ret = mList.front();
    return ret;
  }

  void pop_front()
  {
    std::unique_lock<std::mutex> lck(mtx);
    mList.pop_front();
  }

  void cond_wait()
  {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
  }

  template <class Rep, class Period = std::ratio<1> >
  std::cv_status
  cond_wait_for(const std::chrono::duration<Rep,Period>& rel_time)
  {
    std::unique_lock<std::mutex> lck(mtx);
    return cv.wait_for(lck,rel_time);
  }
};

#endif // __MUTEX_COND_COLLECTIONS_H__
