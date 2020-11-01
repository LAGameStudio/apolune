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

#include "GLWindow.h"
#include "Proce55ors.h"

class WrappingProce55Viewer2d : public GLWindow {
public:
 Cartesian wrapLimit;
 Proce55ors fx;
 float duration;
 float expired;
 bool paused;

 WrappingProce55Viewer2d() : GLWindow() {
  paused=false;
  duration=10.0f;
  expired=0.0f;
 }

 void Init() {
  fx.Go();
 }

 virtual void OnLoad() {}
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}

 void BetweenFrames(void)  {
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
  PreProcessing();
  fx.Redraw(this,this->x,this->y,this->x2,this->y2);
  PostProcessing();
 }
 void Render(void) {
  PreProcessing();
  fx.Render(this,this->x,this->y,this->x2,this->y2);
  PostProcessing();
 }

 void Drop( string name )  { fx.Drop(name);    }
 void Drop( Proce55or *p ) { fx.Remove(p); delete p; }
 void MoveBy( double bx, double by ) {
  EACH(fx.first, Proce55or, p) {
   double tx, ty;
   tx = bx;
   if (p->x + bx < (double) wrapLimit.x) tx += (double) wrapLimit.w;
   else if (p->x + bx > (double) wrapLimit.x2) tx -= wrapLimit.w;
   ty = by;
   if (p->y + by < (double) wrapLimit.y) ty += (double)wrapLimit.h;
   else if (p->y + by > (double) wrapLimit.y2) ty -= (double) wrapLimit.h;
   p->MoveBy(tx, ty);
  }
 }
 void MoveBy(int bx, int by) {
  EACH(fx.first,Proce55or,p) {
   int tx,ty;
   tx = bx;
   if ( p->x+bx < wrapLimit.x ) tx+=wrapLimit.w;
   else if ( p->x+bx > wrapLimit.x2 ) tx-=wrapLimit.w;
   ty = by;
   if ( p->y+by < wrapLimit.y ) ty+=wrapLimit.h;
   else if ( p->y+by > wrapLimit.y2 ) ty-=wrapLimit.h;
   p->MoveBy(tx,ty);
  }
 }
 void MoveTo( int bx, int by ) { 
  EACH(fx.first,Proce55or,p) p->MoveTo(bx,by);
 }

 void Resized(int w,int h) { fx.Resized(w,h);  }
 void   MouseMove(void)    { fx.MouseMove();   }
 void   MouseOver(void)    { fx.MouseOver();   }
 void    LeftDown(void)    { fx.LeftDown();    }
 void   RightDown(void)    { fx.RightDown();   }
 void      LeftUp(void)    { fx.LeftUp();      }
 void     RightUp(void)    { fx.RightUp();     }
 void  MouseEnter(void)    { fx.MouseEnter();  }
 void  MouseLeave(void)    { fx.MouseLeave();  }
 void    MiddleUp(void)    { fx.MiddleUp();    }
 void  MiddleDown(void)    { fx.MiddleDown();  }
 void     WheelUp(void)    { fx.WheelUp();     }
 void   WheelDown(void)    { fx.WheelDown();   }
/*
 virtual void   LeftDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._LeftDouble(mx,my,mods);   if ( fx ) fx._LeftDouble(mx,my,mods);   return true; }
 virtual void  RightDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._RightDouble(mx,my,mods);  if ( fx ) fx._RightDouble(mx,my,mods);  return true; }
 virtual void MiddleDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._MiddleDouble(mx,my,mods); if ( fx ) fx._MiddleDouble(mx,my,mods); return true; }
 */
 virtual void   KeyUp( KeyList *keys ) { fx.KeyUp(keys);   }
 virtual void KeyDown( KeyList *keys ) { fx.KeyDown(keys); }  

};