#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

namespace io = boost::iostreams;

int main()
{
  io::stream_buffer<boost::iostreams::file_sink> buf("log.txt");
  std::ostream out(&buf);

  out << "hello world" << std::endl;
  // out writes to log.txt
}
