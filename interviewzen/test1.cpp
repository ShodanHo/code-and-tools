#include <iostream>

#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

int main()
{
    int array1[] = { 1,2,3,4,5 };
    int array2[] = { 2,3,1,0,5 };
    
    for (unsigned int i = 0; i < LENGTH(array1); ++i) {
        int x = array1[i];
        bool found = false;
        for (unsigned int j = 0; j < LENGTH(array2); ++j) {
            int y = array2[j];
            if (x == y) { found = true; break; }
        }
        if (!found) std::cout << x << std::endl;
    }
    return 0;
}
