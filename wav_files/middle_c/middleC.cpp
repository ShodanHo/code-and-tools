/*
 * middleC.cpp
 *
 *  Created on: 28 Feb 2016
 *      Author: stuartf
 */



// Adapted from http://www.cplusplus.com/forum/beginner/166954/
// and http://stackoverflow.com/questions/22226872/two-problems-when-writing-to-wav-c


#include <iostream>
#include <cstdint>
#include <cmath>
#include <fstream>

using namespace std;

namespace little_endian_io
{
  template <typename Word>
  std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
  {
    for (; size; --size, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
  }
}
using namespace little_endian_io;

int main()
{
  // char                Subchunk2ID[4]; // "data"  string
  // unsigned long       Subchunk2Size;  // Sampled data length
  f << "data----";  // (chunk size to be filled in later)

  // Write the audio samples
  // (We'll generate a single C4 note with a sine wave, fading from left to right)
  constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"

  double hz        = 44100;    // samples per second
  double frequency = 261.626;  // middle C
  double seconds   = 2.5;      // time

  int N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++)
  {
    double amplitude = (double)n / N * max_amplitude;
    //double amplitude = (double)max_amplitude;
    double value     = sin( (two_pi * n * frequency) / hz );
    write_word( f, (int)(                 amplitude  * value), 2 );
    write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
  }

  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();

  // Fix the data chunk header to contain the data size
  f.seekp( data_chunk_pos + 4 );
  write_word( f, file_length - data_chunk_pos + 8 );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f, file_length - 8, 4 );
}
