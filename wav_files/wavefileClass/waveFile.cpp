// http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file
#include "waveFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#define STR(x) #x << '=' << x

// find the file size
int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

std::string
WavHdr::toString(void) const
{
  std::ostringstream oss;
  oss << "RIFF header                :" << RIFF[0] << RIFF[1] << RIFF[2] << RIFF[3] << '\n';
  oss << "WAVE header                :" << WAVE[0] << WAVE[1] << WAVE[2] << WAVE[3] << '\n';
  oss << "FMT                        :" << fmt[0] << fmt[1] << fmt[2] << fmt[3] << '\n';
  oss << "Data size                  :" << ChunkSize << '\n';

  // Display the sampling Rate from the header
  oss << "Sampling Rate              :" << SamplesPerSec << '\n';
  oss << "Number of bits used        :" << bitsPerSample << '\n';
  oss << "Number of channels         :" << NumOfChan << '\n';
  oss << "Number of bytes per second :" << bytesPerSec << '\n';
  oss << "Data length                :" << Subchunk2Size << '\n';
  oss << "Audio Format               :" << AudioFormat << '\n';
  oss << "Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM\n";

  oss << "Block align                :" << blockAlign << '\n';
  oss << "Data string                :" << Subchunk2ID[0] << Subchunk2ID[1] << Subchunk2ID[2] << Subchunk2ID[3] << '\n';

  uint16_t bytesPerSample = bitsPerSample / 8;      //Number     of bytes per sample
  uint64_t numSamples = Subchunk2Size / bytesPerSample; //How many samples are in the wav file?
  oss << "Bytes per sample           :" << bytesPerSample << '\n';
  oss << "Num samples                :" << numSamples << '\n';

  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const WavHdr& wavHdr)
{
  os << wavHdr.toString();
  return os;
}

WaveFile::WaveFile(decltype(fileName) _fileName)
  : fileName(_fileName), fileLen(0)
{
  FILE* wavFile = fopen(fileName.c_str(), "r");
  if (wavFile == nullptr)
  {
    fprintf(stderr, "Unable to open wave file: %s\n", fileName.c_str());
    return;
  }

  //Read the header

  size_t bytesRead = fread(&wavHeader, 1, sizeof(wavHeader), wavFile);
  if (bytesRead == sizeof(wavHeader))
  {
    fileLen = getFileSize(wavFile);
  }
  else
  {
    fprintf(stderr, "Unable to read header: %s\n", fileName.c_str());
  }
  fclose(wavFile);
}

std::size_t WaveFile::getChannelData(unsigned channel, std::size_t from,
                                     int16_t *samples, std::size_t count)
{
  FILE* wavFile = fopen(fileName.c_str(), "r");
  if (wavFile == nullptr)
  {
    fprintf(stderr, "Unable to open wave file: %s\n", fileName.c_str());
    return 0;
  }

  //Read the header

  size_t bytesRead = fread(&wavHeader, 1, sizeof(wavHeader), wavFile);
  if (bytesRead != sizeof(wavHeader)) {
    fclose(wavFile);
    return 0;
  }

  //Read the data
  uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
  auto channels = wavHeader.NumOfChan;
  auto seek = channels * bytesPerSample * from;

  fseek(wavFile, seek, SEEK_CUR);

  unsigned i;
  for (i = 0; i < count; ++i) {
    int16_t channelData[2]; // FIXUP. this only cope with 2 channel files
    bytesRead = fread(channelData, 1, sizeof(channelData), wavFile);
    if (bytesRead != sizeof(channelData)) {
      fprintf(stderr, "unable to read wav file data at sample %u\n", i);
      fprintf(stderr, "read %u bytes\n", bytesRead);
      fclose(wavFile);
      return 0;
    }
    *samples++ = channelData[channel & 1]; // FIXUP. this only cope with 2 channel files
  }

  fclose(wavFile);
  return i;
}

template <typename Word>
std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
  for (; size; --size, value >>= 8)
    outs.put( static_cast <char> (value & 0xFF) );
  return outs;
}

void WaveFile::writeWaveFile(const std::string& filename,
                             uint16_t channels,
                             uint32_t samplesPerSecond, // 44100
                             uint32_t bytesPerSecond, // 176400
                             uint16_t bytesPerSampleForAllChannels, // 4
                             uint16_t bitsPerSample, // 16
                             void *buffer,
                             std::size_t size)
{
  std::ofstream f(filename, std::ios::binary );

  // Write the file headers
  // RIFF Header      Magic header
  // RIFF Chunk Size
  // WAVE Header
  f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
  write_word( f,     16, 4 );  // no extension data

  // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  write_word( f,      1, 2 );  // PCM - integer samples

  // Number of channels 1=Mono 2=Sterio
  write_word( f, channels, 2 );  // two channels (stereo file)

  // Sampling Frequency in Hz
  write_word( f, samplesPerSecond, 4 );  // samples per second (Hz)

  // bytes per second
  write_word( f, bytesPerSecond, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8

  // 2=16-bit mono, 4=16-bit stereo
  write_word( f, bytesPerSampleForAllChannels, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)

  // Number of bits per sample
  write_word( f, bitsPerSample, 2 );  // number of bits per sample (use a multiple of 8)

  // Write the data chunk header
  size_t data_chunk_pos = f.tellp();

  // char                Subchunk2ID[4]; // "data"  string
  // unsigned long       Subchunk2Size;  // Sampled data length
  f << "data----";  // (chunk size to be filled in later)

  // Write the audio samples
#if 0
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
#else
  f.write(reinterpret_cast<const char*>(buffer), size);
#endif
  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();

  // Fix the data chunk header to contain the data size
  f.seekp( data_chunk_pos + 4 );
  write_word( f, file_length - (data_chunk_pos + 8) );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f, file_length - 8, 4 );
}

std::string WaveFile::toString(void) const
{
  std::ostringstream oss;
  oss << STR(fileName) << ',' << STR(wavHeader) << ',' << STR(fileLen);
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const WaveFile& waveFile)
{
  os << waveFile.toString();
  return os;
}

