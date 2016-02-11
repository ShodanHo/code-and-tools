#include <iostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>

namespace io = boost::iostreams;

int main()
{   
    io::filtering_istream in;
    //in.push(decompressor());
    in.push(boost::iostreams::basic_gzip_decompressor<>());
    //in.push(base64_decoder());
    in.push(boost::iostreams::file_source("my_file.txt"));
    // read from in using std::istream interface
    boost::iostreams::copy(in, std::cout);

    std::cout << std::endl;
}
