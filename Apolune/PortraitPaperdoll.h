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

#include "Delay.h"
#include "Strings.h"
#include "PlayerPortrait.h"

enum MouthTypes {
 mt_8Bit,
 mt_Flat,
 mt_Toothed,
 mt_Vampiric
};

enum MouthMoods {
 mt_None=0,
 mt_Frown=1,
 mt_Tease=2,
 mt_Laugh=3,
 mt_Scream=4,
 mt_Smile=5,
 mt_Chew=6,
 mt_Sing=7,
 mt_Explaining=8,
 mt_Normal=9
};

class MouthController {
public:
 MouthController() {
  mouth=mt_8Bit;
  Mouth(mouth);
  mood=mt_Normal;
  duration=2.0;
 }
 Zp<Film> frown,tease,laugh,scream,smile,openclose,chew,sing;
 Zp<GLImage> vf,ah,oh,ooo,th,ee,nn,mm,puh,ts,ll,yy;
 MouthTypes mouth;
 MouthMoods mood;
 Zp<Film> active=openclose;
 Zbool explainingMode,none;
 Zp<GLImage> mouthClosed,defaultLips;
 Zdouble duration;
 void Mouth( MouthTypes m ) {
  mouth=m;
  switch ( m ) {
   case mt_8Bit:
   {
    frown    =vault.find("Mouths","M4-Frown");
    tease    =vault.find("Mouths","M4-Tease");
    laugh    =vault.find("Mouths","M4-Laugh");
    scream   =vault.find("Mouths","M4-Scream");
    smile    =vault.find("Mouths","M4-Smile");
    openclose=vault.find("Mouths","M4-OpenClose");
    chew     =vault.find("Mouths","M4-Chew");
    sing     =vault.find("Mouths","M4-Sing");
    vf =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0009_vf.png");
    ah =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0017_Ah.png");
    oh =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0005_oh.png");
    ooo=library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0013_ooo.png");
    th =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0014_th.png");
    ee =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0016_Eee.png");
    nn =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0002_Nnn.png");
    mm =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0003_MMmmm.png");
    puh=library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0007s_0002_Singing1.png");
    ts =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0018_T.png");
    ll =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0015_L.png");
    yy =library.Load("data/images/lifeforms/mouths/8bit/Mouth4_0019_Y.png");
    if ( openclose ) mouthClosed=openclose->GetFirstFrame();
   }
   break;
   case mt_Flat:
   {
    frown    =vault.find("Mouths","M1-Frown");
    tease    =vault.find("Mouths","M1-Tease");
    laugh    =vault.find("Mouths","M1-Laugh");
    scream   =vault.find("Mouths","M1-Scream");
    smile    =vault.find("Mouths","M1-Smile");
    openclose=vault.find("Mouths","M1-OpenClose");
    chew     =vault.find("Mouths","M1-Chew");
    sing     =vault.find("Mouths","M1-Sing");
    vf =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0009_vf.png");
    ah =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0017_Ah.png");
    oh =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0005_oh.png");
    ooo=library.Load("data/images/lifeforms/mouths/flat/Mouth1_0013_ooo.png");
    th =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0014_th.png");
    ee =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0016_Eee.png");
    nn =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0002_Nnn.png");
    mm =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0003_MMmmm.png");
    puh=library.Load("data/images/lifeforms/mouths/flat/Mouth1_0007s_0002_Singing1.png");
    ts =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0018_T.png");
    ll =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0015_L.png");
    yy =library.Load("data/images/lifeforms/mouths/flat/Mouth1_0019_Y.png");
    if ( openclose ) mouthClosed=openclose->GetFirstFrame();
   }
   break;
   case mt_Toothed:
   {
    frown    =vault.find("Mouths","M2-Frown");
    tease    =vault.find("Mouths","M2-Tease");
    laugh    =vault.find("Mouths","M2-Laugh");
    scream   =vault.find("Mouths","M2-Scream");
    smile    =vault.find("Mouths","M2-Smile");
    openclose=vault.find("Mouths","M2-OpenClose");
    chew     =vault.find("Mouths","M2-Chew");
    sing     =vault.find("Mouths","M2-Sing");
    vf =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0009_vf.png");
    ah =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0017_Ah.png");
    oh =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0005_oh.png");
    ooo=library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0013_ooo.png");
    th =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0014_th.png");
    ee =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0016_Eee.png");
    nn =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0002_Nnn.png");
    mm =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0003_MMmmm.png");
    puh=library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0007s_0002_Singing1.png");
    ts =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0018_T.png");
    ll =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0015_L.png");
    yy =library.Load("data/images/lifeforms/mouths/teeth/Mouth2_0019_Y.png");
    if ( openclose ) mouthClosed=openclose->GetFirstFrame();
   }
   break;
   case mt_Vampiric:
   {
    frown    =vault.find("Mouths","M3-Frown");
    tease    =vault.find("Mouths","M3-Tease");
    laugh    =vault.find("Mouths","M3-Laugh");
    scream   =vault.find("Mouths","M3-Scream");
    smile    =vault.find("Mouths","M3-Smile");
    openclose=vault.find("Mouths","M3-OpenClose");
    chew     =vault.find("Mouths","M3-Chew");
    sing     =vault.find("Mouths","M3-Sing");
    vf =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0009_vf.png");
    ah =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0017_Ah.png");
    oh =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0005_oh.png");
    ooo=library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0013_ooo.png");
    th =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0014_th.png");
    ee =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0016_Eee.png");
    nn =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0002_Nnn.png");
    mm =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0003_MMmmm.png");
    puh=library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0007s_0002_Singing1.png");
    ts =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0018_T.png");
    ll =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0015_L.png");
    yy =library.Load("data/images/lifeforms/mouths/vampiric/Mouth3_0019_Y.png");
    if ( openclose ) mouthClosed=openclose->GetFirstFrame();
   }
   break;
  }
 }
 void Mood( MouthMoods m ) {
  time=0.0;
  mood=m;
  none=false;
  explainingMode=false;
  duration=1.0;
  switch (m ) {
   case mt_Frown: active=frown;             break;
   case mt_Tease: active=tease;             break;
   case mt_Laugh: active=laugh;             break;
   case mt_Scream: active=scream;           break;
   case mt_Smile: active=smile;             break;
   case mt_Chew: active=chew;               break;
   case mt_Sing: active=sing;               break;
   case mt_Explaining: explainingMode=true; break;
   case mt_Normal:break;
   case mt_None: none=true; break;
  }
 }
 static void DeployMoods( Strings *out ) {
  out->Add("Frown",1);
  out->Add("Tease",2);
  out->Add("Laugh",3);
  out->Add("Scream",4);
  out->Add("Smile",5);
  out->Add("Chew",6);
  out->Add("Sing",7);
  out->Add("Explain",8);
  out->Add("Normal",9);
  out->Add("None",0);
 }
 static void DeployMouthTypes( Strings *out ) {
  out->Add("8-Bit");
  out->Add("Flat");
  out->Add("Round");
  out->Add("Fanged");
 }
 Zdouble time;
 void Render( int x, int y, int w, int h ) {
  time+=FRAMETIME;
  Cartesiand mouthPosition(180.0/384.0*(double)w,170.0/384.0*(double)h, 50.0*iratiod(w,384), 36.0*iratiod(h,384) );
  if ( mood == mt_None ) {
   if ( defaultLips ) {
    StretchiNoAlpha(mouthClosed,crayons.Pick(alabaster),transparency,
     x+(int)mouthPosition.x-(int)(mouthPosition.w/2.0),
     y+(int)mouthPosition.y-(int)(mouthPosition.h/2.0),
     (int)mouthPosition.w,(int)mouthPosition.h );
   }
   return;
  }
  if ( mood == mt_Normal && mouthClosed ) StretchiNoAlpha(mouthClosed,crayons.Pick(alabaster),transparency,
     x+(int)mouthPosition.x-(int)(mouthPosition.w/2.0),
     y+(int)mouthPosition.y-(int)(mouthPosition.h/2.0),
     (int)mouthPosition.w,(int)mouthPosition.h );
  else {
   if ( mood == mt_Explaining ) {
    GLImage *frame = openclose->frame(fmod(duration,1.0),1.0);
    if ( frame ) StretchiNoAlpha(frame,crayons.Pick(alabaster),transparency,
     x+(int)mouthPosition.x-(int)(mouthPosition.w/2.0),
     y+(int)mouthPosition.y-(int)(mouthPosition.h/2.0),
     (int)mouthPosition.w,(int)mouthPosition.h );
   } else if ( active ) {
    double instant=time/duration;
    GLImage *frame = ( instant >= 1.0 ) ? active->GetLastFrame() : active->frame(instant);
    if ( frame ) StretchiNoAlpha(frame,crayons.Pick(alabaster),transparency,
     x+(int)mouthPosition.x-(int)(mouthPosition.w/2.0),
     y+(int)mouthPosition.y-(int)(mouthPosition.h/2.0),
     (int)mouthPosition.w,(int)mouthPosition.h
    );
   }
  }
 }
 void Render( GLImage *expression, int x, int y, int w, int h ) {
  if ( !expression ) Render(x,y,w,h);
  Cartesiand mouthPosition(180.0/384.0*(double)w,170.0/384.0*(double)h, 50.0*iratiod(w,384), 36.0*iratiod(h,384) );
  StretchiNoAlpha(expression,crayons.Pick(alabaster),transparency,
     x+(int)mouthPosition.x-(int)(mouthPosition.w/2.0),
     y+(int)mouthPosition.y-(int)(mouthPosition.h/2.0),
     (int)mouthPosition.w,(int)mouthPosition.h );
 }
};

class PortraitPaperdoll {
public:
 PortraitPaperdoll() { p=null; }
 PlayerPortrait portrait;
 MouthController mouth;
 Zstring time;
 Zbool talking,spacesuit;
 void Set( PlayerPortrait *pp ) {
  portrait.Set(pp);
  mouth.defaultLips=portrait.lips;
 }
 void Set( MouthTypes mouthType, MouthMoods moodSetting ) {
  mouth.Mouth(mouthType);
  mouth.Mood(moodSetting);
 }
 Zstring speaking;
 Delay enunciating;
 Zp<GLImage> expression;
 const char *p;
 void Speak( const char *text ) {
  speaking=text;
  talking=true;
  p=speaking.c_str();
 }
 void Render( int x, int y, int w, int h ) {
  portrait.Render(x,y,w,h,spacesuit,false);
  if ( talking ) {
   enunciating.Between();
   if ( p != null && *p == '\0' ) { talking=false; }
   else if ( !enunciating ) {
    switch ( LOWER(*p) ) {
     case 'c':
     case 'a':
     case 'k':
     case 'e': enunciating.Set(0.1); expression=mouth.ee; break;

     case 'b':
     case 'f':
     case 'v': enunciating.Set(0.2); expression=mouth.vf; break;

     case 'w':
     case 'y': enunciating.Set(0.15); expression=mouth.yy; break;

     case 'm': enunciating.Set(0.05); expression=mouth.mm; break;
     case 'n': enunciating.Set(0.05); expression=mouth.nn; break;

     case 'o': enunciating.Set(0.15); expression=mouth.oh; break;
     case 'u': enunciating.Set(0.15); expression=mouth.ooo; break;

     case 'd':
     case 'l':
     case 'z':
     case 's': enunciating.Set(0.075); expression=mouth.ts; break;
     case 't': enunciating.Set(0.15);  if ( *(p+1) == 'h' ) expression=mouth.th; else expression=mouth.ts; break;

     case ' ':
     case ',':
     case '.': if ( *p==' ' ) { while (*p == ' ') p++; p--; } enunciating.Set(0.25); expression=null; break;

     case '\0': expression=null; talking=false; break;
     default: break;
     break;
    }
    if ( *p != '\0' ) p++;
    enunciating.Reset();
   }
  }
  if ( expression ) mouth.Render(expression,x,y,w,h);
  else mouth.Render(x,y,w,h);
 }
 void RenderHeadOnly( int x, int y, int w, int h ) {
  portrait.RenderHeadOnly(x,y,w,h,spacesuit,false);
  if ( talking ) {
   enunciating.Between();
   if ( p != null && *p == '\0' ) { talking=false; }
   else if ( !enunciating ) {
    switch ( LOWER(*p) ) {
     case 'c':
     case 'a':
     case 'k':
     case 'e': enunciating.Set(0.1); expression=mouth.ee; break;

     case 'b':
     case 'f':
     case 'v': enunciating.Set(0.2); expression=mouth.vf; break;

     case 'w':
     case 'y': enunciating.Set(0.15); expression=mouth.yy; break;

     case 'm': enunciating.Set(0.05); expression=mouth.mm; break;
     case 'n': enunciating.Set(0.05); expression=mouth.nn; break;

     case 'o': enunciating.Set(0.15); expression=mouth.oh; break;
     case 'u': enunciating.Set(0.15); expression=mouth.ooo; break;

     case 'd':
     case 'l':
     case 'z':
     case 's': enunciating.Set(0.075); expression=mouth.ts; break;
     case 't': enunciating.Set(0.15);  if ( *(p+1) == 'h' ) expression=mouth.th; else expression=mouth.ts; break;

     case ' ':
     case ',':
     case '.': if ( *p==' ' ) { while (*p == ' ') p++; p--; } enunciating.Set(0.25); expression=null; break;

     case '\0': expression=null; talking=false; break;
     default: break;
     break;
    }
    if ( *p != '\0' ) p++;
    enunciating.Reset();
   }
   if ( expression ) mouth.Render(expression,x,y,w,h);
   else mouth.Render(x,y,w,h);
  }
 }
};