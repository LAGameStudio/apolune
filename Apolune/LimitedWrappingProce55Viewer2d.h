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

class LimitedWrappingProce55Viewer2d : public GLWindow {
public:
 Cartesian wrapLimit;
 Proce55ors fx;
 Zbool paused;

 LimitedWrappingProce55Viewer2d() : GLWindow() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  wrapLimit.SetRect( (-display->w),(-display->h),(display->w*3),(display->h*3) );
 }

 void Init() {
  fx.Go();
 }

 void DebugArrows() {
  Cartesiand move;
  if ( input->KeyDown(DX_LEFT) )  {
   move.x=-1.0;
  }
  if ( input->KeyDown(DX_RIGHT) ) {
   move.x=1.0;
  }
  if ( input->KeyDown(DX_UP) ) {
   move.y=-1.0;
  }
  if ( input->KeyDown(DX_DOWN) ) {
   move.y=1.0;
  }
  if ( move.x != 0 || move.y != 0 ) MoveByd(move.x,move.y);
 }

 virtual void OnLoad() {}
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}

 virtual void BetweenFrames(void)  { // Done this way to avoid delay/duration
  EACH(fx.first,Proce55or,p) p->Between();
 }

 void Reset(bool paused)   { fx.Reset(paused); }

 void Add( Proce55or *p )  { fx.Add(p,this); }
 void AddPrepend( Proce55or *p )  { fx.AddPrepend(p,this); }
 void Add( Proce55or *p, bool init )  {
  fx.Add(p,this);
  if ( init ) {
   p->Init();
   p->Setup();
   p->Customize();
  }
 }
 void AddPrepend( Proce55or *p, bool init )  {
  fx.AddPrepend(p,this);
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

 void Drop( string name )  { fx.Drop(name); }
 void Drop( Proce55or *p ) {
  fx.Remove(p);
  delete p;
 }

 Vertexd excess;
 void MoveByd( double fx, double fy ) {
  int ix,iy;
  fx+=excess.x;
  fy+=excess.y;
  ix=(int) fx;
  iy=(int) fy;
  excess.Set( fx - (double) ix, fy - (double) iy );
  MoveBy(ix,iy);
 }
 void MoveBy( int bx, int by ) {
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
 void MoveBy( Proce55or *p, int bx, int by ) {
  int tx,ty;
  tx = bx;
  if ( p->x+bx < wrapLimit.x ) tx+=wrapLimit.w;
  else if ( p->x+bx > wrapLimit.x2 ) tx-=wrapLimit.w;
  ty = by;
  if ( p->y+by < wrapLimit.y ) ty+=wrapLimit.h;
  else if ( p->y+by > wrapLimit.y2 ) ty-=wrapLimit.h;
  p->MoveBy(tx,ty);
 }

 void MoveTo( int bx, int by ) { 
  EACH(fx.first,Proce55or,p) p->MoveTo(bx,by);
 }
 Vertexd excessmt;
 void MoveTo( double dx, double dy ) { 
  int ix,iy;
  dx+=excessmt.x;
  dy+=excessmt.y;
  ix=(int) dx;
  iy=(int) dy;
  excessmt.Set( dx - (double) ix, dy - (double) iy );
  EACH(fx.first,Proce55or,p) p->MoveTo(ix,iy);
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
