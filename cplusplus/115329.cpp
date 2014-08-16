#include <iostream>
#include <string.h>
/*
 * first duplicate (e) found:
 * cfleece
 *    ^^
 *    ||
 *    ji
 *
 * cflece
 * ^  ^^
 * |  ||
 * ?  ji
 *
 * second duplicate (c) missed
 */
void delete_repeats(char array[], int size)
{
  std::cout << "original array = " << array << std::endl;
  std::cout << "with size = " << size << std::endl;
  int i, j, k;
  for (i=0; i<size; i++) {
    for (j=0; j<i; j++) {
      if (array[i] == array[j]) {
	std::cout << "duplicate " << array[i] << " found at i=" << i
		  << " and j=" << j << std::endl;
	for (k=i; k<size; k++) {
	  array[k]=array[k+1];
	}
	--size;
	std::cout << "new array = " << array << std::endl;
	std::cout << "with size = " << size << std::endl;
	--i; /* need to restart j-loop with current i because
	      * new array[i] can already be duplicated for arrays[]
	      * values that j has stepped past */
	break;
      }
    }
  }
  std::cout << array << std::endl;
  std::cout << "size = " << size << std::endl;
}

int main()
{
  char str[] = "cfleece";

  std::cout << str << std::endl;

  delete_repeats(str, strlen(str));

  std::cout << str << std::endl;

  return 0;
}
