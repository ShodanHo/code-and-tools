#include <iostream>
#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>
//#include <libs/iostreams/example/container_device.hpp> // container_source

namespace boost { namespace iostreams { namespace example {

template<typename Container>
class container_source {
public:
    typedef typename Container::value_type  char_type;
    typedef source_tag                      category;
    container_source(Container& container) 
        : container_(container), pos_(0)
        { }
    std::streamsize read(char_type* s, std::streamsize n)
    {
        using namespace std;
        streamsize amt = static_cast<streamsize>(container_.size() - pos_);
        streamsize result = (min)(n, amt);
        if (result != 0) {
            std::copy( container_.begin() + pos_, 
                       container_.begin() + pos_ + result, 
                       s );
            pos_ += result;
            return result;
        } else {
            return -1; // EOF
        }
    }
    Container& container() { return container_; }
private:
    typedef typename Container::size_type   size_type;
    Container&  container_;
    size_type   pos_;
};

} } } // End namespace boost::iostreams:example

namespace boost { namespace iostreams { namespace example {

template<typename Container>
class transforming_container_source {
public:
  typedef typename Container::value_type  char_type;
  typedef source_tag                      category;
  transforming_container_source(Container& container) 
  : container_(container), pos_(0)
  { }
  std::streamsize read(char_type* s, std::streamsize n)
  {
    using namespace std;
    streamsize amt = static_cast<streamsize>(container_.size() - pos_);
    streamsize result = (min)(n, amt);
    if (result != 0) {
#if 0
      std::copy( container_.begin() + pos_, 
		 container_.begin() + pos_ + result, 
		 s );
#else
      typename Container::iterator first = container_.begin() + pos_;
      typename Container::iterator last = container_.begin() + pos_ + result;
      while (first != last) {
	char_type x = *first ;
	if ((x >= 'A') && (x <='Z'))
	  x = 'a' + x - 'A';
	*s = x;
	++s; ++first;
      }
#endif
      pos_ += result;
      return result;
    } else {
      return -1; // EOF
    }
  }
  Container& container() { return container_; }
private:
  typedef typename Container::size_type   size_type;
  Container&  container_;
  size_type   pos_;
};

} } } // End namespace boost::iostreams:example

namespace io = boost::iostreams;
namespace ex = boost::iostreams::example;

#define STR(x) #x << '=' << x

int main()
{
    using namespace std;
    {
      typedef ex::container_source<string> string_source;

      string                            input = "Hello World!";
      string                            output;
      io::stream<string_source>  in(input);
      getline(in, output);
      std::cout << ' ' << STR(input) << endl;
      std::cout << STR(output) << endl;
      assert(input == output);
    }

    {
      string                            input = "Hello World!";
      string                            output;
      io::stream<ex::transforming_container_source<string> >  in(input);
      getline(in, output);
      std::cout << ' ' << STR(input) << endl;
      std::cout << STR(output) << endl;
      assert("hello world!" == output);
    }
}
