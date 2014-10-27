/*
 * ff_writer_engine.h
 *
 *  Created on: Jul 31, 2014
 *      Author: romar
 */

#ifndef FF_WRITER_ENGINE_H_
#define FF_WRITER_ENGINE_H_

using namespace std;

#include <string>
#include "ffmpeg_pipeline.h"

class FfWriterEngine
{
public:
  FfWriterEngine();
  int Continue();
  bool IsNowDown(void);
  bool IsNowRecording(void);
  string IsNowRecordingToStr(void);
  int SetCurrentChannelToStopped(bool);
  int SetAllChannelsToStopped(bool);
  RecordingState GetRecState(unsigned char);
  void SetRecState(unsigned char, RecordingState);
  int StartRecording(unsigned char);
  int FirstStartRec();
  int StopRecOnSlot(unsigned char);
  bool PipelineIsValid(unsigned char);
  size_t TimerCounter;
  Logging *OutMemData;
  int GetMemoryConsumption(void);
  int SaveMemoryConsumption(int);
  int AppendMemoryData(void);

private:
  FfPipeline *Pipes[2];
  int FfmpegInit(void);
  string RemoveNonDigitChars(string);
};

#endif /* FF_WRITER_ENGINE_H_ */
