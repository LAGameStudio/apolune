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
#pragma once
#include "ListItem.h"
#include "GLWindow.h"
#include "CompositeImage.h"
#include "Vertex.h"
#include "UglyFont.h"

class NotifierMessage : public ListItem {
public:
 CompositeImage icon;
 Vertexd textPosition,fontSize;
 Crayon color;
 string text;
 Zfloat delay,decay;
 Zbool centered;
 NotifierMessage() : ListItem() {
  delay=1.0f;
  fontSize.Set(10.0,14.0,0.0);
 }
 void Render( GLWindow *surface ) {
  Blending(transparency);
  icon.RenderOn(surface,icon.position.x,icon.position.y,icon.position.w,icon.position.h);
  glColor4f(color.rf,color.gf,color.bf,decay/delay);
  MultilineText((char *) text.c_str(), 
   textPosition.x, textPosition.y,
   fontSize.x, fontSize.y, 2.0, centered
  );
  MultilineText((char *) text.c_str(), 
   textPosition.x+1, textPosition.y+1,
   fontSize.x, fontSize.y, 2.0, centered
  );
 }
 void Decay() {
  decay+=FRAMETIME;
 }
};

class NotifierMessages : public LinkedList {
public:
 CLEARLISTRESET(NotifierMessage);
 ~NotifierMessages() { Clear(); }
};

class Notifier : public GLWindow {
public:
 Cartesian iconSize, textStart, textCenter;
 NotifierMessages queue;
 Crayon color;
 Blends blend;
 Zfloat waiting,wait,alpha;
 virtual NotifierMessage *Message( float duration, string text, GLImage *icon ) {
  NotifierMessage *m;
  queue.Append(m=new NotifierMessage);
  m->icon.Overlay(icon);
  m->text=text;
  m->textPosition.Set( (double) textStart.x, (double) textStart.y, 0.0 );
  m->color.fromCrayon(color);
  m->icon.position.SetRect( iconSize.x, iconSize.y, iconSize.w, iconSize.h );
  m->delay=duration;
 }
 virtual NotifierMessage *Centered( float duration, string text, GLImage *icon ) {
  NotifierMessage *m;
  m=Message( duration, text, icon );
  m->centered=true;
  m->textPosition.Set( (double) textCenter.x, (double) textCenter.y, 0.0 );
 }
 void OnLoad() {
  SetPositionAndSize(
   display->w/2-(8+128+16+256+8)/2,
   display->h/2,
   8+128+16+256+8,
   8+128+8
  );
  background.Float( 0.0f, 0.0f, 0.0f, alpha );
  wait=0.5f;
  color.Float(1.0f,0.6f,0.0f,1.0f);
  iconSize.SetRect( 8, 8, 128, 96 );
  textStart.Set(8+128+16,8);
 }
 void Render() {
  NotifierMessage *m=(NotifierMessage *) queue.first;
  if ( m ) {
   m->Decay();
   if ( m->delay <= 0.0f ) { queue.Remove(m); delete m; return; }
   m->Render(this);
  }
 }
};

class StackedNotifier : public Notifier {
};

extern Zpointer<Notifier> notifier, bonusReport;
extern Zpointer<StackedNotifier> stackedNotifier;