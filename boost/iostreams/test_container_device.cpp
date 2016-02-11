#include <iostream>
#include <cassert>
#include <ios> // ios_base::beg
#include <string>
#include <boost/iostreams/stream.hpp>
//#include <libs/iostreams/example/container_device.hpp>

#define STR(x) #x << '=' << x
#define HERE() __FILE__ << '(' << __LINE__ << "):"

namespace boost { namespace iostreams { namespace example {

template<typename Container>
class container_device {
public:
    typedef typename Container::value_type  char_type;
    typedef seekable_device_tag             category;
    container_device(Container& container) 
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
    std::streamsize write(const char_type* s, std::streamsize n)
    {
        using namespace std;
        streamsize result = 0;
        if (pos_ != container_.size()) {
            streamsize amt = 
                static_cast<streamsize>(container_.size() - pos_);
            result = (min)(n, amt);
            std::copy(s, s + result, container_.begin() + pos_);
            pos_ += result;
        }
        if (result < n) {
            container_.insert(container_.end(), s + result, s + n);
            pos_ = container_.size();
        }
        return n;
    }
    stream_offset seek(stream_offset off, std::ios_base::seekdir way)
    {
        using namespace std;

        // Determine new value of pos_
        stream_offset next;
        if (way == ios_base::beg) {
            next = off;
        } else if (way == ios_base::cur) {
            next = pos_ + off;
        } else if (way == ios_base::end) {
            next = container_.size() + off - 1;
        }

        // Check for errors
        if (next < 0 || next >= container_.size())
            throw ios_base::failure("bad seek offset");

        pos_ = next;
        return pos_;
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

int main()
{
  using namespace std;

  {
    typedef ex::container_device<string> string_device;

    string                            one, two;
    io::stream<string_device>  io(one);
    io << "Hello World!";
    io.flush();
    std::cout << STR(one) << std::endl;
    io.seekg(0, BOOST_IOS::beg); // seek to the beginning
    getline(io, two);
    assert(one == "Hello World!");
    assert(two == "Hello World!");
  }

  {
    typedef ex::container_device<string> string_device;

    string                            one, two;
    io::stream<string_device>  io(one);

    io << "0123456789";
    io.flush();
    std::cout << STR(one) << std::endl;

    io << "abcdefghij";
    io.flush();
    std::cout << STR(one) << std::endl;

    io.seekg(0, BOOST_IOS::beg); // seek to the beginning
    getline(io, two);
    std::cout << STR(two) << std::endl;

    assert(one == "0123456789abcdefghij");
    assert(two == "0123456789abcdefghij");
  }

  {
    typedef ex::container_device<string> string_device;

    string                            one, two;
    io::stream<string_device>  io(one);

    io << "0123456789";
    io.flush();
    std::cout << STR(one) << std::endl;

    io << "abcdefghij";
    io.flush();
    std::cout << STR(one) << std::endl;

    io.seekg(10, BOOST_IOS::beg); // seek to the beginning
    getline(io, two);
    std::cout << STR(two) << std::endl;

    assert(one == "0123456789abcdefghij");
    assert(two == "abcdefghij");
  }

  {
    typedef ex::container_device<string> string_device;

    string                            one, two;
    io::stream<string_device>  io(one);

    io << "0123456789";
    io.flush();
    std::cout << STR(one) << std::endl;

    io.seekg(5, BOOST_IOS::beg); // seek to the beginning
    io << "abcdefghij";
    io.flush();
    std::cout << STR(one) << std::endl;

    io.seekg(0, BOOST_IOS::beg); // seek to the beginning
    getline(io, two);
    std::cout << STR(two) << std::endl;

    assert(one == "01234abcdefghij");
    assert(two == "01234abcdefghij");
  }
}
