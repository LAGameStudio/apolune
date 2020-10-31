#pragma once
#include "Audio.h"
#include "Vertexd.h"
#include "Trigger.h"

ONE(DJTrack,{
 volScale=1.0; // limits the volume on tracks that are louder, scaling master music volume
})
 Zstring named,filename;
 Zdouble volScale;
 KEYWORDS({
  KEYWORD("name",{name=w;})
  else KEYWORD("named",{named=w;})
  else KEYWORD("filename",{filename=w;})
  else NUMWORD("volScale",volScale)
 })
 WORDKEYS({
  WORDKEY("name",name)
  WORDKEY("named",named)
  WORDKEY("filename",filename)
  NUMBKEY("volScale",volScale)
 })
 CLONE(DJTrack,{
  DUPE(name)
  DUPE(named)
  DUPE(filename)
  DUPE(volScale)
 })
MANY(DJTrack,DJTrackHandle,DJTrackHandles,"dj",DJTracks,{})
 KEYSWORDSGroups("dj",DJTrack)
 CLONES(DJTrack,DJTracks)
DONE(DJTrack);

class DJStream {
public:
 Zp<ALUREStream> stream;
 Zdouble channelVolume;
 Zbool overdub;
 Zdouble overdubVolume;
 Zp<DJTrack> track,was_playing;
 Zint loops;
 DJStream() {
  overdubVolume=1.0;
 }
 void Set(DJStream *in) {
  stream=in->stream;
  track=in->track;
  channelVolume=in->channelVolume;
  overdub=in->overdub;
  overdubVolume=in->overdubVolume;
  was_playing=in->was_playing;
  loops=in->loops;
 }
 void SetChannelVolume( double v ) {
  if ( track ) channelVolume=v * track->volScale;
  else channelVolume=v;
  if ( stream && stream->source ) {
   stream->gain=stream->source->gain=(float)(channelVolume * (overdub ? overdubVolume : audio.manager.stream_gain));
   stream->source->Update();
  }  
 }
 void Stop() {
  overdub=false;
  if ( stream ) {
   stream->Stop();
   audio.manager.streams.Delete(stream.pointer);
   stream=null;
  }
  if ( track ) {
   was_playing=track;
   track=null;
  }
 }
 void Start( DJTrack *in, bool loopPerpetually=false, bool overdubbed=false ) {
  if ( stream ) Stop();
  track=in;
  loops=loopPerpetually?-1:0;
  overdub=overdubbed;
  stream=audio.manager.AddStream(in->filename.c_str(),null,loops,true);
  stream->Play();
 }
 std::string Status() {
  return FORMAT("Stream: %s\nSource: %s\nTrack: %s\nOverdub? %s\nWas Playing: %s\nLoops: %d\nVolume: %1.1f %%",
   stream ? "yes" : "no",
   stream ? (stream->Playing() ? "playing" : "stopped") : "none",
   track ? track->name.c_str() : "none",
   overdub ? FORMAT("yes - %1.1f",(double)overdubVolume).c_str() : "no",
   was_playing ? was_playing->name.c_str() : "none",
   (int) loops, (double) channelVolume 
  );
 }
};

ONE(DJEvent,{})
 Zp<DJStream> actingOn;
 Zp<DJTrack> toPlay;
 Zbool loopIt,overdubIt;
 Zdouble delay,expired,duration;
 Zbool fading,stopping,playAfterDelay,swapAfter;
 Zdouble startVol,endVol;
 Zbool completed;
 void Between() {
  if ( completed ) return;
  if ( delay > 0.0 ) { delay-=FRAMETIME; if ( delay <= 0.0 ) delay=0.0; }
  if ( delay <= 0.0 ) {
   if ( playAfterDelay && toPlay ) {
    actingOn->Start(toPlay,loopIt,overdubIt);
    toPlay=false;
    ApplyVolume(startVol);
   } 
   expired+=FRAMETIME;
   if ( expired > duration ) {
    completed=true;
    ApplyVolume(endVol);
    if ( stopping ) actingOn->Stop();
    return;
   }
   static Vertexd point;
   Line line(startVol,0.0,endVol,0.0);
   line.Point(expired/duration,&point);
   ApplyVolume(point.x);
  }
 }
 void ApplyVolume( double v ) {
  if ( actingOn ) {
   actingOn->SetChannelVolume(v);
  }
 }
 std::string Status() {
  return FORMAT(
   "Channel: %s\nSource: %s\nTo Play Track: %s\nFade: %s Stop: %s Play: %s Overdub: %s Swap: %s Completed: %s\nDelta Volume: %1.1f to %1.1f (end)\n%d %%    %1.1f / %1.1f = %1.1f",
   actingOn ? "yes" : "no",
   actingOn ? (actingOn->stream ? (actingOn->stream->source ? I2S(actingOn->stream->source->id) : "N/A") : "stream stopped") : "nothing", 
   toPlay ? toPlay->name.c_str() : "no",
   fading ? "yes" : "no ",
   stopping ? "yes" : "no ",
   playing ? "yes" : "no ",
   overdubIt ? "yes" : "no",
   swapAfter ? "yes" : "no ",
   completed ? "yes" : "no ",
   startVol.value, endVol.value,
   (int) (expired/duration*100.0), (double)expired, (double) duration, (double) (expired/duration)
  );
 }
MANY(DJEvent,DJEventHandle,DJEventHandles,"",DJEvents,{})
 DJEvent *Between() {
  DropComplete();
  DJEvent *one=(DJEvent *) first;
  if ( one ) one->Between();
  return one;
 }
 void DropComplete() {
  FOREACHN(DJEvent,d,{
   if ( d->completed ) {
    Remove(d);
    delete d;
   }
  });
 }
 void Find( DJStream *test, DJEventHandles *out ) {
  FOREACH(DJEvent,d) if ( test == d->actingOn ) out->Add(d);
 }
 void Find( DJTrack *test, DJEventHandles *out ) {
  FOREACH(DJEvent,d) if ( test == d->toPlay ) out->Add(d);
 }
 void ActingOn( DJStream *is ) {
  FOREACH(DJEvent,d) d->actingOn=is;
 }
 void RemoveAll( DJEventHandles *in ) {
  EACH(in->first,DJEventHandle,dh) {
   Remove(dh->p.pointer);
   delete dh->p.pointer;
  }
  in->Clear();
 }
 void RemoveRelated( DJStream *in ) {
  FOREACHN(DJEvent,d,{ if ( d->actingOn == in ) { Remove(d); delete d; } })
 }
 std::string Status() {
  Zstring eventsString;
  DJEvent *one=(DJEvent *) first;
  if ( one ) eventsString=one->Status();
  return FORMAT("DKEvents: %d\n",(int)count)+eventsString;
 }
DONE(DJEvent);

class DJ {
public:
 DJTracks tracks;
 DJStream A,B;
 DJEvents eventsA,eventsB;
 Zp<Zdouble> overdubVolume;
 Zp<Zdouble> musicVolume;
 void SetMusicVolume( double d ) {
  audio.SetMusicVolume((float)d);
  if ( musicVolume ) musicVolume->value=d;
  A.SetChannelVolume(A.channelVolume);
  B.SetChannelVolume(B.channelVolume);
 }
 void SetOverdubVolume( double d ) {
  if ( overdubVolume ) overdubVolume->value=d;
  A.overdubVolume=d;
  B.overdubVolume=d;
  A.SetChannelVolume(A.channelVolume);
  B.SetChannelVolume(B.channelVolume);
 }
 void fromString( const char *s ) { tracks.fromString(s); }
 void FadeToSilence( double fadeLength=5.0 ) {
  eventsA.Clear();
  eventsB.Clear();
  if ( A.stream ) {
   DJEvent *e=new DJEvent;
   e->stopping=true;
   e->duration=fadeLength;
   e->startVol=A.channelVolume;
   e->endVol=0.0;
   e->actingOn=&A;
   eventsA.Append(e);
  }
  if ( B.stream ) {
   DJEvent *e=new DJEvent;
   e->stopping=true;
   e->duration=fadeLength;
   e->startVol=B.channelVolume;
   e->endVol=0.0;
   e->actingOn=&B;
   eventsB.Append(e);
  }
 }
 // If you want to leave "A" playing but change "B"
 void Overdub( const char *s, bool loopPerpetually=false, double fadeInLength=0.1 ) {
  DJTrack *d=tracks.firstNamed(s);
  B.Stop();
  eventsB.Clear(); 
  DJEvent *e=new DJEvent;
  e->actingOn=&B;
  e->playAfterDelay=true;
  e->toPlay=d;
  e->loopIt=loopPerpetually;
  e->overdubIt=true;
  e->startVol=0.0;
  e->endVol=1.0;
  eventsB.Append(e);
 }
 void CutTo( const char *music, const char *voiceover, double musicFadeInTime=2.0 ) {
  DJTrack *d=tracks.firstNamed(music);
  DJTrack *j=tracks.firstNamed(voiceover);
  if ( !d || !j ) { OUTPUT("DJ:CutTo(2):Track Not Found Error\n"); return; } 
  {
   A.Stop(); eventsA.Clear();
   DJEvent *a=new DJEvent;  // Defer voiceover to avoid hardware blocking
   a->actingOn=&A;
   a->delay=1.0;
   a->toPlay=j;
   a->playAfterDelay=true;
   a->overdubIt=true;
   a->startVol=1.0;
   a->endVol=1.0;
   eventsA.Append(a);
  }
  {
   B.Stop(); eventsB.Clear();
   DJEvent *b=new DJEvent;
   b->actingOn=&B;
   b->duration=musicFadeInTime;
   b->toPlay=d;
   b->playAfterDelay=true;
   b->loopIt=true;
   b->startVol=0.0;
   b->endVol=1.0;
   eventsB.Append(b);
  }
 }
 void CutTo( const char *s, double fadeLength=0.1, bool loopPerpetually=true ) {
  DJTrack *d=tracks.firstNamed(s);
  if ( !d ) return;
  B.Stop();
  eventsB.Clear(); 
  DJEvent *e=new DJEvent;
  e->actingOn=&B;
  e->playAfterDelay=true;
  e->toPlay=d;
  e->loopIt=loopPerpetually;
  e->startVol=1.0;
  e->endVol=1.0;
  e->swapAfter=true;
  eventsB.Append(e);
 }
 void Play( const char *s, double fadeLength=5.0, bool loopPerpetually=false ) {
  DJTrack *d=tracks.firstNamed(s);
  if ( !d ) return;
  if ( A.stream && B.stream ) {
   eventsB.Clear();
   B.stream->Stop();
  }
  eventsA.Clear();
  eventsB.Clear();
  if ( A.stream ) {
   DJEvent *e=new DJEvent;
   e->actingOn=&A;
   e->duration=fadeLength;
   e->delay=0.1;
   e->startVol=A.channelVolume;
   e->endVol=0.0;
   e->stopping=true;
   eventsA.Append(e);
   e=new DJEvent;
   e->actingOn=&B;
   e->duration=fadeLength;
   e->loopIt=loopPerpetually;
   e->startVol=0.0;
   e->endVol=1.0;
   e->toPlay=d;
   e->playAfterDelay=true;
   e->swapAfter=true;
   eventsB.Append(e);
  } else if ( B.stream ) {
   DJEvent *e=new DJEvent;
   e->actingOn=&B;
   e->duration=fadeLength;
   e->delay=0.1;
   e->startVol=B.channelVolume;
   e->endVol=0.0;
   e->stopping=true;
   eventsB.Append(e);
   e=new DJEvent;
   e->actingOn=&A;
   e->duration=fadeLength;
   e->loopIt=loopPerpetually;
   e->startVol=0.0;
   e->endVol=1.0;
   e->toPlay=d;
   e->playAfterDelay=true;
   eventsA.Append(e);
  } else {
   DJEvent *e=new DJEvent;
   e->actingOn=&A;
   e->duration=fadeLength;
   e->loopIt=loopPerpetually;
   e->startVol=0.0;
   e->endVol=1.0;
   e->toPlay=d;
   e->playAfterDelay=true;
   eventsA.Append(e);
  }
 }
 void SwapChannels() {
  DJStream T;
  T.Set(&A);
  A.Set(&B);
  B.Set(&T);
  DJEvents eventsT;
  eventsT.Concat(&eventsA);
  eventsA.Concat(&eventsB);
  eventsB.Concat(&eventsT);
  eventsA.ActingOn(&A);
  eventsB.ActingOn(&B);
 }
 std::string Status() {
  return string("Channel A: (music)\n")+A.Status()+eventsA.Status()+string("\n---\nChannel B: (music/overdub)\n")+B.Status()+eventsB.Status();
 }
 void Merge( DJTracks *in ) {
  EACH(in->first,DJTrack,d) if ( d->name.length > 0 && !tracks.firstNamed(d->name.c_str()) && file_exists(d->filename.c_str()) ) {
   tracks.Append(d->Duplicate());
  }
 }
 void Between() {
  DJEvent *e=eventsA.Between();
  if ( e && e->completed && e->swapAfter ) SwapChannels();
  e=eventsB.Between();
  if ( e && e->completed && e->swapAfter ) SwapChannels();
 }
 ~DJ() {
  eventsA.Clear();
  eventsB.Clear();
  FadeToSilence(0.25);
  while ( eventsA.count > 0 && eventsB.count > 0 ) Between();
 }
};


extern DJ dJ;