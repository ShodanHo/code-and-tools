// http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file
#include "waveFile.h"
#include <iostream>
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

