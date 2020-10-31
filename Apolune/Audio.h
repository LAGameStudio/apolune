/***********************************************************************************************
 * AL Gore Library: An MSVC++ class implementation that wraps OpenAL and ALURE using other libs*
 ***********************************************************************************************
 * This file is distributed under the Revised BSD License, also known as the "New" BSD License *
 * See the text of the "New BSD License" for rules about using this file in your project.      *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
// Note: this was developed for use with soft_oal.dll renamed to Openal32.dll as a drop-in replacement

#pragma once
#pragma warning (disable:4267)  // conversion warning

#include <al.h>
#include <alc.h>
//#include <AL/alu.h>
#include <AL/alure.h>
#include <efx.h>
#include <EFX-Util.h>
#include <efx-creative.h>
//#include <al/xram.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include <sndfile.h>
#include <string.h>
#include "ZeroTypes.h"
#include "Indexed.h"

using namespace std;
#include <vector>

#ifndef null
#define null nullptr
#endif 

#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"

extern sf_count_t total_memory_for_audio;

// Forward declarations to rectify identities

class ALSource;
class ALBuffer;
class ALWaveForm;
class ALWaveForms;
class ALSound;
class ALUREStream;
class ALSoundManager;

#if defined(LATER)
class EFX;
#endif

// EFX help (where we got the .h): Dmytry Lavrov (Public Domain code samples)

//EAX
//const GUID DSPROPSETID_EAX20_ListenerProperties = { 0x306a6a8, 0xb224, 0x11d2, { 0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };
//const GUID DSPROPSETID_EAX20_BufferProperties   = { 0x306a6a7, 0xb224, 0x11d2, { 0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };

//#ifndef(AL_FORMAT_VORBIS_EXT)
//#define AL_FORMAT_VORBIS_EXT 0x10003
//#endif

#ifndef ALC_EXT_EFX
#define AL_FILTER_TYPE                                     0x8001
#define AL_EFFECT_TYPE                                     0x8001
#define AL_FILTER_NULL                                     0x0000
#define AL_FILTER_LOWPASS                                  0x0001
#define AL_FILTER_HIGHPASS                                 0x0002
#define AL_FILTER_BANDPASS                                 0x0003
#define AL_EFFECT_NULL                                     0x0000
#define AL_EFFECT_EAXREVERB                                0x8000
#define AL_EFFECT_REVERB                                   0x0001
#define AL_EFFECT_CHORUS                                   0x0002
#define AL_EFFECT_DISTORTION                               0x0003
#define AL_EFFECT_ECHO                                     0x0004
#define AL_EFFECT_FLANGER                                  0x0005
#define AL_EFFECT_FREQUENCY_SHIFTER                        0x0006
#define AL_EFFECT_VOCAL_MORPHER                            0x0007
#define AL_EFFECT_PITCH_SHIFTER                            0x0008
#define AL_EFFECT_RING_MODULATOR                           0x0009
#define AL_EFFECT_AUTOWAH                                  0x000A
#define AL_EFFECT_COMPRESSOR                               0x000B
#define AL_EFFECT_EQUALIZER                                0x000C
#define ALC_EFX_MAJOR_VERSION                              0x20001
#define ALC_EFX_MINOR_VERSION                              0x20002
#define ALC_MAX_AUXILIARY_SENDS                            0x20003
#endif

#ifndef ALC_MONO_SOURCES
#define ALC_MONO_SOURCES                         0x1010
#define ALC_STEREO_SOURCES                       0x1011
#endif

#ifndef AL_EFFECT_EAXREVERB
#define AL_EFFECT_EAXREVERB                                0x8000
#endif

#ifndef ALC_CAPTURE_DEVICE_SPECIFIER
/**
 * The Specifier string for default device
 */
#define ALC_DEFAULT_DEVICE_SPECIFIER             0x1004
#define ALC_DEVICE_SPECIFIER                     0x1005
#define ALC_EXTENSIONS                           0x1006

#define ALC_MAJOR_VERSION                        0x1000
#define ALC_MINOR_VERSION                        0x1001

#define ALC_ATTRIBUTES_SIZE                      0x1002
#define ALC_ALL_ATTRIBUTES                       0x1003
/**
 * Capture extension
 */
#define ALC_CAPTURE_DEVICE_SPECIFIER             0x310
#define ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER     0x311
#define ALC_CAPTURE_SAMPLES                      0x312
#endif

/*
 * Implementation of OpenAL via an audio API base class.
 */


// Be very careful with these two parameters
// It is very dependant on the audio hardware your
// user is using. It you get too large, it may work
// on one persons system but not on another.
// TODO Write a hardware test !
#define MAX_AUDIO_BUFFERS   64
#define MAX_AUDIO_SOURCES   16

enum ALState { empty=0, stopped=1, paused=2, playing=3 };
enum ALSoundFormat { mono8=AL_FORMAT_MONO8, mono16=AL_FORMAT_MONO16, stereo8=AL_FORMAT_STEREO8, stereo16=AL_FORMAT_STEREO16 };

// Used during Init(), maybe it shouldn't die() here...
void checkForErrors(bool quit);

static const int indentation = 4;
static const int maxmimumWidth = 79;

void printChar(int c, int *width);
void indent(int *width);

void printList(const char *header, char separator, const char *list);

struct SoundVector
{
public:
 float x,y,z,a,b,c;
 ALfloat f3[3];
 ALfloat f6[6];
 SoundVector(void) {
  f3[0]=f3[1]=f3[2]=f6[0]=f6[1]=f6[2]=f6[3]=f6[4]=f6[5]=x=y=z=a=b=c=0.0f;
 } //SoundVector() { y=1.0f; }
 float *asFloat3( ) { f3[0]=x; f3[1]=y; f3[2]=z; return f3; }
 float *asFloat6( ) { f6[0]=x; f6[1]=y; f6[2]=z; f6[3]=a; f6[4]=b; f6[5]=c; return f6; }
 ALfloat *fromFloat3( float *f ) { x=f[0]; y=f[1]; z=f[2]; return f3; }
 ALfloat *fromFloat6( float *f ) { x=f[0]; y=f[1]; z=f[2]; a=f[3]; b=f[4]; c=f[6]; return f6; }
};

//int unusedBuffers, unusedSources;

void GetALCErrorString(ALenum err);
bool ALError( const char *tb );

class ALBuffer : public ListItem {
public:
 ALuint id;
 inline bool valid() { if ( alIsBuffer( id ) ) return true; return false; }
 float duration() {
  ALint bits, channels, freq, size;
  alGetBufferi(id, AL_BITS, &bits);
  alGetBufferi(id, AL_CHANNELS, &channels);
  alGetBufferi(id, AL_FREQUENCY, &freq);
  alGetBufferi(id, AL_SIZE, &size);
  if(ALError("ALBuffer:duration()")) { return 0.0f; }
  return (float) ((size / channels * 8 / bits ) / freq);
 }
 bool Close() {
  if ( valid() ) {
   alDeleteBuffers(1, &(id));
   if ( ALError( "ALBuffer:Close:alDeleteBuffers") ) return false; //unusedBuffers++;
  }
  return true;
 }
 ALBuffer() {
  id=0;
 }
 ~ALBuffer() { /*Close();*/ }
};

class ALBuffers : public LinkedList {
public:
 void ClearInvalid() {
  EACHN(first, ALBuffer, b, {
   if (!b->valid()) { Remove(b); delete b; }
  });
 }
 ALBuffer *New() {
  ALBuffer *b;
  alGetError();
  //if ( unusedBuffers <= 0 ) { OUTPUT( "No unused buffers available when requesting New() buffer\n" ); delete b; return null; }
  b= new ALBuffer;
  alGenBuffers(1,&(b->id)); if(ALError("ALBuffer:alGenBuffers")) { delete b; return null; } //unusedBuffers--;
  //OUTPUT( "#%d buffer created with ID of %d\n", unusedBuffers, b.id );
  Append(b);
  return b;
 }
 CLEARLISTRESET(ALBuffer);
};

class ALSource : public ListItem {
public:
 ALuint id;
 SoundVector position, velocity; // KittyCat: set a circular path along the X and Y planes. position.x: {-1,0,0} = left, {0,1,0} = center, {1,0,0} = right
 float pitch,gain;
 float rolloff;
 ALboolean loop,relative;
 ALState state;
 ALSource() {
  pitch=1.0f;
  gain=1.0f;
  rolloff=0.0f;
  loop=AL_FALSE;
  relative=AL_FALSE;
  id=0;
  state=(ALState) 0;
 }
   inline bool valid() { if ( alIsSource( id ) ) return true; return false; }
 void bindBuffer( ALBuffer buffer );
 void bindSound( ALSound *sound );
 void bindStream( ALUREStream *stream );
 void LinearPan( float amountNeg1LToPos1R ) { // Works only if listener is at default position
  position.x = amountNeg1LToPos1R;
  position.y = sqrt(1.0f - amountNeg1LToPos1R*amountNeg1LToPos1R);
  position.z = 0.0f;
  Update();
 }
 void Detach() { 
  if ( state == ALState::empty ) { OUTPUT( "ALSource:Detach:Empty source attempted. (Cannot dump a buffer from an empty source)\n" ); return; }
  else if ( state != stopped ) { Stop(); }
  if ( valid() ) { alSourcei(id,AL_BUFFER,0); state= ALState::empty; alGetError(); }
 }
 // Updates changes to the class in the hardware
 inline void Update() {
  alSourcef (id, AL_PITCH,    pitch  );
  alSourcef (id, AL_GAIN,     gain   );
  alSourcefv(id, AL_POSITION, position.asFloat3() );
  alSourcefv(id, AL_VELOCITY, velocity.asFloat3() );
  alSourcef (id, AL_ROLLOFF_FACTOR, rolloff );
  alSourcei (id, AL_LOOPING,  loop   );
  alSourcei (id, AL_SOURCE_RELATIVE, relative );
 }
 inline void Stop()      { if ( valid() && state == playing ) { alGetError(); alSourceStop(id);  alGetError(); state=stopped; } }
 inline void Pause()     { if ( valid() && state == playing ) { alGetError(); alSourcePause(id); alGetError(); state=paused;  } }
 inline void Play()      { if ( valid() && state != playing ) { ALError("ALSound:Pre-Play()"); alSourcePlay(id); ALError("ALSound:Play() had an error."); alGetError(); state=playing; } }
 inline void Resume() { Play(); }
//        void PauseAll()  { if ( valid() && state == playing ) Pause();  if ( next ) next.PauseAll();  }
//        void ResumeAll() { if ( valid() && state == paused  ) Resume(); if ( next ) next.ResumeAll(); }

 bool Close( ) { 
  if ( valid() ) { 
   if ( state != ALState::empty ) Detach();
   alDeleteSources(1, &(id)); if ( ALError( "ALSource:Close:alDeleteSources") ) return false; 
   //unusedSources++; 
   return true;
  }
  return false;
 }
 ~ALSource() { Close(); }
};

class ALSources : public LinkedList
{
 // List functions for managing a multi-source list.
 ALSource *AddNew()
 {
  ALSource *s = null;
  ALuint id;
  alGenSources(1,&id);
  if(!ALError("ALSource:WAV:alGenSources")) { Append(s = new ALSource); s->id; }
  return s;
 }
 CLEARLISTRESET(ALSource);
};


/*
ALURE stream
*/
class ALUREStream : public ListItem {
public:
 Zp<ALSoundManager> manager;
 Zp<alureStream> stream;
 string name;
 int bufferSize;
 int usesBuffers;
 unsigned int samples;
 float duration;
 float refreshRate;
// ALuint *buf;
 Zp<ALSource> source;
 bool created;
 int loops;
 bool badError;
 SoundVector position, velocity;
 float pitch,gain;
 float threadDelay;
 bool terminated;

 ALUREStream() {
  name=string("");
  bufferSize=128*1024;
  usesBuffers=3;//3;
  threadDelay=0.01f;
  samples=0;
  duration=0;
  refreshRate=1.0f/FPS;
  created=false;
  loops=0;
  badError=false;
  pitch=gain=1.0f;
  threadDelay=1.0f/FPS;
  terminated=false;
 }

 ~ALUREStream() {
  if ( Playing() ) Stop();
  alureDestroyStream(stream, 0, null);// usesBuffers, buf);
  delete source;
  //unusedBuffers+=usesBuffers;
 }

 inline bool Playing() { return ( source && source->state == playing ); }

// uint Main() { Sleep(threadDelay); DelayExpired(); return 0; }

 inline void Pause()  { if (created && Playing() ) { alurePauseSource(source->id); } }
 inline void Resume() { if (created && !Playing() ) { alureResumeSource(source->id); } }
 inline void Stop()   {
  OUTPUT("Stop() was called on a stream.\n");
  if (created) { alureStopSource(source->id,AL_TRUE); }
 } //terminated = true; ((GuiApplication)__thisModule).Unlock(); Wait(); ((GuiApplication)__thisModule).Lock(); } }
 inline void Rewind() { if (created) { Stop(); alureRewindStream(stream); } }

 bool LoadIntoSource(const char *fn, ALSource *s, bool CalcSize ) {
  name=string(fn);
  if ( !source ) { OUTPUT("ALUREStream:Create complains the required source was not provided.\n" ); return false; }
  //if ( usesBuffers > unusedBuffers ) { OUTPUT("ALUREStream:Create Not enough buffers available. Requested: %d Available: %d\n", usesBuffers, unusedBuffers ); return false; }
  alGetError();
  source=s;
//  if ( buf ) { OUTPUT( "ALUREStream:Create warns, the buffers were already created, was .Create() mistakenly called twice?\n" ); delete buf; }
  //buf=new ALuint[usesBuffers];
  //unusedBuffers-=usesBuffers;
  stream=alureCreateStreamFromFile((ALchar *) fn,bufferSize,0/*usesBuffers*/,null);//buf);
  if ( !stream ) { OUTPUT( "ALUREStream:Create could not load the file: %s\n", fn, alureGetErrorString()); return false; }
  if ( CalcSize ) CalculateSize(fn);
  created=true;
  return created;
 }

 virtual void OnComplete() { /*OUTPUT( "EOS!\n" );*/ }
 static void eos_callback( void * a, ALuint b ) {
  //ALUREStream *s=(ALUREStream *) a;
  //s->OnComplete();
  // This feature was turned off because the pointer to a was invalid.  You can get around this by using globals here perhaps, however
  // I have opted to temporarily suspend work.  
 }
 bool Play() {
  if ( !this ) { OUTPUT( "ALUREStream:Play:Called on a null object.\n" ); return false; }
  if ( !source ) { OUTPUT( "ALUREStream:Play:No source defined for the stream '%s'.  Did it load properly?\n", name ); return false; }
  if ( source->state == stopped ) {
   OUTPUT ( "Play() was called on a stream.\n" );
   alGetError();
//   alSourceQueueBuffers(source.id,usesBuffers,buf);
//   if ( ALError("ALUREStream:Play:alSourceQueueBuffers") ) { OUTPUT( "ALUREStream:Play failed to queue buffers.\n" ); return false; }
//   alSourcePlay(source.id);
   alurePlaySourceStream(source->id,stream,usesBuffers,loops/*# of loops*/,(eos_callback),this); 
   source->state=playing;
 //  if ( ALError("ALUREStream:Play:alSourcePlay") ) { OUTPUT( "ALUREStream:Play failed to start source.\n" ); return false; }
 //  this.timer.delay = (1.0f/refreshRate);
 //  this.timer.Start();
//   Create();
  } else if ( source->state == paused || source->state==stopped ) { source->Resume(); return true; } 
    else if ( source->state == ALState::empty   ) { OUTPUT( "ALUREStream:Play called on an empty source (no buffer selected).\n" ); return false; }
    else if ( source->state == playing ) { OUTPUT( "ALUREStream:Play() called on a playing stream.\n" ); return true; }
  return true;
 }

 virtual void BetweenFrames() { }
 virtual void OnError() { }

 // Slightly inefficient, but precise.
 int CalculateSize( const char * fn ) {
  ALuint total = 0;
  ALuint b;
  ALint freq = 0;
  alureStream *s = alureCreateStreamFromFile( (ALchar *) fn, 19200, 1, &b);
  if(s) {
   do {
    ALint size, bits, channels;
    alGetBufferi(b, AL_SIZE, &size);
    alGetBufferi(b, AL_BITS, &bits);
    alGetBufferi(b, AL_CHANNELS, &channels);
    if(!freq) alGetBufferi(b, AL_FREQUENCY, &freq);
    total += size / channels * 8 / bits;
   } while(alureBufferDataFromStream(s, 1, &b) > 0);
   alureDestroyStream(s,1,&b);
  }
  duration=(float)total/(float)freq;
  samples=(unsigned int) total;
  return (int) samples;
 }
};

class ALUREStreams : public LinkedList {
public:
 CLEARLISTRESET(ALUREStream);
 ~ALUREStreams() { Clear(); }
};

class ALCaptureStream {
 ALSoundFormat format;
 int sampleFactor;
public:
 unsigned int frequency;
 unsigned int bufferSize;
 int samples;
 ALCint available;
 Zpointer<ALCdevice> captureDevice;
 string deviceName;
 bool capturing;
 unsigned short *target;
 int ofs;

 ALCaptureStream() {
  frequency=bufferSize=samples=0;
  capturing=false;
  target=null;
  ofs=0;
  format=stereo16; 
  sampleFactor=4;
  deviceName=string("");
 }

 void Format( ALSoundFormat value ) {
  switch ( value ) { 
   case mono8: sampleFactor=1; break;  
   case mono16: case stereo8: sampleFactor=2; break;
   case stereo16: sampleFactor=4; break;
  }
 }

 void Start() { if ( captureDevice ) { alcCaptureStart(captureDevice); capturing=true; } }

 unsigned short *Acquire() {
  if ( capturing ) {
   unsigned short *buffer;
   alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &available);
   buffer = new unsigned short[available*sampleFactor];
   memset(buffer,0,available);
   alcCaptureSamples(captureDevice,buffer,(ALuint) available);
   return buffer;
  } 
  return null;
 }

 void AcquireTo( unsigned short **buf ) {
  if ( capturing ) {
   if ( *buf ) delete *buf;
   alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &available);
   *buf = new unsigned short[available*sampleFactor];
   target = *buf;
   alcCaptureSamples(captureDevice,*buf,(ALuint) available);
  } else {
   delete *buf; *buf=null;
  }
 }

 void AcquireAppend( unsigned short *pre ) { 
  if ( capturing ) {
   unsigned short *post=&(pre[ofs]);
   alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &available);
   ofs+=available;
   alcCaptureSamples(captureDevice,post,(ALuint) available);
  }
 }

 void Stop() {
  if ( captureDevice ) { alcCaptureStop(captureDevice); capturing=false; OnCaptureComplete(); }
 }

 virtual void OnCaptureComplete() { }

 bool Init( const char *name ) {
  deviceName=string(name);
  captureDevice = alcCaptureOpenDevice( (ALCchar *) name, frequency, format, (ALCsizei) bufferSize );
  if ( !captureDevice ) {
   OUTPUT( "ALCaptureStream:Init:alcCaptureOpenDevice -> unable to open a device\n" );
   return false;
  }
  return true;
 }

 ~ALCaptureStream() {
  // needs destructor! this code is untested
 }
};


#ifndef		M_PI
#define		M_PI		3.14159265358979323846264338
#endif

enum SoundFileFormats { 	/* Major formats. */
 wav=SF_FORMAT_WAV			,		/* Microsoft WAV format (little endian default). */
 aiff=SF_FORMAT_AIFF		,		/* Apple/SGI AIFF format (big endian). */
 au=SF_FORMAT_AU			,		/* Sun/NeXT AU format (big endian). */
 raw=SF_FORMAT_RAW			,		/* RAW PCM data. */
 paf=SF_FORMAT_PAF			,		/* Ensoniq PARIS file format. */
 svx=SF_FORMAT_SVX			,		/* Amiga IFF / SVX8 / SV16 format. */
 nist=SF_FORMAT_NIST		,		/* Sphere NIST format. */
 voc=SF_FORMAT_VOC			,		/* VOC files. */
 ircam=SF_FORMAT_IRCAM	,		/* Berkeley/IRCAM/CARL */
 wav64riff=SF_FORMAT_W64	,		/* Sonic Foundry's 64 bit RIFF/WAV */
 matlab42=SF_FORMAT_MAT4	,		/* Matlab (tm) V4.2 / GNU Octave 2.0 */
 matlab50=SF_FORMAT_MAT5	,		/* Matlab (tm) V5.0 / GNU Octave 2.1 */
 pvf=SF_FORMAT_PVF			,		/* Portable Voice Format */
 xi=SF_FORMAT_XI			,		/* Fasttracker 2 Extended Instrument */
 htk=SF_FORMAT_HTK			,		/* HMM Tool Kit format */
 midisds=SF_FORMAT_SDS	,		/* Midi Sample Dump Standard */
 avr=SF_FORMAT_AVR			,		/* Audio Visual Research */
 waveformatex=SF_FORMAT_WAVEX,	/* MS WAVE with WAVEFORMATEX */
 sd2=SF_FORMAT_SD2			,		/* Sound Designer 2 */
 flac=SF_FORMAT_FLAC		,		/* FLAC lossless file format */
 caf=SF_FORMAT_CAF			,		/* Core Audio File format */
 wve=SF_FORMAT_WVE			,		/* Psion WVE format */
 ogg=SF_FORMAT_OGG			,		/* Xiph OGG container */
 mpc2d=SF_FORMAT_MPC2K	,		/* Akai MPC 2000 sampler */
 rf64wav=SF_FORMAT_RF64	,		/* RF64 WAV file */
};

/* OR'd Subtypes from here on. */
enum SoundFileFormatOptions {

 pcms8=SF_FORMAT_PCM_S8	,		/* Signed 8 bit data */
 pcm16=SF_FORMAT_PCM_16	,		/* Signed 16 bit data */
 pcm24=SF_FORMAT_PCM_24	,		/* Signed 24 bit data */
 pcm32=SF_FORMAT_PCM_32	,		/* Signed 32 bit data */
 pcmu8=SF_FORMAT_PCM_U8	,		/* Unsigned 8 bit data (WAV and RAW only) */
 float32bit=SF_FORMAT_FLOAT	,		/* 32 bit float data */
 float64bit=SF_FORMAT_DOUBLE,		/* 64 bit float data */
 ulaw=SF_FORMAT_ULAW		,		/* U-Law encoded. */
 alaw=SF_FORMAT_ALAW		,		/* A-Law encoded. */
 ima_adpcm=SF_FORMAT_IMA_ADPCM,/* IMA ADPCM. */
 ms_adpcm=SF_FORMAT_MS_ADPCM,	/* Microsoft ADPCM. */
 gsm610=SF_FORMAT_GSM610	,		/* GSM 6.10 encoding. */
 vox_adpcm=SF_FORMAT_VOX_ADPCM,/* OKI / Dialogix ADPCM */
 g721_adpcm_32kbs=SF_FORMAT_G721_32,		/* 32kbs G721 ADPCM encoding. */
 g723_adpcm_24kbs=SF_FORMAT_G723_24,		/* 24kbs G723 ADPCM encoding. */
 g723_adpcm_40kbs=SF_FORMAT_G723_40,		/* 40kbs G723 ADPCM encoding. */
 dwvw_12=SF_FORMAT_DWVW_12, 		/* 12 bit Delta Width Variable Word encoding. */
 dwvw_16=SF_FORMAT_DWVW_16, 		/* 16 bit Delta Width Variable Word encoding. */
 dwvw_24=SF_FORMAT_DWVW_24, 		/* 24 bit Delta Width Variable Word encoding. */
 dwvw_n =SF_FORMAT_DWVW_N , 		/* N bit Delta Width Variable Word encoding. */
 dpcm_8=SF_FORMAT_DPCM_8	 ,		/* 8 bit differential PCM (XI only) */
 dpcm_16=SF_FORMAT_DPCM_16,		/* 16 bit differential PCM (XI only) */
 vorbis=SF_FORMAT_VORBIS ,		/* Xiph Vorbis encoding. */

 /* Endian-ness options. */
 endian_default=SF_ENDIAN_FILE,	/* Default file endian-ness. */
 endian_little=SF_ENDIAN_LITTLE,	/* Force little endian-ness. */
 endian_big=SF_ENDIAN_BIG,	      /* Force big endian-ness. */
 endian_cpu=SF_ENDIAN_CPU,        /* Force CPU endian-ness. */
 sub=SF_FORMAT_SUBMASK		,
 type=SF_FORMAT_TYPEMASK		,
 endmask=SF_FORMAT_ENDMASK	
};

enum ChannelTypes {
 mono=1,
 stereo=2
};

class ALWaveForm : public ListItem {
public:
 SNDFILE *file;
 string filename;
 int formatOptions;
 int sampleRate;
 int sampleCount;
 int bytesize;
 sf_count_t total;
 ChannelTypes channels;
 std::vector<short> data;
 float duration, amplitude, leftFrequency, rightFrequency;
 ALBuffer buffer;
 SF_FORMAT_INFO info;
 SF_INFO sfinfo;
 ALWaveForm() : ListItem() {
  file=NULL;
  filename=string("");
  formatOptions=0;
  sampleRate = 44100;
  sampleCount = sampleRate; // 1 second
  bytesize=0;
  total=0;
  channels=stereo;
  duration=0.0f;
  amplitude = 	(1.0f * 0x7F000000);
  leftFrequency = 344.0f / (float) sampleRate;
  rightFrequency= 466.0f / (float) sampleRate;
  memset(&info,0,sizeof(info));
  memset (&sfinfo, 0, sizeof (sfinfo)) ;
  sfinfo.samplerate	= sampleRate ;
  sfinfo.frames		= sampleCount ;
  sfinfo.channels		= (int) channels ;
  sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24) ;
 }
 ~ALWaveForm() {
  total_memory_for_audio -= (unsigned int) total;
 }
 void SetDuration( float value )        {
  sampleCount = (int) (sampleRate * value);
  duration=value;
  sfinfo.frames=(int) sampleCount;
 }
 void SetSampleRate( int value )        {
  sampleRate=value;
  sfinfo.samplerate=value;
  sampleCount = (int) (value * duration);
  sfinfo.frames=(int) sampleCount;
 }
 void SetChannels( ChannelTypes value ) {
  channels=value;
  sfinfo.channels=(int) value;
 }
 /* For Generative Sampling */
 virtual bool Process( ALWaveForm w )   { return false; }
 virtual int   EquationMono( int k )            { return (int) (amplitude * (float) sin(leftFrequency * 2 * k * M_PI)); }
 virtual int EquationStereo( int k, bool left ) { return (int) (amplitude * (float) sin( (left ? leftFrequency : rightFrequency) * 2 * k * M_PI));  }
 void Render() { 
  int k;
  data.clear();
  data.resize(sampleCount*(int) channels);
  sfinfo.samplerate	= sampleRate ;
  sfinfo.frames		= sampleCount ;
  sfinfo.channels		= (int) channels ;
  if ( channels == mono ) for ( k=0; k<sampleCount; k++ ) data[k] = (short) EquationMono(k);
  else for ( k=0; k<sampleCount; k++ ) { data[2*k] = (short) EquationStereo(k,true); data[2*k+1] = (short) EquationStereo(k,false); }
 }
 /* For Generative Sampling (Above) */
 bool Load( const char *path, const char * fn );
 void setFormat( SoundFileFormats f )              {
  formatOptions =  (int) f;
  sfinfo.format = formatOptions;
 }
 void addFormatOption ( SoundFileFormatOptions o ) {
  formatOptions |= (int) o;
  sfinfo.format = formatOptions;
 }
 void printInfo() {
  char buffer [128] ;
  int format, major_count, subtype_count, m, s;
  memset (&sfinfo, 0, sizeof (sfinfo));
  buffer [0] = 0;
  sf_command (NULL, SFC_GET_LIB_VERSION, &data[0], data.size());
  if (strlen (buffer) < 1)	{ OUTPUT ("Line %d: could not retrieve lib version.\n", __LINE__); exit (1); }
  OUTPUT ("libSndFile version %s\n\n", buffer);
  sf_command (NULL, SFC_GET_FORMAT_MAJOR_COUNT, &major_count, sizeof (int));
  sf_command (NULL, SFC_GET_FORMAT_SUBTYPE_COUNT, &subtype_count, sizeof (int));
  sfinfo.channels = 1;
  for (m = 0 ; m < major_count ; m++) {
   info.format = m;
   sf_command (NULL, SFC_GET_FORMAT_MAJOR, &info, sizeof (info));
   OUTPUT ("%s  (extension \"%s\")\n", info.name, info.extension);
   format = info.format;
   for (s = 0 ; s < subtype_count ; s++) {
    info.format = s;
    sf_command (NULL, SFC_GET_FORMAT_SUBTYPE, &info, sizeof (info));
    format = (format & SF_FORMAT_TYPEMASK) | info.format;
    sfinfo.format = format;
    if (sf_format_check (&sfinfo)) OUTPUT ("   %s\n", info.name);
   } 
   puts ("");
  }
  puts ("");
 }
 bool OpenFile( const char * fn ) {
  memset (&sfinfo, 0, sizeof (sfinfo));
  return (file = sf_open (fn, SFM_READ, &sfinfo)) != NULL;
 }
 bool Save( const char * fn ) {
  if ( file ) OUTPUT( "ALWaveForm: file pointer was already in use when Save(\"%s\") was called, this may cause file handles to be left open without a reference.  Do not do this.\n", fn );
  if (! (file = sf_open (fn, SFM_WRITE, &sfinfo))) {
   OUTPUT ("ALWaveForm: Write Error - Not able to open output file.\n") ;
   return false;
  }
  if (sf_writef_short (file, &data[0], sampleCount) !=	sampleCount) puts(sf_strerror (file)) ;
  sf_close (file);
  file=NULL;
 }
 void Close() { if ( file ) sf_close(file); file=NULL; }
};

class ALWaveForms : public LinkedList {
public:
 ALWaveForm *find( const char *fn ) { 
  FOREACH(ALWaveForm,s) if ( contains(fn,s->filename.c_str()) ) return s; 
  return null; 
 }
 ALWaveForm *findOrLoad( const char *path, const char *fn ) { 
  ALWaveForm *w=find(fn); 
  if ( !w ) {
   w=new ALWaveForm;
   w->Load(path,fn);
   Append(w);
  } 
  return w; 
 }
 void DeleteAll() { Clear(); }
 CLEARLISTRESET(ALWaveForm);
 ~ALWaveForms() { Clear(); }
};

class ALSound : public ListItem {
public:
 ALWaveForm *waveform;
 string name;
 ALuint id;
 float remaining,duration;
 bool deleteMe;
 ALenum format;
 ALvoid *data;
 ALsizei size,freq;
 ALboolean loop, relative;
 ALSource *source; // sources are points of emitting sound.
 ALSound **pointer;
 SoundVector position, velocity, direction;
 int loops;
 float pitch,gain,min_gain,max_gain,distance,rolloff;
 bool boundToSource;
 bool played;
 bool lastFramePlayState;
 bool Restart;

 bool *ThrowASwitch;

 ALSound() : ListItem() {
  waveform=null;
  name=string("");
  id=0;
  remaining=duration=0.0f;
  deleteMe=false;
  format=0;
  data=null;
  size=freq=0;
  loop=relative=false;
  source=null;
  pitch=1.0f;
  gain=1.0f;
  min_gain=0.0f;
  max_gain=1.0f;
  distance=0.0f;
  rolloff=1.0f;
  boundToSource=false;
  lastFramePlayState=false;
  Restart=false;
  ThrowASwitch=null;
  played=false;
  pointer=null;
  loops=0; // continuous
 }

 void Release() { alSourceStop( id ); }

 int NextAvailableID( int max ) {
  ALSound *s;
  int i;
  Indexed<Zbool> notavailable;
  notavailable.Size(max);
  for ( s=this; s; s=(ALSound *) s->next ) notavailable[s->id-1]=true;
  for ( i=0; i<max; i++ ) if ( notavailable[i] == false ) { return i+1; }
  return -1;
 }

 bool Play( )
 {
  int sourceAudioState = 0;
  played=true;
  remaining=duration;
  WhenPlayed();
  if ( !source ) { OUTPUT( "Audio %s had no source.\n", waveform->filename ); return false; }
  alGetError();
  alGetSourcei( source->id, AL_SOURCE_STATE, &sourceAudioState );
  if ( sourceAudioState == AL_PLAYING ) { 
   if ( Restart ) Stop(); else return false;
  }
  alSourcePlay( source->id );
  if ( ALError( "ALSound:Play:alSourcePlay") ) return false;
  lastFramePlayState=true;
  source->state=playing;
  alGetError();
  return true;
 }

 bool Pause( ) {
  WhenPaused();
  alGetError();
  alSourcePause( source->id );
  if ( ALError( "ALSound:Pause:alSourcePause") ) return false;
  lastFramePlayState=false;
  source->state=paused;
  alGetError();
  return true;
 }

 bool Playing() {
  int sourceAudioState = 0;
  if ( !source ) return false;
  alGetSourcei( source->id, AL_SOURCE_STATE, &sourceAudioState );
  if ( sourceAudioState == AL_PLAYING ) source->state=playing;
  if ( sourceAudioState == AL_PAUSED ) source->state=paused;
  return ( source->state == playing );
 }

 // If the sound was paused the sound will resume, else it will play from the beginning.
 bool Resume( )
 {
  int sourceAudioState = 0;
  WhenResumed();
  alGetError();
  alGetSourcei( source->id, AL_SOURCE_STATE, &sourceAudioState );
  if ( sourceAudioState == AL_PLAYING ) return true;
  if ( sourceAudioState == AL_PAUSED ) { alSourcePlay( source->id ); if ( ALError( "Resume:alSourcePlay") ) return false; }
  if ( ALError( "ALSound:Resume:alSourcePlay") ) return false;
  lastFramePlayState=true;
  source->state=playing;
  alGetError();
  return true;
 }

 // Make sure the audio source ident is valid and usable
 bool Stop( )
 {
  alGetError();
  alSourceStop( source->id );
  if ( ALError( "ALSound:Stop:alSourceStop") ) return false;
  lastFramePlayState=false;
  source->state=stopped;
  alGetError();
  return true;
 }

 bool Position( )
 {
  alGetError();
  alSourcefv(source->id, AL_POSITION, position.asFloat3() ); if ( ALError( "ALSound:position:alSourcefv:AL_POSITION" ) ) return false;
  return true;
 }

 bool SetPVD( )
 {
  if ( !Playing() ) return true;
  alGetError();
  alSourcefv(source->id, AL_POSITION,  position.asFloat3()  ); if ( ALError( "ALSound:SetPVD:alSourcefv:AL_POSITION" ) ) return false;
  alSourcefv(source->id, AL_VELOCITY,  velocity.asFloat3()  ); if ( ALError( "ALSound:SetPVD:alSourcefv:AL_VELOCITY" ) ) return false;
  alSourcefv(source->id, AL_DIRECTION, direction.asFloat3() ); if ( ALError( "ALSound:SetPVD:alSourcefv:AL_DIRECTION" ) ) return false;
  alGetError();
  return true;
 }

 bool SetSound( float maxDistance, float minGain, float maxGain, float rollOff )
 { 
  alGetError();
  alSourcef(source->id, AL_MAX_DISTANCE,   distance=maxDistance ); if ( ALError( "ALSound:SetSound:alSourcef:AL_MAX_DISTANCE"   ) ) return false;
  alSourcef(source->id, AL_MIN_GAIN,       min_gain=minGain     ); if ( ALError( "ALSound:SetSound:alSourcef:AL_MIN_GAIN"       ) ) return false;
  alSourcef(source->id, AL_MAX_GAIN,       max_gain=maxGain     ); if ( ALError( "ALSound:SetSound:alSourcef:AL_MAX_GAIN"       ) ) return false;
  alSourcef(source->id, AL_ROLLOFF_FACTOR, rolloff =rollOff     ); if ( ALError( "ALSound:SetSound:alSourcef:AL_ROLLOFF_FACTOR" ) ) return false;
  return true;
 }

 ALSource *UpdateSource()
 {
  alGetError();
//  alSourcei (source.id, AL_BUFFER,   buffer.id );
  alSourcef (source->id, AL_PITCH,    pitch  );
  alSourcef (source->id, AL_GAIN,     gain   );
  alSourcef (source->id, AL_MAX_DISTANCE,   distance );        if ( ALError( "ALSound:AddSource:AL_MAX_DISTANCE"   ) ) { delete source; return null; }
  alSourcef (source->id, AL_MIN_GAIN,       min_gain );        if ( ALError( "ALSound:AddSource:AL_MIN_GAIN"       ) ) { delete source; return null; }
  alSourcef (source->id, AL_MAX_GAIN,       max_gain );        if ( ALError( "ALSound:AddSource:AL_MAX_GAIN"       ) ) { delete source; return null; }
  alSourcef (source->id, AL_ROLLOFF_FACTOR, rolloff  );        if ( ALError( "ALSound:AddSource:AL_ROLLOFF_FACTOR" ) ) { delete source; return null; }
  alSourcefv(source->id, AL_POSITION,  position.asFloat3()  ); if ( ALError( "ALSound:AddSource:AL_POSITION"       ) ) { delete source; return null; }
  alSourcefv(source->id, AL_VELOCITY,  velocity.asFloat3()  ); if ( ALError( "ALSound:AddSource:AL_VELOCITY"       ) ) { delete source; return null; }
  alSourcefv(source->id, AL_DIRECTION, direction.asFloat3() ); if ( ALError( "ALSound:AddSource:AL_DIRECTION"      ) ) { delete source; return null; }
  alSourcei (source->id, AL_LOOPING,  (ALboolean) loop );
  alSourcePlay(source->id);
  return source;
 }

 void AssignSource( ALSource *s ) {
  alGetError();
  if ( source ) { OUTPUT( "ALSound:AssignSource:Error! Attempted to assign a source when there already was one.\n" ); return; }
  source=s;
  alSourcei (source->id, AL_BUFFER,   waveform->buffer.id );
  UpdateSource();
 }

 ALSource *AddSource()
 {
  if ( !source ) {
   ALSource *s=new ALSource;
   alGetError();
   alGenSources(1, &(s->id));
   if (alGetError() != AL_NO_ERROR) { OUTPUT("ALSound:AddSource(): Error generating audio source."); delete s; return null; }
   alSourcei (s->id, AL_BUFFER,         waveform->buffer.id );
   alSourcef (s->id, AL_PITCH,          pitch  );
   alSourcef (s->id, AL_GAIN,           gain   );
   alSourcef (s->id, AL_MAX_DISTANCE,   distance );        if ( ALError( "ALSound:AddSource:AL_MAX_DISTANCE"   ) ) { delete s; return null; }
   alSourcef (s->id, AL_MIN_GAIN,       min_gain );        if ( ALError( "ALSound:AddSource:AL_MIN_GAIN"       ) ) { delete s; return null; }
   alSourcef (s->id, AL_MAX_GAIN,       max_gain );        if ( ALError( "ALSound:AddSource:AL_MAX_GAIN"       ) ) { delete s; return null; }
   alSourcef (s->id, AL_ROLLOFF_FACTOR, rolloff  );        if ( ALError( "ALSound:AddSource:AL_ROLLOFF_FACTOR" ) ) { delete s; return null; }
   alSourcefv(s->id, AL_POSITION,  position.asFloat3()  ); if ( ALError( "ALSound:AddSource:AL_POSITION"       ) ) { delete s; return null; }
   alSourcefv(s->id, AL_VELOCITY,  velocity.asFloat3()  ); if ( ALError( "ALSound:AddSource:AL_VELOCITY"       ) ) { delete s; return null; }
   alSourcefv(s->id, AL_DIRECTION, direction.asFloat3() ); if ( ALError( "ALSound:AddSource:AL_DIRECTION"      ) ) { delete s; return null; }
   alSourcei (s->id, AL_LOOPING,  (ALboolean) loop );
   alSourcePlay(s->id); 
   this->source=s;
   return s;
  } 
  OUTPUT( "ALSound:AddSource() - source Already Created; File: %s\n", waveform->filename );
  return this->source;
 }

 void PrintInfo( ) {
 }

 virtual void WhenPlayed   ( ){ }
 virtual void WhenPaused   ( ){ } 
 virtual void WhenResumed  ( ){ }
 virtual void WhenComplete ( ){ if ( ThrowASwitch ) *ThrowASwitch=true; } 
 virtual void BetweenFrames( ) { }
 void _BetweenFrames() { 
  int state;
  alGetSourcei(source->id, AL_SOURCE_STATE, &state);
  bool playing;
  if ( loop ) {
   playing = (state == AL_PLAYING);
   remaining-=(float) (1.0f/FPS);
   if ( loops != 0 && remaining <= 0.0f ) {
    remaining=duration;
    loops--;
    if ( loops == 0 ) {
     WhenComplete();
     this->Release();
     this->deleteMe=true; //OUTPUT( "%s sound cleared for deletion.\n", name );
    }
   }
  } else {
   remaining-=(float) (1.0f/FPS);
   playing = (state == AL_PLAYING && (remaining >= 0.0f));
   if ( !playing && played ) {
    WhenComplete();
    this->Release();
    this->deleteMe=true; //OUTPUT( "%s sound cleared for deletion.\n", name );
   }
  }
  if ( playing ) BetweenFrames();
  lastFramePlayState=playing;
 }

 ~ALSound() {
//  OUTPUT( "Deleting sound %s\n", name );
  if ( Playing() ) Stop();
  if ( source ) delete source;
  if ( pointer ) *pointer=null;
 }
};

class ALSounds : public LinkedList
{
public:
 bool inList( const char *fn ) {
  ALSound *s; 
  for ( s=(ALSound *)first; s; s=(ALSound *)(s->next) ) if ( contains(fn,s->name.c_str()) ) return true; 
  return false; 
 }
 ALSound *findInList( const char *fn ) { 
  ALSound *s; 
  for ( s=(ALSound *)first; s; s=(ALSound *)(s->next) ) if ( contains(fn,s->name.c_str()) ) return s; 
  return null;
 }
 void DeleteDone() { 
  ALSound *s,*n;
  for ( s=(ALSound *)first; s; s=n ) {
   n=(ALSound *) s->next;
   if ( s->deleteMe ) {
    Remove(s);
    delete s;
   }
  }
 }
 void DeleteAll() { 
  ALSound *s,*n;
  for ( s=(ALSound *)first; s; s=n ) { n=(ALSound *) s->next; delete s; }
  first=last=null; count=0;
 }
 CLEARLISTRESET(ALSound);
};

class ALCDeviceContext : public ListItem {
public:
 ALCcontext* context;
 ALCdevice* device;
 char *deviceName;
 bool closed;

 ALCDeviceContext() {
  context=null;
  device=null;
  deviceName=null;
  closed=false;
 }

 void PickDefault() {
  device=alcOpenDevice(null);
  context=alcCreateContext(device,null);
  alcMakeContextCurrent(context);
  closed=false;
 }
 
 void Init( const char *context, void *variables ) {
  device = alcOpenDevice(context);
  this->context = alcCreateContext(device, (const ALCint *) variables);
  alcMakeContextCurrent(this->context);
  closed=false;
 }

 void Close() {
  context = alcGetCurrentContext();
  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(null);
  alcDestroyContext(context);
  alcCloseDevice(device);
  closed=true;
 }

 void DeviceInfo( ) {
  OUTPUT( "Sound devices available:\n" ); 
  if ( alcIsExtensionPresent( null, "ALC_ENUMERATION_EXT" ) == AL_TRUE ) {
   OUTPUT( "ALC_DEVICE_SPECIFIER: %s\n", (char*) alcGetString( device, ALC_DEVICE_SPECIFIER ) );
   OUTPUT( "ALC_DEFAULT_DEVICE_SPECIFIER: %s\n", (char*) alcGetString( null, ALC_DEFAULT_DEVICE_SPECIFIER ) );
   OUTPUT( "ALC_EXTENSIONS: %s\n", (char*) alcGetString( null, ALC_EXTENSIONS ) );
  }
  else OUTPUT( " ... enumeration error.\n" );
 }

 ~ALCDeviceContext() { if (!closed) Close(); }
};

class ALCDeviceContexts : public LinkedList 
{
public:
 CLEARLISTRESET(ALCDeviceContext);
 void DeleteAll() { Clear(); }
 ~ALCDeviceContexts() { Clear(); }
};

#if defined(USE_EFX)
enum EFXType {
 none=0,
 reverb=1,
 chorus=2,
 distortion=3,
 echo=4,
 flanger=5,
 frequencyShifter=6,
 vocalMorpher=7,
 pitchShifter=8,
 ringModulator=9,
 autoWah=10,
 compressor=11,
 equalizer=12 
};

enum EFXWaveForm { sinusoid=0, triangle=1 };
enum EFXDirection { down=0, up=1, off=2 };
enum EFXPhoneme { A=0, E=1, I=2, O=3, U=4, AA=5, AE=6, AH=7, AO=8, EH=9, ER=10, IH=11, IY=12, UH=13, AW=14, 
                  B=15, D=16, F=17, G=18, J=19, K=20, L=21, M=22, N=23, P=24, R=25, S=26, T=27, V=28, Z=29 };
enum EFXWaveForm3a { sinusoid=0, triangle=1, sawtooth=2 };
enum EFXWaveForm3b { sinusoid=0, sawtooth=1, square=2   };

class EFXAuxSlot : public ListItem {
public:
 EFX efx;
 ALuint id;
 bool Create() {
  alGetError();
  efx.alGenAuxiliaryEffectSlots(1,&id);
  if ( ALError("EFXAuxSlot:Create:alGenAuxiliaryEffectSlots") ) return false;
  return true;
 }
 bool Destroy() {
  alGetError();
  efx.alDeleteAuxiliaryEffectSlots(1,&id);
  if ( ALError("EFXAuxSlot:Destroy:alDeleteAuxiliaryEffectSlots") ) return false;
  return true;
 }
};

class EFXAuxSlots : public LinkedList
{
  ~EFXAuxSlots() {
  EFXAuxSlot *d,*n;
  for ( d=(EFXAuxSlot *) first; d; d=n ) {
   n=(EFXAuxSlot *) (d->next);
   delete d;
  } 
 }
};

class EFXEffect : public ListItem {
public:
 EFX efx;
 EFXAuxSlot *aux;
 unsigned int id;
 unsigned int type;
 unsigned int firstParameter,lastParameter;
 float directFilter, 
       auxSendFilter, 
       airAbsorption, 
       roomRolloff, 
       coneOuterGainHF, 
       directFilterGainHF, 
       auxSendFilterGainAuto, 
       auxSendFilterGainHFAuto;
 EFXEffect() {
  aux=null;
  id=0;
  type=0;
  firstParameter=lastParameter=0;
  directFilter=auxSendFilter=airAbsorption=roomRolloff=coneOuterGainHF=directFilterGainHF=auxSendFilterGainAuto=auxSendFilterGainHFAuto=0.0f;
 }
 bool Create() {
  alGetError();
  if ( aux ) if ( !aux->Destroy() ) { return false; }
  if ( aux ) delete aux; aux = new EFXAuxSlot; if ( !aux->Create() ) { return false; }
  efx.alGenEffects(1,&id);
  if ( ALError("EFXEffect:Create:alGenEffects") )  return false;
  efx.alEffecti(id,AL_EFFECT_TYPE,type);
  if ( ALError("EFXEffect:Create:alEffecti") ) { efx.alDeleteEffects(1,&id); return false; }
  efx.alAuxiliaryEffectSloti(aux->id, AL_EFFECTSLOT_EFFECT, id);
 }
 bool Destroy() {
  alGetError();
  efx.alAuxiliaryEffectSloti(aux->id, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
  efx.alDeleteEffects(1,&id);
  if ( ALError("EFXEffect:Destroy:alDeleteEffects") )  return false;
 }
 void ApplySharedEffectParameters() {
 }
};

enum EAXReverbPresets { hanger=2,bathroom=1 };

class EAXReverb : EFXEffect {
 float environmentSize, 
       environmentDiffusion,
       density,
       diffusion,
       gain,
       gainHF,
       gainLF,
       decayTime,
       decayHFRatio,
       decayLFRatio,
       reflectionsGain,
       reflectionsDelay,
       lateGain,
       lateDelay,
       reverbDelay,
       echoTime,
       echoDepth,
       modulationTime,
       modulationDepth,
       airAbsorptionGainHF,
       referenceHF,
       referenceLF,
       roomRolloffFactor;
 SoundVector reflectionsPan, latePan;
 int room, roomHF, roomLF, reverb, flags, reflections, environment;
 bool decayHFLimit;
 EAXReverb() { // Defaulted to bathroom effect
  environmentSize=environmentDiffusion=density=diffusion=gain=gainHF=gainLF=decayTime=decayHFRatio=decayLFRatio=reflectionsGain=reflectionsDelay=
  lateGain=lateDelay=reverbDelay=echoTime=echoDepth=modulationTime=modulationDepth=airAbsorptionGainHF=referenceHF=referenceLF=roomRolloffFactor=0;
  room=roomHF=roomLF=reverb=flags=reflections=environment=0;
  decayHFLimit=false;
 }
 bool Apply() {
  alGetError();
  efx.alEffectf(id, AL_EAXREVERB_DENSITY, density);
  efx.alEffectf(id, AL_EAXREVERB_DIFFUSION, diffusion);
  efx.alEffectf(id, AL_EAXREVERB_GAIN, gain);
  efx.alEffectf(id, AL_EAXREVERB_GAINHF, gainHF);
  efx.alEffectf(id, AL_EAXREVERB_GAINLF, gainLF);
  efx.alEffectf(id, AL_EAXREVERB_DECAY_TIME, decayTime);
  efx.alEffectf(id, AL_EAXREVERB_DECAY_HFRATIO, decayHFRatio);
  efx.alEffectf(id, AL_EAXREVERB_DECAY_LFRATIO, decayLFRatio);
  efx.alEffectf(id, AL_EAXREVERB_REFLECTIONS_GAIN, reflectionsGain);
  efx.alEffectf(id, AL_EAXREVERB_REFLECTIONS_DELAY, reflectionsDelay);
  efx.alEffectfv(id, AL_EAXREVERB_REFLECTIONS_PAN, reflectionsPan.asFloat3());
  efx.alEffectf(id, AL_EAXREVERB_LATE_REVERB_GAIN, lateGain);
  efx.alEffectf(id, AL_EAXREVERB_LATE_REVERB_DELAY, lateDelay);
  efx.alEffectfv(id, AL_EAXREVERB_LATE_REVERB_PAN, latePan.asFloat3());
  efx.alEffectf(id, AL_EAXREVERB_ECHO_TIME, echoTime);
  efx.alEffectf(id, AL_EAXREVERB_ECHO_DEPTH, echoDepth);
  efx.alEffectf(id, AL_EAXREVERB_MODULATION_TIME, modulationTime);
  efx.alEffectf(id, AL_EAXREVERB_MODULATION_DEPTH, modulationDepth);
  efx.alEffectf(id, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, airAbsorptionGainHF);
  efx.alEffectf(id, AL_EAXREVERB_HFREFERENCE, referenceHF);
  efx.alEffectf(id, AL_EAXREVERB_LFREFERENCE, referenceLF);
  efx.alEffectf(id, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, roomRolloffFactor);
  efx.alEffecti(id, AL_EAXREVERB_DECAY_HFLIMIT, (ALboolean) decayHFLimit);
  if ( ALError("EAXReverb:Apply") ) return false; 
  return true;
 }
 void Preset( EAXReverbPresets preset ) {
  switch ( preset ) {
         default:
   case bathroom:
    environment=3; environmentSize=1.4f; environmentDiffusion=1.000f;
    room=-1000; roomHF=-1200; roomLF=0;
    decayTime=1.49f; decayHFRatio=0.54f; decayLFRatio=1.00f;
    reflections=-370; reflectionsDelay=0.007f; reflectionsPan.x=0.0f; reflectionsPan.y=0.0f; reflectionsPan.z=0.0f;
    reverb=1030; reverbDelay=0.011f; latePan.x=0.00f; latePan.y=0.00f; latePan.z=0.00f;
    echoTime=0.250f; echoDepth=0.000f;
    modulationTime=0.250f; modulationDepth=0.000f;
    airAbsorptionGainHF=-5.0f;
    referenceHF=5000.0f; referenceLF=250.0f;
    roomRolloffFactor=0.00f;
    flags=0x3f;
   break;
  }
 }
};

class EFXReverb : public EFXEffect {
 float density, 
       diffusion, 
       gain, 
       gainHF, 
       decayTime, 
       decayHFRatio, 
       reflectionsGain, 
       reflectionsDelay, 
       lateGain, 
       lateDelay, 
       airAbsorptionGainHF, 
       roomRolloffFactor;
  bool decayHFLimit;
public:
 EFXReverb() { 
  density             = AL_REVERB_DEFAULT_DENSITY; 
  diffusion           = AL_REVERB_DEFAULT_DIFFUSION;
  gain                = AL_REVERB_DEFAULT_GAIN;
  gainHF              = AL_REVERB_DEFAULT_GAINHF;
  decayTime           = AL_REVERB_DEFAULT_DECAY_TIME;
  decayHFRatio        = AL_REVERB_DEFAULT_DECAY_HFRATIO;
  reflectionsGain     = AL_REVERB_DEFAULT_REFLECTIONS_GAIN;
  reflectionsDelay    = AL_REVERB_DEFAULT_REFLECTIONS_DELAY;
  lateGain            = AL_REVERB_DEFAULT_LATE_REVERB_GAIN;
  lateDelay           = AL_REVERB_DEFAULT_LATE_REVERB_DELAY;
  airAbsorptionGainHF = AL_REVERB_DEFAULT_AIR_ABSORPTION_GAINHF;
  roomRolloffFactor   = AL_REVERB_DEFAULT_ROOM_ROLLOFF_FACTOR;
  decayHFLimit        = AL_REVERB_DEFAULT_DECAY_HFLIMIT;
 }
public:
 void Density( float value )             { density             =RANGE(AL_REVERB_MIN_DENSITY,               value, AL_REVERB_MAX_DENSITY);               }
 void Diffusion( float value )           { diffusion           =RANGE(AL_REVERB_MIN_DIFFUSION,             value, AL_REVERB_MAX_DIFFUSION);             }
 void Gain( float value )                { gain                =RANGE(AL_REVERB_MIN_GAIN,                  value, AL_REVERB_MAX_GAIN);                  } 
 void GainHF( float value )              { gainHF              =RANGE(AL_REVERB_MIN_GAINHF,                value, AL_REVERB_MAX_GAINHF);                } 
 void DecayTime( float value )           { decayTime           =RANGE(AL_REVERB_MIN_DECAY_TIME,            value, AL_REVERB_MAX_DECAY_TIME);            } 
 void DecayHFRatio( float value )        { decayHFRatio        =RANGE(AL_REVERB_MIN_DECAY_HFRATIO,         value, AL_REVERB_MAX_DECAY_HFRATIO);         } 
 void ReflectionGain( float value )      { reflectionsGain     =RANGE(AL_REVERB_MIN_REFLECTIONS_GAIN,      value, AL_REVERB_MAX_REFLECTIONS_GAIN);      } 
 void ReflectionsDelay( float value )    { reflectionsDelay    =RANGE(AL_REVERB_MIN_LATE_REVERB_DELAY,     value, AL_REVERB_MAX_REFLECTIONS_DELAY);     } 
 void LateGain( float value )            { lateGain            =RANGE(AL_REVERB_MIN_LATE_REVERB_GAIN,      value, AL_REVERB_MAX_LATE_REVERB_GAIN);      } 
 void LateDelay( float value )           { lateDelay           =RANGE(AL_REVERB_MIN_LATE_REVERB_DELAY,     value, AL_REVERB_MAX_LATE_REVERB_DELAY);     } 
 void AirAbsorptionGainHF( float value ) { airAbsorptionGainHF =RANGE(AL_REVERB_MIN_AIR_ABSORPTION_GAINHF, value, AL_REVERB_MAX_AIR_ABSORPTION_GAINHF); } 
 void RoomRolloffFactor( float value )   { roomRolloffFactor   =RANGE(AL_REVERB_MIN_ROOM_ROLLOFF_FACTOR,   value, AL_REVERB_MAX_ROOM_ROLLOFF_FACTOR);   } 
 void DecayHFLimit( bool value )         { decayHFLimit        =RANGE(AL_REVERB_MIN_DECAY_HFLIMIT,         value, AL_REVERB_MAX_DECAY_HFLIMIT);         } 
};

class EFXChorus : public EFXEffect {
 EFXWaveForm waveform;
 int phase;
 float rate, depth, feedback, delay;
 EFXChorus() { 
  waveform = (EFXWaveForm) AL_CHORUS_DEFAULT_WAVEFORM;
  phase               = AL_CHORUS_DEFAULT_PHASE; 
  rate                = AL_CHORUS_DEFAULT_RATE;
  depth               = AL_CHORUS_DEFAULT_DEPTH;
  feedback            = AL_CHORUS_DEFAULT_FEEDBACK;
  delay               = AL_CHORUS_DEFAULT_DELAY;
 }
public:
 void Phase( float value )    { phase     =RANGE(AL_CHORUS_MIN_PHASE,    value, AL_CHORUS_MAX_PHASE);     }
 void Rate( float value )     { rate      =RANGE(AL_CHORUS_MIN_RATE,     value, AL_CHORUS_MAX_RATE);      } 
 void Depth( float value )    { depth     =RANGE(AL_CHORUS_MIN_DEPTH,    value, AL_CHORUS_MAX_DEPTH);     } 
 void Feedback( float value ) { feedback  =RANGE(AL_CHORUS_MIN_FEEDBACK, value, AL_CHORUS_MAX_FEEDBACK);  } 
 void Delay( float value )    { delay     =RANGE(AL_CHORUS_MIN_DELAY,    value, AL_CHORUS_MAX_DELAY);     } 
};

class EFXDistortion : public EFXEffect {
 float edge, gain, cutoff, eqCenter, eqBandwidth;
public:
 EFXDistortion() {
  edge         = AL_DISTORTION_DEFAULT_EDGE; 
  gain         = AL_DISTORTION_DEFAULT_GAIN;
  cutoff       = AL_DISTORTION_DEFAULT_LOWPASS_CUTOFF;
  eqCenter     = AL_DISTORTION_DEFAULT_EQCENTER;
  eqBandwidth  = AL_DISTORTION_DEFAULT_EQBANDWIDTH;
 }
 void Edge( float value )      { edge        =RANGE(AL_DISTORTION_MIN_EDGE,           value, AL_DISTORTION_MAX_EDGE);           }
 void Gain( float value )      { gain        =RANGE(AL_DISTORTION_MIN_GAIN,           value, AL_DISTORTION_MAX_GAIN);           }
 void CutOff( float value )    { cutoff      =RANGE(AL_DISTORTION_MIN_LOWPASS_CUTOFF, value, AL_DISTORTION_MAX_LOWPASS_CUTOFF); }
 void Center( float value )    { eqCenter    =RANGE(AL_DISTORTION_MIN_EQCENTER,       value, AL_DISTORTION_MAX_EQCENTER);       }
 void Bandwidth( float value ) { eqBandwidth =RANGE(AL_DISTORTION_MIN_EQBANDWIDTH,    value, AL_DISTORTION_MAX_EQBANDWIDTH);    }
};

class EFXEcho : public EFXEffect {
 float delay, delayLR, damping, feedback, spread;
public:
 EFXEcho() {
  delay     = AL_ECHO_DEFAULT_DELAY; 
  delayLR   = AL_ECHO_DEFAULT_LRDELAY;
  damping   = AL_ECHO_DEFAULT_DAMPING;
  feedback  = AL_ECHO_DEFAULT_FEEDBACK;
  spread    = AL_ECHO_DEFAULT_SPREAD;
 }
 void Delay( float value )    { delay    =RANGE(AL_ECHO_MIN_DELAY,    value, AL_ECHO_MAX_DELAY);    }
 void DelayLR( float value )  { delayLR  =RANGE(AL_ECHO_MIN_LRDELAY,  value, AL_ECHO_MAX_LRDELAY);  }
 void Damping( float value )  { damping  =RANGE(AL_ECHO_MIN_DAMPING,  value, AL_ECHO_MAX_DAMPING);  }
 void Feedback( float value ) { feedback =RANGE(AL_ECHO_MIN_FEEDBACK, value, AL_ECHO_MAX_FEEDBACK); }
 void Spread( float value )   { spread   =RANGE(AL_ECHO_MIN_SPREAD,   value, AL_ECHO_MAX_SPREAD);   }
};

class EFXFlanger : public EFXEffect {
 EFXWaveForm waveform;
 int phase;
 float rate, depth, feedback, delay;
 EFXFlanger() {
  waveform = (EFXWaveForm) AL_FLANGER_DEFAULT_WAVEFORM;
  phase    = AL_FLANGER_DEFAULT_PHASE; 
  rate     = AL_FLANGER_DEFAULT_RATE;
  depth    = AL_FLANGER_DEFAULT_DEPTH;
  feedback = AL_FLANGER_DEFAULT_FEEDBACK;
  delay    = AL_FLANGER_DEFAULT_DELAY;
 }
public:
 void Phase( float value )    { phase    =RANGE(AL_FLANGER_MIN_PHASE,    value, AL_FLANGER_MAX_PHASE);    }
 void Rate( float value )     { rate     =RANGE(AL_FLANGER_MIN_RATE,     value, AL_FLANGER_MAX_RATE);     }
 void Depth( float value )    { depth    =RANGE(AL_FLANGER_MIN_DEPTH,    value, AL_FLANGER_MAX_DEPTH);    }
 void Feedback( float value ) { feedback =RANGE(AL_FLANGER_MIN_FEEDBACK, value, AL_FLANGER_MAX_FEEDBACK); } 
 void Delay( float value )    { delay    =RANGE(AL_FLANGER_MIN_DELAY,    value, AL_FLANGER_MAX_DELAY);    }
};

class EFXFrequencyShift : public EFXEffect {
 float freq;
 EFXDirection L,R;
 EFXFrequencyShift() {
  freq = AL_FREQUENCY_SHIFTER_DEFAULT_FREQUENCY; 
  L    = (EFXDirection) AL_FREQUENCY_SHIFTER_DEFAULT_LEFT_DIRECTION;
  R    = (EFXDirection) AL_FREQUENCY_SHIFTER_DEFAULT_RIGHT_DIRECTION;
 }
public:
 void Freq( float value ) { freq      =RANGE(AL_FREQUENCY_SHIFTER_MIN_FREQUENCY,    value, AL_FREQUENCY_SHIFTER_MAX_FREQUENCY);    }
};

class EFXVocalMorph : public EFXEffect {
 EFXPhoneme phonemeA, phonemeB;
 int phonemeACoarseTuning, phonemeBCoarseTuning;
 EFXWaveForm3a waveform;
 float rate;
public:
 EFXVocalMorph() { 
  phonemeA             = (EFXPhoneme) AL_VOCAL_MORPHER_DEFAULT_PHONEMEA; 
  phonemeB             = (EFXPhoneme) AL_VOCAL_MORPHER_DEFAULT_PHONEMEB;
  phonemeACoarseTuning = AL_VOCAL_MORPHER_DEFAULT_PHONEMEA_COARSE_TUNING;
  phonemeBCoarseTuning = AL_VOCAL_MORPHER_DEFAULT_PHONEMEB_COARSE_TUNING;
  waveform             = (EFXWaveForm3a) AL_VOCAL_MORPHER_DEFAULT_WAVEFORM;
  rate                 = AL_VOCAL_MORPHER_DEFAULT_RATE;
 }
 void PhonemeACoarseTuning( int value ) { phonemeACoarseTuning =RANGE(AL_VOCAL_MORPHER_MIN_PHONEMEA_COARSE_TUNING, value, AL_VOCAL_MORPHER_MAX_PHONEMEA_COARSE_TUNING); }
 void PhonemeBCoarseTuning( int value ) { phonemeBCoarseTuning =RANGE(AL_VOCAL_MORPHER_MIN_PHONEMEB_COARSE_TUNING, value, AL_VOCAL_MORPHER_MAX_PHONEMEB_COARSE_TUNING); }
 void Rate( float value )               { rate                 =RANGE(AL_VOCAL_MORPHER_MIN_RATE,                   value, AL_VOCAL_MORPHER_MAX_RATE);                   }
};

class EFXPitchShift : public EFXEffect {
 int coarse, fine;
 EFXPitchShift() { 
  coarse = AL_PITCH_SHIFTER_DEFAULT_COARSE_TUNE; 
  fine   = AL_PITCH_SHIFTER_DEFAULT_FINE_TUNE;
 }
public:
 void Coarse( int value ) { coarse =RANGE(AL_PITCH_SHIFTER_MIN_COARSE_TUNE, value, AL_PITCH_SHIFTER_MAX_COARSE_TUNE); }
 void Fine( int value )   { fine   =RANGE(AL_PITCH_SHIFTER_MIN_FINE_TUNE,   value, AL_PITCH_SHIFTER_MAX_FINE_TUNE);   }
};

class EFXRingMod : public EFXEffect {
 float frequency, cutoff;
 EFXWaveForm3b waveform;
 EFXRingMod() { 
  frequency = AL_RING_MODULATOR_DEFAULT_FREQUENCY; 
  cutoff    = AL_RING_MODULATOR_DEFAULT_HIGHPASS_CUTOFF;
  waveform  = (EFXWaveForm3b) AL_RING_MODULATOR_DEFAULT_WAVEFORM;
 }
public:
 void Frequency( float value ) { frequency =RANGE(AL_RING_MODULATOR_MIN_FREQUENCY,       value, AL_RING_MODULATOR_MAX_FREQUENCY);       }
 void Cutoff( float value )    { cutoff    =RANGE(AL_RING_MODULATOR_MIN_HIGHPASS_CUTOFF, value, AL_RING_MODULATOR_MAX_HIGHPASS_CUTOFF); }
};

class EFXAutoWah : public EFXEffect {
 float attack, release, resonance, gain;
 EFXAutoWah() { 
  attack    = AL_AUTOWAH_DEFAULT_ATTACK_TIME; 
  release   = AL_AUTOWAH_DEFAULT_RELEASE_TIME;
  resonance = AL_AUTOWAH_DEFAULT_RESONANCE;
  gain      = AL_AUTOWAH_DEFAULT_PEAK_GAIN;
 }
public:
 void Attack( float value )    { attack    =RANGE(AL_AUTOWAH_MIN_ATTACK_TIME,  value, AL_AUTOWAH_MAX_ATTACK_TIME);  }
 void Release( float value )   { release   =RANGE(AL_AUTOWAH_MIN_RELEASE_TIME, value, AL_AUTOWAH_MAX_RELEASE_TIME); }
 void Resonance( float value ) { resonance =RANGE(AL_AUTOWAH_MIN_RESONANCE,    value, AL_AUTOWAH_MAX_RESONANCE);    }
 void Gain( float value )      { gain      =RANGE(AL_AUTOWAH_MIN_PEAK_GAIN,    value, AL_AUTOWAH_MAX_PEAK_GAIN);    }
};

class EFXCompressor : public EFXEffect {
 bool on;
 EFXCompressor() { 
  on             = (bool) AL_COMPRESSOR_DEFAULT_ONOFF; 
 }
public:
};

class EFXEqualizer : public EFXEffect {
 float lowGain, lowCutoff, mid1Gain, mid1Center, mid1Width, mid2Gain, mid2Center, mid2Width, highGain, highCutoff; 
 EFXEqualizer() { 
  lowGain    = AL_EQUALIZER_DEFAULT_LOW_GAIN; 
  lowCutoff  = AL_EQUALIZER_DEFAULT_LOW_CUTOFF;
  mid1Gain   = AL_EQUALIZER_DEFAULT_MID1_GAIN;
  mid1Center = AL_EQUALIZER_DEFAULT_MID1_CENTER;
  mid1Width  = AL_EQUALIZER_DEFAULT_MID1_WIDTH;
  mid2Gain   = AL_EQUALIZER_DEFAULT_MID2_GAIN;
  mid2Center = AL_EQUALIZER_DEFAULT_MID2_CENTER;
  mid2Width  = AL_EQUALIZER_DEFAULT_MID2_WIDTH;
  highGain   = AL_EQUALIZER_DEFAULT_HIGH_GAIN;
  highCutoff = AL_EQUALIZER_DEFAULT_HIGH_CUTOFF;
 }
public:
 void LowGain( float value )     { lowGain    =RANGE(AL_EQUALIZER_MIN_LOW_GAIN,    value, AL_EQUALIZER_MAX_LOW_GAIN);    }
 void LowCutoff( float value )   { lowCutoff  =RANGE(AL_EQUALIZER_MIN_LOW_CUTOFF,  value, AL_EQUALIZER_MAX_LOW_CUTOFF);  }
 void Mid1Gain( float value )    { mid1Gain   =RANGE(AL_EQUALIZER_MIN_MID1_GAIN,   value, AL_EQUALIZER_MAX_MID1_GAIN);   }
 void Mid1Center( float value )  { mid1Center =RANGE(AL_EQUALIZER_MIN_MID1_CENTER, value, AL_EQUALIZER_MAX_MID1_CENTER); }
 void Mid1Width( float value )   { mid1Width  =RANGE(AL_EQUALIZER_MIN_MID1_WIDTH,  value, AL_EQUALIZER_MAX_MID1_WIDTH);  }
 void Mid2Gain( float value )    { mid2Gain   =RANGE(AL_EQUALIZER_MIN_MID2_GAIN,   value, AL_EQUALIZER_MAX_MID2_GAIN);   }
 void Mid2Center( float value )  { mid2Center =RANGE(AL_EQUALIZER_MIN_MID2_CENTER, value, AL_EQUALIZER_MAX_MID2_CENTER); }
 void Mid2Width( float value )   { mid2Width  =RANGE(AL_EQUALIZER_MIN_MID2_WIDTH,  value, AL_EQUALIZER_MAX_MID2_WIDTH);  }
 void HighGain( float value )    { highGain   =RANGE(AL_EQUALIZER_MIN_HIGH_GAIN,   value, AL_EQUALIZER_MAX_HIGH_GAIN);   }
 void HighCutoff( float value )  { highCutoff =RANGE(AL_EQUALIZER_MIN_HIGH_CUTOFF, value, AL_EQUALIZER_MAX_HIGH_CUTOFF); }
};

class EFXFilter {
};

class EFX { 
public:
 EFXEffect effects;
 int version_major,version_minor,max_aux_sends,meters_per_unit;

 EFX() {
  version_major=version_minor=max_aux_sends=meters_per_unit=0;
 }

 LPALGENEFFECTS alGenEffects;
 LPALDELETEEFFECTS alDeleteEffects;
 LPALISEFFECT alIsEffect;
 LPALEFFECTI alEffecti;
 LPALEFFECTIV alEffectiv;
 LPALEFFECTF alEffectf;
 LPALEFFECTFV alEffectfv;
 LPALGETEFFECTI alGetEffecti;
 LPALGETEFFECTIV alGetEffectiv;
 LPALGETEFFECTF alGetEffectf;
 LPALGETEFFECTFV alGetEffectfv;
 LPALGENFILTERS alGenFilters;
 LPALDELETEFILTERS alDeleteFilters;
 LPALISFILTER alIsFilter;
 LPALFILTERI alFilteri;
 LPALFILTERIV alFilteriv;
 LPALFILTERF alFilterf;
 LPALFILTERFV alFilterfv;
 LPALGETFILTERI alGetFilteri;
 LPALGETFILTERIV alGetFilteriv;
 LPALGETFILTERF alGetFilterf;
 LPALGETFILTERFV alGetFilterfv;
 LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
 LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
 LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
 LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
 LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
 LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
 LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
 LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
 LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
 LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
 LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

 bool Init()
 {
  bool hasEFX=false;
  ALCdevice *device;
  ALuint obj;
  int i;
  const ALenum effects[] = {
   AL_EFFECT_EAXREVERB, AL_EFFECT_REVERB, AL_EFFECT_CHORUS,
   AL_EFFECT_DISTORTION, AL_EFFECT_ECHO, AL_EFFECT_FLANGER,
   AL_EFFECT_FREQUENCY_SHIFTER, AL_EFFECT_VOCAL_MORPHER,
   AL_EFFECT_PITCH_SHIFTER, AL_EFFECT_RING_MODULATOR, AL_EFFECT_AUTOWAH,
   AL_EFFECT_COMPRESSOR, AL_EFFECT_EQUALIZER, AL_EFFECT_NULL
  };
  char effectNames[] = "EAX Reverb,Reverb,Chorus,Distortion,Echo,Flanger,Frequency Shifter,Vocal Morpher,pitch Shifter,Ring Modulator,Autowah,Compressor,Equalizer,";
  const ALenum filters[] = {
   AL_FILTER_LOWPASS, AL_FILTER_HIGHPASS, AL_FILTER_BANDPASS,
   AL_FILTER_NULL
  };
  char filterNames[] = "Low-pass,High-pass,Band-pass,";
  char *current;

  device = alcGetContextsDevice(alcGetCurrentContext());
  if(alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_FALSE) { OUTPUT("EFX not available.\n"); return; }

  alcGetIntegerv(device, ALC_EFX_MAJOR_VERSION, 1, &version_major);
  alcGetIntegerv(device, ALC_EFX_MINOR_VERSION, 1, &version_minor);
  checkForErrors();
  OUTPUT("EFX version: %d.%d\n", (int)version_major, (int)version_minor);

  alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &max_aux_sends);
  checkForErrors();
  OUTPUT("Max auxiliary sends: %d\n", (int)max_aux_sends);

  hasEFX = alcIsExtensionPresent(null, ALC_EXT_EFX_NAME );
  if ( hasEFX ) {
   OUTPUT( "EFX extension available: \n" );
   alGenEffects                 = (LPALGENEFFECTS) alGetProcAddress( "alGenEffects" ); if ( alGenEffects == null ) hasEFX = false;
   alDeleteEffects              = (LPALDELETEEFFECTS) alGetProcAddress( "alDeleteEffects" ); if ( alDeleteEffects == null ) hasEFX = false; 
   alIsEffect                   = (LPALISEFFECT) alGetProcAddress( "alIsEffect" ); if ( alIsEffect == null ) hasEFX = false;
   alEffecti                    = (LPALEFFECTI) alGetProcAddress( "alEffecti" ); if ( alEffecti == null ) hasEFX = false;
   alEffectiv                   = (LPALEFFECTIV) alGetProcAddress( "alEffectiv" ); if ( alEffectiv == null ) hasEFX = false;
   alEffectf                    = (LPALEFFECTF) alGetProcAddress( "alEffectf" ); if ( alEffectf == null ) hasEFX = false;
   alEffectfv                   = (LPALEFFECTFV) alGetProcAddress( "alEffectfv" ); if ( alEffectfv == null ) hasEFX = false;
   alGetEffecti                 = (LPALGETEFFECTI) alGetProcAddress( "alGetEffecti" ); if ( alGetEffecti == null ) hasEFX = false;
   alGetEffectiv                = (LPALGETEFFECTIV) alGetProcAddress( "alGetEffectiv" ); if ( alGetEffectiv == null ) hasEFX = false;
   alGetEffectf                 = (LPALGETEFFECTF) alGetProcAddress( "alGetEffectf" ); if ( alGetEffectf == null ) hasEFX = false;
   alGetEffectfv                = (LPALGETEFFECTFV) alGetProcAddress( "alGetEffectfv" ); if ( alGetEffectfv == null ) hasEFX = false;
   alGenFilters                 = (LPALGENFILTERS) alGetProcAddress( "alGenFilters" ); if ( alGenFilters == null ) hasEFX = false;
   alDeleteFilters              = (LPALDELETEFILTERS) alGetProcAddress( "alDeleteFilters" ); if ( alDeleteFilters == null ) hasEFX = false;
   alIsFilter                   = (LPALISFILTER) alGetProcAddress( "alIsFilter" ); if ( alIsFilter == null ) hasEFX = false;
   alFilteri                    = (LPALFILTERI) alGetProcAddress( "alFilteri" ); if ( alFilteri == null ) hasEFX = false;
   alFilteriv                   = (LPALFILTERIV) alGetProcAddress( "alFilteriv" ); if ( alFilteriv == null ) hasEFX = false;
   alFilterf                    = (LPALFILTERF) alGetProcAddress( "alFilterf" ); if ( alFilterf == null ) hasEFX = false;
   alFilterfv                   = (LPALFILTERFV) alGetProcAddress( "alFilterfv" ); if ( alFilterfv == null ) hasEFX = false;
   alGetFilteri                 = (LPALGETFILTERI) alGetProcAddress( "alGetFilteri" ); if ( alGetFilteri == null ) hasEFX = false;
   alGetFilteriv                = (LPALGETFILTERIV) alGetProcAddress( "alGetFilteriv" ); if ( alGetFilteriv == null ) hasEFX = false;
   alGetFilterf                 = (LPALGETFILTERF) alGetProcAddress( "alGetFilterf" ); if ( alGetFilterf == null ) hasEFX = false;
   alGetFilterfv                = (LPALGETFILTERFV) alGetProcAddress( "alGetFilterfv" ); if ( alGetFilterfv == null ) hasEFX = false;
   alGenAuxiliaryEffectSlots    = (LPALGENAUXILIARYEFFECTSLOTS) alGetProcAddress( "alGenAuxiliaryEffectSlots" ); if ( alGenAuxiliaryEffectSlots == null ) hasEFX = false;
   alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS) alGetProcAddress( "alDeleteAuxiliaryEffectSlots" ); if ( alDeleteAuxiliaryEffectSlots == null ) hasEFX = false;
   alIsAuxiliaryEffectSlot      = (LPALISAUXILIARYEFFECTSLOT) alGetProcAddress( "alIsAuxiliaryEffectSlot" ); if ( alIsAuxiliaryEffectSlot == null ) hasEFX = false;
   alAuxiliaryEffectSloti       = (LPALAUXILIARYEFFECTSLOTI) alGetProcAddress( "alAuxiliaryEffectSloti" ); if ( alAuxiliaryEffectSloti == null ) hasEFX = false;
   alAuxiliaryEffectSlotiv      = (LPALAUXILIARYEFFECTSLOTIV) alGetProcAddress( "alAuxiliaryEffectSlotiv" ); if ( alAuxiliaryEffectSlotiv == null ) hasEFX = false;
   alAuxiliaryEffectSlotf       = (LPALAUXILIARYEFFECTSLOTF) alGetProcAddress( "alAuxiliaryEffectSlotf" ); if ( alAuxiliaryEffectSlotf == null ) hasEFX = false;
   alAuxiliaryEffectSlotfv      = (LPALAUXILIARYEFFECTSLOTFV) alGetProcAddress( "alAuxiliaryEffectSlotfv" ); if ( alAuxiliaryEffectSlotfv == null ) hasEFX = false;
   alGetAuxiliaryEffectSloti    = (LPALGETAUXILIARYEFFECTSLOTI) alGetProcAddress( "alGetAuxiliaryEffectSloti" ); if ( alGetAuxiliaryEffectSloti == null ) hasEFX = false;
   alGetAuxiliaryEffectSlotiv   = (LPALGETAUXILIARYEFFECTSLOTIV) alGetProcAddress( "alGetAuxiliaryEffectSlotiv" ); if ( alGetAuxiliaryEffectSlotiv == null ) hasEFX = false;
   alGetAuxiliaryEffectSlotf    = (LPALGETAUXILIARYEFFECTSLOTF) alGetProcAddress( "alGetAuxiliaryEffectSlotf" ); if ( alGetAuxiliaryEffectSlotf == null ) hasEFX = false;
   alGetAuxiliaryEffectSlotfv   = (LPALGETAUXILIARYEFFECTSLOTFV) alGetProcAddress( "alGetAuxiliaryEffectSlotfv" ); if ( alGetAuxiliaryEffectSlotfv == null ) hasEFX = false;
  }
  alGetError();
  if ( !hasEFX ) { ALError( "Failed to retreive EFX extension functions.\n" ); return; }

  alGenFilters(1, &obj);
  checkForErrors();
  current = filterNames;
  for(i = 0;filters[i] != AL_FILTER_NULL;i++)
  {
   char *next = strchr(current, ',');

   alFilteri(obj, AL_FILTER_TYPE, filters[i]);
   if(alGetError() == AL_NO_ERROR) current = next+1;
   else memmove(current, next+1, strlen(next));
  }
  alDeleteFilters(1, &obj);
  checkForErrors();
  printList("Supported filters", ',', filterNames);

  alGenEffects(1, &obj);
  checkForErrors();
  current = effectNames;
  for(i = 0;effects[i] != AL_EFFECT_NULL;i++) {
   char *next = strchr(current, ',');
   alEffecti(obj, AL_EFFECT_TYPE, effects[i]);
   if(alGetError() == AL_NO_ERROR)
   current = next+1;
   else
   memmove(current, next+1, strlen(next));
  }
  alDeleteEffects(1, &obj);
  checkForErrors();
  printList("Supported effects", ',', effectNames);
  return hasEFX=true;
 }
};
#endif

class ALDeviceDescription : public ListItem {
public:
 Zstring name;
};

class ALDeviceDescriptions : public LinkedList
{
public:
 bool inList(char *fn) {
  FOREACH(ALDeviceDescription,a) if ( contains(a->name, fn) ) return true;
  return false;
 }
 ALDeviceDescription *find(char *fn) {
  FOREACH(ALDeviceDescription,a) if ( contains(a->name, fn) ) return a;
  return null;
 }
 CLEARLISTRESET(ALDeviceDescription);
};

// Simple Sound Manager
class ALSoundManager {
public:
 char *path; // The path prefix for loading files
 ALWaveForms waveforms;
 ALUREStreams streams;
 ALSounds sounds;
 ALCDeviceContexts deviceContexts;
 bool error,initialized,muted,hasOggExtension,hasEAX;
 float distance, gain, max_gain, min_gain, dopplerFactor, dopplerVelocity;
 float stream_gain, sounds_gain;
 int version_major, version_minor, refreshRate;
// uint bufferClamp, sourceClamp;
 ALCint monoSources, stereoSources;
 ALDeviceDescriptions outputs;
 ALDeviceDescriptions inputs;

 ALSoundManager();

 ~ALSoundManager()
 {
  OUTPUT( "ALSoundManager: Releasing %d sounds.\n", sounds.count );
  EACH(sounds.first,ALSound,s) s->Release();
  OUTPUT( "ALSoundManager: shutting down device.\n");
  alureShutdownDevice();
 }

 virtual void Virtual() { }

 void BetweenFrames() {
  alureUpdate(); //OUTPUT( "alureUpdate()\n" ); ALError("Update: ");
  EACH(sounds.first,ALSound,s) s->_BetweenFrames();
  sounds.DeleteDone();
 }

#if defined(USE_EFX)
 // EAX 2.0 GUIDs
 EAXSet eaxSet; // EAXSet function, retrieved if EAX Extension is supported
 EAXGet eaxGet; // EAXGet function, retrieved if EAX Extension is supported
 EFX efx;    // EFX function bindings
#endif 
 SoundVector position, velocity, orientation;
                               // ^orientation of the Listener. (first 3 elements are "at", second 3 are "up")
                               //  Also note that these should be units of '1'.

 inline void ClearError() { alGetError(); }
 ALCdevice *currentDevice() { return alcGetContextsDevice(alcGetCurrentContext()); }
 ALCcontext *currentContext() { return alcGetCurrentContext(); }

 // Forces initialization defaulting to software when available.
 // You would use this to support EFX on most contemporary sound devices.
 // The device context is returned, or null if none was initialized.
 ALCDeviceContext *InitSoftware() {
  ALCDeviceContext *alc=new ALCDeviceContext;
  alc->Init("Generic Software",0);
  if ( !Init(alc) ) { delete alc; return null; }
  return alc;
 }
 ALCDeviceContext *InitDirectSound() {
  ALCDeviceContext *alc=new ALCDeviceContext;
  alc->Init("DirectSound",0);
  if ( !Init(alc) ) { delete alc; return null; }
  return alc;
 }
 ALCDeviceContext *InitContext( const char * context ) {
  ALCDeviceContext *alc=new ALCDeviceContext;
  alc->Init(context,0);
  if ( !Init(alc) ) { delete alc; return null; }
  return alc;
 }

 // Initialize OpenAL and clear the error bit.
 bool Init( ALCDeviceContext *alc ) {
  const char *str;
  if ( initialized ) return true;
  if ( alc ) { deviceContexts.Append(alc); }
  else {
   alc=new ALCDeviceContext;
   alc->PickDefault();
   deviceContexts.Append(alc);
  }
  str = alcGetString(alc->device, ALC_DEVICE_SPECIFIER);
  OUTPUT( "OpenAL initialized: %s\n", str );
  //unusedBuffers=bufferClamp;
  //unusedSources=sourceClamp;
  alGetError();
  if ( alGetError() != AL_NO_ERROR ) {
   OUTPUT( "Init: Error initializing audio!\n" );
   initialized=false;
   return false;
  }
  OUTPUT( "ALSound: Initializing.\n" );
  { ALCdevice *device = alcGetContextsDevice(alcGetCurrentContext());
    alcGetIntegerv(alc ? alc->device : device, ALC_MONO_SOURCES, 1, &monoSources);
    alcGetIntegerv(alc ? alc->device : device, ALC_STEREO_SOURCES, 1, &stereoSources);
  }
  OUTPUT( "Detected %d hinted mono source%s, and %d hinted stereo source%s.\n", 
          monoSources, monoSources != 1 ? "s" : "", stereoSources, stereoSources != 1 ? "s" : "" );
  if ( alIsExtensionPresent( "AL_EXT_vorbis" ) != AL_TRUE ) {
   OUTPUT( "Ogg Vorbis extension not available!  This is probably not Linux.\n" );
   hasOggExtension = false;
  } else hasOggExtension = true;

#if defined(USE_EFX)
  // Check for EAX 2.0 support and
  // Retrieves function entry addresses to API ARB extensions, in this case,
  // for the EAX extension. See Appendix 1 (Extensions) of
  // http://www.openal.org/openal_webstf/specs/OpenAL1-1Spec_html/al11spec7.html
  hasEAX = alIsExtensionPresent( "EAX2.0" );
  if ( hasEAX ) {
   OUTPUT( "EAX 2.0 extension is available.\n" );
   eaxSet = (EAXSet) alGetProcAddress( "EAXSet" ); if ( eaxSet == null ) hasEAX = false;
   eaxGet = (EAXGet) alGetProcAddress( "EAXGet" ); if ( eaxGet == null ) hasEAX = false;
   if ( !hasEAX ) ALError( "Failed to retreive EAX extension functions.\n" );
  }
  if ( !efx.Init() ) { delete efx; efx=null; OUTPUT( "EFX did not initialize properly, and therefore is unavailable.\n" ); }
#endif

  DeviceInfo( );
  alGetError();
  initialized=true;
  return initialized;
 }

 ALSound *Cue( const char *filename, float pitch, float gain, bool looped, bool createSource ) {
  if ( gain <= 0.0 ) return null;
  if ( sounds.count >= monoSources ) return null;
  ALSound *sound;
  if ( !filename ) return null;
  alGetError();
  sound = new ALSound;
  sound->pitch=pitch;
  sound->gain=gain*sounds_gain;
  sound->loop=(ALboolean) looped;
  sound->name=string(filename);
  sound->waveform=waveforms.findOrLoad(path,filename); // null/crash here means you have no context
  sound->format=sound->waveform->sfinfo.format;
  sound->size=sound->waveform->sampleCount * sound->waveform->sfinfo.channels *2;
  sound->freq=(unsigned int) sound->waveform->sfinfo.samplerate;
  float pitchDurationFactor=pitch >= 1.0f ? 1.0f : (1.0f+(10.0f-pitch*10.0f));
  sound->duration=sound->waveform->duration*pitchDurationFactor; // kept cutting it off, sound playback length effected
  sound->source = new ALSource;
  alGenSources(1, &(sound->source->id));
  if(ALError("ALSoundManager:Cue:alGenSources")) { 
   delete sound;
   return null;
  }
  sound->source->bindSound(sound);
  sounds.Append(sound);
  return sound;
 }
 
 ALSound *CueAndPlay( const char *filename, double pitch, double gain, bool looped=false, bool createSource=true ) {
  return CueAndPlay(filename,(float)pitch,(float)gain,looped,createSource);
 }
 ALSound *CueAndPlay( string filename, double pitch, double gain, bool looped, bool createSource ) {
  if ( gain <= 0.0 ) return null;
  return filename.length() > 0 ? CueAndPlay((char *)filename.c_str(),(float)pitch,(float)gain,looped,createSource) : null;
 }
 ALSound *CueAndPlay( string filename, float pitch, float gain, bool looped, bool createSource ) {
  if ( gain <= 0.0 ) return null;
  return filename.length() > 0 ? CueAndPlay((char *)filename.c_str(),pitch,gain,looped,createSource) : null;
 }
 ALSound *CueAndPlay( const char *filename, float pitch, float gain, bool looped=false, bool createSource=true ) {
  if ( gain <= 0.0 ) return null;
  ALSound *s = Cue( filename, pitch, gain, looped, createSource );
//  OUTPUT( "Attempting to play %s\n", filename );
//  if ( !s ) OUTPUT( "Sound was null, so it couldn't have been played.\n" );
//  else
//  OUTPUT ( "Playing sound '%s' buffer %d source %d\n", s.name, (int) s.buffer.id, (int) s.source.id );
  if ( s ) s->Play();
  return s;
 }

 ALSound *CueAndPlayIfNotPlaying( string filename, float pitch, float gain, bool looped, bool createSource ) { return filename.length() > 0 ? CueAndPlayIfNotPlaying((char *)filename.c_str(),pitch,gain,looped,createSource) : null; }
 ALSound *CueAndPlayIfNotPlaying( const char *filename, float pitch, float gain, bool looped=false, bool createSource=true ) {
  if ( gain <= 0.0 ) return null;
  EACH(sounds.first,ALSound,p) if ( !str_cmp( p->name.c_str(), filename ) ) return NULL;
  ALSound *s = Cue( filename, pitch, gain, looped, createSource );
//  OUTPUT( "Attempting to play %s\n", filename );
//  if ( !s ) OUTPUT( "Sound was null, so it couldn't have been played.\n" );
//  else  
//  OUTPUT ( "Playing sound '%s' buffer %d source %d\n", s.name, (int) s.buffer.id, (int) s.source.id );
  if ( s ) s->Play();
  return s;
 }

 ALSound *CueAndPlayLimited( string filename, int simultaneous, float pitch, float gain, bool looped, bool createSource ) { return filename.length() > 0 ? CueAndPlayLimited((char *) filename.c_str(), simultaneous, pitch, gain, looped, createSource) : null; }
 ALSound *CueAndPlayLimited( const char *filename, int simultaneous, float pitch, float gain, bool looped=false, bool createSource=true ) {
  if ( gain <= 0.0 ) return null;
  int currently=0;
  EACH(sounds.first,ALSound,p) if ( !str_cmp( p->name.c_str(), filename ) ) currently++;
  if ( currently >= simultaneous ) return NULL;
  ALSound *s = Cue( filename, pitch, gain, looped, createSource );
//  OUTPUT( "Attempting to play %s\n", filename );
//  if ( !s ) OUTPUT( "Sound was null, so it couldn't have been played.\n" );
//  else
//  OUTPUT ( "Playing sound '%s' buffer %d source %d\n", s.name, (int) s.buffer.id, (int) s.source.id );
  if ( s ) s->Play();
  return s;
 }

 ALUREStream *AddStream( string fn, ALSource *source, int loops, bool calcLength ) { return fn.length() > 0 ? AddStream((char *)fn.c_str(), source, loops, calcLength) : null; }
 ALUREStream *AddStream( const char *fn, ALSource *source, int loops=-1, bool calcLength=true ) {
  if ( gain <= 0.0 ) return null;
  ALUREStream *stream=new ALUREStream;
  if ( !source ) {	 // Bind the buffer with the source.
   stream->source =new ALSource;
   alGenSources(1, &(stream->source->id));
   if(ALError("ALSoundManager:WAV:alGenSources")) { return stream; }
   stream->source->bindStream(stream);
   //Do another error check and return.
   if(ALError("ALSoundManager:AddStream:alSource")) { return stream; }
  }
  else stream->source=source;
  streams.Append(stream);
  stream->manager=this;
  stream->loops=loops;
  stream->gain*=stream_gain;
  if ( !stream->LoadIntoSource(fn,stream->source,calcLength) ) { 
   OUTPUT("ALSoundManager:AddStream:stream.Create() had an error.\n" );
   return stream;
  }
  return stream;
 }

 void SetListenerValues()
 {
  alListenerfv(AL_POSITION,    position.asFloat3()    );
  alListenerfv(AL_VELOCITY,    velocity.asFloat3()    );
  alListenerfv(AL_ORIENTATION, orientation.asFloat6() );
 }

 void SetGain( float f ) {
  SetMasterGain(f);
 }

 void SetMasterGain( float f ) {
  gain=f;
  alListenerf( AL_GAIN, gain );
 }

 void SetGain( float sounds, float music ) {
  this->sounds_gain=sounds;
  this->stream_gain=music;
 }

 // Detaches all sources from buffers.  This is done before all sounds are discarded.
 void Clear() { }

 bool ResumeAll( void )
 {
  ALSound *s;
  int sourceAudioState = 0;
  alGetError();
  for ( s=(ALSound *) (sounds.first); s; s=(ALSound *) (s->next)) { 
   alGetSourcei( s->source->id, AL_SOURCE_STATE, &sourceAudioState );
   if ( sourceAudioState == AL_PAUSED ) // Are we currently playing the audio source?
   {
    alSourcePlay( s->source->id );
    if ( ALError( "ALSoundManager:ResumeAll:alSourcePlay") ) return false;
    s->source->state=playing;
   }
  }
  muted=false;
  return true;
 }

 bool PauseAll() {
  int sourceAudioState = 0;
  alGetError();
  EACH(sounds.first,ALSound,s) {
   if ( s->Playing() ) {  // Are we currently playing the audio source?
    alGetSourcei( s->source->id, AL_SOURCE_STATE, &sourceAudioState );
    if ( sourceAudioState != AL_PAUSED ) {
     alSourcePause( s->source->id );
     if ( ALError( "ALSoundManager:PauseAll:alSourcePause") ) return false;
     s->source->state=paused;
    }
   }
  }
  muted=true;
  return true;
 }

 bool StopAll( void ) {
  ALSound *s;
  alGetError();
  for ( s=(ALSound *) (sounds.first); s; s=(ALSound *) (s->next))
   if ( s->Playing() ) s->Stop();
  if ( ALError( "ALSoundManager:StopAll:alSourceStop") ) return false;
  return true;
 }

 /****************************************************************************/
 bool SetListener( )
 { 
  alListenerfv( AL_POSITION, position.asFloat3() );       if ( ALError( "ALSoundManager:SetListener:alListenerfv:AL_POSITION" )  ) return false;
  alListenerfv( AL_VELOCITY, velocity.asFloat3() );       if ( ALError( "ALSoundManager:SetListener:alListenerfv:AL_VELOCITY" )  ) return false;
  alListenerfv( AL_ORIENTATION, orientation.asFloat6() ); if ( ALError( "ALSoundManager:SetListener:alListenerfv:AL_DIRECTION" ) ) return false;
  alListenerf( AL_MAX_DISTANCE, distance );
  alListenerf( AL_MIN_GAIN, min_gain );
  alListenerf( AL_MAX_GAIN, max_gain );
  alListenerf( AL_GAIN, gain );
  return true;
 }

 void printDevice(ALCenum which, const char *kind)
 {
  const char *s = alcGetString(NULL, which);
  OUTPUT("Available %s devices:\n", kind);
  if(s == NULL || *s == '\0') OUTPUT("    (none!)\n");
  else do {
   OUTPUT("    %s\n", s);
   while(*s++ != '\0');
  } while(*s != '\0');
 }

 void FindDevices() {
  const char *s = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  const char *t = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
  if ( s==NULL || *s == '\0' ) { } else do {
   ALDeviceDescription *device=new ALDeviceDescription;
   device->name=string(s);
   outputs.Append(device);
   while( *s++ != '\0' );
  } while(*s != '\0');
  if ( t==NULL || *t == '\0' ) { } else do {
   ALDeviceDescription *device=new ALDeviceDescription;
   device->name=string(t);
   inputs.Append(device);
   while( *t++ != '\0' );
  } while(*t != '\0'); 
 }

 void PrintDevices() {
  if ( inputs.count==0 && outputs.count==0 ) FindDevices();
  OUTPUT( "Inputs: ");
  EACH(inputs.first,ALDeviceDescription,a) OUTPUT( "%s%s", a->name, a->next ? "s" : "");
  OUTPUT( "\nOutputs: ");
  EACH(outputs.first,ALDeviceDescription,a) OUTPUT( "%s%s", a->name, a->next ? "s" : "");
  OUTPUT( "\n" );
 }

 void DeviceInfo( ) {
  ALCdevice *device;
  OUTPUT ( "OpenAL Information:\n" );
  if(alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {
  /* if(alcIsExtensionPresent(NULL,"ALC_ENUMERATE_ALL_EXT") == AL_TRUE) printDevice(ALC_ALL_DEVICES_SPECIFIER, "playback");
   else*/ printDevice(ALC_DEVICE_SPECIFIER, "playback");
   printDevice(ALC_CAPTURE_DEVICE_SPECIFIER, "capture");
  } else OUTPUT("No device enumeration available\n");

  OUTPUT("Default device: %s\n", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
  OUTPUT("Default capture device: %s\n", (alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER)));
  device = alcGetContextsDevice(alcGetCurrentContext());
  checkForErrors(true);

  alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &version_major);
  alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &version_minor);
  checkForErrors(true);
  OUTPUT("ALC version: %d.%d\n", (int)version_major, (int)version_minor);

  printList("ALC extensions", ' ', alcGetString(device, ALC_EXTENSIONS));
  checkForErrors(true);

  OUTPUT("OpenAL vendor string: %s\n", alGetString(AL_VENDOR));
  OUTPUT("OpenAL renderer string: %s\n", alGetString(AL_RENDERER));
  OUTPUT("OpenAL version string: %s\n", alGetString(AL_VERSION));
  printList("OpenAL extensions", ' ', alGetString(AL_EXTENSIONS));
  checkForErrors(true);
 }

 void ShowDopplerEquation() {
  OUTPUT( "ALDopplerEquation: shift = DOPPLER_FACTOR * freq * (DOPPLER_VELOCITY - l.velocity) / (DOPPLER_VELOCITY + s.velocity) \n" );
 }
 void SetDoppler( float Factor, float velocity ) {
  alDopplerFactor  ( dopplerFactor=Factor );
  if(ALError("ALSoundManager:SetDoppler:alDopplerFactor"  )){}
  alDopplerVelocity( dopplerVelocity=velocity );
  if(ALError("ALSoundManager:SetDoppler:alDopplerVelocity")){}
 }
};


class Audio {
public:
 ALSoundManager manager;
 char *path;
 Audio() {
  path=null;
  manager.Init(null);
 }
 void SetVolume( float v ) {
  manager.SetGain(v);
 }
 void SetSoundVolume( float v ) {
  manager.SetGain(v,manager.stream_gain);
 }
 void SetMusicVolume( float v ) {
  manager.SetGain(manager.sounds_gain,v);
 }
 inline void Load( const char *fn )       { manager.waveforms.findOrLoad(path,fn); }
 inline void findOrLoad( const char *fn ) { manager.waveforms.findOrLoad(path,fn); }
 bool Mute()    { return manager.PauseAll();  } 
 bool Pause()   { return manager.PauseAll();  }
 bool Resume()  { return manager.ResumeAll(); }
 bool Unpause() { return manager.ResumeAll(); }
 bool Unmute()  { return manager.ResumeAll(); }
 void Stop()    { manager.StopAll(); }
 void Clear()   { manager.Clear();   }
 // Gets you a gain factor based on x,y pixel distance 1.loud->1.0->0.5->0.0 (at 2x)
 double GetNearGain( double x, double y , double loud=0.25 );
};

class SoundEvent;
class SoundEventALUREStream : public ALUREStream {
public:
 Zp<SoundEvent> parentEvent;
 void OnComplete();
};

class SoundEvent : public ListItem {
public:
 Zstring sound;
 Zp<SoundEventALUREStream> stream;
 Zbool loop, fire_and_forget;
 Zint sampleLength;
 Zint filesize;
 Zfloat elapsed,delay,mark;
 Zbool expired,played;

 SoundEvent() : ListItem() { }
 SoundEvent( const char *fn, ALSource *source, int loops, bool calcLength );
 SoundEvent( SoundEventALUREStream *s, float d ) : ListItem() {
  stream=s;
  delay=d;
 }
 SoundEvent( SoundEventALUREStream *s ) : ListItem() {
  stream=s;
 }
 void Init( ) { 
 }
 virtual void Completed() {
 }
 virtual void OnMark() { }
 ~SoundEvent () { 
  OUTPUT( "Deleting event.\n" ); 
  stream.Delete();
 }
};

class SoundEvents : public LinkedList {
public:
 CLEARLISTRESET(SoundEvent);
 void Free() { Clear(); }
};

class SoundSequencer : public ListItem {
public:
 Zp<Audio> audio;
 SoundEvents events;
 SoundSequencer() : ListItem() {}

 void Add( SoundEvent *s ) { 
  events.Append(s);
  s->Init();
 }
 void Update( float rate ) { 
  SoundEvent *e;
  for (e=(SoundEvent *) (events.first); e; e=(SoundEvent *)e->next) {
   if ( !e->expired && ( e->elapsed+=rate ) >= e->delay+(float)e->mark ) { 
     e->expired=true; e->OnMark();
   } else { 
    if ( e->elapsed >= e->delay ) { 
     if ( e->stream && !e->played ) {
      if ( !e->stream->Playing() ) e->stream->Play();
      e->played=true;
      continue;
     }
     if ( e->sound.length>0 && !e->played ) { 
      audio->manager.CueAndPlay( e->sound.value, 1.0f, 1.0f, false, true );
      e->played=true;
     } 
    }
   }
  }
  return; 
 }
 void Stop()   { 
  SoundEvent *e;
  for (e=(SoundEvent *)(events.first); e; e=(SoundEvent *)e->next) {
   if ( e->stream ) 
    if ( e->stream->source ) 
     if ( e->stream->source->valid() ) 
      if ( e->stream->source->state == playing ) e->stream->Stop(); 
  }
 }
 void Pause()  { 
  SoundEvent *e;
  for (e=(SoundEvent *)(events.first); e; e=(SoundEvent *)e->next) 
   if ( e->stream && e->stream->source && e->stream->source->valid()
    && e->stream->source->state == playing ) e->stream->Pause();
 }
 void Resume() {
  SoundEvent *e;
  for (e=(SoundEvent *)(events.first); e; e=(SoundEvent *)e->next)
   if ( e->stream && e->stream->source && e->stream->source->valid()
    && e->stream->source->state == paused  ) e->stream->Resume();
 }
 void Reset()  {
  SoundEvent *e;
  for (e=(SoundEvent *)(events.first); e; e=(SoundEvent *)e->next) {
   e->elapsed=0.0f;
   e->played=false;
   if ( e->stream && e->stream->source
     && e->stream->source->state != stopped ) {
    e->stream->Rewind();
    e->Init();
   }
  }
 }
 ~SoundSequencer() { 
  Stop(); 
  events.Free(); 
 }
};

/*
#include "HRTimer.h"

class ALSoundManagerTimer : public HRTimer
{
public:
  Audio *audio;
 ALSoundManagerTimer( Audio *a ) { 
  expires=(int) ((1.0f/30.0f)*1000.0f);
  expired=0;
  delayExpired=NULL;
  looping=true;
  audio=a;
  Start();
  destroyMe=false;
 }
 virtual bool DelayExpired(void) {
  audio->manager.Virtual();
  return true;
 }
};
 */

extern Audio audio;

float distanceGain(int x, int y, float dampening); // calculates distance based on screen position, dampening off-screen objects

