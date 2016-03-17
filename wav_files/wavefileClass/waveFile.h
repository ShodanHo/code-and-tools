/****************************************************************************************
 *
 * Description: WAV file processing.
 *
 * Class to process .wav files.
 * Reference:
 *  http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file
 *
 ***************************************************************************************/

#ifndef __WAVEFILE_H__
#define __WAVEFILE_H__

#include <stdint.h>
#include <string>

struct  WavHdr
{
  /* RIFF Chunk Descriptor */
  uint8_t         RIFF[4];        // RIFF Header Magic header
  uint32_t        ChunkSize;      // RIFF Chunk Size
  uint8_t         WAVE[4];        // WAVE Header
  /* "fmt" sub-chunk */
  uint8_t         fmt[4];         // FMT header
  uint32_t        Subchunk1Size;  // Size of the fmt chunk
  uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
  uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
  uint32_t        bytesPerSec;    // bytes per second
  uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
  uint16_t        bitsPerSample;  // Number of bits per sample
  /* "data" sub-chunk */
  uint8_t         Subchunk2ID[4]; // "data"  string
  uint32_t        Subchunk2Size;  // Sampled data length

  std::string toString(void) const;
};

std::ostream& operator<<(std::ostream& os, const WavHdr& wavHdr);

class UnableToOpenFileException : public std::exception
{
  std::string whatMsg;
public:
  UnableToOpenFileException(const std::string& filename) :
    whatMsg(std::string("Unable to open file:") + filename) {}

  virtual const char* what() const noexcept { return whatMsg.c_str(); }
};

class UnableToReadWavefileHeaderException : public std::exception
{
  std::string whatMsg;
public:
  UnableToReadWavefileHeaderException(const std::string& filename) :
    whatMsg(std::string("Unable to read wave file header of file:") + filename) {}

  virtual const char* what() const noexcept { return whatMsg.c_str(); }
};

class UnableToReadWavefileDataException : public std::exception
{
  std::string whatMsg;
public:
  UnableToReadWavefileDataException(const std::string& filename,
                                    unsigned readPosition);

  virtual const char* what() const noexcept { return whatMsg.c_str(); }
};

class WaveFileReader
{
private:
  const std::string fileName;
  WavHdr wavHeader;
  int fileLen;

  void ReadHeaderAndLength()
    throw(UnableToOpenFileException,UnableToReadWavefileHeaderException);

public:
  WaveFileReader(decltype(fileName) _filename)
    throw(UnableToOpenFileException,UnableToReadWavefileHeaderException);

  uint16_t numberOfChannels() const { return wavHeader.NumOfChan; }
  uint32_t samplesPerSec() const { return wavHeader.SamplesPerSec; }
  uint16_t numberBytesPerSample() const { return wavHeader.bitsPerSample / 8; }
  uint32_t numberOfSamples() const { return wavHeader.Subchunk2Size / numberBytesPerSample(); }
  uint32_t numberOfSamplesPerChannel() const { return numberOfSamples() / numberOfChannels(); }
  int fileLength() const { return fileLen; }


  /**
   * @brief Gets data for one channel.
   * Note that the size_t are not true sizeof() counts, but sizeof(int16_t) counts.
   *
   * @param channel The channel to get data for. First channel is 0.
   *
   * @param samples Where to put the data (16-bit data).
   *
   * @param fromSample Sample from which to start taking the data (a counter of 16-bit data).
   *
   * @param numSamples How much data to copy (a counter of 16-bit data).
   *
   */
  // only copes with 16-bit data
  std::size_t getChannelData(uint16_t channel, std::size_t fromSample, int16_t *samples,
                             std::size_t numSamples)
    throw (UnableToOpenFileException, UnableToReadWavefileHeaderException, UnableToReadWavefileDataException);

  std::string toString(void) const;
};

std::ostream& operator<<(std::ostream& os, const WaveFileReader& waveFile);

class WriteWavefile
{
  const std::string filename;
  const uint16_t channels;
  const uint32_t samplesPerSecond;
  const uint32_t bytesPerSecond;
  const uint16_t bytesPerSampleForAllChannels;
  const uint16_t bitsPerSample;
  bool headerWritten = false;

  void WriteHeader();

public:
  WriteWavefile(const std::string& _filename,
                uint16_t _channels,
                uint32_t _samplesPerSecond,
                uint32_t _bytesPerSecond,
                uint16_t _bytesPerSampleForAllChannels,
                uint16_t _bitsPerSample)
  : filename(_filename)
  , channels(_channels)
  , samplesPerSecond(_samplesPerSecond)
  , bytesPerSecond(_bytesPerSecond)
  , bytesPerSampleForAllChannels(_bytesPerSampleForAllChannels)
  , bitsPerSample(_bitsPerSample){}

  void AppendData(void *buffer, std::size_t size);

  void FinalizeHeader(); // write the size into the header
};

#endif // __WAVEFILE_H__
