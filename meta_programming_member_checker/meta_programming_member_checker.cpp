// http://stackoverflow.com/questions/33660319/metaprogramming-member-checker-doesnt-work-with-function-members-returning-a-ty

// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector

#include <iostream>

// single argument and return type
#define DECL_hasMemberRetArg(MEMBER)                                                   \
template<typename T, typename RESULT, typename ARG1>                                   \
class Detect_RetArg_##MEMBER                                                           \
{                                                                                      \
    template <typename U, RESULT (U::*)(ARG1)> struct Check;                           \
    template <typename U> static char func(Check<U, &U::MEMBER> *);	\
    template <typename U> static int func(...);                                        \
  public:                                                                              \
    typedef Detect_RetArg_##MEMBER type;                                               \
    enum { value = sizeof(func<T>(0)) == sizeof(char) };                               \
}

// two arguments and return type
#define DECL_hasMemberRetArgArg(MEMBER)                                                \
template<typename T, typename RESULT, typename ARG1, typename ARG2>                    \
class Detect_RetArgArg_##MEMBER                                                        \
{                                                                                      \
    template <typename U, RESULT (U::*)(ARG1, ARG2)> struct Check;                     \
    template <typename U> static char func(Check<U, &U::MEMBER> *);                    \
    template <typename U> static int func(...);                                        \
  public:                                                                              \
    typedef Detect_RetArgArg_##MEMBER type;                                            \
    enum { value = sizeof(func<T>(0)) == sizeof(char) };                               \
}

class a_t
{
  char get(int);
  char put(int,long);
};

struct b_t
{
  char get(int);
  char put(int,long);
};

struct c_t : public b_t
{
};

DECL_hasMemberRetArg(get);
DECL_hasMemberRetArgArg(put);

int main()
{
  std::cout << Detect_RetArg_get<a_t, char, int>::value << "\n";          // 0
  std::cout << Detect_RetArgArg_put<a_t, char, int, long>::value << "\n"; // 0
  std::cout << Detect_RetArg_get<b_t, char, int>::value << "\n";          // 1
  std::cout << Detect_RetArgArg_put<b_t, char, int, long>::value << "\n"; // 1
  std::cout << Detect_RetArg_get<c_t, char, int>::value << "\n";          // 0
  std::cout << Detect_RetArgArg_put<c_t, char, int, long>::value << "\n"; // 0
  return 0;
}
