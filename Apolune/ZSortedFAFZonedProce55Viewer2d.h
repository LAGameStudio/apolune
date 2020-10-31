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
#include "ZonedProce55ors.h"
#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"

class ZSortedFAFZonedProce55Viewer2d : public GLWindow {
public:
 ZoneData zoneDimensions,zoneSize;
 Cartesian midpoint;  // What "0,0" is
 ZonedProce55orsGrid grid;
 Zbool paused;
 ScreenPosition screen;
 Zpointer<ZonedProce55orsList> active;
 ZSortedFAFZonedProce55Viewer2d() : GLWindow() {
  screen.Set(0.0,0.0,display->wd,display->hd);
  zoneSize.x=display->w*2;
  zoneSize.y=display->h*2;
 }

 void CenterScreen() {
  screen.Set(-display->wd/2.0,-display->hd/2.0,display->wd,display->hd);
  grid.screen.Set(&screen);
  grid.activeArea.PositionX( screen.dx+screen.dw/2.0-grid.activeArea.dw/2.0 );
  grid.activeArea.PositionY( screen.dy+screen.dh/2.0-grid.activeArea.dh/2.0 );
 }

 void SetExtents( int zw, int zh ) {  
  zoneDimensions.x=zw;
  zoneDimensions.y=zh;
  grid.Dimensions(zoneSize.x,zoneSize.y,zoneDimensions.x,zoneDimensions.y,true);
  CenterScreen();
 }

 void Init( bool between ) {
  for ( int i=0; i<zoneDimensions.x; i++ ) {
   for ( int j=0; j<zoneDimensions.y; j++ ) {
    ZonedProce55ors *p=&grid.zones(i,j);
    p->fx.Go();
    if ( between ) p->fx.Between();
   }
  }
 }

 virtual void OnLoad() {}
 virtual void PreProcessing() {}
 virtual void PostProcessing() {}

 virtual void BetweenFrames(void)  {
  active.Delete();
  active=grid.GetActiveZones();
  if ( active ) EACH(active->first,ZonedProce55orsHandle,p) p->p->fx.Between();
  for ( ZonedProce55orsHandle *v=(ZonedProce55orsHandle *) active->first; v; v=(ZonedProce55orsHandle *) v->next ) { 
   Proce55or *next_proce55or_in_list=null;
   for ( Proce55or *d=(Proce55or *) v->p->fx.first; d; d=next_proce55or_in_list ) { 
    next_proce55or_in_list=(Proce55or *) d->next;
    if ( d->Done() ) {
     v->p->fx.Remove(d);
     delete d;
    } else if ( d->rezoneMe ) {
     this->grid.Rezone(d);
    }
   }
  }
 }

 void Reset(bool paused)   {}

 void Add( Proce55or *p )  { grid.Add(p,this); }
 void Add( Proce55or *p, bool init )  {
  grid.Add(p,this);
  if ( init ) {
   p->Init();
   p->Setup();
   p->Customize();
  }
 }
 void Add( Proce55or *p, FireAndForgetLimitedWrappingProce55Viewer2d *faf, bool init )  {
  grid.Add(p,faf);
  if ( init ) {
   p->Init();
   p->Setup();
   p->Customize();
  }
 }
 bool AddIfLessThan( int limit, Proce55or *p, bool init )  {
  ZoneData zone=*grid.find(p->x,p->y);
  ZonedProce55ors *m=&grid.zones(zone.x,zone.y);
  if ( m->fx.count < limit ) {
   grid.Add(p,this);
   if ( init ) {
    p->Init();
    p->Setup();
    p->Customize();
   }
   return true;
  }
  return false;
 }
 bool RemoveDisposeIfGreaterThan( int limit, Proce55or *p ) {
  ZonedProce55ors *m=&grid.zones(p->zone.x,p->zone.y);
  if ( &m->fx == p->proce55ors && m->fx.count > limit ) {
   m->fx.Delete(p);
   return true;
  }
  return false;
 }
 void AddOriginCentered( Proce55or *p ) {
  grid.AddOriginCentered(p,this);
 }
 void Redraw(void) {}
 virtual void Render(void) {
  PreProcessing();
  ZonedProce55orsList *active=grid.GetActiveZones();
  if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{
   p->p->fx.RenderRelativeSorted(this,&screen);
  });
  PostProcessing();
 }

 void Drop( string name )  {
  for ( int i=0; i<zoneDimensions.x; i++ ) {
   for ( int j=0; j<zoneDimensions.y; j++ ) {
    ZonedProce55ors *p=&grid.zones(i,j);
    p->fx.Drop(name);
   }
  }
 }

 void Drop( Proce55or *p ) {
  grid.Remove(p);
  delete p;
 }

 virtual void WhenMovedTo( double fx, double fy ) {}
 virtual void WhenMoved( double fx, double fy ) {}
 void MoveByd( double dx, double dy ) {
  screen.MoveBy(dx,dy);
  grid.screen.Set(&screen);
  grid.activeArea.MoveBy(dx,dy);
  WhenMoved(dx,dy);
 }

 virtual void MousePosition( int *mx, int *my ) {
  *mx=(input->mxi-display->w2)+(int)(screen.x+screen.w/2.0);
  *my=(input->myi-display->h2)+(int)(screen.y+screen.w/2.0);
 }

 void MoveBy( int fx, int fy ) {
  screen.MoveBy((double)fx,(double)fy);
  grid.screen.Set(&screen);
  grid.activeArea.MoveBy((double)fx,(double)fy);
  WhenMoved((double)fx,(double)fy);
 }

 void MoveBy( double x, double y ) { 
  screen.MoveBy(x,y);
  grid.screen.Set(&screen);
  grid.activeArea.MoveBy(x,y);
  WhenMoved(x,y);
 }

 void MoveTod( double x, double y ) {
  screen.MoveTo(x-screen.dw2,y-screen.dh2);
  grid.screen.Set(&screen);
  grid.activeArea.PositionX(screen.dcx-grid.activeArea.dw/2.0);
  grid.activeArea.PositionY(screen.dcy-grid.activeArea.dh/2.0);
  WhenMovedTo(x,y);
 }

 void MoveTo( int x, int y ) {
  screen.MoveTo((double) x-screen.dw2, (double) y-screen.dh2 );
  grid.screen.Set(&screen);
  grid.activeArea.PositionX( screen.dcx-grid.activeArea.dw/2.0 );
  grid.activeArea.PositionY( screen.dcy-grid.activeArea.dh/2.0 );
  WhenMovedTo((double)x,(double)y);
 }
 
 virtual void Resized(int w,int h) {}

 virtual void   MouseMove(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MouseMove(); });  }
 virtual void   MouseOver(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MouseOver(); });  }
 virtual void    LeftDown(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.LeftDown(); });   }
 virtual void   RightDown(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.RightDown(); });  }
 virtual void      LeftUp(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.LeftUp(); });     }
 virtual void     RightUp(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.RightUp(); });    }
 virtual void  MouseEnter(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MouseEnter(); }); }
 virtual void  MouseLeave(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MouseLeave(); }); }
 virtual void    MiddleUp(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MiddleUp(); });   }
 virtual void  MiddleDown(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.MiddleDown(); }); }
 virtual void     WheelUp(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.WheelUp(); });    }
 virtual void   WheelDown(void) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.WheelDown(); });  }
/*
 virtual void   LeftDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._LeftDouble(mx,my,mods);   if ( fx ) fx._LeftDouble(mx,my,mods);   return true; }
 virtual void  RightDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._RightDouble(mx,my,mods);  if ( fx ) fx._RightDouble(mx,my,mods);  return true; }
 virtual void MiddleDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._MiddleDouble(mx,my,mods); if ( fx ) fx._MiddleDouble(mx,my,mods); return true; }
 */
 virtual void   KeyUp( KeyList *keys ) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.KeyUp(keys); });   }
 virtual void KeyDown( KeyList *keys ) { if ( active ) EACHN(active->first,ZonedProce55orsHandle,p,{ p->p->fx.KeyDown(keys); }); }

 Crayon drawing;
 void DebugDrawScreenPosition();
 virtual void Debug();
 virtual void MiniDebug();
};

class ZSortableFAFZonedProce55Viewer2d : public ZSortedFAFZonedProce55Viewer2d {};
class FAFZonedProce55Viewer2d : public ZSortedFAFZonedProce55Viewer2d {
public:
 virtual void Render(void) {
  PreProcessing();
  ZonedProce55orsList *active=grid.GetActiveZones();
#if defined(GRANULAR_DEBUG)
  DWORD renderTime=timeGetTime();
  int i=0;
  EACHN(active->first,ZonedProce55orsHandle,p,{
   renderTime=timeGetTime();
   p->p->fx.RenderRelative(this,&screen);
   if ( input->KeyDown(DX_PERIOD) ) {
    renderTime=timeGetTime()-renderTime;
    OUTPUT("->FAFZonedProce55Viewer2d::Render() zone %d took %d ms\n",
     (int) i,(int)renderTime);
    i++;
   }
  });
#else
  EACHN(active->first,ZonedProce55orsHandle,p,{
   p->p->fx.RenderRelative(this,&screen);
  });
#endif
  PostProcessing();
 }
 void Clear() {
  grid.zones.resize(0,0);
 }
};