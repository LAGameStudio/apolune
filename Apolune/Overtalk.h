/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "LinkedList.h"
#include "TextureLibrary.h"
#include "SoundEffect.h"

class Campaign;

ONE(OvertalkBeat,{
 button="Roger";
}) 
 Zstring person,content,mood,seed,button,script;
 Zstring lifeformReference,poseReference;
 Zbool isPlayer,male;
 Zp<GLImage> image;
 Zp<Film> film;
 SoundEffect effect;
 KEYWORDS({
  TXTWORD("name",name)
  else TXTWORD("person",person)
  else TXTWORD("content",content)
  else TXTWORD("mood",mood)
  else TXTWORD("seed",seed)
  else TXTWORD("lifeform",lifeformReference)
  else TXTWORD("pose",poseReference)
  else TAGWORD("player",isPlayer)
  else TAGWORD("male",male)
  else TXTWORD("script",script)
  else TXTWORD("button",button)
  else IMGWORD("image",image)
  else SUBWORD("effect",effect)
  else BADKEYWORD;
 })
 WORDKEYS({
  TXTKEY("name",name)
  TXTKEY("person",person)
  TXTKEY("content",content)
  TXTKEY("mood",mood)
  TXTKEY("seed",seed)
  TXTKEY("lifeform",lifeformReference)
  TXTKEY("pose",poseReference)
  TAGKEY("player",isPlayer)
  TAGKEY("male",male)
  TXTKEY("button",button)
  TXTKEY("script",script)
  IMGKEY("image",image)
  SSUBKEY("effect",effect)
 })
MANY(OvertalkBeat,OvertalkBeatHandle,OvertalkBeatHandles,"otb",OvertalkBeats,{})
 KEYSWORDSGroups("otb",OvertalkBeat)
DONE(OvertalkBeat)

ONE(Overtalk,{})
 Zp<Campaign> campaign;
 OvertalkBeats beats;
 KEYWORDS({
  TXTWORD("name",name)
  else SUBWORD("beats",beats)
  else BADKEYWORD;
 })
 WORDKEYS({
  TXTKEY("name",name)
  SSUBKEY("beats",beats)
 })
MANY(Overtalk,OvertalkHandle,OvertalkHandles,"talk",Overtalks,{})
 KEYSWORDSGroups("talk",Overtalk)
 void AssignUnassigned( Campaign *c );
 void Query( Campaign *c, OvertalkHandles *out );
DONE(Overtalk)

extern Overtalks overtalks;
