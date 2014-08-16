#include <iostream>
#include <map>

int main()
{
  std::map<std::string, std::string> kv;

  for (int i = 0; i < 4; ++i) {

    std::string key, value;

    std::cin >> key >> value;
    kv[key] = value;
  }

  for (std::map<std::string,std::string>::const_iterator it = kv.begin();
       it!= kv.end();
       ++it) {
    if (it != kv.begin())
      std::cout << ' ';
    std::cout << it->second;
  }
  std::cout << std::endl;
  return 0;
}
