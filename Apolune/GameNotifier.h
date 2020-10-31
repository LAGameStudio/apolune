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

#include "TextureLibrary.h"
#include "TexFont.h"
#include "Audio.h"

#include "FastGUI.h"
#include "MoneyCounter.h"

extern Zint stashed_Icon, stashed_OuterBox;

ONE(GameNotification,{ overlayBlend=transparency; iconTint.Pick(alabaster); overlayTint.Pick(alabaster); alpha=1.0; border=true; })
 Zp<GLImage> icon,overlay;
 Blends overlayBlend;
 Crayon iconTint,overlayTint;
 Zstring sfx;
 Zstring text1,text2; Crayon tint1,tint2;
 Zint x,y;
 Zbool border,showMoney;
 Zdouble oldMoney,newMoney;
 fx_MoneyCounter moneyCounter;
 ///
 Zdouble alpha;
 Cartesian iconBox,textBox,text1Pos,text2Pos;
 Zdouble time;
 Crayon borderColor,bgColor;
 Zbool done;
 void Setup() {
  if ( sfx.length >0 ) audio.manager.CueAndPlayIfNotPlaying(sfx.c_str(),1.0f,1.0f);
  Cartesian textBounds;
  TexFontBoxBounds(&textBounds,glowTermFont,4,-5,15,20,text1.length > text2.length ? text1.c_str() : text2.c_str() );
  if ( text1.length > 0 || text2.length > 0 ) {
   int margin=16;
   iconBox.SetRect(x+128,y+256,72,72);
   text1Pos.SetRect(iconBox.x2+margin,iconBox.y+margin+16,10,14);
   text2Pos.SetRect(iconBox.x2+margin,iconBox.y+margin+4+16+14,10,14);  
   int textBoxWidth=textBounds.w+iconBox.w+margin*2;
   textBox.SetRect(0,0,textBoxWidth+margin*2,textBoxWidth+margin*2);
  } else {
   iconBox.SetRect(x+128,y+256,72,72);
  }
  moneyCounter.SetExtents(text1Pos.x,text1Pos.y-64,textBox.w-32,54);
 }
 void RenderFromCampaign();
 void Render() {
  time+=FRAMETIME;
  alpha=time < 1.0 ? time : time > 5.0 ? 1.0-(time-5.0) : 1.0;
  if ( time > 6.0 ) done=true;
  Blending(transparency);
  if ( border ) {
   borderColor.Pick(skyBlue);
   borderColor.Alpha(alpha);
   borderColor.gl();
   bgColor.Pick(indigo);
   bgColor.Alpha(alpha);
   bgColor.gl();
   art.Element2(borderColor,bgColor,stashed_OuterBox,(double)(iconBox.x-24),(double)(iconBox.y-24),(double)(textBox.w+iconBox.w+48),(double)(iconBox.h+48));
  }
  if ( icon ) {
//   Crayon t=borderColor; t.ScaleByAlphaf((float)alpha);
   Crayon s=iconTint; s.ScaleByAlphaf((float)alpha);
   art.Element(icon,s,transparency,stashed_Icon,(double)iconBox.x,(double)iconBox.y,(double)iconBox.w,(double)iconBox.h);
  }
  if ( overlay ) {
//   Crayon t=borderColor; t.ScaleByAlphaf((float)alpha);
   Crayon s=overlayTint; s.ScaleByAlphaf((float)alpha);
   art.Element(overlay,s,overlayBlend,stashed_Icon,(double)iconBox.x,(double)iconBox.y,(double)iconBox.w,(double)iconBox.h);
  }
  RenderFromCampaign();
  if ( text1.length > 0 || text2.length > 0 ) {
   Crayon t1,t2;
   t1=tint1; /*t1.ScaleByAlphaf((float)alpha);*/ t1.Alpha(alpha);
   t2=tint2; /*t2.ScaleByAlphaf((float)alpha);*/ t2.Alpha(alpha);
   Text(text1,t1,transparency,text1Pos.x,text1Pos.y,10,14,2.0,1.0,false);
   Text(text2,t2,transparency,text2Pos.x,text2Pos.y,10,14,2.0,1.0,false);
  }
  if (showMoney) {
   moneyCounter.Between();
   moneyCounter.Render();
  }
 }
MANY(GameNotification,GameNotificationHandle,GameNotificationHandles,"GameNotification",GameNotifications,{})
DONE(GameNotification);

class GameNotifier {
public:
 GameNotifier() {
  if ( stashed_Icon == 0 )     stashed_Icon=art.stash.GenRoundRect(0.2);
  if ( stashed_OuterBox == 0 ) stashed_OuterBox=art.stash.GenRoundRect(0.1,0.2);
 }
 GameNotifications notes;
 void Checkpoint( bool verbose=false ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/gamesaved.png");
  gn->text1=verbose ? "Checkpoint Reached" : "";
  gn->tint1.Pick(cyan);
  gn->text2=verbose ? "Your game has been saved." : "";
  gn->tint2.Pick(alabaster);
  gn->sfx=("data/sounds/UI/gamesaved.wav");
  gn->border=false;
  gn->x=128+32;
  gn->y=-192;
  notes.Append(gn);
 }
 void Bonus( const char *statname, int amount ) {
  if ( amount > 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icon/gain.png");
   gn->text1="Bonus";
   gn->tint1.Pick(gold);
   gn->text2=statname;
   gn->tint2.Pick(macNcheese);
   gn->sfx=("data/sounds/UI/Pickup_Coin.wav");
   notes.Append(gn);
  }
 }
 void Taught( const char *skname, int amount ) {
  if ( amount > 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icon/brain.png");
   gn->text1="Skill Increased";
   gn->tint1.Pick(cyan);
   gn->text2=skname;
   gn->tint2.Pick(alabaster);
   notes.Append(gn);
  }
 }
 void Paid( int amount );
 void Charged( int amount );
 void GainedXP( int amount );
 void LevelUp();
 void Fuel( int amount ) {
  if ( amount == 0 ) return;
  if ( amount > 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icons/gas.png");
   gn->sfx=("data/sounds/FX/gainfuel.wav");
   gn->text1="Your ship gained";
   gn->tint1.Pick(skyBlue);
   gn->text2=FORMAT("%d jump fuel!",amount);
   gn->tint2.Pick(rgb3F3);
   notes.Append(gn);
  } else if ( amount < 0 ) {
   GameNotification *gn=new GameNotification;
   gn->icon=library.Load("data/images/icon/loss.png");
   gn->sfx=("data/sounds/FX/drama.wav");
   gn->text1="You lost";
   gn->tint1.Pick(macNcheese);
   gn->text2=FORMAT("%d jump fuel!",abs(amount));
   gn->tint2.Pick(rgbF33);
   notes.Append(gn);
  }
 }
 void CrewBoarded( const char *crewname ) {
  GameNotification *gn=new GameNotification;
  gn->text1="New Crew Member";
  gn->icon=library.Load("data/images/icon/crew.png");
  gn->sfx=("data/sounds/UI/Randomize16.wav");
  gn->tint1.Pick(cyan);
  gn->text2=crewname;
  gn->tint2.Int(32,255,32,255);
  notes.Append(gn);
 }
 void SkillFailed( const char *skillname ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/loss.png");
  gn->sfx=("data/sounds/UI/Randomize14.wav");
  gn->text1="Failed";
  gn->tint1.Pick(red255);
  gn->text2=skillname;
  gn->tint2.Int(32,255,32,255);
  notes.Prepend(gn);
 }
 void StatFailed( const char *statname ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/loss.png");
  gn->sfx=("data/sounds/UI/Randomize14.wav");
  gn->text1="Failed";
  gn->tint1.Pick(red255);
  gn->text2=statname;
  gn->tint2.Int(32,255,32,255);
  notes.Prepend(gn);
 }
 void NewSkill( const char *skillname ) {
  GameNotification *gn=new GameNotification;
  gn->text1="New Skill Learned";
  gn->icon=library.Load("data/images/icon/brain.png");
  gn->tint1.Pick(cyan);
  gn->text2=skillname;
  gn->tint2.Int(32,255,32,255);
  gn->sfx=("data/sounds/UI/Pickup_Coin.wav");
  notes.Append(gn);
 }
 void NewQuest( const char *questname ) {
  GameNotification *gn=new GameNotification;
  gn->text1="New Quest Added";
  gn->icon=library.Load("data/images/icon/quest.png");
  gn->sfx=("data/sounds/FX/quest_complete.wav");
  gn->tint1.Pick(macNcheese);
  gn->text2=questname;
  gn->tint2.Int(127,255,32,255);
  notes.Prepend(gn);
 }
 void QuestComplete( const char *questname ) {
  GameNotification *gn=new GameNotification;
  gn->icon=library.Load("data/images/icon/quest.png");
  gn->sfx=("data/sounds/FX/quest.wav");
  gn->text1="Quest Completed!";
  gn->tint1.Pick(green255);
  gn->text2=questname;
  gn->tint2.Pick(macNcheese);
  gn->Setup();
  notes.Prepend(gn);
 }
 Zdis<GameNotification> showing;
 Zdouble time;
 void ShowNext() {
  time=0.0;
  if ( notes.count > 0 ) {
   showing.Recycle((GameNotification *)notes.first);
   notes.Remove(notes.first);
   showing->Setup();
  } else if ( showing ) {
   showing.Recycle(null);
  }
 }
 void Between() {
  if ( !showing || (showing && showing->done) ) ShowNext();
#if defined(DEBUG)
  if ( input.KeyHit(DX_0) ) NewQuest( "A test quest" );
#endif
 }
 void Render() {
  Between();
  if ( showing ) showing->Render();
 }
};