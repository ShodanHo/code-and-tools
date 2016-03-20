#include "file_write_thread.h"
#include <chrono>
#include <iostream>
#include "waveFile.h"

#define HERE() __FILE__ << ':' << __FUNCTION__ << ':' << __LINE__

void StartWritingFile(WriteWavefile* wav_file)
{
  wav_file->AppendData(0,0); // force the header to be written
}

void AppendDataToFile(WriteWavefile* wav_file, const FileWriteData& data)
{
  std::cout << HERE() << '\n';
  //wav_file.AppendData(*data.data,sizeof(*data.data)); // force the header to be written
  wav_file->AppendData(data.data.get(),sizeof(*data.data)); // force the header to be written

}

void StopWritingFile(const std::string& filename)
{
  std::cout << HERE() << '\n';

}

void file_write_thread(referenced_conditioned_list<FileWriteThreadCmds>* cmdInput,
                       referenced_conditioned_list<FileWriteData>* dataInput)
{
  bool writingFile = false; // startup not writing to file
  std::string filename; // name of the file we are writing

  WriteWavefile *wav_file = nullptr;
  for (;;)
  {
    std::cout << HERE() << '\n';
    cmdInput->cond_wait_for(std::chrono::milliseconds(500)); // wait for some input

    // is there a command
    while (cmdInput->size() > 0)
    {
      auto cmd = cmdInput->front_value();
      cmdInput->pop_front();
      switch (cmd.cmd) {
        case FileWriteThreadActions::WTA_TERMINATE:
          if (writingFile == true)
            StopWritingFile(filename);
          return; // close file and terminate

        case FileWriteThreadActions::WTA_WRITE_FILE_START:
          if (writingFile == false)
          {
            filename = cmd.filename;
            wav_file = new WriteWavefile(filename,
                                         1, // uint16_t channels,
                                         48100, // uint32_t samplesPerSecond = 44100,
                                         48100 * sizeof(int16_t), // uint32_t bytesPerSecond = 176400,
                                         sizeof(int16_t), // uint16_t bytesPerSampleForAllChannels = 4,
                                         sizeof(int16_t) * 8); // uint16_t bitsPerSample = 16)
            StartWritingFile(wav_file);
          }
          writingFile = true;
          break;

        case FileWriteThreadActions::WTA_WRITE_FILE_STOP:
          if (writingFile == true)
            StopWritingFile(filename);
          writingFile = false;
          break;

      }
    }

    // is there some data
    while (dataInput->size() > 0)
    {
      if (writingFile == false)
      {
        // discard all data
        dataInput->pop_front();
      }
      else
      {
        auto data = dataInput->front_value();
        dataInput->pop_front();
        AppendDataToFile(wav_file,data);
      }
    }
  }
}
