#pragma once

#include <windows.h>
#include <avinclude.h>
#include <waveout.h>
#include <exception.h>

/* Video.h
 * -------
 * Original Author: E. Pronk
 * Descr.: Video stream object that handles all the video streaming tasks
 */

class VideoStream;
class AVPacketQueueItem;
class AVPacketQueue;

#ifndef AV_AUDIO_BUFFER_SIZE 
#define AV_AUDIO_BUFFER_SIZE AVCODEC_MAX_AUDIO_FRAME_SIZE
#endif

GENERATE_EXCEPTION(FileNotFoundException, Exception);
GENERATE_EXCEPTION(AVStreamException, Exception);

class VideoStream : public AudioProvider {
public:
 VideoStream() : AudioProvider() {}
 ~VideoStream() {}

 void Create(const char* filename, int reqWidth, int reqHeight);

 // Start playing the video, don't call this twice (for now).
 void Start();

 int Width() const { if (_videoCodecCtx) return _videoCodecCtx->width; return 0; }
 int Height() const { if (_videoCodecCtx) return _videoCodecCtx->height; return 0; }

 // The buffer needs to be of size 3 * reqWidth * reqHeight, as specified in the constructor
 int NextFrame(void* buffer);

 // The buffer will be allocated for you, and length will be set.
 // If this function is called because a audio block just finished playing
 // set elapsed to the buffersize, so the video can sync to the audio.
 int NextAudioBuffer(void** buffer, int* len, int elapsed);

 int64_t _audioElapsed;

private:
 void Load(const char* filename);
 static DWORD WINAPI AVStreamProc(LPVOID parameter);
 static int GetBuffer(AVCodecContext* c, AVFrame* pic);
 static void ReleaseBuffer(AVCodecContext* c, AVFrame* pic);
 static uint64_t _s_pts;

 AVPacketQueue* _audioQueue;
 AVPacketQueue* _videoQueue;

 SwsContext* _swsCtx;
 AVFormatContext* _formatCtx;
 AVCodecContext* _videoCodecCtx;
 AVCodecContext* _audioCodecCtx;
 AVCodec* _videoCodec;
 AVCodec* _audioCodec;
 AVFrame* _currentFrame;
 uint8_t* _currentBuffer;
 int64_t _currentPts;
 int64_t _currentFramePts;
 int64_t _baseTime;

 bool     _started;
 WaveOut* _waveout;

 int _reqWidth, _reqHeight;
 int _videoStreamIndex, _audioStreamIndex;
};


#pragma region Inline classes

class AVPacketQueueItem
{
public:
 AVPacketQueueItem(AVPacket* packet)
 {
  if (packet) _packet = new AVPacket(*packet);
  else _packet = (AVPacket*)-1;
  _next = 0;
 }

private:
 AVPacket* _packet;
 AVPacketQueueItem* _next;

 friend class AVPacketQueue;
};

class AVPacketQueue
{
public:
 AVPacketQueue()
 {
  _first = NULL;
  _mutex = CreateMutex(NULL, FALSE, NULL);
 }

 ~AVPacketQueue()
 {
  WaitForSingleObject(_mutex, INFINITE);
  while (_first)
  {
   void* del = _first;
   _first = _first->_next;
   delete del;
  }
  ReleaseMutex(_mutex);
  CloseHandle(_mutex);
 }

 void Enqueue(AVPacket* packet)
 {
  WaitForSingleObject(_mutex, INFINITE);
  AVPacketQueueItem** ptr = &_first;
  while (*ptr) ptr = &(*ptr)->_next;
  *ptr = new AVPacketQueueItem(packet);
  ReleaseMutex(_mutex);
 }

 AVPacket* Dequeue()
 {
  AVPacket* result = NULL;
  WaitForSingleObject(_mutex, INFINITE);
  if (_first)
  {
   result = _first->_packet;
   void* del = _first;
   _first = _first->_next;
   delete del;
  }
  ReleaseMutex(_mutex);
  return result;
 }

private:
 AVPacketQueueItem* _first;
 HANDLE _mutex;
};