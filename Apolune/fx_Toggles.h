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

#include "Interface.h"
#include "Audio.h"
#include "Art.h"
#include "Crayon.h"
#include "TextureLibrary.h"
#include "Proce55or.h"
#include "fx_TexFont.h"
#include "UglyFont.h"

extern Interface input;

class fx_ToggleBox : public Proce55or {
public:
 Zpointer<TexFont> font;
 Zpointer<bool> binding;
 Zbool value;
 Zstring label;
 Zint x3,y3,bx,by,bw,bh,labx,laby,spacerw,hofs;
 Crayon border,selected,unselected,hovering,labelColor;
 Zint len;
 Zbool hover;
 Zfloat clickDelay;
 fx_ToggleBox() : Proce55or() {
  font=microFont;
  border.fromCrayon(crayons.jam(0, 172, 255, 255, 255 ));
  selected.Pick(gray);
  unselected.Pick(black);
  hovering.Pick(red255);
  labelColor.Pick(azure);
  duration=10.0f;
 }
 void Bind( bool *b ) {
  binding=b;
  SetAsBound();
 }
 void SetAsBound() { value=*binding; }
 void Setup() {
  len=(int)label.length;
  hover=false;
 }
 void Positions( int X, int Y, int W, int H, int boxspacew, int boxspaceh, int space, int heightofs ) {
  x=X;
  y=Y;
  // w
  // h
  x2=x+W;
  y2=y+H;
  bw=x2-x;
  bh=y2-y;
  bx=x+(boxspacew);
  by=y+(boxspaceh);
  x3=x+(W-boxspacew);
  y3=y+(H-boxspaceh);
  bw=W;
  bh=H;
  hofs=heightofs;
  labx=x+space+W;
  laby=y+hofs;
 }
 void Render() {
  Draw(parent);
 }
 void Render(GLWindow *surface ) {
  Draw(surface);
 }
 void Draw(GLWindow *surface) {
//  surface.foreground=ColorRGBA { 172, 255, 255, 255 };
  WriteTexFont(surface,font,labelColor,additive,labx,laby,7,16,2,label.c_str());
//  surface.WriteText( lx,y, label, len );
  Rectangle( surface, border, x,y, x2,y2 );
  if ( value ) {
   Area( surface, selected, bx,by,x3,y3 );
  } else {
   Area( surface, unselected, bx,by,x3,y3 );
  }
  if ( hover ) {
   Area( surface, hovering, bx,by,x3,y3 );
  }
 }
 void MouseMove() {
  hover=WITHIN(input.mxi-parent->x,input.myi-parent->y,x,y,x2,y2);
 }
 void Between() {
  if ( clickDelay > 0.0f ) clickDelay-=1.0f/FPS;
 }
 void LeftDown() {
  if ( hover && clickDelay <= 0.0f ) {
   if ( binding ) *binding=!(*binding);
   value=!value;
   OnChange();
   clickDelay=0.5f;
  }
 }
 virtual void OnChange() {}
};


class fx_ToggleIcon : public Proce55or {
public:
 Zpointer<Tween> pulse;
 Zpointer<GLImage> icon,checked,unchecked,backing,hover,border;
 Crayon iconColor,checkedColor,uncheckedColor,clickColor,cancelColor,backingColor,hoverColor,borderColor;
 Blends iconBlend,checkBlend,backingBlend,hoverBlend,borderBlend;
 Cartesian checkRect;
 Cartesian iconRect;
 Zint margin;
 Zpointer<bool> bound;
 Zbool value;
 Zbool hovering,clicking,cancelling;
 Zbool drawIconHighlight;
 Zfloat clickDelay;
 Zfloat time;
 virtual void Checked() {}
 virtual void Unchecked() {}
 fx_ToggleIcon() : Proce55or() {
  pulse=tweens.find("Sine");
  iconBlend=checkBlend=borderBlend=additive;
  hoverBlend=additive;
  backingBlend=transparency;
  backingColor.Pick(black);
  iconColor.Pick(alabaster);
  checkedColor.Pick(alabaster);
  uncheckedColor.Pick(alabaster);
  hoverColor.Pick(green);
  clickColor.Pick(red255);
  cancelColor.Pick(red255);
  borderColor.Pick(alabaster);
  checked=library.Load("data/images/icons/dagobert83_apply.png");
  unchecked=library.Load("data/images/icons/dagobert83_cancel.png");
  hover=library.Load("data/images/elements/rounded.png");
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  margin=3;
  w=64;
  h=64;
  SetExtents(x,y,w,h);
  checkRect.SetRect(x+w/2+w/4,y,w/4,w/4);
  duration=10.0f;
  drawIconHighlight=true;
  clickDelay=0.0f;
  time=0.0f;
 }
 fx_ToggleIcon( int x, int y, int w, int h, int M ) : Proce55or() {
  pulse=tweens.find("Sine");
  iconBlend=checkBlend=borderBlend=additive;
  hoverBlend=additive;
  backingBlend=transparency;
  backingColor.Pick(black);
  iconColor.Pick(alabaster);
  checkedColor.Pick(alabaster);
  uncheckedColor.Pick(alabaster);
  hoverColor.Pick(green);
  clickColor.Pick(red255);
  cancelColor.Pick(red255);
  borderColor.Pick(alabaster);
  checked=library.Load("data/images/icons/dagobert83_apply.png");
  unchecked=library.Load("data/images/icons/dagobert83_cancel.png");
  hover=library.Load("data/images/elements/rounded.png");
  backing=library.Load("data/images/elements/transrounded512borderless.png");
  margin=M;
  SetExtents(x,y,w,h);
  checkRect.SetRect(x+w/2+w/4,y,w/4,w/4);
  duration=10.0f;
  drawIconHighlight=true;
  clickDelay=0.0f;
  time=0.0f;
 }
 void SetMargin( int M ) {
  margin=M;
  SetExtents(x,y,w,h);
 }
 void SetExtents( int x, int y, int w, int h ) {
  this->x=x;
  this->y=y;
  this->w=w;
  this->h=h;
  x2=x+w;
  y2=y+h;
  iconRect.SetRect(x+margin,y+margin,w-margin*2,h-margin*2);
  checkRect.SetRect(x+w/2+w/4,y,w/4,w/4);
 }
 void Bind( bool *b ) {
  bound=b;
  value=*b;
 }
 void SetValue( bool b ) {
  if ( bound ) *bound=b;
  value=b;
 }
 void Between() {
  if ( hidden ) return;
  time+=FRAMETIME;
  if ( time > 1.0f ) time=0.0f;
  if ( clickDelay > 0.0f ) clickDelay-=FRAMETIME;
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  clicking=hovering&&input.left;
  cancelling=hovering&&input.right;
  if ( clicking && clickDelay <= 0.0f ) {
   SetValue(!value);
   clickDelay=0.5f;
   if ( value ) Checked(); else Unchecked();
  } else if ( value && cancelling && clickDelay <= 0.0f ) {
   SetValue(false);
   clickDelay=0.5f;
   Unchecked();
  }
 }
 void Render( GLWindow *surface ) { Render(); }
 void Draw( GLWindow *surface ) { Render(); }
 void Render() {
  if ( hidden ) return;
  if ( backing ) {
   QuadiNoAlpha(backing,backingColor,backingBlend,x,y,w,h);
  }
  if ( border ) {
   QuadiNoAlpha(border,borderColor,borderBlend,x,y,w,h);
  }
  if ( drawIconHighlight && hover && hovering ) {
   if ( pulse ) {
    Crayon drawing;
    drawing.fromCrayon(clicking?clickColor:cancelling?cancelColor:hoverColor);
    if ( hoverBlend == transparency ) drawing.Alpha(pulse->tweenf(time));
    else drawing.ScaleByAlphaf(pulse->tweenf(time));
    QuadiNoAlpha(hover,drawing,hoverBlend,iconRect.x,iconRect.y,iconRect.w,iconRect.h);
   } else
   QuadiNoAlpha(hover,clicking?clickColor:cancelling?cancelColor:hoverColor,hoverBlend,iconRect.x,iconRect.y,iconRect.w,iconRect.h);
  }
  if ( icon ) {
   QuadiNoAlpha(icon,iconColor,iconBlend,iconRect.x,iconRect.y,iconRect.w,iconRect.h);
  }
  if ( backing ) {  
   QuadiNoAlpha(backing,backingColor,backingBlend,checkRect.x,checkRect.y,checkRect.w,checkRect.h);
  }
  if ( !drawIconHighlight && hover && hovering ) {
   if ( pulse ) {
    Crayon drawing;
    drawing.fromCrayon(clicking?clickColor:cancelling?cancelColor:hoverColor);
    if ( hoverBlend == transparency ) drawing.Alpha(pulse->tweenf(time));
    else drawing.ScaleByAlphaf(pulse->tweenf(time));
    QuadiNoAlpha(hover,drawing,hoverBlend,checkRect.x,checkRect.y,checkRect.w,checkRect.h);
   } else
   QuadiNoAlpha(hover,clicking?clickColor:cancelling?cancelColor:hoverColor,hoverBlend,checkRect.x,checkRect.y,checkRect.w,checkRect.h);
  }
  if ( value ) {
   QuadiNoAlpha(checked,checkedColor,checkBlend,checkRect.x,checkRect.y,checkRect.w,checkRect.h);
  } else {
   QuadiNoAlpha(unchecked,uncheckedColor,checkBlend,checkRect.x,checkRect.y,checkRect.w,checkRect.h);
  }
 }
};


class fx_UglyToggleBox : public Proce55or {
public:
 Zpointer<bool> binding;
 Zbool value;
 Zstring label;
 Zint x3,y3,bx,by,bw,bh,labx,laby,spacerw,hofs;
 Crayon border,selected,unselected,hovering,labelColor;
 Zint len;
 Zbool hover;
 Zfloat clickDelay;
 fx_UglyToggleBox() : Proce55or() {
  border.fromCrayon(crayons.jam(0, 172, 255, 255, 255 ));
  selected.Pick(gray);
  unselected.Pick(black);
  hovering.Pick(red255);
  labelColor.Pick(azure);
  duration=10.0f;
 }
 void Bind( bool *b ) {
  binding=b;
  SetAsBound();
 }
 void SetAsBound() { value=*binding; }
 void Setup() {
  len=(int)label.length;
  hover=false;
 }
 void Positions( int X, int Y, int W, int H, int boxspacew, int boxspaceh, int space, int heightofs ) {
  x=X;
  y=Y;
  // w
  // h
  x2=x+W;
  y2=y+H;
  bw=x2-x;
  bh=y2-y;
  bx=x+(boxspacew);
  by=y+(boxspaceh);
  x3=x+(W-boxspacew);
  y3=y+(H-boxspaceh);
  bw=W;
  bh=H;
  hofs=heightofs;
  labx=x+space+W;
  laby=y+hofs;
 }
 void Render() {
  Draw(parent);
 }
 void Render(GLWindow *surface ) {
  Draw(surface);
 }
 void Draw(GLWindow *surface) {
//  surface.foreground=ColorRGBA { 172, 255, 255, 255 };
  Blending(additive);
  glColor4fv(labelColor.floats);
  Text(label,labx,laby,6,8,false);
//  surface.WriteText( lx,y, label, len );
  Rectangle( border, x,y, x2,y2 );
  if ( value ) {
   Area( selected, bx,by,x3,y3 );
  } else {
   Area( unselected, bx,by,x3,y3 );
  }
  if ( hover ) {
   Area( hovering, bx,by,x3,y3 );
  }
 }
 void MouseMove() {
  hover=WITHIN(input.mxi,input.myi,x,y,x2,y2);
 }
 void Between() {
  if ( clickDelay > 0.0f ) clickDelay-=1.0f/FPS;
 }
 void LeftDown() {
  if ( hover && clickDelay <= 0.0f ) {
   if ( binding ) *binding=!(*binding);
   value=!value;
   OnChange();
   clickDelay=0.5f;
  }
 }
 virtual void OnChange() {}
};
