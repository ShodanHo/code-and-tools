/*
 * middleC.cpp
 *
 *  Created on: 28 Feb 2016
 *      Author: stuartf
 */



// Adapted from:
// http://www.cplusplus.com/forum/beginner/166954/
// http://stackoverflow.com/questions/22226872/two-problems-when-writing-to-wav-c
// http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file

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
  std::ofstream f( "example.wav", std::ios::binary );

  typedef struct  WAV_HEADER{
      char                RIFF[4];        // RIFF Header      Magic header
      unsigned long       ChunkSize;      // RIFF Chunk Size
      char                WAVE[4];        // WAVE Header
      char                fmt[4];         // FMT header
      unsigned long       Subchunk1Size;  // Size of the fmt chunk
      unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
      unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio
      unsigned long       SamplesPerSec;  // Sampling Frequency in Hz
      unsigned long       bytesPerSec;    // bytes per second
      unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
      unsigned short      bitsPerSample;  // Number of bits per sample
      char                Subchunk2ID[4]; // "data"  string
      unsigned long       Subchunk2Size;  // Sampled data length

  }wav_hdr;
  // Write the file headers
  // RIFF Header      Magic header
  // RIFF Chunk Size
  // WAVE Header
  f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
  write_word( f,     16, 4 );  // no extension data

  // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  write_word( f,      1, 2 );  // PCM - integer samples

  // Number of channels 1=Mono 2=Sterio
  write_word( f,      2, 2 );  // two channels (stereo file)

  // Sampling Frequency in Hz
  write_word( f,  44100, 4 );  // samples per second (Hz)

  // bytes per second
  write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8

  // 2=16-bit mono, 4=16-bit stereo
  write_word( f,      4, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)

  // Number of bits per sample
  write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)

  // Write the data chunk header
  size_t data_chunk_pos = f.tellp();

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
