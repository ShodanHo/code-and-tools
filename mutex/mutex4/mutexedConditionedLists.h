#ifndef __MUTEXEDCONDITIONEDLISTS_H__
#define __MUTEXEDCONDITIONEDLISTS_H__

#include <mutex>
#include <condition_variable>
#include <list>

/**
 * A thread-safe list. A reader can block on pushes to the list
 */
template<typename X>
  class conditioned_list
  {
    mutable std::mutex mtx; // mutable so that size() can be const
    std::condition_variable cv;
    std::list<X> mList;
  public:
    conditioned_list () :
        mtx (), cv (), mList ()
    {
    }

    void
    push_back (const X& x)
    {
      std::unique_lock<std::mutex> lck (mtx);
      mList.push_back (x);
      cv.notify_one ();
    }

    typename std::list<X*>::size_type
    size () const noexcept
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mList.size ();
    }

    X
    front_value () // not reference front(); or const_reference front() const;
    {
      std::unique_lock<std::mutex> lck (mtx);
      X ret = mList.front ();
      return ret;
    }

    void
    pop_front ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      mList.pop_front ();
    }

    void
    cond_wait ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      if (mList.size () > 0)
        return; // return if collection is not empty
      cv.wait (lck);
    }

    template<class Rep, class Period = std::ratio<1> >
      std::cv_status
      cond_wait_for (const std::chrono::duration<Rep, Period>& rel_time)
      {
        std::unique_lock<std::mutex> lck (mtx);
        if (mList.size () > 0)
          return std::cv_status::no_timeout; // return if collection is not empty
        return cv.wait_for (lck, rel_time);
      }
  };

/**
 * A pair of thread-safe lists. A reader can block on pushes to either list.
 */
template<typename T1, typename T2>
  class dual_conditioned_lists
  {
    mutable std::mutex mtx; // mutable so that size() can be const
    std::condition_variable cv;
    std::list<T1> mListT1;
    std::list<T2> mListT2;
  public:
    dual_conditioned_lists () :
        mtx (), cv (), mListT1 (), mListT2 ()
    {
    }

    void
    push_back (const T1& t1)
    {
      std::unique_lock<std::mutex> lck (mtx);
      mListT1.push_back (t1);
      cv.notify_one ();
    }

    void
    push_back (const T2& t2)
    {
      std::unique_lock<std::mutex> lck (mtx);
      mListT2.push_back (t2);
      cv.notify_one ();
    }

    typename std::list<T1*>::size_type
    sizeT1 () const noexcept
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mListT1.size ();
    }

    typename std::list<T1*>::size_type
    sizeT2 () const noexcept
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mListT2.size ();
    }

    T1
    front_valueT1 () // not reference front(); or const_reference front() const;
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mListT1.front ();
    }

    T2
    front_valueT2 () // not reference front(); or const_reference front() const;
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mListT2.front ();
    }

    void
    pop_frontT1 ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      mListT1.pop_front ();
    }

    void
    pop_frontT2 ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      mListT2.pop_front ();
    }

    void
    cond_wait ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      if (mListT1.size () > 0)
        return; // return if collection is not empty
      if (mListT2.size () > 0)
        return; // return if collection is not empty
      cv.wait (lck);
    }

    template<class Rep, class Period = std::ratio<1> >
      std::cv_status
      cond_wait_for (const std::chrono::duration<Rep, Period>& rel_time)
      {
        std::unique_lock<std::mutex> lck (mtx);
        if (mListT1.size () > 0)
          return std::cv_status::no_timeout; // return if collection is not empty
        if (mListT2.size () > 0)
          return std::cv_status::no_timeout; // return if collection is not empty
        return cv.wait_for (lck, rel_time);
      }
  };

/**
 * A pair of thread-safe lists. A reader can block on pushes to either list.
 * Items will come back in the order they were pushed.
 * Each type must have a default constructor.
 */
template<typename T1, typename T2>
  class dual_conditioned_lists_ordered
  {
  public:
    /**
     * enumeration of either or neither type
     */
    enum class typeType
    {
      typeNone, typeT1, typeT2
    };

  private:
    /**
     * structure of both types, with one being valid
     */
    struct typeT1T2
    {
      typeType type; //!< which one is being pushed
      T1 t1;
      T2 t2;
      typeT1T2 (const T1& _t1) :
          type (typeType::typeT1), t1 (_t1), t2 ()
      {
      }
      typeT1T2 (const T2& _t2) :
          type (typeType::typeT2), t1 (), t2 (_t2)
      {
      }
    };

    mutable std::mutex mtx; // mutable so that size() can be const
    std::condition_variable cv;
    std::list<typeT1T2> mList;
  public:
    dual_conditioned_lists_ordered () :
        mtx (), cv (), mList ()
    {
    }

    typename std::list<typeT1T2>::size_type
    size () const noexcept
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mList.size ();
    }

    template<typename T>
      void
      push_back (const T& t)
      {
        std::unique_lock<std::mutex> lck (mtx);
        mList.push_back (typeT1T2 (t));
        cv.notify_one ();
      }

    typeType
    front_type () const
    {
      std::unique_lock<std::mutex> lck (mtx);
      if (mList.size () == 0)
        return typeType::typeNone;
      return mList.front ().type;
    }

    T1
    front_valueT1 () // not reference front(); or const_reference front() const;
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mList.front ().t1;
    }

    T2
    front_valueT2 () // not reference front(); or const_reference front() const;
    {
      std::unique_lock<std::mutex> lck (mtx);
      return mList.front ().t2;
    }

    void
    pop_front ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      mList.pop_front ();
    }

    void
    cond_wait ()
    {
      std::unique_lock<std::mutex> lck (mtx);
      if (mList.size () > 0)
        return; // return if collection is not empty
      cv.wait (lck);
    }

    template<class Rep, class Period = std::ratio<1> >
      std::cv_status
      cond_wait_for (const std::chrono::duration<Rep, Period>& rel_time)
      {
        std::unique_lock<std::mutex> lck (mtx);
        if (mList.size () > 0)
          return std::cv_status::no_timeout; // return if collection is not empty
        return cv.wait_for (lck, rel_time);
      }
  };

#endif // __MUTEXEDCONDITIONEDLISTS_H__
