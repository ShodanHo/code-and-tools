#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/concepts.hpp>    // output_filter
#include <boost/iostreams/operations.hpp>  // put
#include <boost/iostreams/device/file.hpp>
//#include <boost/iostreams/copy.hpp>

namespace boost { namespace iostreams { namespace example {

class shell_comments_output_filter : public output_filter {
public:
    explicit shell_comments_output_filter(char comment_char = '#')
        : comment_char_(comment_char), skip_(false)
        { }

    template<typename Sink>
    bool put(Sink& dest, int c)
    {
        skip_ = c == comment_char_ ?
            true :
            c == '\n' ?
                false :
                skip_;

        if (skip_)
            return true;

        return iostreams::put(dest, c);
    }

    template<typename Source>
    void close(Source&) { skip_ = false; }
private:
    char comment_char_;
    bool skip_;
};

} } } // End namespace boost::iostreams:example

namespace io = boost::iostreams;

int main()
{   
  io::filtering_ostream out;
  //boost::iostreams::copy(out, std::cin);
  //out.push(compressor());
  out.push(boost::iostreams::example::shell_comments_output_filter());
  //out.push(base64_encoder());
  //out.push(boost::iostreams::file_sink("comments_file.txt"));
  out.push(std::cout);
  // write to out using std::ostream interface
  out << "hello # world";
}
