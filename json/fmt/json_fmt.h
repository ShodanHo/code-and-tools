#ifndef __JSON_FMT_H__
#define __JSON_FMT_H__

#include <string>

class JsonKeyValue
{
  std::string ret;
public:
  JsonKeyValue(const std::string& key, const std::string& val) : ret(std::string("\"") + key + "\":\"" + val + "\"") {}

  std::string operator()() const { return ret; }
};

class JsonZTime
{
  struct timeval tv;
public:
  JsonZTime(const timeval& _tv) : tv(_tv) {}

  std::string operator()() const;
};

class JsonFmtValueRunningStopped
{
  bool running;
public:
  JsonFmtValueRunningStopped(bool _running) : running(_running) {}

  std::string operator()() const;
};

class JsonFmtLabelXyzValueRunningZTime
{
  bool running;
  struct timeval tv;
public:
  JsonFmtLabelXyzValueRunningZTime(bool _running, const timeval& _tv) : running(_running), tv(_tv) {}

  std::string operator()() const;
};

#endif // __JSON_FMT_H__
