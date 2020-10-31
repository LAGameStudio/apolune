/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "Audio.h"
#include "Strings.h"

enum SoundEffectPlaybackHints {
 sfx_Singleton=0,
 sfx_OnLoopedUntilOff=1,
 sfx_Impulse=2,
 sfx_Sporadic=3,
 sfx_Combined=4 // play all of the sounds at once
};

ONE(SoundEffect,{
 playback=sfx_Impulse;
 pitchLow=0.9;
 pitchHigh=1.0;
 gainLow=0.9;
 gainHigh=1.0;
})
 SoundEffectPlaybackHints playback;
 Strings sounds; // A cluster of sound sources
 Zdouble pitchLow,pitchHigh;
 Zdouble gainLow,gainHigh;
 void Set(SoundEffect *in) {
  playback=in->playback;
  sounds.Clear(); sounds.Copy(&in->sounds);
  pitchLow=in->pitchLow;
  pitchHigh=in->pitchHigh;
  gainLow=in->gainLow;
  gainHigh=in->gainHigh;
 }
 KEYWORDS({
  KEYWORD("playback",{playback=(SoundEffectPlaybackHints)atoi(w);})
  else KEYWORD("pitchLow",{pitchLow=w;})
  else KEYWORD("pitchHigh",{pitchHigh=w;})
  else KEYWORD("gainLow",{gainLow=w;})
  else KEYWORD("gainHigh",{gainHigh=w;})
  else KEYWORD("sounds",{sounds.fromStringList(w);})
 })
 WORDKEYS({
  WORDKEY("pitchLow",pitchLow)
  WORDKEY("pitchHigh",pitchLow)
  WORDKEY("gainLow",pitchLow)
  WORDKEY("gainHigh",pitchLow)
  SSUBKEYCustom("sounds",sounds,toStringList)
 })
 void Impulse( double distanceAttenuationMultiplier=1.0 ) {
  if ( distanceAttenuationMultiplier < 0.1 ) return;
  float tempPitch = (float)(uniform()*(pitchHigh-pitchLow)+pitchLow);
  float tempGain = (float)(uniform()*(gainHigh-gainLow)+gainLow);
  int selected=upto(32768)%sounds.count;
  string picked=sounds.Get(selected);
  if ( picked.length() == 0 ) return; // failure to find any
  if ( playback == sfx_Impulse || playback == sfx_OnLoopedUntilOff ) { // TODO: support loop
   audio.manager.CueAndPlay(picked.c_str(), tempPitch,tempGain );
  } else {
   audio.manager.CueAndPlayIfNotPlaying(picked.c_str(),tempPitch,tempGain);
  }
 }
 void Singleton( double distanceAttenuationMultiplier=1.0 ) {
  if ( distanceAttenuationMultiplier < 0.1 ) return;
  float tempPitch = (float)(uniform()*(pitchHigh-pitchLow)+pitchLow);
  float tempGain = (float)(uniform()*(gainHigh-gainLow)+gainLow);
  int selected=upto(32768)%sounds.count;
  string picked=sounds.Get(selected);
  if ( picked.length() == 0 ) return; // failure to find any
  audio.manager.CueAndPlayIfNotPlaying(picked.c_str(),tempPitch,tempGain);
 }
 CLONE(SoundEffect,{
  DUPE(playback)
  a->sounds.DuplicateAndPush(&sounds);
  DUPE(pitchLow)
  DUPE(pitchHigh)
  DUPE(gainLow)
  DUPE(gainHigh)
 })
MANY(SoundEffect,SoundEffectHandle,SoundEffectHandles,"sound",SoundEffects,{})
 CLONES(SoundEffect,SoundEffects)
DONE(SoundEffect);
extern SoundEffects soundEffects;