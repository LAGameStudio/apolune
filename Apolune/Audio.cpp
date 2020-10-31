/***********************************************************************************************
 * AL Gore Library: An MSVC++ class implementation that wraps OpenAL and ALURE using other libs*
 ***********************************************************************************************
 * This file is distributed under the Revised BSD License, also known as the "New" BSD License *
 * See the text of the "New BSD License" for rules about using this file in your project.      *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <Windows.h>
#include "Audio.h"
#include	<sndfile.h>

#include "Display.h"

 double Audio::GetNearGain( double x, double y, double loud ) {
  double factor=ddistance(x,y,display.w2d,display.h2d)/UMAX(display.w2d,display.h2d);
  if ( factor <= 0.1 ) return factor/0.1*loud+1.0;
  if ( factor > 2.0 ) return 0.0;
  if ( factor > 1.0 ) return (1.0-(factor-1.0))*0.5;
  return factor*0.5+0.5;
 }
 
sf_count_t total_memory_for_audio=0;

// Used during Init(), maybe it shouldn't die() here...
void checkForErrors(bool quit)
{
 ALCdevice *device = alcGetContextsDevice(alcGetCurrentContext());
 ALCenum ALCerror = alcGetError(device);
 ALenum ALerror;
 bool error_condition=false;
 if(ALCerror != ALC_NO_ERROR) { error_condition=true; OUTPUT( "ALC %s\n",  alcGetString(device, ALCerror)); }
 ALerror = alGetError();
 if(ALerror != AL_NO_ERROR) { error_condition=true; OUTPUT("AL %s\n", alGetString(ALerror)); }
 #if defined(WIN32)
 if ( error_condition ) {
  string message=
   FORMAT("OpenAL encountered a fatal condition: the application must exit. "
          "This can be caused by no output devices available, a condition that occurs there is no audio device available for playback. "
          "Are your speakers hooked up or your headphones plugged in? "
          "Is OpenAL (oalinst.zip) installed? Do you have the latest drivers for your sound device(s) installed? "
          "Errors: ALC: `%s` AL: `%s`",alcGetString(device,ALCerror),alGetString(ALerror));
  int result = MessageBox( NULL, message.c_str(), "OpenAL Error (Audio)", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
  if ( quit ) switch ( result ) { case IDCONTINUE: exit(1); break; default: exit(1); break; }
 }
#endif
}

void printChar(int c, int *width) { OUTPUT("%c",c); *width = ((c == '\n') ? 0 : ((*width) + 1)); }
void indent(int *width) { int i; for(i = 0; i < indentation; i++) printChar(' ', width); }

void printList(const char *header, char separator, const char *list)
{
    int width = 0, start = 0, end = 0;
    OUTPUT("%s:\n", header);
    if(list == NULL || list[0] == '\0') return;
    indent(&width);
    while(1)    {
        if(list[end] == separator || list[end] == '\0')        {
            if(width + end - start + 2 > maxmimumWidth)            {
                printChar('\n', &width);
                indent(&width);
            }
            while(start < end)            {
                printChar(list[start], &width);
                start++;
            }
            if(list[end] == '\0')                break;
            start++;
            end++;
            if(list[end] == '\0')                break;
            printChar(',', &width);
            printChar(' ', &width);
        }
        end++;
    }
    printChar('\n', &width);
}

//int unusedBuffers, unusedSources;

void GetALCErrorString(ALenum err){
 switch(err) {
  case ALC_NO_ERROR: OUTPUT("AL_NO_ERROR"); break;
  case ALC_INVALID_DEVICE: OUTPUT("ALC_INVALID_DEVICE"); break;
  case ALC_INVALID_CONTEXT: OUTPUT("ALC_INVALID_CONTEXT"); break;
  case ALC_INVALID_ENUM: OUTPUT("ALC_INVALID_ENUM"); break;
  case ALC_INVALID_VALUE: OUTPUT("ALC_INVALID_VALUE"); break;
  case ALC_OUT_OF_MEMORY: OUTPUT("ALC_OUT_OF_MEMORY"); break;
 }
}

bool ALError( const char *tb ) {
 ALenum errCode;
 if ( ( errCode = alGetError() ) != AL_NO_ERROR )  {
  OUTPUT( "Audio %s Error #%d %s\n", tb, errCode, (char *) alGetString( errCode ) );
  return true;
 }
 return false;
}

bool ALWaveForm::Load( const char *path, const char * fn ) {
 char buf[1024];
 FORMAT( buf, 1024, "%s/%s", path ? path : "", fn );
 sf_count_t got, running = 0;
 OUTPUT( "Loading audio file: %s\n", fn );
 if ( file ) OUTPUT( "ALWaveForm: file pointer was already in use when Load(\"%s\") was called, this may cause file handles to be left open without a reference.  Do not do this.\n", fn );
 filename=string(fn);
 memset (&sfinfo, 0, sizeof (sfinfo)) ;
 if ((file = sf_open (fn, SFM_READ, &sfinfo)) == NULL) {
  OUTPUT ("\nALWaveForm: failed to open sound file '%s': %s\n\n", fn,  sf_strerror (NULL)) ;
  return false;
 }
 total=sfinfo.samplerate * 2 * sfinfo.channels;
 data.resize((unsigned int) total);
 while ((got=sf_readf_short(file, &data[(unsigned int) running], (total-running)/sfinfo.channels)) > 0 ) {
  running += got * sfinfo.channels;
  if (running < total) break;
  total*=2;
  try {
   data.resize((unsigned int) total);
  } catch (std::exception &e) {
   OUTPUT("Method exception: %s on %d of %d total previously allocated\nAborting partially read file!\n",
    e.what(), (int) total, (int)total_memory_for_audio );
   sf_close(file);
   return false;
  }
 }
 data.resize((unsigned int) running);
 bytesize=(int) (running * sizeof(short));
 sf_close(file);
 file=NULL;
 ALError("ALWaveForm:Load() general error check failed.");
 alGenBuffers(1,&(buffer.id)); 
 if(ALError("ALBuffer:alGenBuffers")) { buffer.id=0; return false; }
 //unusedBuffers--;
 OUTPUT("Creating %s buffer of %d at %dhz\n", channels==mono?"mono":"stereo", bytesize, sfinfo.samplerate ); 
 alBufferData( buffer.id, ((channels==mono)?AL_FORMAT_MONO16:AL_FORMAT_STEREO16), &data[0], bytesize, sfinfo.samplerate ); 
 ALError( "ALWaveForm:Load:alBufferData" );
 total_memory_for_audio += (unsigned int) total;
 duration= (float) ((double) bytesize/(double) sfinfo.channels/(double) sampleRate/(double) sfinfo.channels);
 return true;
}


void ALSource::bindBuffer( ALBuffer buffer ) {
 alGetError();
 alSourcei (id, AL_BUFFER,   buffer.id );
 alSourcef (id, AL_PITCH,    pitch  );
 alSourcef (id, AL_GAIN,     gain   );
 alSourcefv(id, AL_POSITION, position.asFloat3() );
 alSourcefv(id, AL_VELOCITY, velocity.asFloat3() );
 alSourcef (id, AL_ROLLOFF_FACTOR, rolloff );
 alSourcei (id, AL_LOOPING,  loop   );
 alSourcei (id, AL_SOURCE_RELATIVE, relative );
 ALError("ALSource:bindBuffer() had an error");
 state=stopped;
}

void ALSource::bindSound( ALSound *sound ) {
 alGetError();
 alSourcei (id, AL_BUFFER,   sound->waveform->buffer.id );
 alSourcef (id, AL_PITCH,    pitch=sound->pitch  );
 alSourcef (id, AL_GAIN,     gain=sound->gain   );
 alSourcefv(id, AL_POSITION, position.fromFloat3(sound->position.asFloat3()) );
 alSourcefv(id, AL_VELOCITY, velocity.fromFloat3(sound->velocity.asFloat3()) );
 alSourcef (id, AL_ROLLOFF_FACTOR, rolloff=sound->rolloff );
 alSourcei (id, AL_LOOPING,  loop=sound->loop   );
 alSourcei (id, AL_SOURCE_RELATIVE, relative=sound->relative );
 ALError("ALSource:bindSound() had an error");
 state=stopped;
}

void ALSource::bindStream( ALUREStream *stream ) {
 alGetError();
 alSourcef (id, AL_PITCH,    pitch  );
 alSourcef (id, AL_GAIN,     gain   );
 alSourcefv(id, AL_POSITION, position.asFloat3() );
 alSourcefv(id, AL_VELOCITY, velocity.asFloat3() );
 alSourcef (id, AL_ROLLOFF_FACTOR, rolloff );
 alSourcei (id, AL_LOOPING,  AL_FALSE   ); // handled elsewhere
 alSourcei (id, AL_SOURCE_RELATIVE, relative );
 stream->source=this;
 ALError("ALSource:bindStream() had an error");
 state=stopped;
}

ALSoundManager::ALSoundManager() {
  path=null;  // if its null, we don't use it
  //bufferClamp=MAX_AUDIO_BUFFERS;
  //sourceClamp=MAX_AUDIO_SOURCES;
  error=initialized=muted=hasOggExtension=hasEAX=false;
  monoSources=stereoSources=0;
  version_major=version_minor=0;
  refreshRate=60;
  distance = 10000.0f; gain=1.0f; max_gain=1.0f; min_gain=0.0f; stream_gain=1.0f; sounds_gain=1.0f;
  position.x=0.0f;
  position.y=0.0f;
  position.z=0.0f; 
  velocity.x=0.0f;
  velocity.y=0.0f;
  velocity.z=0.0f;
  orientation.x=0.0f;
  orientation.y=0.0f;
  orientation.z=-1.0f;
  orientation.a=0.0f;
  orientation.b=1.0f;
  orientation.c=0.0f;
  dopplerFactor=0.0f;
  dopplerVelocity=0.0f;
//  timers.AddTimer(new ALSoundManagerTimer(&audio));
 }

#include "Display.h"

float distanceGain(int x, int y, float dampening) {
 float d=idistance(x,y,display.w2,display.h2)/((display.wf+display.hf)/2.0f/2.0f);
 if ( d>2.0f ) d=0.0f;
 else
 if ( d>1.0f ) d=dampening;
 else d=1.0f-d;
 return d;
}

SoundEvent::SoundEvent( const char *fn, ALSource *source, int loops, bool calcLength ) {
 stream=new SoundEventALUREStream;
 if ( !source ) {	 // Bind the buffer with the source.
  stream->source =new ALSource;
  alGenSources(1, &(stream->source->id));
  if(ALError("SoundEvent:alGenSources")) {
   OUTPUT("SoundEvent() had an error.\n" );
   return;
  }
  stream->source->bindStream(stream);
 	//Do another error check and return.
 	if(ALError("SoundEvent:alSource")) {
   OUTPUT("SoundEvent() had an error.\n" );
   return;
  }
 }
 else stream->source=source;
 audio.manager.streams.Append(stream);
 stream->manager=&audio.manager;
 stream->loops=loops;
 if ( !stream->LoadIntoSource(fn,stream->source,calcLength) ) { 
  OUTPUT("SoundEvent() had an error.\n" );
 }
}

void SoundEventALUREStream::OnComplete() {
 parentEvent->stream=null;
 parentEvent->Completed();
}