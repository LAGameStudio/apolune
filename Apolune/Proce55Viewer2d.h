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
#include "ListItem.h"
#include "LinkedList.h"
#include "Vertex.h"
#include "Proce55or.h"
#include "Slide.h"
#include "Presentation.h"

#include "TexFont.h"

class Proce55Viewer2d : public GLWindow {
public:
 Proce55ors fx;
 Zfloat duration;
 Zfloat expired;
 Zbool paused;

 PROGRAMMABLE(Proce55Viewer2d,RenderNormalPtr,renderNormal,RenderNormal);
 PROGRAMMABLE(Proce55Viewer2d,RenderAfterPtr,renderDeferred,RenderDeferred);

 Proce55Viewer2d() {
  this->InitGLWindow();
  duration=10.0f;
  renderNormal=&Proce55Viewer2d::Now;
  renderDeferred=&Proce55Viewer2d::NotNow;
 }

 // Defers the rendering method to put children behind proce55or gauges
 void Deferred() {
  renderNormal=&Proce55Viewer2d::NotNow;
  renderDeferred=&Proce55Viewer2d::Now;
 }

 void Init() {
  fx.Go();
 }

 virtual void OnLoad() {}
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}

 void Between(void)  {
  if ( paused ) return;
  expired+=(float) (1.0/FPS);
  if ( expired >= duration ) {
   expired=0.0f;
   fx.Reset(paused);
  }
  fx.Between(this->x,this->y,this->x2,this->y2);
 }

 void Reset(bool paused)   { fx.Reset(paused); }

 void Add( Proce55or *p )  { fx.Add(p,this); }
 void Redraw(void) {
  Now();
 }
 void Render(void) {
  RenderNormal();
 }
 void RenderAfter(void) {
  RenderDeferred();
 }

 void NotNow(void) {}
 void Now(void) {
  PreProcessing();
  fx.Render(this,this->x,this->y,this->x2,this->y2);
  PostProcessing();
 }

 void Drop( string name )  { fx.Drop(name); }
 void Drop( Proce55or *p ) { fx.Remove(p); delete p; }
 void MoveBy( int bx, int by ) { 
  EACH(fx.first,Proce55or,p) p->MoveBy(bx,by);
 }
 void MoveTo( int bx, int by ) { 
  EACH(fx.first,Proce55or,p) p->MoveTo(bx,by);
 }

 void Resized(int w,int h) { fx.Resized(w,h); }
 virtual void   MouseMove(void)    { fx.MouseMove();  }
 virtual void   MouseOver(void)    { fx.MouseOver();  }
 virtual void    LeftDown(void)    { fx.LeftDown();   }
 virtual void   RightDown(void)    { fx.RightDown();  }
 virtual void      LeftUp(void)    { fx.LeftUp();     }
 virtual void     RightUp(void)    { fx.RightUp();    }
 virtual void  MouseEnter(void)    { fx.MouseEnter(); }
 virtual void  MouseLeave(void)    { fx.MouseLeave(); }
 virtual void    MiddleUp(void)    { fx.MiddleUp();   }
 virtual void  MiddleDown(void)    { fx.MiddleDown(); }
 virtual void     WheelUp(void)    { fx.WheelUp();    }
 virtual void   WheelDown(void)    { fx.WheelDown();  }
/*
 virtual void   LeftDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._LeftDouble(mx,my,mods);   if ( fx ) fx._LeftDouble(mx,my,mods);   return true; }
 virtual void  RightDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._RightDouble(mx,my,mods);  if ( fx ) fx._RightDouble(mx,my,mods);  return true; }
 virtual void MiddleDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._MiddleDouble(mx,my,mods); if ( fx ) fx._MiddleDouble(mx,my,mods); return true; }
 */
 virtual void   KeyUp( KeyList *keys ) { fx.KeyUp(keys);   }
 virtual void KeyDown( KeyList *keys ) { fx.KeyDown(keys); }  

};