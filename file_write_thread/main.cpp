#include <gtest/gtest.h>
#include "file_write_thread.h"
#include <mutex>
#include <thread>
#include "common_utils.h"
#include "waveFile.h"
#include <fstream>

static size_t fileSize(const std::string& filename)
{
  std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

  size_t fileSize = f.tellp();

  f.close(); // probably done by destructor
  return fileSize;
}

TEST(FileWriteThread,StartAndStopThread)
{
  std::mutex mtx;
  std::condition_variable cv;
  referenced_conditioned_list<FileWriteThreadCmds> cmdInput(mtx,cv);
  referenced_conditioned_list<FileWriteData> dataInput(mtx,cv);
  std::thread fileWriter = std::thread(file_write_thread, &cmdInput, &dataInput);

  EXPECT_EQ(fileWriter.joinable(), true);

  cmdInput.push_back(FileWriteThreadCmds(FileWriteThreadActions::WTA_TERMINATE,""));

  fileWriter.join();
  EXPECT_EQ(fileWriter.joinable(), false);
}

TEST(FileWriteThread,WriteHeaderOnly)
{
  std::mutex mtx;
  std::condition_variable cv;
  referenced_conditioned_list<FileWriteThreadCmds> cmdInput(mtx,cv);
  referenced_conditioned_list<FileWriteData> dataInput(mtx,cv);
  std::thread fileWriter = std::thread(file_write_thread, &cmdInput, &dataInput);

  cmdInput.push_back(FileWriteThreadCmds(FileWriteThreadActions::WTA_WRITE_FILE_START,"headeronly"));
  cmdInput.push_back(FileWriteThreadCmds(FileWriteThreadActions::WTA_TERMINATE,""));

  fileWriter.join();

  EXPECT_EQ(fileSize("headeronly"),sizeof(WavHdr));
}

TEST(FileWriteThread,WriteData)
{
  std::mutex mtx;
  std::condition_variable cv;
  referenced_conditioned_list<FileWriteThreadCmds> cmdInput(mtx,cv);
  referenced_conditioned_list<FileWriteData> dataInput(mtx,cv);
  std::thread fileWriter = std::thread(file_write_thread, &cmdInput, &dataInput);

  cmdInput.push_back(FileWriteThreadCmds(FileWriteThreadActions::WTA_WRITE_FILE_START,"data.wav"));
  std::shared_ptr<int16_t> ptr( new WavFileData, []( WavFileData *p ) { delete[] p; } );

  dataInput.push_back(FileWriteData(ptr));

  sleep(1);
  cmdInput.push_back(FileWriteThreadCmds(FileWriteThreadActions::WTA_TERMINATE,""));

  fileWriter.join();

  EXPECT_EQ(fileSize("data.wav"),sizeof(WavHdr));
}

int main(int argc, char **argv)
{
  //std::thread fileWriter = std::thread(file_write_thread, &cmdInput, &dataInput)

  std::cout << "Hello, world\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
