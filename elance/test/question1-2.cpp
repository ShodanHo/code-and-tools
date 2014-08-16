#include <iostream>
#include <map>
#include <stdlib.h>

#define STR(x) #x << '=' << x

int
main()
{
  std::map<std::string,std::string> number_score;

  for (int i = 0; i < 10; ++i) {
    std::string line;

    std::getline(std::cin, line);

    std::string number(line, 0, 4);
    std::string score(line, 5, 2);

    //std::cout << STR(number) << std::endl;
    //std::cout << STR(score) << std::endl;

    if (number_score.find(number) == number_score.end()) {
      number_score[number] = score;
    } else {
      //std::cout << "checking" << std::endl;
      if (atoi(score.c_str()) > atoi(number_score[number].c_str())) {
	//std::cout << "overwritting" << std::endl;
	number_score[number] = score;
      }
    }
  }
  std::cout << std::endl;

  std::map<std::string,std::string> score_number;

  for (std::map<std::string,std::string>::const_iterator it =
	 number_score.begin();
       it != number_score.end();
       ++it) {
    score_number[it->second] = it->first;
  }

  for (std::map<std::string,std::string>::const_reverse_iterator it =
	 score_number.rbegin();
       it != score_number.rend();
       ++it) {
    std::cout << it->second << '-' << it->first << std::endl;
  }
}

// 15 minutes
