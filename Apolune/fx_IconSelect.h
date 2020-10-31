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

#include "macros.h"
#include "moremath.h"

#include "GLWindow.h"
#include "GLImage.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55Controller.h"
#include "Proce55or.h"
#include "Art.h"
#include "Audio.h"

#include "TextureLibrary.h"

#include "Interface.h"
#include "SafeMediaLookup.h"

extern Interface input;

class fx_IconSelectItem : public ListItem {
public:
 Zpointer<GLImage> item;
 Zint v;
 fx_IconSelectItem() : ListItem() {}
 fx_IconSelectItem( GLImage *item, int v ) : ListItem() { this->item=item; this->v=v; }
};

class fx_IconSelectItems : public LinkedList {
public:
 fx_IconSelectItem *find( int ii ) {
  for ( fx_IconSelectItem *v = (fx_IconSelectItem *) (this->first); v; v=(fx_IconSelectItem *) (v->next) ) if ( v->v == ii ) return v;  
  return null;
 }
 void Push( const char *s, int ii ) { 
  fx_IconSelectItem *i=new fx_IconSelectItem;
  i->item=library.find(s);
  i->v=ii;
  this->Append(i);
 }
 void Push( SafeImageLookupInt *icons ) {
  EACH(icons->first,SafeImageElement,s) {
   fx_IconSelectItem *i=new fx_IconSelectItem;
   i->v=s->value;
   i->item=s->lookup;
   Append(i);
  }
 }
 CLEARLISTRESET(fx_IconSelectItem);
};

class fx_IconSelect : public Proce55or {
public:
 fx_IconSelectItems items;
 Zpointer<fx_IconSelectItem> prior;
 Zpointer<fx_IconSelectItem> selected;
 Zfloat waiting;
 Zbool hovering, clicking;
 Zpointer<GLImage> glow_c, glow_h, glow_n, backing;
 Zpointer<int> binding;
 Zint value;
 Crayon normal, hover, click, f_normal, f_hover, f_click, t_normal, t_hover, t_click;
 Blends blend;
 Zint mx,my;

 virtual void OnSelect() {}
 virtual void WhenClicked() {}

 void Bind( int *binds ) {
  binding=binds;
  SetValue(*binds);
 }

 int GetValue() { 
  return selected->v;
 }

 void SetValue( int targetValue ) {
  value=targetValue;
  selected=items.find(value);
 }

 void Select( int targetValue ) { SetValue(targetValue); }

 fx_IconSelect() {
  Constructor();
  blend=additive;
  prior=null;
  selected=null;
  mx=-1;
  my=-1;
  hovering=clicking=hidden=false;
  glow_c=glow_h=glow_n=backing=null;
  binding=null;
  backing = library.find("data/images/elements/buttonglow_1024_1.png");
  normal.Pick(alabaster);
  hover.Pick(blue);
  click.Pick(red255);
  f_normal.Pick(alabaster);
  f_hover.Pick(cyan);
  f_click.Pick(orange);
  t_normal.Pick(alabaster);
  t_hover.Pick(yellow);
  t_click.Pick(alabaster);
  waiting=0.0f;
  value=0;
 }

 void Setup()
 {
  x2=x+w; y2=y+h;
 }
 
 void Between() {
  if ( hidden ) {
   hovering=false;
   return;
  }
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  if ( waiting > 0.0f ) waiting-=FRAMETIME;
 }

 void Render() {
   if ( hidden ) return;
   Crayon c;
   Variable *v=null;
   this->controller.Updatef();
   if ( clicking ) {
     if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_click;

     if ( glow_c ) StretchiNoAlpha(glow_c,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
      c=click;
      c.ScaleByAlphaf( v->f ); 
     } else c= click;

     StretchiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
      c=t_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_click;

     if ( selected ) StretchiNoAlpha(selected->item,c,additive,x,y,w,h);

   } else if ( hovering ) {

     if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_hover;

     if ( glow_h ) StretchiNoAlpha(glow_h,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
      c=hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= hover;

     StretchiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
      c=t_hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_hover;

     if ( selected ) StretchiNoAlpha(selected->item,c,additive,x,y,w,h);

   } else {

     if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
      c=f_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_normal;

     if ( glow_n ) StretchiNoAlpha(glow_n,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
      c=normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= normal;

     StretchiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("t")) && v->tween ) {
      c=t_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_normal;

     if ( selected ) StretchiNoAlpha(selected->item,c,blend,x,y,w,h);     
   }  
 }
 void Render(GLWindow *surface) {
   if ( hidden ) return;
   Crayon c;
   Variable *v;
   this->controller.Updatef();
   if ( clicking ) {
     if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_click;

     if ( glow_c ) QuadiNoAlpha(glow_c,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
      c=click;
      c.ScaleByAlphaf( v->f ); 
     } else c= click;

     QuadiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
      c=t_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_click;

     if ( selected ) QuadiNoAlpha(selected->item,c,additive,x,y,w,h);

   } else if ( hovering ) {

     if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_hover;

     if ( glow_h ) QuadiNoAlpha(glow_h,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
      c=hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= hover;

     QuadiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
      c=t_hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_hover;

     if ( selected ) QuadiNoAlpha(selected->item,c,additive,x,y,w,h);

   } else {

     if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
      c=f_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_normal;

     if ( glow_n ) QuadiNoAlpha(glow_n,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
      c=normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= normal;

     QuadiNoAlpha(backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("t")) && v->tween ) {
      c=t_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_normal;

     if ( selected ) QuadiNoAlpha(selected->item,c,blend,x,y,w,h);     
   }  
 }
 void Draw(GLWindow *surface)
 {
   if ( hidden ) return;
   Crayon c;
   Variable *v;
   this->controller.Updatef();
   if ( clicking ) {
     if ( (v=this->controller.variables.Get("clo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_click;

     if ( glow_c ) StretchiNoAlpha(surface,glow_c,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("chi")) && v->tween ) {
      c=click;
      c.ScaleByAlphaf( v->f ); 
     } else c= click;

     StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ct")) && v->tween ) {
      c=t_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_click;

     if ( selected ) StretchiNoAlpha(surface,selected->item,c,additive,x,y,w,h);

   } else if ( hovering ) {

     if ( (v=this->controller.variables.Get("hlo")) && v->tween ) {
      c=f_click;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_hover;

     if ( glow_h ) StretchiNoAlpha(surface,glow_h,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hhi")) && v->tween ) {
      c=hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= hover;

     StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("ht")) && v->tween ) {
      c=t_hover;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_hover;

     if ( selected ) StretchiNoAlpha(surface,selected->item,c,additive,x,y,w,h);

   } else {

     if ( (v=this->controller.variables.Get("lo")) && v->tween ) {
      c=f_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= f_normal;

     if ( glow_n ) StretchiNoAlpha(surface,glow_n,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("hi")) && v->tween ) {
      c=normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= normal;

     StretchiNoAlpha(surface,backing,c,additive,x,y,w,h);

     if ( (v=this->controller.variables.Get("t")) && v->tween ) {
      c=t_normal;
      c.ScaleByAlphaf( v->f ); 
     } else c= t_normal;

     if ( selected ) StretchiNoAlpha(surface,selected->item,c,blend,x,y,w,h);     
   }  
 }

 void LeftDown() {
  if ( hidden ) return;
  if ( !hovering ) return;
  if ( waiting > 0.0f ) return;
  clicking=true;
  waiting=0.25f;
  SetNext();
  this->WhenClicked();
 }

 void LeftUp() {
  clicking=false;
  waiting=0.0f;
 }
 
 void RightDown() {
  if ( hidden ) return;
  if ( !hovering ) return;
  if ( waiting > 0.0f ) return;
  clicking=true;
  waiting=0.25f;
  SetPrevious();
  this->WhenClicked();
 }

 void RightUp() {
  clicking=false;
  waiting=0.0f;
 }

 void WheelUp() {
  if ( !hidden && hovering ) {
   SetNext();
  }
 }
 void WheelDown() {
  if ( !hidden && hovering ) {
   SetPrevious();
  }
 }
 
 void SetNext() {
  prior=selected;
  if ( !selected ) selected=(fx_IconSelectItem *) items.first; 
  else {
   selected=(fx_IconSelectItem *) (selected->next); 
   if ( !selected ) selected=(fx_IconSelectItem *) items.first;
  }
  if ( binding ) *binding=selected->v;
  OnSelect();
 }

 void SetPrevious() {
  prior=selected;
  if ( !selected ) selected=(fx_IconSelectItem *) items.first; 
  else {
   selected=(fx_IconSelectItem *) (selected->prev); 
   if ( !selected ) selected=(fx_IconSelectItem *) items.last;
  }
  if ( binding ) *binding=selected->v;
  OnSelect();
 }

 void SetPrior() {
  if ( prior ) {
   selected=prior;
   if ( binding ) *binding=selected->v;
   OnSelect();
  }
 }

 void MouseLeave() {
  clicking=hovering=false;
  waiting=0.0f;
 }
};

