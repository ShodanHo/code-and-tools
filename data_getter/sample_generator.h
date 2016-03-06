#ifndef __GETTERS_H__
#define __GETTERS_H__

#include <string>
#include <sstream> // ostringstream
#include <vector>
#include "common_utils.h"

template<typename T>
class ConstLimitedGetter
{
  const T getValue;
  const std::size_t getLimit;
  std::size_t getCount;

public:
  ConstLimitedGetter(const decltype(getValue)& val, const decltype(getLimit)& limit) :
    getValue(val), getLimit(limit), getCount(0) {}

  std::size_t get(T *dst, std::size_t count)
  {
    //std::cout << HERE() << ' ' << STR(count) << ' ' << STR(getCount) << '\n';
    if (this->getCount >= this->getLimit)
      return 0; // don't return any more

    std::size_t i;
    for (i = 0; (i < count) && (getCount < getLimit); ++i, ++getCount)
      *dst++ = getValue;
    //std::cout << HERE() << ' ' << STR(getCount) << ' ' << STR(i) << '\n';
    return i;
  }

  void restart(void)
  {
    getCount = 0;
  }

  std::string toString(void) const {
    std::ostringstream oss;
    oss << '('
        << STR(getValue) << ','
        << STR(getLimit) << ','
        << STR(getCount) << ')';
    return oss.str();
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const ConstLimitedGetter<T>& cg)
{
  os << cg.toString();
  return os;
}

template<typename T>
class ConstUnlimitedGetter
{
  const T getValue;

public:
  ConstUnlimitedGetter(const decltype(getValue)& val) :
    getValue(val) {}

  std::size_t get(T *dst, std::size_t count)
  {
    //std::cout << HERE() << ' ' << STR(count) << '\n';
    std::size_t i;
    for (i = 0; i < count; ++i)
      *dst++ = getValue;
    //std::cout << HERE() << ' ' << STR(i) << '\n';
    return i;
  }

  std::string toString(void) const {
    std::ostringstream oss;
    oss << '('
        << STR(getValue) << ')';
    return oss.str();
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const ConstUnlimitedGetter<T>& cg)
{
  os << cg.toString();
  return os;
}

template<typename T>
class VariableLimitedGetter
{
  std::vector<T> values;
  const std::size_t getLimit;
  std::size_t getCount;

public:
  VariableLimitedGetter(const decltype(values)& val, const decltype(getLimit)& limit) :
    values(val), getLimit(limit), getCount(0) {}

  std::size_t get(T *dst, std::size_t count)
  {
    //std::cout << HERE() << ' ' << STR(count) << ' ' << STR(getCount) << '\n';
    if (this->getCount >= this->getLimit)
      return 0; // don't return any more

    std::size_t i;
    for (i = 0; (i < count) && (getCount < getLimit); ++i, ++getCount)
      *dst++ = values.at(getCount % values.size());
    //std::cout << HERE() << ' ' << STR(getCount) << ' ' << STR(i) << '\n';
    return i;
  }

  void restart(void)
  {
    getCount = 0;
  }

  std::string toString(void) const {
    std::ostringstream oss;
    oss << '('
        << STR(values) << ','
        << STR(getLimit) << ','
        << STR(getCount) << ')';
    return oss.str();
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const VariableLimitedGetter<T>& cg)
{
  os << cg.toString();
  return os;
}

template<typename T>
class VariableUnlimitedGetter
{
  std::vector<T> values;
  const std::size_t getLimit;
  std::size_t getCount;

public:
  VariableUnlimitedGetter(const decltype(values)& val, const decltype(getLimit)& limit) :
    values(val), getLimit(limit), getCount(0) {}

  std::size_t get(T *dst, std::size_t count)
  {
    //std::cout << HERE() << ' ' << STR(count) << ' ' << STR(getCount) << ' '
    //<< STR(getLimit) << ' ' << STR(values.size())<< '\n';
    std::size_t i;
    for (i = 0; i < count; ++i)
      *dst++ = values.at((getCount++ % getLimit) % values.size());
    //std::cout << HERE() << ' ' << STR(getCount) << ' ' << STR(i) << '\n';
    return i;
  }

  void restart(void)
  {
    getCount = 0;
  }

  std::string toString(void) const {
    std::ostringstream oss;
    oss << '('
        << STR(values) << ','
        << STR(getLimit) << ','
        << STR(getCount) << ')';
    return oss.str();
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const VariableUnlimitedGetter<T>& cg)
{
  os << cg.toString();
  return os;
}

template <typename T>
class ClgVlgCug
{
  ConstLimitedGetter<T> clg;
  bool clgDone;
  VariableLimitedGetter<T> vlg;
  bool vlgDone;
  ConstUnlimitedGetter<T> cug;

public:
  ClgVlgCug(const T& clgVal, const T& clgLimit,
            const std::vector<T>& vlgVal, const std::size_t& vlgLimit,
            const T& cugVal) :
    clg(clgVal, clgLimit), clgDone(false),
    vlg(vlgVal, vlgLimit), vlgDone(false),
    cug(cugVal) {}

  std::size_t get(T *dst, std::size_t count)
  {
    std::size_t sz1 = 0;
    if (!clgDone)
    {
      sz1 = clg.get(dst, count);
      if (sz1 == count)
      {
        return sz1;
      }
      count -= sz1;
      dst += sz1;
      clgDone = true;
    }

    std::size_t sz2 = 0;
    if (!vlgDone)
    {
      sz2 = vlg.get(dst, count);
      if (sz2 == count)
      {
        return sz1 + sz2;
      }
      count -= sz2;
      dst += sz2;
      vlgDone = true;
    }

    std::size_t sz3 = 0;
    sz3 = cug.get(dst, count);
    return sz1 + sz2 + sz3;
  }
};


#endif // __GETTERS_H__
