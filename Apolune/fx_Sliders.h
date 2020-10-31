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

#include "Art.h"
#include "Crayon.h"
#include "TextureLibrary.h"

#include "Audio.h"
#include "Interface.h"

#include "Proce55Controller.h"
#include "Proce55or.h"
#include "fx_TexFont.h"
#include "UglyFont.h"


extern Tweens tweens;

class fx_LabeledSlider : public Proce55or {
public:
 Zstring label;
 Zint len,loc;
 Zpointer<GLImage> backing,slider,glow;
 Zpointer<TexFont> font;
 Zfloat perc; 
 Zpointer<float> binding;
 Zbool hovering,clicking;
 Crayon groove,peg,click,hover,labelColor;
 Zint sw,sx,sy,sx2,sy2, labx,laby, xmf, mx,my;

 fx_LabeledSlider() : Proce55or() {
  backing=library.find("data/images/elements/horizslider.png");
  slider=library.find("data/images/elements/buttonglow_1024_1.png");
  glow=library.find("data/images/elements/buttonglow_1024_4.png");
  groove.Pick(alabaster);
  peg.Pick(azure);
  click.Pick(green255);
  hover.Pick(red255);
  labelColor.Pick(red255);
  font=microFont;
 }

 void Setup() {
  x2=x+w;
  y2=y+h;
  loc=y-10;
  len=(int)label.length;
  SliderTo(perc);
 }

 virtual void WhenChanged() { }
 void SliderTo( float p ) {
  int a;
  perc=p;
  if ( binding ) *binding=p;
  a=(int) (p*(float) (w-sw));
  sy=y+0; 
  sy2=h;
  sx=x+a;
  sx2=sw;
  WhenChanged();
 }
 
 void Render() {
  if ( hidden ) return;
  Variable *v;
  WriteTexFont(font,labelColor,additive,labx,laby,7,16,2,(char*)label.c_str());
  this->controller.Updatef();
   // draw the backing
  StretchiNoAlpha(backing,groove,additive,x,y,w,h);

  // draw the slider peg
  Stretchi(backing,peg,1.0f,additive,sx,sy,sx2,sy2);

  if ( glow ) {
   if ( clicking ) 
   (v=this->controller.variables.Get("click")) && v->tween ?
     Stretchi(backing,click,v->f,additive,sx,sy,sx2,sy2) 
   : StretchiNoAlpha(backing,click,additive,sx,sy,sx2,sy2);
  } else if ( hovering ) {
   (v=this->controller.variables.Get("hover")) && v->tween ? 
     Stretchi(glow,hover,v->f,additive,sx,sy,sx2,sy2) 
   : StretchiNoAlpha(glow,hover,additive,sx,sy,sx2,sy2);
  } else {
  // draw the slider peg
  Stretchi(backing, (hovering ? hover : peg) ,1.0f,additive,sx,sy,sx2,sy2);
  }
 }

 void Render(GLWindow *surface) {
  Draw(surface);
 }

 void Draw(GLWindow *surface) {
  if ( hidden ) return;
  Variable *v;
  WriteTexFont(surface,font,labelColor,additive,labx,laby,7,16,2,(char *)label.c_str());
  this->controller.Updatef();
  if ( hidden ) return;  
   // draw the backing
  StretchiNoAlpha(surface,backing,groove,additive,x,y,w,h);

  // draw the slider peg
  Stretchi(surface,backing,peg,1.0f,additive,sx,sy,sx2,sy2);

  if ( glow ) {
   if ( clicking ) 
   (v=this->controller.variables.Get("click")) && v->tween ?
     Stretchi(surface,backing,click,v->f,additive,sx,sy,sx2,sy2) 
   : StretchiNoAlpha(surface,backing,click,additive,sx,sy,sx2,sy2);
  } else if ( hovering ) {
   (v=this->controller.variables.Get("hover")) && v->tween ? 
     Stretchi(surface,glow,hover,v->f,additive,sx,sy,sx2,sy2) 
   : StretchiNoAlpha(surface,glow,hover,additive,sx,sy,sx2,sy2);
  } else {
  // draw the slider peg
  Stretchi(surface,backing, (hovering ? hover : peg) ,1.0f,additive,sx,sy,sx2,sy2);
  }
 }

 bool within( int mx, int my ) {
  return WITHIN(mx,my,x,y,x2,y2);
 }

 void MouseMove() {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hovering=within(mx,my);
  if ( (clicking) && hovering ) SliderTo( iratiof(mx-x,w) ); 
//  else if ( clicking ) SliderTo( iratiof(mx,parent->w) );
 }

 void LeftDown() {  
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hovering=within(mx,my);
  if ( hovering ) { clicking=true; SliderTo( iratiof(mx-x,w) ); }
 }

 void LeftUp() {
//  if ( hidden ) return;
  clicking=hovering=false;
 }

};


class fx_VerticalSlider : public Proce55or {
public:
 Zpointer<GLImage> backing,slider,glow;
 Zfloat perc;
 Zpointer<float> binding;
 Zbool hovering,clicking;
 Crayon groove,peg,click,hover;
 Blends hoverBlend;
 Zint sh,sx,sy,sx2,sy2;
 Zint mx,my;

 fx_VerticalSlider() : Proce55or() {
  backing=library.find("data/images/elements/vertslider.png");
  slider=library.find("data/images/elements/buttonglow_1024_1.png");
  glow=library.find("data/images/elements/buttonglow_1024_4.png");
  sh=10;
  sy2=10;
  w=10;
 }

 void Setup() {
  x2=x+w;
  y2=y+h;
  sy2=sy+sh;
  SliderTo(perc);
 }

 bool within( int mx, int my ) {
  return WITHIN(mx,my,x,y,x2,y2);
 }
 virtual void WhenChanged() { }
 void SliderTo( float p ) {
  int a;
  perc=p;
  if ( binding ) *binding=p;
  a=(int) (p*(float) (h-sh));
  sx=x+0;
  sx2=w;
  sy=y+a;
  sy2=sh;
  WhenChanged();
 }

 void Render() {
   Variable *v;
   this->controller.Updatef();
   if ( hidden ) return;  

   // draw the backing
   QuadiNoAlpha(backing,groove,additive,x,y,w,h);

   // draw the slider peg
   QuadiNoAlpha(slider,peg,additive,sx,sy,sx2,sy2);

   if ( glow ) {
    if ( clicking ) {
     (v=this->controller.variables.Get("click")) && v->tween 
      ? Stretchi(this->parent,glow,click,v->f,additive,sx,sy,sx2,sy2) 
      : StretchiNoAlpha(this->parent,glow,click,additive,sx,sy,sx2,sy2);
    } else if ( hovering ) {
     (v=this->controller.variables.Get("hover")) && v->tween 
      ? Stretchi(this->parent,glow,hover,v->f,hoverBlend,sx,sy,sx2,sy2) 
      : StretchiNoAlpha(this->parent,glow,hover,hoverBlend,sx,sy,sx2,sy2);
    }
   }
 }

 void Render(GLWindow *surface) {
   Draw(surface);
 }

 void Draw(GLWindow *surface) {
   Variable *v;
   this->controller.Updatef();
   if ( hidden ) return;  

   // draw the backing
   StretchiNoAlpha(surface,backing,groove,additive,x,y,w,h);

   // draw the slider peg
   StretchiNoAlpha(surface,slider,peg,additive,sx,sy,sx2,sy2);

   if ( glow ) {
    if ( clicking ) {
     (v=this->controller.variables.Get("click")) && v->tween 
      ? Stretchi(surface,glow,click,v->f,additive,sx,sy,sx2,sy2) 
      : StretchiNoAlpha(surface,glow,click,additive,sx,sy,sx2,sy2);
    } else if ( hovering ) {
     (v=this->controller.variables.Get("hover")) && v->tween 
      ? Stretchi(surface,glow,hover,v->f,hoverBlend,sx,sy,sx2,sy2) 
      : StretchiNoAlpha(surface,glow,hover,hoverBlend,sx,sy,sx2,sy2);
    }
   }
 }

 void MouseMove() {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  hovering=within(mx,my);
 }

 void LeftDown() {
  if ( hidden ) return;
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
  if ( within(mx,my) ) {
   clicking=true;
   SliderTo( (float) (my-y) / (float) h );
    if ( hovering ) SliderTo( (float) (my-y) / (float) h ); 
  }
 }

 void LeftUp(int mx, int my, Modifiers mods) {
  if ( hidden ) return;
  clicking=false; 
 }

};

class fx_RangeSelector : public Proce55or {
public:
 Zstring label;
 Zint len,loc;
 Zpointer<GLImage> backing,slider,glow;
 Zfloat AA,BB;
 Zbool hovering,clickingA,clickingB;
 Crayon groove,peg,click,hover,labelcolor;
 Zint sw,Asx,Asy,Asx2,Asy2, Bsx,Bsy,Bsx2,Bsy2, xmf;
 Zint mx,my;

 fx_RangeSelector() : Proce55or() {
  BB=1.0f;
  backing=library.find("data/images/elements/horizslider.png");
  slider=library.find("data/images/elements/buttonglow_1024_1.png");
  glow=library.find("data/images/elements/buttonglow_1024_4.png");
  mx=my=-1;
 }

 void Setup() {
  x2=x+w; y2=y+h;
  loc=y-10;
  if ( label.length > 0 ) len=(int)label.length;
  SliderToA(AA);
  SliderToB(BB);
 }

 virtual void WhenChanged() { }

 void SliderToA( float p ) {
  int a;
  AA=p;
  a=(int) (p*(float) (w-sw));
  Asy=y+0;
  Asy2=h;
  Asx=x+a;
  Asx2=sw;
  WhenChanged();
 }

 void SliderToB( float p ) {
  int a;
  BB=p;
  a=(int) (p*(float) (w-sw));
  Bsy=y+0;
  Bsy2=h;
  Bsx=x+a;
  Bsx2=sw;
  WhenChanged();
 }
 
 void Render(GLWindow *surface) { Draw(surface); }
 void Draw(GLWindow *surface) {
   Variable *v;
   this->controller.Updatef();
   if ( hidden ) return;  

   // draw the backing
   StretchiNoAlpha(surface,backing,groove,additive,x,y,w,h);

   // draw the slider peg for the A
   Stretchi(surface,slider,peg,0.5f,additive,Asx,Asy,Asx2,Asy2);

   // draw the slider peg for the B
   Stretchi(surface,slider,peg,0.5f,additive,Bsx,Bsy,Bsx2,Bsy2);

   // connect the slider pegs
   Stretchi(surface,slider,peg,0.5f,additive,Asx,Asy,Bsx2,Bsy2);

   if ( glow ) {
    if ( clickingA ) {
     if ( (v=this->controller.variables.Get("click")) && v->tween ) {
      Stretchi(surface,glow,click,v->f,additive,Asx,Asy,Asx2,Asy2);
     } else StretchiNoAlpha(surface,glow,click,additive,Asx,Asy,Asx2,Asy2);
    } else
    if ( clickingB ) {
     if ( (v=this->controller.variables.Get("click")) && v->tween ) {
      Stretchi(surface,glow,click,v->f,additive,Bsx,Bsy,Bsx2,Bsy2);
     } else StretchiNoAlpha(surface,glow,click,additive,Bsx,Bsy,Bsx2,Bsy2);
    } 
    else if ( hovering ) {
     if ( (v=this->controller.variables.Get("hover")) && v->tween ) {
      Stretchi(surface,glow,hover,v->f,additive,Asx,Asy,Asx2,Asy2);
      Stretchi(surface,glow,hover,v->f,additive,Bsx,Bsy,Bsx2,Bsy2);
     } else {
      StretchiNoAlpha(surface,glow,click,additive,Asx,Asy,Asx2,Asy2);
      StretchiNoAlpha(surface,glow,click,additive,Bsx,Bsy,Bsx2,Bsy2);
     }
    }
   }

   if ( len>0 ) {
//    surface.foreground=labelcolor;
//    surface.WriteText(x,loc,label,len);
   }
 }
 
 void Render() {
   Variable *v;
   this->controller.Updatef();
   if ( hidden ) return;  

   // draw the backing
   StretchiNoAlpha(backing,groove,additive,x,y,w,h);

   // draw the slider peg for the A
   Stretchi(slider,peg,0.5f,additive,Asx,Asy,Asx2,Asy2);

   // draw the slider peg for the B
   Stretchi(slider,peg,0.5f,additive,Bsx,Bsy,Bsx2,Bsy2);

   // connect the slider pegs
   Stretchi(slider,peg,0.5f,additive,Asx,Asy,Bsx2,Bsy2);

   if ( glow ) {
    if ( clickingA ) {
     if ( (v=this->controller.variables.Get("click")) && v->tween ) {
      Stretchi(glow,click,v->f,additive,Asx,Asy,Asx2,Asy2);
     } else StretchiNoAlpha(glow,click,additive,Asx,Asy,Asx2,Asy2);
    } else
    if ( clickingB ) {
     if ( (v=this->controller.variables.Get("click")) && v->tween ) {
      Stretchi(glow,click,v->f,additive,Bsx,Bsy,Bsx2,Bsy2);
     } else StretchiNoAlpha(glow,click,additive,Bsx,Bsy,Bsx2,Bsy2);
    } 
    else if ( hovering ) {
     if ( (v=this->controller.variables.Get("hover")) && v->tween ) {
      Stretchi(glow,hover,v->f,additive,Asx,Asy,Asx2,Asy2);
      Stretchi(glow,hover,v->f,additive,Bsx,Bsy,Bsx2,Bsy2);
     } else {
      StretchiNoAlpha(glow,click,additive,Asx,Asy,Asx2,Asy2);
      StretchiNoAlpha(glow,click,additive,Bsx,Bsy,Bsx2,Bsy2);
     }
    }
   }

   if ( len>0 ) {
//    surface.foreground=labelcolor;
//    surface.WriteText(x,loc,label,len);
   }
 }
 void MouseLeave() {
  hovering=clickingA=clickingB=false;
 }

 void MouseMove() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  hovering=within(mx,my);
  if ( hovering ) { 
   if ( clickingA ) SliderToA( (float) (mx-x) / (float) w ); 
   if ( clickingB ) SliderToB( (float) (mx-x) / (float) w ); 
  }
 }

 void LeftDown() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  if ( hovering=within(mx,my) ) { 
   clickingA=true; 
   SliderToA( (float) (mx-x) / (float) w ); 
  }
  else clickingA=hovering=false;
 }

 void LeftUp() {
  clickingA=false;
 }

 void RightDown() {
  if ( hidden ) return;
  MousePosition(&mx,&my);
  if ( hovering=within(mx,my) ) { 
   clickingB=true;
   SliderToB( (float) (mx-x) / (float) w ); 
  }
  else clickingB=hovering=false;
 }

 void RightUp() {
  clickingB=false;
 }

};

class fx_CoordPlane : public Proce55or {
public:
 Cartesiand low,high;
 Zbool hovering,clicking;
 Zint mx,my;
 Zint selectedX,selectedY;
 Zint selectorX,selectorY;
 Zdouble X,Y;
 Zdouble vX,vY;
 Zpointer<double> tXd,tYd;
 Zpointer<float> tXf,tYf;
 Zint yh2,xw2;
 Crayon clearColor, coordTint, bgTint, gridTint, originTint, borderTint, selectorTint, selectedTint;
 Cartesian rect;
 fx_CoordPlane() : Proce55or() {
  SetValue(0.5,0.5);
  clearColor.Pick(clear);
  coordTint.Pick(hotPink);
  bgTint.Float(0.2f,0.2f,0.4f,1.0f);
  gridTint.Float(0.3f,0.3f,0.6f,1.0f);
  originTint.Float(0.45f,0.45f,0.45f,1.0f);
  borderTint.Float(0.3f,0.5f,0.4f,1.0f);
  selectorTint.Float(1.0f,0.2f,0.3f,1.0f);
  selectedTint.Float(1.0f,1.0f,1.0f,1.0f);
  duration=10.0f;
 }
 void SetValue( double X, double Y ) {
   this->X=X;
   this->Y=Y;
   selectedX=x+(int)(X*(double)w);
   selectedY=y+(int)(Y*(double)h);
   vX=mapRanged(X,0.0f,1.0f,low.x,low.y);
   vY=mapRanged(Y,0.0f,1.0f,high.x,high.y);
   if ( tXd ) *tXd=vX;
   if ( tYd ) *tYd=vY;
   if ( tXf ) *tXf=(float)vX;
   if ( tYf ) *tYf=(float)vY;
   WhenClicked();
 }
 void SetExtents( int xx, int yy, int ww, int hh ) {
  x=xx;
  y=yy;
  w=ww;
  h=hh;
  x2=w+x;
  y2=h+y;
  xw2=x+w/2;
  yh2=y+h/2;
  rect.SetRect(x,y,w,h);
  SetValue(X,Y);
 }
 void MouseMove() {
  if ( hidden ) return;
  clicking=(input.left!=0);
  mx=input.mxi;
  my=input.myi;
  selectorX=input.mxi;
  selectorY=input.myi;
  if ( hovering=WITHIN(mx,my,x,y,x2,y2) && clicking ) {
   X=iratiod(input.mxi-x,w);
   Y=iratiod(input.myi-y,h);
   selectedX=input.mxi;
   selectedY=input.myi;
   vX=mapRanged(X,0.0f,1.0f,low.x,low.y);
   vY=mapRanged(Y,0.0f,1.0f,high.x,high.y);
   if ( tXd ) *tXd=vX;
   if ( tYd ) *tYd=vY;
   if ( tXf ) *tXf=(float)vX;
   if ( tYf ) *tYf=(float)vY;
   WhenClicked();
  }
 }
 virtual void WhenClicked() {}
 void Render() {
  if ( hidden ) return;
  Blending(none);
  Area(bgTint,x,y,x2,y2);
  Rectangle(borderTint,&rect);
  // Draw grid
  Blending(transparency);
  float deltaGrid=iratiof(w,10);
  for ( int i=1; i<10; i++ ) {
   int ofs=x+(int)(deltaGrid*(float)i);
   DrawLine2Color(gridTint,clearColor,ofs,y,ofs,yh2);
   DrawLine2Color(clearColor,gridTint,ofs,yh2,ofs,y2);
  }
  for ( int i=1; i<10; i++ ) {
   int ofs=y+(int)(deltaGrid*(float)i);
   DrawLine2Color(gridTint,clearColor,x,ofs,xw2,ofs);
   DrawLine2Color(clearColor,gridTint,xw2,ofs,x2,ofs);
  }
  // Draw origin
  DrawLine(originTint,x+w/2,y,x+w/2,y2);
  DrawLine(originTint,x,y+h/2,x2,y+h/2);
  // Draw selected
  DrawLine2Color(borderTint,selectedTint,x,selectedY,selectedX,selectedY);
  DrawLine2Color(selectedTint,borderTint,selectedX,selectedY,x2,selectedY);
  DrawLine2Color(borderTint,selectedTint,selectedX,y,selectedX,selectedY);
  DrawLine2Color(selectedTint,borderTint,selectedX,selectedY,selectedX,y2);
  DrawLine2Color(borderTint,selectedTint,x,selectedY,selectedX,selectedY);
  DrawLine2Color(selectedTint,borderTint,selectedX,selectedY,x2,selectedY);
  DrawLine2Color(borderTint,selectedTint,selectedX,y,selectedX,selectedY);
  DrawLine2Color(selectedTint,borderTint,selectedX,selectedY,selectedX,y2);
  // Draw selector
  Blending(additive);
  if ( WITHIN(selectorX,selectorY,x,y,x2,y2) ) {
   DrawLine2Color(borderTint,selectorTint,x,selectorY,selectorX,selectorY);
   DrawLine2Color(selectorTint,borderTint,selectorX,selectorY,x2,selectorY);
   DrawLine2Color(borderTint,selectorTint,selectorX,y,selectorX,selectorY);
   DrawLine2Color(selectorTint,borderTint,selectorX,selectorY,selectorX,y2);
   DrawLine2Color(borderTint,selectorTint,x,selectorY,selectorX,selectorY);
   DrawLine2Color(selectorTint,borderTint,selectorX,selectorY,x2,selectorY);
   DrawLine2Color(borderTint,selectorTint,selectorX,y,selectorX,selectorY);
   DrawLine2Color(selectorTint,borderTint,selectorX,selectorY,selectorX,y2);
  }
  // Draw coords
  glColor4fv(coordTint.floats);
  char buf[64];
  FORMAT(buf,64,"X: %1.2f", (double) X);
  Text(buf,(double)x+5,(double)y+15,5,7,false);
  FORMAT(buf,64,"Y: %1.2f", (double) Y);
  Text(buf,(double)x+5,(double)y+15+10,5,7,false);
 }
 void Render( GLWindow *surface ) {
  Render();
 }
 void Draw( GLWindow *surface ) {
  Render();
 }
};

class fx_ReadOnlySlider : public Proce55or {
public:
 Crayon border,hot,cold,labelColor;
 Cartesian rect;
 Zpointer<float> a;
 Zfloat aMax,aMin;
 Zpointer<float> b;
 Zfloat bMax,bMin;
 string label;
 fx_ReadOnlySlider() : Proce55or() {
  border.Pick(green);
  hot.Pick(red255);
  cold.Pick(cyan);
  labelColor.Pick(hotPink);
 }
 void SetExtents( int xx, int yy, int ww, int hh ) {
  x=xx;
  y=yy;
  w=ww;
  h=hh;
  rect.SetRect(x,y,w-16,h);
 }
 void Draw( GLWindow *surface ) {
  Render();
 }
 void RenderIndicatorTag( Crayon color, float value, int ourY ) {
  Cartesian tag;
  tag.SetRect(x+(w-16)+6,ourY-7,30,10);
  // Render a line at ourY
  Blending(none);
  DrawLine(crayons.Pick(alabaster),x+(w-16),ourY,x+(w-16)+6,ourY);
  Area(color,tag.x,tag.y,tag.x2,tag.y2);
  // Set color to black and render text on the tag
  glColor3d(0.0,0.0,0.0);
  char buf[64];
  FORMAT(buf,64,"%1.1f", (float) value);
  Text(buf,(double)(x+(w-16)+8),(double)ourY,4,6,false);
 }
 void Render() {
  if ( a ) {
   int ourY=rect.y+h-(int)((float)rect.h*((*a-aMin)/(aMax-aMin)));
   Blending(additive);
   Area(hot,rect.x,ourY,rect.x2,rect.y2);
   RenderIndicatorTag( hot, *a, ourY );
  }
  if ( b ) {
   int ourY=rect.y+h-(int)((float)rect.h*((*b-bMin)/(bMax-bMin)));
   Blending(additive);
   Area(cold,rect.x,ourY,rect.x2,rect.y2);
   RenderIndicatorTag( cold, *b, ourY );
  }
  Blending(none);
  Rectangle(border,&rect);
  Text((char *)label.c_str(),(double)(x+(w-16)/2),(double)(y-20),8,8,true);
  if ( a ) {   
   glColor4fv(hot.floats);
   char buf[64];
   FORMAT(buf,64,"%1.1f", (float)(*a) );
   Text(buf,(double)(x+(w-16)+8),(double)(y+h+4+8),8,8,true);
  }
  if ( b ) {   
   glColor4fv(cold.floats);
   char buf[64];
   FORMAT(buf,64,"%1.1f", (float)(*b) );
   Text(buf,(double)(x+(w-16)+8),(double)(y+h+4+8+10),8,8,true);
  }
 }
};


#include "UglyFont.h"
#include "Cartesian.h"
class fx_UglySlider : public Proce55or {
public:
 Zstring label;
 Cartesian labelPosition,pegRect,grooveRect;
 Zpointer<GLImage> backing,slider,glow;
 Zfloat value;
 Zpointer<float> binding;
 Zbool hovering,clicking;
 Crayon groove,peg,click,hover;
 Blends hoverBlend;
 Zint mx,my;
 fx_UglySlider() : Proce55or() {}
 fx_UglySlider( int x, int y, string label, float *binding ) : Proce55or() {
  backing=library.find("data/images/elements/horizslider.png");
  slider=library.find("data/images/elements/rounder1.png");
  glow=library.find("data/images/elements/rounder4.png");
  hovering=clicking=hidden=false;
  groove.Pick(gray);
  peg.Pick(alabaster);
  this->x=x;
  this->y=y;
  w=128;
  h=32;
  x2=x+w;
  y2=y+h;
  labelPosition.SetRect(x,y-8,6,8);
  pegRect.SetRect(x,y,16,h);
  grooveRect.Corners(x,y,x+w,y+h);
  w=128;
  duration=10.0f;
  hoverBlend=additive;
  Bind(binding);
  SetLabel(label);
 }

 void Bind( float *b ) {
  binding=b;
  value=*binding;
  SliderTo(value);
 }

 void SetLabel( string label ) {
  this->label=label;
 }

 bool within( int mx, int my ) { return WITHIN(mx,my,x,y,x2,y2); }

 virtual void WhenChanged() { }
 void SliderTo( float p ) {
  value=p;
  if ( binding ) *binding=p;
  pegRect.SetRect(x+(int)(value*(grooveRect.w-pegRect.w)),y,pegRect.w,pegRect.h);
  WhenChanged();
 }

 void Render() {
  if ( hidden ) return;
  this->controller.Updatef();
  QuadiNoAlpha(backing,groove,additive,&grooveRect);
  QuadiNoAlpha(slider,peg,additive,&pegRect);
  Blending(additive);
  int tempx=pegRect.x+pegRect.w/2;
  DrawLine(groove,tempx,pegRect.y,tempx,pegRect.y2);
  if ( glow ) {
   Variable *v;
   if ( clicking ) {
    (v=this->controller.variables.Get("click")) && v->tween 
     ? Quadi(glow,click,v->f,additive,pegRect.x,pegRect.y,pegRect.w,pegRect.h) 
     : QuadiNoAlpha(glow,click,additive,&grooveRect);
   } else if ( hovering ) {
    (v=this->controller.variables.Get("hover")) && v->tween 
     ? Quadi(glow,click,v->f,hoverBlend,pegRect.x,pegRect.y,pegRect.w,pegRect.h) 
     : QuadiNoAlpha(glow,hover,hoverBlend,&grooveRect);
   }
  }
  Blending(none);
  glColor3d(1.0,1.0,1.0);
  Text(label,labelPosition.x,labelPosition.y,labelPosition.w,labelPosition.h,false);
 }
 
 void Render(GLWindow *surface) { Render(); }
 void Draw(GLWindow *surface) { Render(); }

 void MouseMove() {
  if ( hidden ) return;
 }

 void Between() {
  hovering=within(input.mxi,input.myi);
 }
 void LeftDown() {
  if ( hidden ) return;
  if ( hovering ) {
   clicking=true;
   SliderTo( iratiof((input.mxi-x),grooveRect.w) );
  }
 }

 void LeftUp(int mx, int my, Modifiers mods) {
  if ( hidden ) return;
  clicking=false; 
 }

};