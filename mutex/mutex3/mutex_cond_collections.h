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

/**
 * Similar to conditioned_list, but with references to the mutex and condition.
 * This enables sharing of these variables amongst multiple collection class
 * instances, so that one listener can block on all of them.
 */
template <typename X>
class referenced_conditioned_list
{
  std::mutex &mtx;
  std::condition_variable &cv; //!< set on push_back()
  std::list<X> xList; //!< the std::list

public:
  referenced_conditioned_list(std::mutex &_mtx, std::condition_variable &_cv) : mtx(_mtx), cv(_cv), xList() {}

  //! equivalent of std::list<>.push_back()
  void push_back(const X& x)
  {
    std::unique_lock<std::mutex> lck(mtx);
    xList.push_back(x);
    cv.notify_one();
  }

  //! equivalent of std::list<>.size()
  typename std::list<X*>::size_type size() const noexcept
  {
    std::unique_lock<std::mutex> lck(mtx);
    return xList.size();
  }

  //! return front(). not a reference!!
  X front_value()
  {
    std::unique_lock<std::mutex> lck(mtx);
    X ret = xList.front();
    return ret;
  }

  //! equivalent of std::list<>.pop_front()
  void pop_front()
  {
    std::unique_lock<std::mutex> lck(mtx);
    xList.pop_front();
  }

  //! block on condition being set
  void cond_wait()
  {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
  }

  //! block for a period on condition being set
  template <class Rep, class Period = std::ratio<1> >
  std::cv_status
  cond_wait_for(const std::chrono::duration<Rep,Period>& rel_time)
  {
    std::unique_lock<std::mutex> lck(mtx);
    return cv.wait_for(lck,rel_time);
  }
};

#endif // __MUTEX_COND_COLLECTIONS_H__
