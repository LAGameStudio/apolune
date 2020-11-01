/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
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

#include "Overtalk.h"
#include "Delay.h"
#include "PlayerPortrait.h"
#include "PortraitPaperdoll.h"
#include "Campaign.h"

class Player;

ONE(Overtalker,{
 playDelay.Set(5.0);
 playDelay.Reset();
})
 Zp<GLWindow> back_to;
 Cartesian extents,iconArea;
 Zp<Overtalk> o;
 Zdouble time;
 Delay nextTime;
 Zbool hasButton,buttonPressed;
 Zint buttonWidth,buttonHeight;
 Zbool hasPortrait;
 PlayerPortrait portrait;
 PortraitPaperdoll portraitPaperdoll;
 Zp<Species> species;
 Zp<Lifeform> lifeform;
 Zp<Poseable2d> paperdoll;
 Zp<Pose2dSequence> pose;
 Zdouble poseTime;
 Zp<OvertalkBeat> ob;
 Zint seed;
 Zstring content;
 Delay playDelay,gamepadButtonDelay;
 Zbool started;
 Zdouble timeout;
 void Set( Overtalk *o ) {
  this->o=o;
  SetBeat((OvertalkBeat *)o->beats.first);
 }
 void SetBeat( OvertalkBeat *ob );
 void Between() {
  time+=FRAMETIME;
  if ( timeout > 0.0 ) timeout-=FRAMETIME;
  if ( ob ) {
   if ( !hasButton ) nextTime.Between();
   if ( !nextTime || buttonPressed ) {
    ExecuteScript(ob->script.c_str());
    SetBeat((OvertalkBeat *)ob->next);
   }
  }
 }
 void ExecuteScript(const char *s);
 void Render() {
  if ( !started ) {
   playDelay.Between();
   if ( !playDelay ) {
    started=true;
    if ( ob ) ob->effect.Impulse();
   }
   return;
  }
  Between();
  if ( ob ) {
   if ( species && paperdoll && pose ) {
    Cartesian mouse; mouse.Set(iconArea.x+iconArea.w/2,iconArea.y+iconArea.h/2);
    Cartesian dollArea; dollArea.SetRect(iconArea.x-iconArea.w*2,iconArea.y-iconArea.h*2,iconArea.w*4,iconArea.h*4);
    art.Element2(crayons.Pick(skyBlue),crayons.Pick(indigo),none,art.stash.rr_Standard+10,dollArea.x+dollArea.w/4,dollArea.y+dollArea.h/4+dollArea.h/8,dollArea.w/2,dollArea.h/2);
    pose->Render(&paperdoll->parts,dollArea,mouse,poseTime);
    poseTime+=FRAMETIME;
    if ( poseTime > pose->totalDuration ) poseTime=0.0;
   } else {
    art.Element2(crayons.Pick(skyBlue),crayons.Pick(indigo),none,art.stash.rr_Standard+10,iconArea.x,iconArea.y,iconArea.w,iconArea.h);
    if ( ob->film ) {
    }
    if ( ob->image ) {
     art.Stretch(ob->image,crayons.Pick(alabaster),transparency,iconArea.x,iconArea.y,iconArea.w,iconArea.h);
    }
    if ( seed > 0 || hasPortrait ) {
     portraitPaperdoll.RenderHeadOnly(iconArea.x-64+16,iconArea.y,256,192);
    }
   }
   art.Element2(crayons.Pick(skyBlue),crayons.Pick(indigo),none,art.stash.rr_Standard+10,extents.x,extents.y,extents.w,extents.h);
   WriteTexFont(glowTermFont,crayons.Pick(macNcheese),transparency,extents.x+16,extents.y+16,16,20,4,ob->person.c_str(),-2);
   WriteTexFont(glowTermFont,crayons.Pick(orange),transparency,extents.x+16,extents.y+16+(20+4),16,20,4,content.c_str(),-2);
   if ( hasButton ) {
    gamepadButtonDelay.Between();
    int to=(int)timeout;
    if ( fast.button(digicaps,timeout>0.0?FORMAT("%d",UMAX(1,to)).c_str():ob->button.c_str(),
         (int)(extents.x2-buttonWidth-16),(int)(extents.y2-buttonHeight/2),(int)buttonWidth,(int)buttonHeight,16,20,false) ) {
     if ( timeout <= 0.0 ) {
      buttonPressed=true;
      audio.manager.CueAndPlay("data/sounds/UI/tinybeep.wav",1.0f,0.8f,false,true);
     }
    }
   }
  }
 }
MANY(Overtalker,OvertalkerHandle,OvertalkerHandles,"Overtalker",Overtalkers,{})
 void Add( GLWindow *g, Overtalk *ot, having highPriority ) {
  Overtalker *o=new Overtalker;
  o->back_to=g;
  o->Set(ot);
  Prepend(o);
 }
 void Add( GLWindow *g, Overtalk *ot ) {
  Overtalker *o=new Overtalker;
  o->back_to=g;
  o->Set(ot);
  Append(o);
 }
 void Render (){
  Overtalker *active=(Overtalker *)first;
  if ( !active ) return;
  active->Render();
  if ( !active->ob ) {
   Remove(active);
   delete active;
   return;
  }
 }
DONE(Overtalker)

