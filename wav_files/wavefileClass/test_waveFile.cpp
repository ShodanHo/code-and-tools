#include <iostream>
#include <fstream>
#include <cmath> // sin()
#include <gtest/gtest.h>
#include "common_utils.h"
#include "util_templates.h"
#include "waveFile.h"

static void writeConstantValueFile(const std::string& filename, std::size_t size)
{
  std::ofstream outfile(filename, std::ios::out | std::ios::trunc | std::ios::binary);

  while (size--)
    outfile << "a";

  outfile.close();
}

static size_t fileSize(const std::string& filename)
{
  std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

  size_t fileSize = f.tellp();

  f.close(); // probably done by destructor
  return fileSize;
}

TEST(WaveFileReader, UnableToReadWavefileDataExceptions)
{
  // sizeof(WavHdr)=44

  { // no data, read nothing
    std::string filename("same.txt");
    writeConstantValueFile(filename, sizeof(WavHdr)); // same size as the header
    try {
      WaveFileReader wf(filename);
      SUCCEED(); // should reach here

      int16_t *samples = nullptr;
      try {
        wf.getChannelData(0,0,samples,0);
        SUCCEED(); // should reach here
      } catch (const UnableToReadWavefileDataException& e) {
        FAIL(); // should not reach here
      } catch (...) {
        FAIL(); // should not reach here
      }
    } catch (const UnableToReadWavefileHeaderException& e) {
      FAIL(); // should not reach here
      std::cout << e.what() << '\n';
      EXPECT_EQ(std::string(e.what()), std::string("Unable to read wave file header of file:small.txt"));
    } catch (...) {
      FAIL(); // should not reach here
    }

    remove(filename.c_str());
  }

  { // no data, read something
    std::string filename("same.txt");
    writeConstantValueFile(filename, sizeof(WavHdr)); // same size as the header
    try {
      WaveFileReader wf(filename);
      SUCCEED(); // should reach here

      int16_t *samples = nullptr;
      try {
        wf.getChannelData(0,0,samples,1);
        FAIL(); // should not reach here
      } catch (const UnableToReadWavefileDataException& e) {
        SUCCEED(); // should reach here
      } catch (...) {
        FAIL(); // should not reach here
      }
    } catch (const UnableToReadWavefileHeaderException& e) {
      FAIL(); // should not reach here
      std::cout << e.what() << '\n';
      EXPECT_EQ(std::string(e.what()), std::string("Unable to read wave file header of file:small.txt"));
    } catch (...) {
      FAIL(); // should not reach here
    }

    remove(filename.c_str());
  }
}

TEST(WaveFileReader, UnableToReadWavefileHeaderExceptions)
{
  // sizeof(WavHdr)=44
  {
    std::string filename("small.txt");
    writeConstantValueFile(filename, sizeof(WavHdr) - 1); // just smaller than the header
    try {
      WaveFileReader wf(filename);
      FAIL(); // should not reach here
    } catch (const UnableToReadWavefileHeaderException& e) {
      SUCCEED(); // should reach here
      std::cout << e.what() << '\n';
      EXPECT_EQ(std::string(e.what()), std::string("Unable to read wave file header of file:small.txt"));
    } catch (...) {
      FAIL(); // should not reach here
    }
    remove(filename.c_str());
  }

  {
    std::string filename("same.txt");
    writeConstantValueFile(filename, sizeof(WavHdr)); // same size as the header
    try {
      WaveFileReader wf(filename);
      SUCCEED(); // should reach here
    } catch (const UnableToReadWavefileHeaderException& e) {
      FAIL(); // should not reach here
      std::cout << e.what() << '\n';
      EXPECT_EQ(std::string(e.what()), std::string("Unable to read wave file header of file:small.txt"));
    } catch (...) {
      FAIL(); // should not reach here
    }
    remove(filename.c_str());
  }

  {
    std::string filename("same.txt");
    writeConstantValueFile(filename, sizeof(WavHdr) + 1); // a bit larger than the header
    try {
      WaveFileReader wf(filename);
      SUCCEED(); // should reach here
    } catch (const UnableToReadWavefileHeaderException& e) {
      FAIL(); // should not reach here
      std::cout << e.what() << '\n';
      EXPECT_EQ(std::string(e.what()), std::string("Unable to read wave file header of file:small.txt"));
    } catch (...) {
      FAIL(); // should not reach here
    }
    remove(filename.c_str());
  }
}

TEST(WaveFileReader, UnableToOpenFileException)
{
  try {
    WaveFileReader wf("abc.def");
    FAIL(); // should not reach here
  } catch (const UnableToOpenFileException& e) {
    SUCCEED(); // should reach here
    std::cout << e.what() << '\n';
    EXPECT_EQ(std::string(e.what()), std::string("Unable to open file:abc.def"));
  } catch (...) {
    FAIL(); // should not reach here
  }
}

TEST(WaveFileReader, getters)
{
  WaveFileReader wf("example.wav");
  EXPECT_EQ(wf.numberOfChannels(), 2);
  EXPECT_EQ(wf.samplesPerSec(), 44100);
  EXPECT_EQ(wf.numberBytesPerSample(), 2);
  EXPECT_EQ(wf.numberOfSamples(), 220500);
  EXPECT_EQ(wf.numberOfSamplesPerChannel(), 110250);
}

TEST(WaveFileReader, channel1of2)
{
  WaveFileReader wf("example.wav");
  unsigned channel = 0;
  std::size_t from = 1000;
  int16_t samples[10];
  std::size_t count = array_length(samples);

  std::size_t size = wf.getChannelData(channel, from, samples, count);
  EXPECT_TRUE(size == array_length(samples));
  //std::cout << STR(size) << " samples read\n";

  constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"

  double hz        = 44100;    // samples per second
  double frequency = 261.626;  // middle C
  double seconds   = 2.5;      // time

  int N = hz * seconds;  // total number of samples
  for (unsigned n = 0; n < count; n++)
  {
    double amplitude = (double)(n + from) / N * max_amplitude;
    //double amplitude = (double)max_amplitude;
    double value     = sin( (two_pi * (n + from) * frequency) / hz );
    int channel0 = (int)(                 amplitude  * value);
    int channel1 = (int)((max_amplitude - amplitude) * value);
    //std::cout << STR(channel0) << ' '  << STR(channel1) << ' ' << STR(samples[n]) << '\n';
    if (channel == 0) {
      EXPECT_TRUE(channel0 == samples[n]);
    }
    else
    {
      EXPECT_TRUE(channel1 == samples[n]);
    }
  }

}

TEST(WaveFileWriter, headerOnlyNoData)
{
  uint16_t channels = 1;
  uint32_t samplesPerSecond = 44100;
  WriteWavefile wf("tmp.wav",
                   channels, // uint16_t channels,
                   samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                   samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                   sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                   sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

  wf.FinalizeHeader();

  EXPECT_EQ(fileSize("tmp.wav"), sizeof(WavHdr));
}

TEST(WaveFileWriter, sizeTracksData)
{
  char data[100];
  {
    uint16_t channels = 1;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    wf.AppendData(0,0); // add nothing
    wf.FinalizeHeader();

    EXPECT_EQ(fileSize("tmp.wav"), sizeof(WavHdr));
  }
  {
    uint16_t channels = 1;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    wf.AppendData(data,1); // add something
    wf.FinalizeHeader();

    EXPECT_EQ(fileSize("tmp.wav"), sizeof(WavHdr) + 1); // see it add to size
  }
  {
    uint16_t channels = 1;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    wf.AppendData(data,10); // add something
    wf.FinalizeHeader();

    EXPECT_EQ(fileSize("tmp.wav"), sizeof(WavHdr) + 10); // see it add to size
  }
}

TEST(WaveFileWriter, checkHeaderAgainstReader)
{
  { // empty
    uint16_t channels = 33;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    wf.FinalizeHeader(); // no data

    WaveFileReader wr("tmp.wav");

    EXPECT_EQ(wr.numberOfChannels(), 33);
    EXPECT_EQ(wr.samplesPerSec(), 44100);
    EXPECT_EQ(wr.numberBytesPerSample(), 2);
    EXPECT_EQ(wr.numberOfSamples(), 0);
    EXPECT_EQ(wr.numberOfSamplesPerChannel(),0);
    EXPECT_EQ(wr.fileLength(),sizeof(WavHdr));
  }
  { //data
    uint16_t channels = 2;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    const char data[] = { 1,2 };
    wf.AppendData((void *)data,sizeof(data)); // add something
    wf.FinalizeHeader();

    WaveFileReader wr("tmp.wav");
    std::cout << HERE() << ':' << STR(wr) << '\n';

    EXPECT_EQ(wr.numberOfChannels(), 2);
    EXPECT_EQ(wr.samplesPerSec(), 44100);
    EXPECT_EQ(wr.numberBytesPerSample(), 2);
    EXPECT_EQ(wr.numberOfSamples(), 1);
    EXPECT_EQ(wr.numberOfSamplesPerChannel(),0);
    EXPECT_EQ(wr.fileLength(),sizeof(WavHdr) + sizeof(data));
  }
  { //data
    uint16_t channels = 1;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    const char data[] = { 1,2,3,4 };
    wf.AppendData((void *)data,sizeof(data)); // add something
    wf.FinalizeHeader();

    WaveFileReader wr("tmp.wav");

    EXPECT_EQ(wr.numberOfChannels(), 1);
    EXPECT_EQ(wr.samplesPerSec(), 44100);
    EXPECT_EQ(wr.numberBytesPerSample(), 2);
    EXPECT_EQ(wr.numberOfSamples(), 2);
    EXPECT_EQ(wr.numberOfSamplesPerChannel(),2);
    EXPECT_EQ(wr.fileLength(),sizeof(WavHdr) + sizeof(data));
  }
}

TEST(WaveFileWriter, checkDataAgainstReader)
{
  { // empty
    uint16_t channels = 33;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    wf.FinalizeHeader(); // no data

    WaveFileReader wr("tmp.wav");

    int16_t data[10];
    try {
      wr.getChannelData(0,0,data,1);
      FAIL(); // should not get here
    } catch (const UnableToReadWavefileDataException& e) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
  { // some data
    uint16_t channels = 1;
    uint32_t samplesPerSecond = 44100;
    WriteWavefile wf("tmp.wav",
                     channels, // uint16_t channels,
                     samplesPerSecond, // uint32_t samplesPerSecond = 44100,
                     samplesPerSecond * sizeof(int16_t) * channels, // uint32_t bytesPerSecond = 176400,
                     sizeof(int16_t) * channels, // uint16_t bytesPerSampleForAllChannels = 4,
                     sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)

    { // scope out the data
      int16_t data[] = { 1,2,3 };
      wf.AppendData(data,sizeof(data));
    }
    wf.FinalizeHeader();

    WaveFileReader wr("tmp.wav");

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,0,data,1);
      SUCCEED();
      EXPECT_EQ(data[0],1);
      EXPECT_EQ(data[1],0);
      EXPECT_EQ(data[2],0);
    } catch (const UnableToReadWavefileDataException& e) {
      FAIL(); // should not get here
    } catch (...) {
      FAIL();
    }

    try { // same read
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,0,data,1);
      SUCCEED();
      EXPECT_EQ(data[0],1);
      EXPECT_EQ(data[1],0);
      EXPECT_EQ(data[2],0);
    } catch (const UnableToReadWavefileDataException& e) {
      FAIL(); // should not get here
    } catch (...) {
      FAIL();
    }

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,0,data,2); // more
      SUCCEED();
      EXPECT_EQ(data[0],1);
      EXPECT_EQ(data[1],2);
      EXPECT_EQ(data[2],0);
    } catch (const UnableToReadWavefileDataException& e) {
      FAIL(); // should not get here
    } catch (...) {
      FAIL();
    }

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,0,data,3); // more
      SUCCEED();
      EXPECT_EQ(data[0],1);
      EXPECT_EQ(data[1],2);
      EXPECT_EQ(data[2],3);
    } catch (const UnableToReadWavefileDataException& e) {
      FAIL(); // should not get here
    } catch (...) {
      FAIL();
    }

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,1,data,2); // more
      SUCCEED();
      EXPECT_EQ(data[0],2);
      EXPECT_EQ(data[1],3);
      EXPECT_EQ(data[2],0);
    } catch (const UnableToReadWavefileDataException& e) {
      FAIL(); // should not get here
    } catch (...) {
      FAIL();
    }

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,0,data,4); // too much
      FAIL(); // should not get here
    } catch (const UnableToReadWavefileDataException& e) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }

    try {
      int16_t data[] = {0,0,0,0,0};
      wr.getChannelData(0,1,data,3); // too much
      FAIL(); // should not get here
    } catch (const UnableToReadWavefileDataException& e) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
}

template <typename Word>
std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
  for (; size; --size, value >>= 8)
    outs.put( static_cast <char> (value & 0xFF) );
  return outs;
}

static void writeWaveFile()
{
  std::ofstream f( "example.wav", std::ios::binary );

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
  write_word( f, file_length - (data_chunk_pos + 8) );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f, file_length - 8, 4 );
}

int main(int argc, char **argv)
{
  std::cout << STR(sizeof(WavHdr)) << '\n';

  writeWaveFile();

  WaveFileReader wf("example.wav");

  std::cout << wf << '\n';

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
