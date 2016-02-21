#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include <iomanip> // setfill,setw

#define STR(x) #x << '=' << (x)
#define TOSTR(x) #x << '=' << (x).toString()
#define STRFW(x,f,w) std::setfill(f) << std::setw(w) << STR(x)

#ifndef HERE
#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'
#endif

#define CASE_LITERAL_STREAM_BREAK(x,y) case y: x << #y; break

#endif // __COMMON_UTILS_H__


