#if 1

#include<iostream>
#include<string>
#include <sstream>
using namespace std;


#define LENGTH(x) (sizeof(x)/sizeof(*(x)))
#define STR(x) #x << '=' << x

char english[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		   'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		   'U', 'V', 'W', 'X', 'Y', 'Z' };
string morse[] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
		   "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
		   "..-", "...-", ".--", "-..-", "-.--", "--.." };

void EnglishToMorseTranslation()
{
  cout << "Please enter a word or phrase in English for conversion" << endl;
  string EnglishToMorse;
  getline (cin, EnglishToMorse);
	
  for (unsigned i = 0; i < EnglishToMorse.length(); ++i) {
    for (unsigned b = 0; b < 26; ++b) {
      if ( toupper(EnglishToMorse[i]) == english[b]) {
	cout << morse[b] << ' ';
      }
    }
  }
  cout << endl;
}

void MorseToEnglishTranslation()
{
  cout << "Please enter a word or phrase in Morse Code for Translation" << endl;
  string MorseToEnglish;
  getline (cin, MorseToEnglish);

  std::istringstream iss(MorseToEnglish); // make a stream
  while (!iss.eof()) {
    string str;
    iss >> str; // split at spaces

    for (unsigned b = 0; b < LENGTH(morse); ++b) { // decode each morse fragment
      if ( str == morse[b]) {
	cout<< english[b]<< ' ';
      }
    }
  }
  cout << endl;
}

int main()
{
  cout<<" Please choose from the following menu"<<endl;
  cout<<"1) to convert from English to Morse Code"<<endl;
  cout<<"2) to convert from Morse Code to English"<<endl;
  string choice;
  getline (cin, choice);

  if(choice=="1") {
    EnglishToMorseTranslation( );
  }
  if (choice=="2") {
    MorseToEnglishTranslation();
  }

  return 0; 
}

#else

#include<iostream>
#include <map>
#include <sstream>
using namespace std;

const std::map<char,std::string> morse_map = {
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', "-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." }
};

void EnglishToMorseTranslation()
{
  cout << "Please enter a word or phrase in English for conversion" << endl;
  string EnglishToMorse;
  getline (cin, EnglishToMorse);
	
  for (auto it1 = EnglishToMorse.cbegin(); it1 != EnglishToMorse.cend(); ++it1)
  {
    char c = toupper(*it1);
    auto it2 = morse_map.find(c);
    if (it2 != morse_map.end())
      std::cout << it2->second << ' ';
  }

  cout << endl;
}

void MorseToEnglishTranslation()
{
  cout << "Please enter a word or phrase in Morse Code for Translation" << endl;
  string MorseToEnglish;
  getline (cin, MorseToEnglish);

  std::istringstream iss(MorseToEnglish); // make a stream
  while (!iss.eof()) {
    string str;
    iss >> str; // split at spaces

    for (auto it1 = morse_map.cbegin(); it1 != morse_map.cend(); ++it1)
    {
      if (it1->second == str)
	cout << it1->first << ' ';
    }
  }
  cout << endl;
}

#if 0
int main()
{
  cout << "Please enter a word or phrase in English for conversion" << endl;
  string EnglishToMorse;
  getline (cin, EnglishToMorse);

  for (auto it1 = EnglishToMorse.cbegin();
       it1 != EnglishToMorse.cend();
       ++it1) {
    char c = toupper(*it1);
    auto it2 = morse_map.find(c);
    if (it2 != morse_map.end())
      std::cout << it2->second << std::endl;
  }

  return 0;
}
#endif

int main()
{
  cout<<" Please choose from the following menu"<<endl;
  cout<<"1) to convert from English to Morse Code"<<endl;
  cout<<"2) to convert from Morse Code to English"<<endl;
  string choice;
  getline (cin, choice);

  if(choice=="1") {
    EnglishToMorseTranslation( );
  }
  if (choice=="2") {
    MorseToEnglishTranslation();
  }

  return 0; 
}

#endif
