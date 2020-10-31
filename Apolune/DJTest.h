#pragma once
#include "FastGUI.h"
#include "DJ.h"

class DJTest : public GLWindow {
public:
 Strings tracks;
 Zint tSelected,tScroll;
 Zbool toggle;
 Zdouble musicVolume,overdubVolume;
 void OnLoad() {
  musicVolume=1.0;
  overdubVolume=1.0;
  background.Pick(darkChocolate);
  Fullscreen();
  EACH(dJ.tracks.first,DJTrack,d) tracks.Add(d->name.c_str());
 }
 void Render() {
  fast.text(dJ.Status().c_str(),1024,16+64,false);
  int selected=fast.list("Tracks",&tracks,16,16,256,40,&tSelected.value,&tScroll.value);
  if ( selected >= 0 ) {
   tSelected=selected;
   if ( toggle ) 
   dJ.CutTo(((String *)tracks.Element(selected))->s.c_str(),0.1,true);
   else
   dJ.Play(((String *)tracks.Element(selected))->s.c_str(),12.0,true);
  }
  toggle=fast.toggle("Cut To",toggle,512,512,32,32);
  if ( fast.button("Cut to Voiceover+",512,512+64,256,24) ) {
   dJ.CutTo("track_exploration","track_voiceover");
  }
  if ( fast.button("Overdub Track2",512,512+64+64,256,24) ) {
   dJ.Overdub(((String *)tracks.Element(selected))->s.c_str());
  }
  if ( fast.button("Fade to Silence",512,512+64+128,256,24) ) {
   dJ.FadeToSilence(5.0);
  }
  double was_musicVolume=musicVolume;
  musicVolume=fast.hslider("Music Volume",musicVolume,0.0,1.0,512,256,256,24,false);
  if( was_musicVolume != musicVolume ) dJ.SetMusicVolume(musicVolume);
  double was_overdubVolume=overdubVolume;
  overdubVolume=fast.hslider("Overdub Volume",overdubVolume,0.0,1.0,512,256-64,256,24,false);
  if( was_overdubVolume != overdubVolume ) dJ.SetOverdubVolume(overdubVolume);
 }
};