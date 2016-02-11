#include <iostream>
#include <string>
#include <map>

class dictionary {
public:
  void add(std::string key, const std::string& value);
  void replace(std::string& key);
private:
  typedef std::map<std::string, std::string> map_type;
  void tolower(std::string& str);
  map_type map_;
};

inline void dictionary::add(std::string key, const std::string& value)
{
  tolower(key);
  map_[key] = value;
}

inline void dictionary::replace(std::string& key)
{
  using namespace std;
  string copy(key);
  tolower(copy);
  map_type::iterator it = map_.find(copy);
  if (it == map_.end())
    return;
  string& value = it->second;
  if (!value.empty() && !key.empty() && std::isupper((unsigned char) key[0]))
    value[0] = std::toupper((unsigned char) value[0]);
  key = value;
  return;
}

inline void dictionary::tolower(std::string& str)
{
  for (std::string::size_type z = 0, len = str.size(); z < len; ++z)
    str[z] = std::tolower((unsigned char) str[z]);
}

int main()
{

  dictionary d;

  d.add("0","zero");
  d.add("1","one");
  d.add("2","two");
  d.add("3","three");
  d.add("4","four");
  d.add("5","five");
  d.add("6","six");
  d.add("7","seven");
  d.add("8","eight");
  d.add("9","nine");
  d.add("10","ten");
  d.add("twenty", "a20");

  {
    std::string str("0");
    d.replace(str);
    std::cout << str << std::endl;
  }

  {
    std::string str("twenty");
    d.replace(str);
    std::cout << str << std::endl;
  }

  {
    std::string str("Twenty");
    d.replace(str);
    std::cout << str << std::endl;
  }

  return 0;
}
