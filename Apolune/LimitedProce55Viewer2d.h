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

#include "GLWindow.h"
#include "Proce55ors.h"

class LimitedProce55Viewer2d : public GLWindow {
public:
 Proce55ors fx;
 float duration;
 float expired;
 bool paused;

 LimitedProce55Viewer2d() {
  paused=false;
  this->InitGLWindow();
  duration=10.0f;
  expired=0.0f;
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
  fx.Between(this);
 }

 void Reset(bool paused)   { fx.Reset(paused); }

 void Add( Proce55or *p )  { fx.Add(p,this); }
 void Add( Proce55or *p, bool init )  {
  fx.Add(p,this);
  if ( init ) {
   p->Init();
   p->Setup();
   p->Customize();
  }
 }
 void Redraw(void) {
  PreProcessing();
  fx.Redraw(this);
  PostProcessing();
 }
 void Render(void) {
  PreProcessing();
  fx.Render(this);
  PostProcessing();
 }

 void Drop( string name )  { fx.Drop(name);          }
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
 virtual void   KeyUp( KeyList *keys ) { fx.KeyUp(keys);    }
 virtual void KeyDown( KeyList *keys ) { fx.KeyDown(keys);  } 
};