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

#include "Presentation.h"
#include "PresentationEditor.h"
#include "QMap.h"
#include "Player.h"

class QMapViewer : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<QMap> qmap;
 Zp<PresentationProfile> presiprofi;
 Zdis<Presentation> presi;
 Zp<FBOColor> fboPresi;
 Zp<GLImage> completedIcon;
 Zdouble time;
 Zbool justATest;
 QMapViewer() : GLWindow() {}
 QMapViewer( QMap *q ) : GLWindow() { Set(q); }
 void OnLoad() {
  Fullscreen();
  background.Pick(black);
  completedIcon=library.Load("data/images/icon/check.png");
 }
 void Set( QMap *q ) {
  qmap=q;
  PresentationProfile *pp=presentationProfiles.named(qmap->presentation.c_str());
  SetPresentation(pp);
  ExecuteScript(q->script.c_str());
 }
 void SetPresentation(PresentationProfile *p) {
  if ( presi ) { presi.Deinstantiate(); }
  if  ( !p ) return;
  presi=p->Create();
  presi->loop=p->looping;
  presi->freezeLast=p->freezeLast;
  presiprofi=p;
  if ( back_to ) back_to->visible=false;
  fboPresi=&fbo1080p6;
  Fullscreen();
  background.fromCrayon(presiprofi->background);
  presi->Init(false);
 }
 void BetweenPresi() {
  if ( !presi ) return;
  presi->Between();
  presi->BetweenFrames();
  if ( presi->looped > 0 && !presi->freezeLast ) {
   presi.Recycle(presiprofi->Create());
   presi->loop=presiprofi->looping;
   presi->freezeLast=presiprofi->freezeLast;
   presi->Init(false);
   presi->Between();
   presi->BetweenFrames();   
  }
 }
 void RenderPresi() {
  if ( !presi ) return;
  fboPresi->background.Pick(clear);
  fboPresi->Bind();
  presi->Render();
  fboPresi->Unbind();
  fboPresi->blend=presiprofi->blend;
  fboPresi->tint.Pick(alabaster);
  fboPresi->RenderUpsideDown(0,0,(int)w,(int)h);
 }
 void RenderOverlay() {}
 void Between() {
  if ( destroyMe || !visible ) return;
  BetweenPresi();
  time+=FRAMETIME;
 }
 void Render() {
  if ( destroyMe || !visible ) return;
  art.PushViewport(w,h);
  if ( qmap->background ) StretchiNoAlpha(qmap->background,qmap->bgTint,transparency,0,0,w,h);
  RenderPresi();
  QMapMarker *hoveredMarker=null;
  EACH(qmap->markers.first,QMapMarker,q) if ( !player || q->requires.length == 0 || player->cloud.match(q->requires.c_str()) ) {
   Cartesian pen(q->xywh.x+q->xywh.w/2,q->xywh.y+q->xywh.h/2);
   Cartesian area; area.Corners(q->xywh.x,q->xywh.y,q->xywh.x2,q->xywh.y2);
   GLImage *img=q->image;
   bool completed=player->completed.match(q->markCompleted.c_str())!=null;
   if ( completed  ) {
    if ( img ) StretchiNoAlpha(img,q->tint,q->b,area.x,area.y,area.w,area.h);
    if ( completedIcon ) StretchiNoAlpha(completedIcon,q->tint,q->b,area.x2+2,area.y,area.w,area.h);
    fast.text(digicaps,q->caption.c_str(),pen.x,pen.y);
    continue;
   }
   bool hovered=fast.hoverarea(area.x,area.y,area.w,area.h);
   if ( hovered ) {
    hoveredMarker=q;
    q->hover.Singleton();
   }
   if ( hovered && q->enlargeOnHover ) {
    area.SetRect(area.x-16,area.y-16,area.w+32,area.h+32);
    fast.text(digicaps,q->description.c_str(),w/2,h-32);
   }
   if ( hovered && fast.clicked && !completed ) {
    if ( !justATest ) ExecuteScript(q->script.c_str());
    destroyMe=true;
    art.PopViewport();
    if ( back_to ) back_to->visible = true;
    return;
   }
   if ( q->film ) {
    q->film->frame(fmod(time,q->filmDuration),q->filmDuration);
   }
   if ( q->captionIsButton ) {
    if ( img ) StretchiNoAlpha(img,q->tint,q->b,area.x,area.y,area.w,area.h);
    if ( q->buttonOnHover ) {
     if ( hovered ) {
      if ( fast.button(digicaps,q->caption.c_str(),pen.x,pen.y) ) {}
     }
    } else {
     if ( fast.button(digicaps,q->caption.c_str(),pen.x,pen.y) ) {}
    }
   } else if ( img && q->showAsButton ) {
    fast.text(digicaps,q->caption.c_str(),pen.x,pen.y);
    if ( fast.button(img,area.x,area.y,area.w,area.h) ) {}
   } else {
    if ( q->showAsButton ? fast.backlessbutton("",area.x,area.y,area.w,area.h) : fast.buttonarea(area.x,area.y,area.w,area.h) ) {
    }
    if ( img ) StretchiNoAlpha(img,q->tint,q->b,area.x,area.y,area.w,area.h);
   }
   if ( !q->captionIsButton && (!q->buttonOnHover) ) {
    fast.SaveState();
    fast.tfont.w/=2;
    fast.tfont.h/=2;
    fast.text(digicaps,q->caption.c_str(),area.x+area.w/2,area.y2+fast.tfont.h,crayons.Pick(green255));
    fast.RecallState();
   }
  }
  if ( hoveredMarker != null ) {
   if ( !hoveredMarker->captionIsButton && hoveredMarker->buttonOnHover ) {
    Cartesian area; area.Corners(hoveredMarker->xywh.x,hoveredMarker->xywh.y,hoveredMarker->xywh.x2,hoveredMarker->xywh.y2);
    fast.SaveState();
    fast.tfont.w/=2;
    fast.tfont.h/=2;
    fast.text(digicaps,hoveredMarker->caption.c_str(),area.x+area.w/2,area.y2+fast.tfont.h,crayons.Pick(green255));
    fast.RecallState();
   }
  }
  RenderOverlay();
  fast.text(digicaps,qmap->area_name.c_str(),w/2-WidthTexFont(digicaps,fast.tfont.w,fast.charSpace,qmap->area_name.c_str())/2,32);
  if ( qmap->description.length > 0 ) {
  }
  if ( player ) player->notifier.Render();
  art.PopViewport();
 }
 bool ExecuteScript( const char *script );
 void Return() {
  if ( back_to ) {
   back_to->visible=true;
  } else {
   if ( player && player->interiorGameView ) player->interiorGameView->visible=true;
   else if ( player && player->spaceGameView ) player->spaceGameView->visible=true;
  }
  this->destroyMe=true;
 }
};

