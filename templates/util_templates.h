#ifndef __UTIL_TEMPLATES_H__
#define __UTIL_TEMPLATES_H__

template <typename T, std::size_t Length>
constexpr std::size_t array_length(T(&)[Length])
{
  return Length;
}

#endif // __UTIL_TEMPLATES_H__
