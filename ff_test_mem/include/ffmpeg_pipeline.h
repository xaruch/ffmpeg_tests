#ifndef FFMPEG_PIPELINE_H_
#define FFMPEG_PIPELINE_H_

#ifndef INT64_C
#define INT64_C(val)   val##i64
#define UINT64_C(val)   val##ui64
#endif   //   INT64_C

#include "include/vars.h"
#include <pthread.h>
#include "include/logs.h"
#include <stdint.h>
#include <glib.h>

extern "C"
{
#include "libavutil/mathematics.h"
#include "libavutil/log.h"
#include <libavutil/opt.h>
#include <libavutil/dict.h>
#include <libavutil/rational.h>
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <libavformat/avio.h>
#include <libavcodec/avcodec.h>
}

#ifdef av_err2str
#undef av_err2str
#define av_err2str( errcode )   av_make_error_string( g_szErrorBuf, AV_ERROR_MAX_STRING_SIZE, errcode )
#endif      //   av_err2str

static char g_szErrorBuf[ AV_ERROR_MAX_STRING_SIZE ] = "";         //   Error String Temp Buffer

#pragma comment( lib, "avutil.lib" )      //   _av_free
#pragma comment( lib, "avformat.lib" )      //   _avio_close
#pragma comment( lib, "avcodec.lib" )      //   _avcodec_*
#pragma comment( lib, "swscale.lib" )      //   _sws_*

enum RecordingState
{
  REC_ERROR = -1,
  REC_NOTINITIALIZED = 0,
  REC_INITIALIZED = 1,
  REC_STOPPED = 2,
  REC_WRITING = 3
};

class FfPipeline
{
public:

  gpointer PartnerPipePointer;
  unsigned char Slot;

  FfPipeline(unsigned char);
  ~FfPipeline(void);
  int Start(void);
  RecordingState GetRecState(void);
  void SetRecState(RecordingState);
  void SetPartnerPipePointer(gpointer);

  void SetNeedStop(void);

  void r_avformat_close_input_v24(AVFormatContext **);

private:
  RecordingState RecState;
  time_t PrevPadTimestamp;
  bool IsNeedStopFlag;
  bool WaitKeyframeFlag;
  int64_t LastMuxDts;

  int SetPartnerToStop(void);
  void SetWaitingKeyframeFlag(void);
  void ClearWaitingKeyframeFlag(void);
  bool IsWaitingKeyframe(void);

  void RescalePtsDts24(AVPacket *, AVStream *, AVStream *);
  void WriteFrame_v242(AVFormatContext *, AVStream *, AVPacket *, AVBitStreamFilterContext *);
  bool IsNeedStop(void);
  void ClearNeedStop(void);
  int DoRecording(void);
  int ThInitiateRecording(void);
  static void *ThRecording(void *);

  static av_always_inline AVRational av_inv_q(AVRational q)
  {
    AVRational r = { q.den, q.num };
    return r;
  }

};

#endif /* FFMPEG_PIPELINE_H_ */
