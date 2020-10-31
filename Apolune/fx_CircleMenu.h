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
#include "macros.h"
#include "Audio.h"
#include "fx_FireAndForget.h"
#include "FDP2d.h"

class MenuItem : public ListItem {
public:
 Blends bgBlend,blend;
 Crayon bgTint,selectTint;
 Crayon tint,text;
 GLImage *backing;
 GLImage *icon;
 string label;
 string sound_hover;
 string sound_unhover;
 FDPNode *node;
 int w,h,textSpacing;
 double fw,fh,labelWidth;
 bool hovering,clicking;
 bool unrolled;
 bool opening,closing;
 float rollDuration;
 float hoverTime,sinceHovering,clickDelay;
 Cartesian location;
 MenuItem( string label, GLImage *icon ) : ListItem() {
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  bgBlend=transparency;
  bgTint.Float(0.0f,0.0f,0.2f,1.0f);
  blend=additive;
  tint.Pick(alabaster);
  text.Pick(gold);
  this->icon=icon;
  SetLabel(label);
  w=32;
  h=32;
  textSpacing=8;
  node=null;
  fw=7.5;
  fh=10.0;
  hoverTime=0.0f;
  sinceHovering=0.0f;
  clickDelay=0.0f;
  opening=true;
  closing=false;
  hovering=clicking=false;
  rollDuration=1.0f;
  sound_hover=string("");
  sound_unhover=string("");
  unrolled=false;
 }
 virtual void OnClick() {}
 void SetLabel( string label ) {
  this->label=label;
  labelWidth=TextLength(label,fw,fh);
 }
 void RollRight() {
  float perc=hoverTime/rollDuration;
  Crayon drawing;
  drawing.fromCrayon(tint);
  if ( blend == transparency ) drawing.af=(perc);
  else drawing.ScaleByAlphaf(perc);
  QuadiNoAlpha(icon,drawing,blend,location.x,location.y,w,h);
  drawing.fromCrayon(text);
  drawing.ScaleByAlphaf(perc);
  double roll=(double)textSpacing+labelWidth;
  scissors.Push(location.x,location.y,w+(int)(roll*perc),h);
  Blending(none);
  glColor4fv(text.floats);
  Text(label,(double)(location.x+w),(double)(location.y+h),fw,fh,false);   
  scissors.Pop();
 }
 void RollLeft() {
  float perc=hoverTime/rollDuration;
  Crayon drawing;
  drawing.fromCrayon(tint);
  if ( blend == transparency ) drawing.af=(perc);
  else drawing.ScaleByAlphaf(perc);
  QuadiNoAlpha(icon,drawing,blend,location.x,location.y,w,h);
  drawing.fromCrayon(text);
  drawing.ScaleByAlphaf(perc);
  double roll=(double)textSpacing+labelWidth;
  scissors.Push(location.x-(int)(roll*perc),location.y,w+(int)(roll*perc),h);
  Blending(none);
  glColor4fv(text.floats);
  Text(label,(double)(location.x-roll),(double)(location.y),fw,fh,false);   
  scissors.Pop();
 }
 void UnrollRight() {
  float perc=1.0f-sinceHovering/rollDuration;
  Crayon drawing;
  drawing.fromCrayon(tint);
  if ( blend == transparency ) drawing.af=(perc);
  else drawing.ScaleByAlphaf(perc);
  QuadiNoAlpha(icon,drawing,blend,location.x,location.y,w,h);
  drawing.fromCrayon(text);
  drawing.ScaleByAlphaf(perc);
  double roll=(double)textSpacing+labelWidth;
  scissors.Push(location.x,location.y,w+(int)(roll*perc),h);
  Blending(none);
  glColor4fv(text.floats);
  Text(label,(double)(location.x+w),(double)(location.y+h),fw,fh,false);   
  scissors.Pop();
 }
 void UnrollLeft() {
  float perc=1.0f-sinceHovering/rollDuration;
  Crayon drawing;
  drawing.fromCrayon(tint);
  if ( blend == transparency ) drawing.af=(perc);
  else drawing.ScaleByAlphaf(perc);
  QuadiNoAlpha(icon,drawing,blend,location.x,location.y,w,h);
  drawing.fromCrayon(text);
  drawing.ScaleByAlphaf(perc);
  double roll=(double)textSpacing+labelWidth;
  scissors.Push(location.x-(int)(roll*perc),location.y,w+(int)(roll*perc),h);
  Blending(none);
  glColor4fv(text.floats);
  Text(label,(double)(location.x-roll),(double)(location.y),fw,fh,false);   
  scissors.Pop();
 }
 virtual void Render( int px, int py, int pw, int ph, bool animating ) {
  if ( clickDelay>0.0f ) clickDelay-=FRAMETIME;
  location.Set(px+pw/2+(int)(node->pos.x*(float)(pw/2)),py+ph/2+(int)(node->pos.y*(float)(pw/2)));
  if ( !animating ) {
   bool wasHovering=hovering;
   hovering=node->pos.x >= 0.0f
    ?WITHIN(input.mxi,input.myi,location.x,location.y,location.x+labelWidth+w,location.y+h)
    :WITHIN(input.mxi,input.myi,location.x-labelWidth,location.y,location.x+w,location.y+h);
   if ( !hovering && wasHovering ) {
    hoverTime=0.0f;
    sinceHovering=FRAMETIME;
    audio.manager.CueAndPlayLimited(sound_unhover,10,1.0f,1.0f,false,true);
    unrolled=false;
   } else if ( hovering ) {
    if ( !wasHovering ) {
     audio.manager.CueAndPlayLimited(sound_hover,10,1.0f,1.0f,false,true);
    }
    hoverTime+=FRAMETIME;
    sinceHovering=0.0f;
   } else sinceHovering+=FRAMETIME;
   clicking=clickDelay<=0.0f&&hovering&&input.left;
   if ( clicking ) {
    OnClick();
    clickDelay=0.1f;
   }
  }
  if ( hovering ) {
   if ( hoverTime < rollDuration ) {
    if ( node->pos.x >= 0.0f ) RollRight();
    else RollLeft();
   } else {
    Blending(none);
    glColor4fv(text.floats);
    Text(label,(double)(location.x+w),(double)(location.y+h),fw,fh,false);
   }
  } else {
   if ( sinceHovering < rollDuration ) {
    sinceHovering+=FRAMETIME;
    if ( node->pos.x >= 0.0f ) UnrollRight();
    else UnrollLeft();
   } else unrolled=false;
  }
  QuadiNoAlpha(icon,tint,blend,location.x,location.y,w,h);
 }
};

class MenuItems : public LinkedList {
public:
 bool clicked;
 CLEARLISTRESET(MenuItem);
 void Render( int x, int y, int w, int h, bool animating ) {
  clicked=false;
  FOREACH(MenuItem,m) {
   m->Render(x,y,w,h,animating);
   if ( m->clicking ) clicked=true;
  }
 }
};

class fx_CircleMenu : public FAFProce55or {
public:
 MenuItems items;
 FDP2d fdp;
 bool opening;
 bool closing,closed;
 float expired;
 float animationDuration;
 fx_CircleMenu( int x, int y, int w, int h ) : FAFProce55or() {
  opening=true;
  closing=false;
  closed=false;
  this->x=x;
  this->y=y;
  fdp.mode=FDPPolynet;
  fdp.width=(float)w;
 fdp.height=(float)h;
  fdp.Initialize(0);
  animationDuration=3.0f;
 }
 bool Done() {
  return closed;
 }
 void Reset() {
  expired=0.0f;
 }
 void AddItem( string label, GLImage *icon ) {
  AddItem(new MenuItem(label,icon));
 }
 void AddItem( MenuItem *i ) {
  fdp.AddNode();
  i->node=(FDPNode *) fdp.nodes.last;
  items.Append(i);
 }
 void Between() {
  expired+=FRAMETIME;
  if ( opening ) {
   if ( expired >= animationDuration ) opening=false;
   else {
    fdp.Iterate();
   }
  } else
  if ( closing ) {
   if ( expired >= animationDuration ) closed=true;
   else {
    fdp.Iterate();
   }
  }
 }
 void Render( GLWindow *surface ) { Render(); }
 void Draw( GLWindow *surface ) { Render(); }
 void Render() {
  items.Render(x,y,w,h,opening||closing);
  if ( items.clicked ) {
   closing=true;
   expired=0.0f;
  }
 }
};


class fx_CircleMenuTest : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 fx_CircleMenu *menu;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  menu=null;
  Init();
 }
 void LeftUp() {
  if ( fx.count == 0 ) Repopulate(input->mxi,input->myi);
 }
 void Repopulate(int mx, int my) {
  fx.Clear();
  menu=new fx_CircleMenu(mx,my,256,256);
  menu->AddItem(string("Walking"),library.Load("data/images/icons/emyller_alert.png"));
  menu->AddItem(string("Talking"),library.Load("data/images/icons/emyller_alert.png"));
  menu->AddItem(string("Dancing"),library.Load("data/images/icons/emyller_alert.png"));
  menu->AddItem(string("Hopping"),library.Load("data/images/icons/earth.png"));
  menu->AddItem(string("Backpack"),library.Load("data/images/icons/backpack.png"));
  Add(menu,true);
 }
};