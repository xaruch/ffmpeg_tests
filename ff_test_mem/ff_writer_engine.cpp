#include "include/ff_writer_engine.h"
#include "include/vars.h"
#include "include/logs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

FfWriterEngine::FfWriterEngine()
{
  FfmpegInit();

  Pipes[0] = new FfPipeline(0);
  Pipes[1] = new FfPipeline(1);
  Pipes[0]->SetPartnerPipePointer(Pipes[1]);
  Pipes[1]->SetPartnerPipePointer(Pipes[0]);
  TimerCounter = 0;
  OutMemData = new Logging("mem.log", 7);
}

int FfWriterEngine::Continue()
{
  const char *class_name = "WriterEngine::Continue";

  if (Pipes[0]->GetRecState() != REC_WRITING)
    {
      sprintf(str, "DEBUG: Cam Slot 0 not writing now. Start Slot 0 recording.\n");
      AppendLog(str, class_name);

      Pipes[0]->Start();
    }
  else if (Pipes[1]->GetRecState() != REC_WRITING)
    {
      sprintf(str, "DEBUG: Cam Slot 1 not writing now. Start Slot 1 recording.\n");
      AppendLog(str, class_name);

      Pipes[1]->Start();
    }
  else
    {
      sprintf(str, "ERROR: Cam Both recording slots are simultaneous writing! Skip!!! Need attention.\n");
      AppendLog(str, class_name);
      return -1;
    }

  return 0;
}

bool FfWriterEngine::IsNowDown()
{
  if ((Pipes[0]->GetRecState() != REC_WRITING) & (Pipes[1]->GetRecState() != REC_WRITING))
    return true;
  else
    return false;
}

bool FfWriterEngine::IsNowRecording()
{
  if ((Pipes[0]->GetRecState() == REC_WRITING) || (Pipes[1]->GetRecState() == REC_WRITING))
    return true;
  else
    return false;
}

string FfWriterEngine::IsNowRecordingToStr()
{
  if (IsNowRecording())
    return "on";
  else
    return "off";

}

int FfWriterEngine::SetCurrentChannelToStopped(bool NoNeedToDb)
{
  const char *class_name = "WriterEngine::SetCurrentChannelToStopped";

  if (Pipes[0]->GetRecState() == REC_WRITING)
    {
      sprintf(str, "Cam Channel 0 writing now so set it to stopping\n");
      AppendLog(str, class_name);

      if (NoNeedToDb)
        Pipes[0]->SetRecState(REC_STOPPED);

      Pipes[0]->SetNeedStop();
    }
  else if (Pipes[1]->GetRecState() == REC_WRITING)
    {
      sprintf(str, "Cam Channel 1 writing now so set it to stopping\n");
      AppendLog(str, class_name);

      if (NoNeedToDb)
        Pipes[1]->SetRecState(REC_STOPPED);

      Pipes[1]->SetNeedStop();
    }
  else
    {
      sprintf(str, "WARNING: Cam haven't got any recording slots at this time\n");
      AppendLog(str, class_name);
      return -1;
    }

  return 0;
}

int FfWriterEngine::SetAllChannelsToStopped(bool NoNeedToDb)
{
  if (NoNeedToDb)
    Pipes[0]->SetRecState(REC_STOPPED);

  Pipes[0]->SetNeedStop();

  if (NoNeedToDb)
    Pipes[1]->SetRecState(REC_STOPPED);

  Pipes[1]->SetNeedStop();

  return 0;
}

RecordingState FfWriterEngine::GetRecState(unsigned char slot)
{
  return Pipes[slot]->GetRecState();
}

void FfWriterEngine::SetRecState(unsigned char slot, RecordingState state)
{
  Pipes[slot]->SetRecState(state);
}

int FfWriterEngine::StartRecording(unsigned char slot)
{
  return Pipes[slot]->Start();
}

int FfWriterEngine::FirstStartRec()
{
  return StartRecording(0);
}

int FfWriterEngine::StopRecOnSlot(unsigned char slot)
{
  const char *class_name = "WriterEngine::StopRecOnSlot";

  if (Pipes[slot]->GetRecState() != REC_WRITING)
    {
      AppendLog("CRITICAL: Critical error: Need to stop not writing slot. Try it anyway\n", class_name);
      Pipes[slot]->SetNeedStop();
      return 1;

    }
  else
    {
      Pipes[slot]->SetNeedStop();
    }

  sprintf(str, "DEBUG: Slot '%d' was stopped.\n", slot);
  AppendLog(str, class_name);
  return 0;
}

int FfWriterEngine::FfmpegInit()
{
  av_log_set_level(AV_LOG_INFO);

  avcodec_register_all();
  av_register_all();
  avformat_network_init();

  return 0;
}

string FfWriterEngine::RemoveNonDigitChars(string inpus_str)
{
  inpus_str.erase(remove_if(inpus_str.begin(), inpus_str.end(), not1(ptr_fun(static_cast<int(*)(int)>(isdigit)))), inpus_str.end());

  return inpus_str;
}

int FfWriterEngine::GetMemoryConsumption(void)
{
  const char *class_name = "ProcResource::GetMemoryConsumption";
  pid_t ppid = getpid();
  ifstream proc_file;
  string line;
  char fpath[BUF_SIZE];
  size_t pos;
  int result;

  sprintf(fpath, "/proc/%d/status", (int) ppid);

  proc_file.open(fpath, ios::in);

  if (proc_file.is_open())
    {
      while (getline(proc_file, line))
        {
          if ((pos = line.find("VmRSS")) != string::npos)
            {
              line = RemoveNonDigitChars(line);
              istringstream(line) >> result;
              proc_file.close();
              return result;
            }
        }
      proc_file.close();
    }
  else
    {
      AppendLog("Error opening file with process status!\n", class_name);
    }

  return -1;
}

int FfWriterEngine::SaveMemoryConsumption(int data)
{
  char ostr[BUF_SIZE];
  sprintf(ostr, "%d\n", data);

  OutMemData->AppendData(ostr);

  return 0;
}

int FfWriterEngine::AppendMemoryData(void)
{
  return SaveMemoryConsumption(GetMemoryConsumption());
}
