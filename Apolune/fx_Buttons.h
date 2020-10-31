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

#include <Windows.h>
#include <math.h>
#include "moremath.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55Controller.h"
#include "Proce55or.h"
#include "Art.h"
#include "Audio.h"
#include "Interface.h"

#include "UglyFont.h"
#include "TexFont.h"


/******************
 * fx_CoolButton  *
 ******************/

class fx_CoolButton : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing, *blocker;
 string label, sound_click, sound_hover;
 GLImage *buttontext;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, b_tint;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 Blends textblend,blockerBlend;
// float hoverFadeDuration;

 Crayon c; 
 
 fx_CoolButton() {
  Constructor();
  glow_c=glow_h=glow_n=backing=blocker=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  b_tint.Pick(black);
  buttontext=null;
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  blockerBlend=transparency;
 }

 void Label( const char *l ) { label=string(l); }
 void Setup() {
  x2=x+w;
  y2=y+h;
  textblend=additive;
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }
 virtual void Pre() {
 }
 virtual void Post() {
 }
 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  Pre();
  if ( blocker ) {
   StretchiNoAlpha(surface,blocker,b_tint,blockerBlend,x,y,w,h);
  }
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   if ( backing ) StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   if ( backing ) StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else {//

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( label.length() >0 ) Text(label,(double)(this->parent->x+this->x+this->w/2),(double)(this->parent->y+this->y),7.0,10.0,true);
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);
  }
  Post();
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  bool washovering=hovering;
  hovering=within(mx,my);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length() > 0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( sound_hover, 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  if ( within(mx,my) ) {
   if ( sound_click.length() > 0 ) audio.manager.CueAndPlay( sound_click, 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};


class fx_DirectButton : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing, *blocker;
 string label, sound_hover, sound_click;
 GLImage *buttontext;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, b_tint;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 Blends textblend,blockerBlend;
// float hoverFadeDuration;

 Crayon c; 
 
 fx_DirectButton() {
  Constructor();
  glow_c=glow_h=glow_n=backing=blocker=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  b_tint.Pick(black);
  buttontext=null;
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  blockerBlend=transparency;
 }

 void Label( const char *l ) { label=string(l); }
 void Setup() {
  x2=x+w;
  y2=y+h;
  textblend=additive;
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }
 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  if ( blocker ) {
   QuadiNoAlpha(blocker,b_tint,blockerBlend,x,y,w,h);
  }
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) QuadiNoAlpha(glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   if ( backing ) QuadiNoAlpha(backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( buttontext ) QuadiNoAlpha(buttontext,c,additive,x,y,w,h);

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) QuadiNoAlpha(glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   if ( backing ) QuadiNoAlpha(backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   if ( buttontext ) QuadiNoAlpha(buttontext,c,additive,x,y,w,h);

  } else {//

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) QuadiNoAlpha(glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  QuadiNoAlpha(backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( label.length() > 0 ) Text(label,(double)(this->parent->x+this->x+this->w/2),(double)(this->parent->y+this->y),7.0,10.0,true);
   if ( buttontext ) QuadiNoAlpha(buttontext,c,additive,x,y,w,h);
  }
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  bool washovering=hovering;
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length() > 0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( sound_hover, 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  if ( hidden ) return;
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  if ( hovering ) {
   if ( sound_click.length() > 0 ) audio.manager.CueAndPlay( sound_click, 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  if ( hidden ) return;
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  if ( clicking && hovering ) {
   clicking=false;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};

class fx_UglyCoolButton : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing, *blocker;
 string label, sound_hover,sound_click;
 GLImage *buttontext;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, b_tint;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 Blends textblend,blockerBlend;
// float hoverFadeDuration;

 Crayon c; 
 
 fx_UglyCoolButton() {
  Constructor();
  glow_c=glow_h=glow_n=backing=blocker=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  b_tint.Pick(black);
  buttontext=null;
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  blockerBlend=transparency;
 }

 void Label( const char *l ) { label=string(l); }
 void Setup() {
  x2=x+w;
  y2=y+h;
  textblend=additive;
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }
 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  if ( blocker ) {
   StretchiNoAlpha(surface,blocker,b_tint,blockerBlend,x,y,w,h);
  }
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else {//

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( label.length() > 0 ) Text(label,(double)(this->parent->x+this->x+this->w/2),(double)(this->parent->y+this->y),7.0,10.0,true);
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);
   
  }
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  bool washovering=hovering;
  hovering=within(mx,my);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length() > 0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( sound_hover, 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  if ( within(mx,my) ) {
   if ( sound_click.length() > 0 ) audio.manager.CueAndPlay( sound_click, 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};

class fx_TexFontCoolButton : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing, *blocker;
 string label, sound_hover, sound_click;
 GLImage *buttontext;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, b_tint;
 TexFont *font;
 Crayon labelColor;
 Blends labelBlend;
 Cartesian labelOffset,labelSize;
 int labelCharacterSpace;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 Blends textblend,blockerBlend;
// float hoverFadeDuration;

 Crayon c; 
 
 fx_TexFontCoolButton() {
  Constructor();
  glow_c=glow_h=glow_n=backing=blocker=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  b_tint.Pick(black);
  buttontext=null;
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  blockerBlend=transparency;
  font=calibri;
  labelColor.Pick(alabaster);
  labelBlend=additive;
  labelOffset.Set(16,3);
  labelSize.Set(8,17);
  labelCharacterSpace=4;
 }

 void Label( const char *l ) { label=string(l); } //FORMAT( label,64, "%s", l ); }
 void Setup() {
  x2=x+w;
  y2=y+h;
  textblend=additive;
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }
 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  if ( blocker ) {
   StretchiNoAlpha(surface,blocker,b_tint,blockerBlend,x,y,w,h);
  }
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;

   if ( label.length() > 0 )
    Text(font,labelColor,labelBlend,(this->parent->x+x+labelOffset.x),(this->parent->y+y+labelOffset.y),labelSize.x,labelSize.y,0,labelCharacterSpace,
     (char*)label.c_str());

   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else {//

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( label.length() > 0 )
    Text(font,labelColor,labelBlend,
     (this->parent->x+x+labelOffset.x),
     (this->parent->y+y+labelOffset.y),
     labelSize.x,labelSize.y,0,labelCharacterSpace,label.c_str()
    );
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);
   
  }
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  bool washovering=hovering;
  hovering=within(mx,my);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length()>0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( sound_hover, 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  if ( within(mx,my) ) {
   if ( sound_click.length()>0 ) audio.manager.CueAndPlay( sound_click, 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};




/************************
 * fx_CoolButtonToggle  *
 ************************/

class fx_CoolButtonToggle : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing;
 string label;
 string sound_click, sound_hover;
 GLImage *buttontext;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, selected;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 Blends textblend;
// float hoverFadeDuration;

 bool *binding;
 bool value;
 virtual void Bind( bool *bound ) {
  binding=bound;
  value=*binding;
 }

 Crayon c; 
 
 fx_CoolButtonToggle() {
  Constructor();
  glow_c=glow_h=glow_n=backing=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  selected.Pick(orangeRed);
  buttontext=null;
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  binding=null;
  value=false;
 }

 void Label( const char *l ) { label=string(l); }
 void Setup() {
  x2=x+w;
  y2=y+h;
  textblend=additive;
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }
 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);

  } else {//

   if ( value ) {
    c.fromCrayon(selected);
    StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);
   }

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( buttontext ) StretchiNoAlpha(surface,buttontext,c,additive,x,y,w,h);
   
  }
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  bool washovering=hovering;
  hovering=within(mx,my);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length() > 0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( sound_hover, 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  if ( within(mx,my) ) {
   if ( sound_click.length() > 0 ) audio.manager.CueAndPlay( sound_click, 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  int mx=input.mxi-this->parent->x,my=input.myi-this->parent->y;
  if ( hidden ) return;
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false;
   if ( binding ) *binding=!(*binding);
   value=!value;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};




/******************
 * fx_UglyButton  *
 ******************/
#include "UglyFont.h"
#include "Vertex.h"

class fx_UglyButton : public Proce55or {
public:
 string label;
 int fw,fh,w2,h2,mx,my;
 GLImage *glow_c, *glow_h, *glow_n, *backing;
 string sound_hover,sound_click;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click;
 bool hovering, clicking;
 bool DoNotShowBackingNormal;
 bool fromTop;
 Blends textblend;
 Vertexd center;
 Vertexd labelOffset;

 Crayon c; 
 
 fx_UglyButton() {
  Constructor();
  glow_c=glow_h=glow_n=backing=null;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  hovering=clicking=hidden=false;
  DoNotShowBackingNormal=false;
  textblend=additive;
  fw=8;
  fh=10;
  w=h=h2=w2=0;
  fromTop=false;
  mx=my=0;
 }

 void SetLabel( const char *l ) {
  label=string(l);
 }
 void SetSounds( const char *click, const char *hover ) {
  sound_click=string(click);
  sound_hover=string(hover);
 }
 void SetSounds( string click, string hover ) {
  sound_click=(click);
  sound_hover=(hover);
 }
 void Setup() {
  w2=w/2;
  h2=h/2;
  MoveToCentered(x,y);
 }
 
 void MoveToCentered( int xx, int yy ) {
  x=xx-w2;
  y=yy-h2;
  x2=w+x;
  y2=h+y;
  if ( !fromTop ) {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2);
  } else {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2-fh);
  }
 }
 void MoveTo( int xx, int yy ) {
  x=xx;
  y=yy;
  x2=w+xx;
  y2=h+yy;
  if ( !fromTop ) {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2);
  } else {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2-fh);
  }
 }
 void MoveBy( int xx, int yy ) {
  x+=xx;
  y+=yy;
  x2+=xx;
  y2+=yy;
  if ( !fromTop ) {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2);
  } else {
   center.x=labelOffset.x+(double)(x+w2);
   center.y=labelOffset.y+(double)(y+h2-fh);
  }
 }

 void Between() {
  if ( hidden ) return;
  this->controller.Updatef();
 }

 void Render() {
  Draw(this->parent);
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }

 void Draw(GLWindow *surface)
 {  
  if ( hidden ) return;
  Variable *v;
  if ( clicking ) {//

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   RenderLabel();

  } else if ( hovering ) {//

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   c.Pick(black);
   Blending(none);
   glColor4fv(c.floats);
   Text((char *) label.c_str(),center.x+1,center.y+1,fw,fh,true);
   RenderLabel();
  } else {//

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( !DoNotShowBackingNormal && glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   if ( !DoNotShowBackingNormal && backing )  StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   RenderLabel();
  }
 }

 virtual void RenderLabel() {  
   Blending(additive);
   glColor4fv(c.floats);
   Text((char *) label.c_str(),center.x,center.y,(double)fw,(double)fh,true);
 }

 virtual void WhenClicked() { }
 virtual void WhenActivated() { }
 virtual void WhenDeactivated() { }

/*
 void MouseLeave() {
  if ( hovering ) WhenDeactivated(); 
  hovering=clicking=false; 
 }
 */

 void MouseMove() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  bool washovering=hovering;
  hovering=within(mx,my);
  if ( !hovering ) {
   clicking=false;
   if ( washovering ) WhenDeactivated();
  } else {
   if ( sound_hover.length() > 0 ) {
    if ( !washovering ) audio.manager.CueAndPlay( (char *) sound_hover.c_str(), 1.0f, 1.0f, false, true );  
    hovering=true;
   }
   WhenActivated();
  }
 }

 void LeftDown() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  if ( within(mx,my) ) {
   if ( sound_click.length() > 0 ) audio.manager.CueAndPlay( (char *) sound_click.c_str(), 1.0f, 1.0f, false, true ); 
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false;
   WhenClicked();
  }
  else clicking=hovering=false;
 }

};



/******************
 * fx_CoolToggle  *
 ******************/

class fx_CoolToggle : public Proce55or {
public:
 GLImage *glow_c, *glow_h, *glow_n, *backing, *ledOn, *ledOff;
 int Lx,Ly,Lx2,Ly2;
 string label;
 string sound_click, sound_hover;
 bool *binding;
 bool isOn;
 GLImage *icon;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click, led;
 bool hovering, clicking;
 Blends textblend;
 int mx,my;
// float hoverFadeDuration;

 Crayon c; 
 
 fx_CoolToggle() {
  Constructor();
  glow_c=glow_h=glow_n=backing=null;
  Lx=Ly=Lx2=Ly2=0;
  label=string("");
  sound_click=string("");
  sound_hover=string("");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  hovering=clicking=hidden=false;
  isOn=false;
  icon=null;
  textblend=additive;
  binding=null;
  ledOn=ledOff=null;
  mx=my=0;
 }

 void Label( const char *l ) { label=string(l); }
 void Setup() { 
  x2=x+w; 
  y2=y+h; 
  Lx=x+3*(w/4); 
  Ly=y+0; 
  Lx2=w/4; 
  Ly2=h/4; 
  textblend=additive; 
  if ( binding ) isOn=*binding;
 }
 void Clean() { }
 void Between() {
  this->controller.Updatef();
 }

 void Draw(GLWindow *surface)
 {  
  Variable *v;
  if ( hidden ) return;
  if ( clicking ) {

   if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
    c.fromCrayon(f_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_click;
   if ( glow_c ) Stretchi(surface,glow_c,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
    c.fromCrayon(click); 
    c.ScaleByAlphaf( v->f ); 
   } else c=click;
   Stretchi(surface,backing,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
    c.fromCrayon(t_click);
    c.ScaleByAlphaf( v->f ); 
   } else c=t_click;
   if ( icon ) Stretchi(surface,icon,c,c.af,additive,x,y,w,h);

  } else if ( hovering ) {

   if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
    c.fromCrayon(f_hover);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_hover;
   if ( glow_h ) Stretchi(surface,glow_h,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
    c.fromCrayon(hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=hover;
   Stretchi(surface,backing,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
    c.fromCrayon(t_hover); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_hover;
   if ( icon ) Stretchi(surface,icon,c,c.af,additive,x,y,w,h);

  } else {

   if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
    c.fromCrayon(f_normal);
    c.ScaleByAlphaf( v->f ); 
   } else c=f_normal;
   if ( glow_n )      Stretchi(surface,glow_n,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
    c.fromCrayon(normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=normal;
   Stretchi(surface,backing,c,c.af,additive,x,y,w,h);

   if ( (v=this->controller.variables.Get("t")) && v->tween ) {
    c.fromCrayon(t_normal); 
    c.ScaleByAlphaf( v->f ); 
   } else c=t_normal;
   if ( icon ) Stretchi(surface,icon,c,c.af,additive,x,y,w,h);
  }

  if ( isOn || (binding && *binding) ) Stretchi(surface,ledOn,led,led.af,additive,Lx,Ly,Lx2,Ly2);
  else Stretchi(surface,ledOff,led,led.af,additive,Lx,Ly,Lx2,Ly2);
 }

 virtual void WhenClicked() { }

 void MouseLeave(Modifiers mods) {
  hovering=clicking=false;
 }

 void MouseMove() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  if ( within(mx,my) ) {
   if ( !hovering && sound_hover.length() > 0 )
    audio.manager.CueAndPlayLimited( sound_hover, 15, 1.0f, 1.0f, false, true );
   hovering=true;
  }
  else clicking=hovering=false;
 }

 void LeftDown() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  if ( within(mx,my) ) {
   clicking=true; 
  }
  else clicking=hovering=false;
 }

 void LeftUp() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=within(mx,my);
  if ( clicking && hovering ) {
   clicking=false; 
   isOn=!isOn; 
   if ( binding ) *binding=!(*binding); 
   if ( sound_click.length() > 0 )
    audio.manager.CueAndPlayLimited( sound_click, 15, 1.0f, 1.0f, false, true ); 
   WhenClicked(); 
  }
  else clicking=hovering=false;
 }

};