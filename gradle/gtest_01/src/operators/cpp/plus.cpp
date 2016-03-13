#include "operators.h"

int plus(int a, int b) {
#ifdef PLUS_BROKEN
    return a + b;
#else
    return a + b;
#endif
}
