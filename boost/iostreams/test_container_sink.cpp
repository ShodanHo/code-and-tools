#include <iostream>
#include <cassert>
#include <string>
#include <boost/iostreams/stream.hpp>
//#include <libs/iostreams/example/container_device.hpp> // container_source
#include <boost/iostreams/filtering_stream.hpp>

namespace boost { namespace iostreams { namespace example {

template<typename Container>
class container_sink {
public:
    typedef typename Container::value_type  char_type;
    typedef sink_tag                        category;
    container_sink(Container& container) : container_(container) { }
    std::streamsize write(const char_type* s, std::streamsize n)
    {
        container_.insert(container_.end(), s, s + n);
        return n;
    }
    Container& container() { return container_; }
private:
    Container& container_;
};

} } } // End namespace boost::iostreams:example

namespace io = boost::iostreams;
namespace ex = boost::iostreams::example;

#define STR(x) #x << '=' << x

int main()
{
  using namespace std;
  typedef ex::container_sink<string> string_sink;

  {
    string                          result;
    io::stream<string_sink>  out(result);
    out << "Hello World!";
    out.flush();
    std::cout << STR(result) << std::endl;
    assert(result == "Hello World!");
  }

  {
    string                 result;
    io::filtering_ostream  out(back_inserter(result));
    out << "Hello World!";
    out.flush();
    std::cout << STR(result) << std::endl;
    assert(result == "Hello World!");
  }
}
