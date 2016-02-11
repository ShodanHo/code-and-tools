#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>

namespace boost { namespace iostreams { namespace example {

class shell_comments_input_filter : public input_filter {
public:
    explicit shell_comments_input_filter(char comment_char = '#')
        : comment_char_(comment_char), skip_(false)
        { }

    template<typename Source>
    int get(Source& src)
    {
        int c;
        while (true) {
            if ((c = boost::iostreams::get(src)) == EOF || c == WOULD_BLOCK)
                break;
            skip_ = c == comment_char_ ?
                true :
                c == '\n' ?
                    false :
                    skip_;
            if (!skip_)
                break;
        }
        return c;
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
    io::filtering_istream in;
    //in.push(decompressor());
    //in.push(boost::iostreams::basic_gzip_decompressor<>());
    //in.push(base64_decoder());
    in.push(boost::iostreams::example::shell_comments_input_filter());
    in.push(std::cin);
    // read from in using std::istream interface
    boost::iostreams::copy(in, std::cout);

    std::cout << std::endl;
    std::cout << "done" << std::endl;
}
