#include <iostream>
#include <map>

int main()
{
  std::map<std::string, std::string> number_score;

  for (int i = 0; i < 10; ++i) {
    std::string line;
    std::getline(std::cin, line);

    std::string number(line, 0, 4);
    std::string score(line, 5, 2);

    if (number_score.find(number) == number_score.end()) {
      number_score[number] = score;
    } else {
      if (score > number_score[number])
	number_score[number] = score;
    }
  }

  std::cout << std::endl;

  std::map<std::string, std::string> score_number;
  for (std::map<std::string, std::string>::const_iterator it =
	 number_score.begin();
       it != number_score.end();
       ++it)
    score_number[it->second] = it->first;

  for (std::map<std::string, std::string>::const_reverse_iterator it =
	 score_number.rbegin();
       it != score_number.rend();
       ++it)
    std::cout << it->second << '-' << it->first << std::endl;
}

// 7:36
