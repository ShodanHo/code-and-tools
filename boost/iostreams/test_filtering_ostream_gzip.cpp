#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace io = boost::iostreams;

int main()
{   
  io::filtering_ostream out;
    //out.push(compressor());
    out.push(boost::iostreams::basic_gzip_compressor<>());
    //out.push(base64_encoder());
    out.push(boost::iostreams::file_sink("my_file.txt"));
    // write to out using std::ostream interface
    out << "hello world";
}
