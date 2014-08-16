#include <iostream>

void introduction();
//Explains what the program does

void fill_array(char a[], int size, int& number_used);
//Array a[] is filled with character data from the keyboard

void delete_repeats(char a[], int& number_used);
//Function will remove all repeated characters and move the rest of the characters
//foward to fill in the gap.

void output(char a[], int& number_used);
//Outputs the contents of the array and outs the new size of the array

int main()
{
  using namespace std;
  char array[100];
  int number_used;
  introduction();
  fill_array(array, 100, number_used);
  output(array, number_used);
}

//uses iostream
void introduction()
{
  using namespace std;
  cout << "This program will will ask the user to type in a sentence\n"
       << "and then will delete all repeated characters of the sentence.\n"
       << "The program will then output the new sentence with all repeated\n"
       << "letters deleted\n";
}

//uses iostream
void fill_array(char a[], int size, int& number_used)
{
  using namespace std;
  char c;
  int index = 0;
  cout << "Please type in a sentence and then press enter.\n";
  cin.get(c);


  while (c != '\n' && index < size) {
    a[index] = c;
    cin.get(c);
    index++;
  }
  number_used = index;


  for (int i = 0; i < number_used; i++) {
    for (int j = i + 1; j < number_used; j++) {
      if (a[i] == a[j]) {
	cout << a[i] << " duplicates found at "
	     << i << " and " << j <<std::endl;
	number_used=number_used-1;
	for (int k = j; k < number_used; k++)
	  a[k] = a[k + 1];
	a[number_used] = '\0';
	cout << a <<std::endl;
	cout << number_used<<std::endl;
	--i; break;
      }
    }
    cout << a[i]<<std::endl;
    cout << number_used<<std::endl;
  }
  cout << number_used<<std::endl;
}



//uses iostream
void output(char a[], int& number_used)
{
  using namespace std;
  cout << "The new sentence without the repeated letters is:\n";
  for (int i = 0; i < number_used; i++) {
    cout << a[i];
  }
  cout << "\nThe size of the new array is "
       << number_used
       << endl;


  cin.ignore();
  cin.get();
}
