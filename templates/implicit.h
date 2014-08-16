#ifndef __EXTERN_H__
#define __EXTERN_H__

#include <iostream>

template < template<class,class> class C, class T>
std::ostream& operator<<(std::ostream& os, const C<T, std::allocator<T> >& c);

template < template<class,class,class,class> class C,
	   class Key, class T,
	   class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os, const C<Key, T, Compare, Alloc>& c);

#endif // __EXTERN_H__
