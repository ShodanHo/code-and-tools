#include <iostream>
#include <regex>
#include <iostream>
#include <string>
#include <regex>
#include <iterator>

int main()
{
   std::string text = "Quick brown fox";
   std::regex vowel_re("a|e|i|o|u");
 
   // write the results to an output iterator
   std::regex_replace(std::ostreambuf_iterator<char>(std::cout),
                      text.begin(), text.end(), vowel_re, std::string("*"));
 
   // construct a string holding the results
   std::cout << '\n' << std::regex_replace(text, vowel_re,
					   std::string("[$&]")) << '\n';
#if 0
  {
    std::string s ("there is a subsequence in the string\n");
    std::regex e ("[a-z]");   // matches words beginning by "sub"

    // using string/c-string (3) version:
    std::cout << std::regex_replace (s,e,std::string("$$"));
  }
  return 0;
#endif
  std::string line("hello world");
  //std::cin >> line;
  //std::regex expr ("\\<[a-z]");
  std::regex expr ("[^\\w]+");
  //std::string repl("\\u&");
  std::string repl("_");

  std::cout << std::regex_replace (line, expr, repl) << std::endl;
  std::cout << line << std::endl;

  return 0;
}
