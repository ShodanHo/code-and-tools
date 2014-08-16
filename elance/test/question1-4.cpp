#include <iostream>
#include <map>

int main()
{
  std::map<std::string, std::string> num_score;

  for (int i = 0; i < 10; ++i) {
    std::string line;
    getline(std::cin, line);

    std::string num(line, 0, 4);
    std::string score(line, 5, 2);

    if (num_score.find(num) == num_score.end()) {
      num_score[num] = score;
    } else {
      if (score > num_score[num])
	num_score[num] = score;
    }
  }
  std::cout << std::endl;

  std::multimap<std::string, std::string> score_num;
  for (std::map<std::string, std::string>::const_iterator it =
	 num_score.begin();
       it != num_score.end();
       ++it)
    score_num.insert(std::pair<std::string,std::string>(it->second,
							it->first));

  for (std::map<std::string, std::string>::const_reverse_iterator it =
	 score_num.rbegin();
       it != score_num.rend();
       ++it)
    std::cout << it->second << '-' << it->first << std::endl;

  return 0;
}

// 11:38
