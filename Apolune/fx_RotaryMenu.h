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
#include "fx_FireAndForget.h"

class RotaryMenuItem : public ListItem {
public:
 GLImage *backing;
 GLImage *icon;
 Blends bgBlend,blend;
 Crayon tint,hover,click,bgTint,lineColor,text;
 string label;
 Cartesiand start,end;
 Cartesian iconSize;
 Cartesiand labelOffset;
 string sound_hover,sound_unhover,sound_click;
 bool hovering,clicking,clicked;
 bool line;
 float lineWidth,clickDelay;
 bool lineSmoothing;
 RotaryMenuItem( GLImage *icon, string label ) {
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  this->icon=icon;
  bgBlend=transparency;
  bgTint.Float(0.0f,0.0f,0.4f,1.0f);
  blend=additive;
  tint.Pick(alabaster);
  hover.Pick(gold);
  click.Pick(red255);
  lineColor.fromCrayon(bgTint);
  iconSize.SetRect(0,0,64,64);
  labelOffset.y=-40;
  this->label=label;
  line=true;
  clicked=false;
  lineWidth=2.0f;
  clickDelay=0.0f;
  lineSmoothing=true;
  hovering=clicking=false;
  sound_hover=string("data/sounds/UI/benk.wav");
  sound_unhover=string("");
  sound_click=string("data/sounds/UI/radar.wav");
  text.Pick(green);
 }
 RotaryMenuItem( GLImage *icon ) {
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  this->icon=icon;
  bgBlend=transparency;
  bgTint.Float(0.0f,0.0f,0.4f,1.0f);
  blend=additive;
  tint.Pick(alabaster);
  hover.Pick(gold);
  click.Pick(red255);
  lineColor.fromCrayon(bgTint);
  iconSize.SetRect(0,0,64,64);
  labelOffset.y=-40;
  line=true;
  clicked=false;
  lineWidth=2.0f;
  clickDelay=0.0f;
  lineSmoothing=true;
  hovering=clicking=false;
  sound_hover=string("data/sounds/UI/benk.wav");
  sound_unhover=string("");
  sound_click=string("data/sounds/UI/radar.wav");
  text.Pick(green);
 }
 RotaryMenuItem() {
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  icon=null;
  bgBlend=transparency;
  bgTint.Float(0.0f,0.0f,0.4f,1.0f);
  blend=additive;
  tint.Pick(alabaster);
  hover.Pick(gold);
  click.Pick(red255);
  lineColor.fromCrayon(bgTint);
  iconSize.SetRect(0,0,64,64);
  labelOffset.y=-34;
  line=true;
  clicked=false;
  lineWidth=2.0f;
  clickDelay=0.0f;
  lineSmoothing=true;
  hovering=clicking=false;
  sound_hover=string("data/sounds/UI/benk.wav");
  sound_unhover=string("");
  sound_click=string("data/sounds/UI/radar.wav");
 }
 virtual void WhenClicked() {}
 Cartesiand lerp,rect;
 void Between( float evolution ) {
  if ( clickDelay > 0.0f ) clickDelay-=FRAMETIME;
  lerp.Lerp(&start,&end,(double)evolution);
  float
   w=iconSize.w*evolution,
   h=iconSize.h*evolution;
  rect.SetRect(
   lerp.x-(double)w/2.0,
   lerp.y-(double)h/2.0,
   w,h
  );
  if ( evolution != 1.0f ) return;
  bool wasHovering=hovering;
  hovering=WITHIN((double)input.mxi,(double)input.myi,rect.x,rect.y,rect.x2,rect.y2);
  if ( !wasHovering && hovering ) {
   audio.manager.CueAndPlay(sound_hover,1.0f,1.0f,false,true);
  } else if ( wasHovering && !hovering ) {
   audio.manager.CueAndPlay(sound_unhover,1.0f,1.0f,false,true);
  }
  bool wasClicking=clicking;
  clicked=false;
  clicking=hovering&&(input.left && clickDelay<=0.0f);
  if ( wasClicking && !clicking ) {
   clicked=true;
   audio.manager.CueAndPlay(sound_click,1.0f,1.0f,false,true);
   WhenClicked();
   clickDelay=1.0f;
  }
 }
 void Render() {
  Crayon hi,hiBack;
  if ( clicking ) {
   hi.fromCrayon(click);
   hiBack.fromCrayon(click);
  } else if ( hovering ) {
   hi.fromCrayon(hover);
   hiBack.fromCrayon(hover);
  } else {
   hi.fromCrayon(lineColor);
   hiBack.fromCrayon(bgTint);
  }
  hi.fromCrayon(clicking?click:(hovering?hover:lineColor));
  if ( line ) {
   if ( lineSmoothing )   glEnable(GL_LINE_SMOOTH);
   glLineWidth(lineWidth);
   Blending(bgBlend);
   DrawLine2Color(crayons.Pick(clear),hi,start.x,start.y,lerp.x,lerp.y);
   if ( lineSmoothing )   glDisable(GL_LINE_SMOOTH);
   glLineWidth(1);
  }
  QuaddNoAlpha(backing,hiBack,bgBlend,rect.x,rect.y,rect.w,rect.h);
  QuaddNoAlpha(icon,tint,blend,rect.x,rect.y,rect.w,rect.h);
  if ( hovering ) {
   Blending(none);
   glColor3i(0,0,0);
   Text(label,lerp.x+labelOffset.x+1,lerp.y+labelOffset.y+1,6.0,7.0,true);
   glColor4fv(text.floats);
   Text(label,lerp.x+labelOffset.x,lerp.y+labelOffset.y,6.0,7.0,true);
  }
 }
};

class RotaryMenuItems : public LinkedList {
public:
 bool clicked;
 double Angle;
 RotaryMenuItems() : LinkedList() {
  Angle=0.0;
  clicked=false;
 }
 void Rotate( int x, int y, double byAngle, double radius ) {
  double dA=TWO_PI/(double)count;
  Angle+=byAngle;
  double A=Angle;
  FOREACH(RotaryMenuItem,r) {
   r->start.Set((double)x,(double)y);
   r->end.Set(
    (double)x+(radius*cos(A)),
    (double)y+(radius*sin(A))
   );
   A+=dA;
  }
 }
 void Recenter( int x, int y, double radius ) {
  double dA=TWO_PI/(double)count;
  double A=Angle;
  FOREACH(RotaryMenuItem,r) {
   r->start.Set((double)x,(double)y);
   r->end.Set(
    (double)x+(radius*cos(A)),
    (double)y+(radius*sin(A))
   );
   A+=dA;
  }
 }
 void MoveBy( double x, double y ) {
  FOREACH(RotaryMenuItem,r) {
   r->start.MoveBy(x,y);
   r->end.MoveBy(x,y);
  }
 }
 void Between( float evolution ) {
  clicked=false;
  FOREACH(RotaryMenuItem,r) {
   r->Between(evolution);
   if ( r->clicked ) clicked=true;
  }
 }
 void Render() { FOREACH(RotaryMenuItem,r) r->Render(); }
 CLEARLISTRESET(RotaryMenuItem);
};

class fx_RotaryMenu : public FAFProce55or {
public:
 RotaryMenuItems items;
 bool closeWhenClicked;
 double radius;
 float expired;
 float in,out;
 bool opening,closing;
 fx_RotaryMenu( int x, int y, double r ) : FAFProce55or() {
  this->x=x;
  this->y=y;
  this->w=(int)r;
  this->h=(int)r;
  this->x2=this->x+this->w;
  this->y2=this->y+this->h;
  in=0.5f;
  out=0.25f;
  radius=r;
  opening=true;
  closing=false;
  expired=0.0f;
  closeWhenClicked=true;
 }
 void AddItem( RotaryMenuItem *i ) {
  items.Append(i);
 }
 void Recenter() {
  items.Recenter(x,y,radius);
 }
 void Setup() {
  Recenter();
  items.Between(0.0f);
 }
 void Rotate( double byAngle ) { items.Rotate(x,y,byAngle,radius); }
 virtual void WhenDone(){}
 virtual bool Done() {
  bool done= ( closing && expired >= out );
  if ( done ) WhenDone();
  return done;
 }
 void Between() {
  expired+=FRAMETIME;
  if ( !opening && !closing ) {
   closing=(double)idistance(x,y,input.mxi,input.myi) > radius*2.0;
   if ( closing ) expired=0.0f;
  }
  if ( opening ) {
   opening= expired < in; 
   if ( opening ) {
    items.Between(expired/in);
    Rotate(deg2rad(1.0));
   }
  } else if ( closing ) {
    items.Between(1.0f-expired/out);
    Rotate(deg2rad(-1.0));
  } else {
   items.Between(1.0f);
   if ( closeWhenClicked && items.clicked ) {
    expired=0.0f;
    closing=true;
   }
  }
 }
 void Render() {
  items.Render();
 }
 void Render( GLWindow *surface ) {
  Render();
 }
 void Draw( GLWindow *surface ) {
  Render();
 }

};




class fx_RotaryMenuTest : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 fx_RotaryMenu *menu;
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
  menu=new fx_RotaryMenu(mx,my,128.0);
  RotaryMenuItem *i;
  menu->AddItem(i=new RotaryMenuItem(library.Load("data/images/icons/emyller_alert.png")));
  i->label=string("Alert");
  i->hover.Pick(green);
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/earth.png"),string("Earth")));
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/emyller_alert.png")));
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/earth.png")));
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/emyller_alert.png")));
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/earth.png")));
  menu->AddItem(new RotaryMenuItem(library.Load("data/images/icons/backpack.png")));
  Add(menu,true);
 }
};