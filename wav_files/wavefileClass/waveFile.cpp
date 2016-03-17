/****************************************************************************************
 *
 * Description: WAV file processing.
 *
 * Class to process .wav files.
 * Reference:
 *  http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file
 *
 ***************************************************************************************/

#include "waveFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "common_utils.h"
#include "string_utils.h"

UnableToReadWavefileDataException::UnableToReadWavefileDataException(const std::string& filename,
                                                                     unsigned readPosition) :
    whatMsg(std::string("Unable to read data from wave file:") +
            filename + " at position " + common_string_utils::toString(readPosition))
{
}

// find the file size
static int getFileSize(FILE* inFile)
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

WaveFileReader::WaveFileReader(decltype(fileName) _fileName)
throw(UnableToOpenFileException,UnableToReadWavefileHeaderException)
  : fileName(_fileName), fileLen(0)
{
  ReadHeaderAndLength(); // non-RAII
}

void WaveFileReader::ReadHeaderAndLength() throw(UnableToOpenFileException,UnableToReadWavefileHeaderException)
{
  FILE* wavFile = fopen(fileName.c_str(), "r");
  if (wavFile == nullptr)
  {
    throw UnableToOpenFileException(fileName);
  }

  //Read the header
  size_t bytesRead = fread(&wavHeader, 1, sizeof(wavHeader), wavFile);
  if (bytesRead == sizeof(wavHeader))
  {
    fileLen = getFileSize(wavFile);
  }
  else
  {
    fclose(wavFile);
    throw UnableToReadWavefileHeaderException(fileName);
  }
  fclose(wavFile);
}

std::size_t WaveFileReader::getChannelData(uint16_t channel, std::size_t fromSample, int16_t *samples,
                                     std::size_t numSamples)
  throw (UnableToOpenFileException,
         UnableToReadWavefileHeaderException,
         UnableToReadWavefileDataException)
{
  FILE* wavFile = fopen(fileName.c_str(), "r");
  if (wavFile == nullptr)
  {
    throw UnableToOpenFileException(fileName);
  }

  //Read the header
  size_t bytesRead = fread(&wavHeader, 1, sizeof(wavHeader), wavFile);
  if (bytesRead == sizeof(wavHeader))
  {
    fileLen = getFileSize(wavFile);
  }
  else
  {
    fclose(wavFile);
    throw UnableToReadWavefileHeaderException(fileName);
  }

  // seek to the start of the data
  uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
  auto channels = wavHeader.NumOfChan;
  auto seek = sizeof(wavHeader) + channels * bytesPerSample * fromSample;

  if (fseek(wavFile, seek, SEEK_SET) != 0) // from the start of the file
  {
    // seek error
    fclose(wavFile);
    throw UnableToReadWavefileDataException(fileName,seek);
  }

  std::size_t bytesToRead = (channels == 1) ? sizeof(int16_t) : 2 * sizeof(int16_t);
  unsigned i;
  for (i = 0; i < numSamples; ++i) {
    int16_t channelData[2]; // FIXUP. this only cope with 2 channel files
    bytesRead = fread(channelData, 1, bytesToRead, wavFile);

    if (bytesRead != bytesToRead) {
      // reading error
      fprintf(stderr, "unable to read wav file data at sample %u\n", fromSample + i);
      fprintf(stderr, "read %u bytes of %u\n", bytesRead, bytesToRead);
      fclose(wavFile);

      throw UnableToReadWavefileDataException(fileName, fromSample + i);
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

std::string WaveFileReader::toString(void) const
{
  std::ostringstream oss;
  oss << STR(fileName) << ',' << STR(wavHeader) << ',' << STR(fileLen);
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const WaveFileReader& waveFile)
{
  os << waveFile.toString();
  return os;
}

void WriteWavefile::WriteHeader()
{
  std::ofstream f(filename, std::ios::trunc | std::ios::binary); // start with empty file

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
  std::cout << STR(data_chunk_pos) << '\n';

  // char                Subchunk2ID[4]; // "data"  string
  // unsigned long       Subchunk2Size;  // Sampled data length
  f << "data----";  // (chunk size to be filled in later)

  f.close(); // probably done by destructor
  headerWritten = true;
}

void WriteWavefile::AppendData(void *buffer, std::size_t size)
{
  if (!headerWritten)
    this->WriteHeader();

  std::ofstream f(filename, std::ios::binary | std::ios::app); // append to the end

  f.write(reinterpret_cast<const char*>(buffer), size);

  f.close(); // probably done by destructor
}

void WriteWavefile::FinalizeHeader()
{
  if (!headerWritten)
    this->WriteHeader();

  std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate); // NB: now an fstream. start at the end

  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();


  std::cout << STR(filename) << ' ' << STR(file_length) << '\n';

  // Fix the data chunk header to contain the data size
  size_t data_chunk_pos = (int)&(((WavHdr *)0)->Subchunk2ID);
  std::cout << STR(data_chunk_pos) << '\n';

  f.seekp(data_chunk_pos + 4, std::ios_base::beg ); //from the beginning
  write_word( f, file_length - (data_chunk_pos + 8) );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f, file_length - 8, 4 );

  f.close(); // probably done by destructor
}
