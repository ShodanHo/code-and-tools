#ifndef __FILE_WRITE_THREAD_H_
#define __FILE_WRITE_THREAD_H_

#include "mutex_cond_collections.h"
#include <memory>

enum class FileWriteThreadActions
{
  WTA_TERMINATE,
  WTA_WRITE_FILE_START,
  WTA_WRITE_FILE_STOP
};

struct FileWriteThreadCmds
{
  FileWriteThreadActions cmd;
  std::string filename;

  FileWriteThreadCmds(const FileWriteThreadActions& _cmd,
                      const std::string& _filename) : cmd(_cmd), filename(_filename) {}
};

struct WavFileData {
  int16_t data[1024];
};

struct FileWriteData
{
  typedef std::shared_ptr<WavFileData> shared_int_ptr;
  shared_int_ptr data;
  FileWriteData(shared_int_ptr _data) : data(_data) {}
};

void file_write_thread(referenced_conditioned_list<FileWriteThreadCmds>* cmdInput,
                       referenced_conditioned_list<FileWriteData>* dataInput);

#endif //__FILE_WRITE_THREAD_H_
