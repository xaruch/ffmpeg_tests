#include "include/ffmpeg_pipeline.h"

FfPipeline::FfPipeline(unsigned char slot_num)
{
  Slot = slot_num;
  RecState = REC_NOTINITIALIZED;
  PrevPadTimestamp = 0;
  WaitKeyframeFlag = false;
  IsNeedStopFlag = false;
  Index = 0;
  Ist = NULL;
  Ost = NULL;
  Iccx = NULL;
  Ofcx = NULL;
  Ifcx = NULL;
  LastMuxDts = AV_NOPTS_VALUE;
}

FfPipeline::~FfPipeline()
{
}

void FfPipeline::r_avformat_close_input_v24(AVFormatContext **ps)
{
  return avformat_close_input(ps);
}

void FfPipeline::ClearNeedStop()
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&mutex);
  IsNeedStopFlag = false;
  pthread_mutex_unlock(&mutex);
}

void FfPipeline::SetNeedStop()
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&mutex);
  IsNeedStopFlag = true;
  pthread_mutex_unlock(&mutex);
}

void FfPipeline::SetPartnerPipePointer(gpointer p)
{
  PartnerPipePointer = p;
}

void FfPipeline::SetRecState(RecordingState rst)
{
  RecState = rst;
}

int FfPipeline::SetPartnerToStop()
{
  const char *class_name = "FfPipeline::SetPartnerToStop";
  FfPipeline *Pipe = (FfPipeline *) PartnerPipePointer;

  Pipe->SetNeedStop();

  sprintf(str, "DEBUG: Slot '%d' was marked for stopping by SetNeedStopFlag().\n", Pipe->Slot);
  AppendLog(str, class_name);
  return 0;
}

void FfPipeline::SetWaitingKeyframeFlag()
{
  WaitKeyframeFlag = true;
}

void FfPipeline::ClearWaitingKeyframeFlag()
{
  WaitKeyframeFlag = false;
}

bool FfPipeline::IsWaitingKeyframe()
{
  return WaitKeyframeFlag;
}

bool FfPipeline::IsNeedStop()
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  bool retval;

  pthread_mutex_lock(&mutex);
  retval = IsNeedStopFlag;
  pthread_mutex_unlock(&mutex);

  return retval;
}

int FfPipeline::ThInitiateRecording()
{
  const char *class_name = "FfPipeline::ThInitiateRecording";
  pthread_t ptid;

  sprintf(str, "DEBUG: Pipeline slot '%d'. Try to starting thread with stopping...\n", Slot);
  AppendLog(str, class_name);

  pthread_create(&ptid, NULL, &ThRecording, (void *) this);

  pthread_detach(ptid);

  sprintf(str, "DEBUG: Pipeline slot '%d'. Starting thread ended!\n", Slot);
  AppendLog(str, class_name);

  return 0;
}

void *FfPipeline::ThRecording(void *pdata)
{
  FfPipeline *Pip = (FfPipeline *) pdata;

  Pip->DoRecording();

  return pdata;
}

RecordingState FfPipeline::GetRecState()
{
  return RecState;
}

int FfPipeline::Start()
{
	return ThInitiateRecording();
}

void FfPipeline::WriteFrame_v242(AVFormatContext *s, AVStream *st, AVPacket *pkt, AVBitStreamFilterContext *bsfc)
{
  const char *class_name = "FfPipeline::WriteFrame_v242";
  int ret;
  AVCodecContext *avctx = st->codec;

  if ((avctx->codec_type == AVMEDIA_TYPE_AUDIO || avctx->codec_type == AVMEDIA_TYPE_VIDEO) && pkt->dts != AV_NOPTS_VALUE)
    {
      int64_t max = st->cur_dts + !(s->oformat->flags & AVFMT_TS_NONSTRICT);
      if (st->cur_dts && st->cur_dts != AV_NOPTS_VALUE && max > pkt->dts)
        {
          AppendLog("WARNING: PTS:DTS < PRId64 invalid, clipping\n", class_name);
          if (pkt->pts >= pkt->dts)
            pkt->pts = FFMAX(pkt->pts, max);
          pkt->dts = max;
        }
    }

  ret = av_interleaved_write_frame(s, pkt);

  if (ret < 0)
    {
      sprintf(str, "ERROR: Error executing av_interleaved_write_frame()...Error code: %d. Error: '%s'.\n", ret, av_err2str(ret));
      AppendLog(str, class_name);
    }
}

void FfPipeline::RescalePtsDts24(AVPacket *pkt, AVStream *in_stream, AVStream *out_stream)
{
  pkt->pts = av_rescale_q(pkt->pts, in_stream->time_base, out_stream->time_base);
  pkt->dts = av_rescale_q(pkt->dts, in_stream->time_base, out_stream->time_base);
  pkt->duration = av_rescale_q(pkt->duration, in_stream->time_base, out_stream->time_base);
}

int FfPipeline::DoRecording(void)
{
  const char *class_name = "FfPipeline::DoRecording";
  AVFormatContext *ifcx = NULL;
  AVFormatContext *ofcx = NULL;
  AVDictionary *opts = NULL;
  AVOutputFormat *ofmt = NULL;
  AVPacket pkt;
  uint64_t a_packets = 0, v_packets = 0;
  size_t i;
  int ret;
  char path[BUF_SIZE];

  sprintf(path, "%s_%d.mkv", SAVE_PATH, ++Count);

  sprintf(str, "DEBUG: Output path: '%s'\n", path);
  AppendLog(str, class_name);

  av_dict_set(&opts, "rtsp_transport", "tcp", 0);

  if (avformat_open_input(&ifcx, CAM_URL, NULL, &opts) < 0)
    {
      avformat_close_input(&ifcx);
      ifcx = NULL;

      av_dict_free(&opts);

      return -1;
    }
  else
    {
    }

  av_dict_free(&opts);

  if (avformat_find_stream_info(ifcx, NULL) < 0)
    {
      avformat_close_input(&ifcx);
      ifcx = NULL;

      return -1;
    }

  avformat_alloc_output_context2(&ofcx, NULL, NULL, path);

  if (ofcx == NULL)
    {
      return -1;
    }
  else
    {
    }

  ofmt = ofcx->oformat;

  if (ofmt == NULL)
    {
      return -1;
    }

  for (i = 0; i < ifcx->nb_streams; i++)
    {
      AVStream *in_stream = ifcx->streams[i];
      AVStream *out_stream = avformat_new_stream(ofcx, in_stream->codec->codec);

      if (!out_stream)
        {
          AppendLog("Failed allocating output stream\n", class_name);
          return -1;
        }

      ret = avcodec_copy_context(out_stream->codec, in_stream->codec);

      if (ret < 0)
        {
          return ret;
        }

      out_stream->time_base = in_stream->time_base;
      out_stream->codec->time_base = out_stream->time_base;
      out_stream->codec->codec_tag = 0;

      if (ofcx->oformat->flags & AVFMT_GLOBALHEADER)
        out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;

    }   // End of for streams

  if (!(ofmt->flags & AVFMT_NOFILE))
    {
      ret = avio_open(&ofcx->pb, path, AVIO_FLAG_WRITE);
      if (ret < 0)
        {
          return ret;
        }
    }
  else
    {
      AppendLog("WARNING: for ofmt set flag AVFMT_NOFILE so we skip open file.\n", class_name);
    }

  ret = avformat_write_header(ofcx, NULL);

  if (ret < 0)
    {
      return ret;
    }

  SetWaitingKeyframeFlag();
  ClearNeedStop();

  while((av_read_frame(ifcx, &pkt) >= 0) && (!IsNeedStop()))
    {
      AVStream *in_stream = NULL, *out_stream = NULL;

      if ((IsWaitingKeyframe() && !(pkt.flags & AV_PKT_FLAG_KEY)))
        {
          av_free_packet(&pkt);
          continue;
        }

      if (IsWaitingKeyframe())
        {
          ClearWaitingKeyframeFlag();
          pkt.pts = pkt.dts = AV_NOPTS_VALUE;
          SetRecState(REC_WRITING);
          SetPartnerToStop();
        }

      in_stream  = ifcx->streams[pkt.stream_index];
      out_stream = ofcx->streams[pkt.stream_index];

      if (!in_stream)
        {
          AppendLog("ERROR: in_stream is NULL pointer! Skip this packet!\n", class_name);
          av_free_packet(&pkt);

          continue;
        }

      if (!out_stream)
        {
          AppendLog("ERROR: out_stream is NULL pointer! Skip this packet!\n", class_name);
          av_free_packet(&pkt);

          continue;
        }

      if (out_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        a_packets++;
      else if (out_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        v_packets++;

      RescalePtsDts24(&pkt, in_stream, out_stream);

      WriteFrame_v242(ofcx, out_stream, &pkt, NULL);

      av_free_packet(&pkt);
    }

  // Now we stopping
  sprintf(str, "DEBUG: Now we stopping Slot %d\n", Slot);
  AppendLog(str, class_name);

  if (!IsNeedStop())
    av_free_packet(&pkt);

  sprintf(str, "DEBUG: Try to av_write_trailer() for Slot %d\n", Slot);
  AppendLog(str, class_name);

  av_write_trailer(ofcx);

  sprintf(str, "DEBUG: Try to close all streams codec for Slot %d\n", Slot);
  AppendLog(str, class_name);

  for (i = 0; i < ofcx->nb_streams; i++)
    {
      avcodec_close(ofcx->streams[i]->codec);
      av_freep(ofcx->streams[i]);
    }

  sprintf(str, "DEBUG: Try to avio_close() for Slot %d\n", Slot);
  AppendLog(str, class_name);

  avio_close(ofcx->pb);

  sprintf(str, "DEBUG: Try to av_free() for Slot %d\n", Slot);
  AppendLog(str, class_name);

  av_free(ofcx);

  sprintf(str, "DEBUG: Try to avformat_close_input() for Slot %d\n", Slot);
  AppendLog(str, class_name);

  avformat_close_input(&ifcx);

  SetRecState(REC_STOPPED);

  return 0;
}
