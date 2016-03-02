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

class WaveFile
{
private:
  const std::string fileName;
  WavHdr wavHeader;
  int fileLen;

public:
  WaveFile(decltype(fileName) _filename);

  uint16_t numberOfChannels() const { return wavHeader.NumOfChan; }
  uint32_t samplesPerSec() const { return wavHeader.SamplesPerSec; }
  uint16_t numberBytesPerSample() const { return wavHeader.bitsPerSample / 8; }
  uint32_t numberOfSamples() const { return wavHeader.Subchunk2Size / numberBytesPerSample(); }
  uint32_t numberOfSamplesPerChannel() const { return numberOfSamples() / numberOfChannels(); }

  int fileLength() const { return fileLen; }

  std::string toString(void) const;
};

std::ostream& operator<<(std::ostream& os, const WaveFile& waveFile);

#endif // __WAVEFILE_H__
