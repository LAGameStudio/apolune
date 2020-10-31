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
// Beware of the Turing tar-pit in which everything is possible but nothing of interest is easy.
// —Alan Perlis, Epigrams on Programming

#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

#include "ListItem.h"
#include "LinkedList.h"
#include "Proce55Controller.h"
#include "GLWindow.h"
#include "Art.h"
#include "UglyFont.h"
#include "ScreenPosition.h"
#include "RectPlacer.h"
#include "ZIndexed.h"

class Collision2dProce55or;

class ZonedProce55orsGrid;

extern Interface input;

class Proce55ors;

struct ZoneData { 
 Zint x,y;
 Zbool unzoned,left,upLeft,up,upRight,right,downRight,down,downLeft;
 ZoneData() { unzoned=true; }
 string toString() {
  char buf[64];
  FORMAT(buf,64,"%d %d",(int)x,(int)y);
  return string(buf);
 }
 void fromString( const char *t ) {
  const char *p= t;
  char buf[64];
  p=one_argument(p,buf);
  x=atoi(buf);
  p=one_argument(p,buf);
  y=atoi(buf);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->write(&x.value);
  open->write(&y.value);
 }
 void BinaryRead( BinaryFile *open ) {
  open->read(&x.value);
  open->read(&y.value);
 }
 void Set( int x, int y ) { this->x=x; this->y=y; }
};

class Presentation;
ONE(Proce55or,{ Constructor(); })
 Zpointer<GLWindow> parent;
 Zpointer <Presentation> presentationParent;
 Zpointer<Proce55or> head_of_list,next_in_list;
 Zpointer<Proce55ors> proce55ors;
 Proce55Controller controller;
 ZoneData zone;
 Zdouble expired, duration, delay, delayplusduration;
 Zint x, y, w, h, x2, y2; // actual
 Cartesiand position,velocity,centerOffset;
 Zbool rezoneMe,unRezoned,farStorage,perpetual;
 Zbool hidden,dropMe,completed;
 Zstring debugString;
 Zint grouping;
 /// Collision
 Zpointer<Collision2dProce55or> collision;
 Cartesian collision2d;
 Zbool showingDebug;   // Use in Render() if the Proce55or is in debug mode.
 virtual void OnCollide2d() {}
 ///
 // Used by RenderRelative and the "Zoned Proce55or Grid" etc
 ScreenPosition translated;
 virtual void Translate( ScreenPosition *screen );

 virtual void Hide() { hidden=true; }
 virtual void Reveal() { hidden=false; }

 virtual void SetExtents( int x, int y, int w, int h ) {
  this->x=x;
  this->y=y;
  this->w=w;
  this->h=h;
  this->x2=x+w;
  this->y2=y+h;
  OnSetExtents();
 }
 virtual void OnSetExtents() {
 }

 void Constructor();
 Proce55or( string n ) : ListItem() {
  Constructor();
  name=n;
 }
 virtual void OnDrop() {} // Use this one if when you proce55ors->Drop()

 // OnDispose() has been removed because the virtual is never called on the Derived,
 // however C++ destructors are called in reverse of construction, ie: ~Derived,~Base, so use them.
 ~Proce55or(void) {}

 virtual void GetMouseBox( Cartesian *out ) {}

 virtual void MousePosition( Zint *mx, Zint *my ) {
  *mx=input.mxi-this->parent->x;
  *my=input.myi-this->parent->y;
 }

 virtual void MousePosition( int *mx, int *my ) {
  *mx=input.mxi-this->parent->x;
  *my=input.myi-this->parent->y;
 }
 virtual bool within( int mx, int my );
 
 virtual void MovedTo( double nx, double ny ) {}
 virtual void MoveTo( double nx, double ny ) {
  this->x=nx;
  this->y=ny;
  x2=nx+w;
  y2=ny+h;
  MovedTo(nx,ny);
 }
 virtual void MoveTo( int nx, int ny ) {
  this->x=nx;
  this->y=ny;
  x2=nx+w;
  y2=ny+h;
  MovedTo((double)nx,(double)ny);
 }
 virtual void MovedBy(double byx,double byy) {}
 virtual void MoveBy( double byx, double byy ) {
  x+=(int)byx;
  y+=(int)byy;
  x2+=(int)byx;
  y2+=(int)byy;
  MovedBy(byx,byy);
 }
 virtual void MoveBy( int byx, int byy ) {
  x+=byx;
  y+=byy;
  x2+=byx;
  y2+=byy;
  MovedBy((double)byx,(double)byy);
 }
 virtual bool Receive( Proce55or *sender, int code, void *message );
 void OnReset( bool paused );
 
 virtual void Attach() {}
 virtual void OnLoad(void)     {}
 virtual void Init() {}
 void _Setup() {
  delayplusduration=delay+duration;
  Setup();
 }
 virtual void Setup()     {}
 virtual void Customize() {}
 virtual void PostSetup() {} // Not fully supported everywhere
 void RepopulateOthers();
 virtual void OnRepopulate() {}
 virtual void Reset( bool paused ) {}
 virtual void Redraw();
 virtual void RenderHeight() {}     ///If you declare these here, Proce55or::RenderHeight won't be called on derived?
 virtual void RenderEmissive() {}   ///If you declare these here, Proce55or::RenderEmissive won't be called on derived?
 virtual void RenderMultiply() {}   ///If you declare these here, Proce55or::RenderEmissive won't be called on derived?
 virtual void RenderHeight( bool alternate ) {}     ///If you declare these here, Proce55or::RenderHeight won't be called on derived?
 virtual void RenderEmissive( bool alternate ) {}   ///If you declare these here, Proce55or::RenderEmissive won't be called on derived?
 virtual void RenderMultiply( bool alternate ) {}   ///If you declare these here, Proce55or::RenderEmissive won't be called on derived?
 virtual void Render(); 
 virtual void Redraw(GLWindow *target);
 virtual void Render(GLWindow *target); 
 virtual void BetweenFrames(); // has default functionality for Presentation, should be used only for Presentation, override if needed
 virtual void Between() {}
 virtual void Draw(GLWindow *surface);
 virtual void RenderRelative( GLWindow *surface, Cartesiand *offset ) {}
 virtual void RenderRelative( GLWindow *surface, ScreenPosition *offset ) {}
 virtual void Clean() {}

 virtual void Resized( int w, int h ) {}
 virtual void OnUnload(void)   {}
 virtual void OnFocus()        {}
 virtual void OnFocusLost()    {}
 virtual void MouseMove(void)  {}
 virtual void MouseOver(void)  {}
 virtual void LeftDown(void)   {}
 virtual void RightDown(void)  {}
 virtual void LeftUp(void)     {}
 virtual void RightUp(void)    {}
 virtual void MouseEnter(void) {}
 virtual void MouseLeave(void) {}
 virtual void MiddleUp(void)   {}
 virtual void MiddleDown(void) {}
 virtual void WheelUp(void)    {}
 virtual void WheelDown(void)  {}
 /*
 virtual void LeftDouble();
 virtual void RightDouble();
 virtual void MiddleDouble();
 */
 virtual void KeyUp( KeyList *keys )   {}
 virtual void KeyDown( KeyList *keys ) {}
 
 // Used by the ZSorter, allows you to customize what the "Y" value is
 virtual int Y() { return y2; }

  virtual bool OnScreen() {
  return ( x >= -w && x2 <= display.w+w ) && ( y >= -h && y2 <= display.h+h );
 }
  // Customized by child
  virtual bool OnScreenTranslated();

 void GetNormalizedZonedPosition( ZonedProce55orsGrid *grid, double *outx, double *outy );
  
 virtual void OnExpired() {}
 virtual void OnComplete() {}
 // Used in "FireAndForget" presentation classes (not Presenter)
 // Tells you when it's "done" so it can be deleted.
 // Use perpetual to override. (This is a recent change and may require updates to all Proce55or childs
 virtual bool Done() {
  if (perpetual) return false;
  bool status=(expired>duration);
  if ( status && !completed ) {
   completed=true;
   OnComplete();
   OnExpired();
  }
  return status;
 }
 
 Zint storedI,storedJ; Zbool storedFar;
 virtual void OnStore( int i, int j ) {}
 virtual void OnRetrieve() { storedFar=false; }

 // YOU CAN'T USE THIS (AND I DON'T UNDERSTAND WHY).  SO..
 // virtual void Clone( Proce55or **_out ) {}
 // virtual void Mutate() {} // Modifies a clone in some logical way.
 // DO FUNCTION POINTERS< GROSS >

 void Rezone();
 virtual void Debug() {}
 
MANY(Proce55or,Proce55orHandle,Proce55orHandles,"Proce55or",Proce55ors,{})
 Zbool Zsort;
 Zp<GLWindow> lastParentAdded;
 
 Proce55or *Within( int mx, int my ) {
  Proce55or *top=null;
  FOREACH(Proce55or,p) if ( p->within(mx,my) ) top=p;
  return top;
 }

 void GetNearby( int px, int py, int dist, Proce55orHandles *out ) {
  FOREACH(Proce55or,p) if ( idistance(px,py,p->x,p->y) < dist ) out->Add(p);
 }

 // ZSorting
 SORTING(Proce55or,ZSort,{},{ if ( A->Y()<B->Y() ) result=-1; },{})

 // Rendered when the ZSorter encounters the insertion.y value, used by RenderZInserted()
 // Used by the older ZSorter classes
 Cartesian insertion;
 virtual void RenderInsert() {}
 virtual void RenderInsert(GLWindow *t) {}

 // Needs a parent window provided, executes Attach() virtual
 void Add( Proce55or *p, Presentation *pp, Proce55or *placeBehind=null  );
 void Add( Proce55or *p, GLWindow *pp, Proce55or *placeBehind=null );
 void Add( Proce55or *p, Proce55or *placeBehind=null );
 // Needs a parent window provided, executes Attach() virtual, prepends to list.
 void AddPrepend( Proce55or *p, Presentation *pp );
 void AddPrepend( Proce55or *p, GLWindow *pp );
 void AddPrepend( Proce55or *p );
 Proce55or *find( const char *name ) {
  string n=string(name);
  EACH(first,Proce55or,p) if ( n==p->name ) return p;
  return null;
 }

 Proce55or *pointTest( int x, int y ) {
  EACH(first,Proce55or,p) if ( p->within(x,y) ) return p;
  return null;
 }

 Proce55or *boxTest( int x, int y, int x2, int y2 ) {
  EACH(first,Proce55or,p) 
   if ( p->within(x,y) 
     || p->within(x2,y2)
     || p->within(x,y2)
     || p->within(x2,y)
    ) return p;
  return null;
 }

 Cartesiand ____excess;
 void MoveBy( float x, float y ) {
  double fx=x+____excess.x, fy=y+____excess.y;
  int ix=(int) fx, iy=(int) fy;
  ____excess.Set( fx - (double) ix, fy - (double) iy );
  EACH(first,Proce55or,p) p->MoveBy(ix,iy);
 }
 void MoveBy( double x, double y ) { EACH(first,Proce55or,p) p->MoveBy(x,y); }
 void MoveBy( int x, int y ) { EACH(first,Proce55or,p) p->MoveBy(x,y); }

 
 void MoveTo( double x, double y ) { EACH(first,Proce55or,p) p->MoveTo(x,y); }
 void MoveTo( int x, int y ) { EACH(first,Proce55or,p) p->MoveTo(x,y); }

 void Go() {
  Proce55or *p;
  for ( p=(Proce55or *)first; p; p=(Proce55or *) (p->next) ) {
   p->OnLoad();
   p->Init();
   p->Customize();
   p->Setup(); // was before customize.. should it be?
   p->PostSetup();
  }
 }

 void Setup(void)           { EACH(first,Proce55or,p) p->_Setup();      }
 void Reset( bool paused )  { EACH(first,Proce55or,p) p->OnReset(paused);    }
 void Repopulate()          { EACH(first,Proce55or,p) p->OnRepopulate();     }
 void Between(void)         { EACH(first,Proce55or,p) p->BetweenFrames();    } // subroutine calls Between() when appropriate
 void Between( int x, int y, int x2, int y2 ) {
  EACH(first,Proce55or,p) {
   if ( p->x > x-p->w  && p->y > y-p->h
     && p->x < x2+p->w && p->y < y2+p->h )
    p->Between();
  }
 }
 void Between( GLWindow *t ) {
  EACH(first,Proce55or,p) p->Between();
 }
 void Redraw(void)          { EACH(first,Proce55or,p) p->Redraw();     }
 void Redraw( GLWindow *t, int x, int y, int x2, int y2 ) {
  EACH(first,Proce55or,p) {
   if ( p->x > x-p->w  && p->y > y-p->h
     && p->x < x2+p->w && p->y < y2+p->h )
    p->Render(t);
  }
 }
 void Redraw( GLWindow *t ) {
  EACH(first,Proce55or,p) p->Render(t);
 }
 void RenderHeight(void)    { FOREACH(Proce55or,p) p->RenderHeight();   }
 void RenderEmissive(void)  { FOREACH(Proce55or,p) p->RenderEmissive(); }
 void RenderMultiply(void)  { FOREACH(Proce55or,p) p->RenderMultiply(); }
 void RenderHeight( having alternate )    { FOREACH(Proce55or,p) p->RenderHeight(true);   }
 void RenderEmissive( having alternate )  { FOREACH(Proce55or,p) p->RenderEmissive(true); }
 void RenderMultiply( having alternate )  { FOREACH(Proce55or,p) p->RenderMultiply(true); }
 void Render(void)          { EACH(first,Proce55or,p) p->Render();      }
 void Render( GLWindow *t, int x, int y, int x2, int y2 ) {
  EACH(first,Proce55or,p) {
   if ( p->x > x-p->w  && p->y > y-p->h
     && p->x < x2+p->w && p->y < y2+p->h )
    p->Render(t);
  }
 }
 void Render( GLWindow *t ) {
  EACH(first,Proce55or,p) p->Render(t);
 }
 void RenderRelative( GLWindow *t, ScreenPosition *screen ) {  
#if defined(GRANULAR_DEBUG)
 DWORD renderTime=timeGetTime();
 int i=0;
#endif
  EACH(first,Proce55or,p) {
#if defined(GRANULAR_DEBUG)
   renderTime=timeGetTime();
#endif
   p->Translate(screen);
   p->RenderRelative(t,screen);
#if defined(GRANULAR_DEBUG)
   if ( input.KeyDown(DX_PERIOD) ) {
    renderTime=timeGetTime()-renderTime;
    OUTPUT("->Proce55ors::RenderRelative() proce55or %d took %d ms\n",
     (int)i,(int)renderTime);
    i++;
   }
#endif
  }
 }
 void RenderRelativeSorted( GLWindow *t, ScreenPosition *screen ) {
  EACH(first,Proce55or,p) p->Translate(screen);
  // Y() must be overridden in any Proce55or child used in a ZonedProce55orGrid
  EACH(first,Proce55or,p) p->RenderRelative(t,screen);
 }

 void RenderZInserted(void) {
  Proce55or *p=(Proce55or *) first;
  if ( !p ) return;
  while ( p->y <= insertion.y ) {
   p->Render();
   p=(Proce55or *) p->next;
  }
  RenderInsert();
  while ( p ) {
   p->Render();
   p=(Proce55or *) p->next;
  }
 }

 void RenderZInserted( GLWindow *t ) {
  Proce55or *p=(Proce55or *) first;
  if ( !p ) return;
  while ( p->y <= insertion.y ) {
   if ( p->OnScreen() ) p->Render(t);
   p=(Proce55or *) p->next;
  }
  RenderInsert(t);
  while ( p ) {
   if ( p->OnScreen() ) p->Render(t);
   p=(Proce55or *) p->next;
  }
 }

 void OnLoad(void)          { EACH(first,Proce55or,p) p->OnLoad();     }
 void Resized(int w, int h) { EACH(first,Proce55or,p) p->Resized(w,h); }
 void OnUnload(void)        { EACH(first,Proce55or,p) p->OnUnload();   }
 void MouseMove(void)       { EACH(first,Proce55or,p) p->MouseMove();  }
 void MouseOver(void)       { EACH(first,Proce55or,p) p->MouseOver();  }
 void LeftDown(void)        { EACH(first,Proce55or,p) p->LeftDown();   }
 void RightDown(void)       { EACH(first,Proce55or,p) p->RightDown();  }
 void LeftUp(void)          { EACH(first,Proce55or,p) p->LeftUp();     }
 void RightUp(void)         { EACH(first,Proce55or,p) p->RightUp();    }
 void MouseEnter(void)      { EACH(first,Proce55or,p) p->MouseEnter(); }
 void MouseLeave(void)      { EACH(first,Proce55or,p) p->MouseLeave(); }
 void MiddleUp(void)        { EACH(first,Proce55or,p) p->MiddleUp();   }
 void MiddleDown(void)      { EACH(first,Proce55or,p) p->MiddleDown(); }
 void WheelUp(void)         { EACH(first,Proce55or,p) p->WheelUp();    }
 void WheelDown(void)       { EACH(first,Proce55or,p) p->WheelDown();  }
 /*
 void LeftDouble();
 void RightDouble();
 void MiddleDouble();
 */
 void KeyUp( KeyList *keys )   { Proce55or *p; for ( p=(Proce55or *)first; p; p=(Proce55or *) (p->next ) ) p->KeyUp(keys);   }
 void KeyDown( KeyList *keys ) { Proce55or *p; for ( p=(Proce55or *)first; p; p=(Proce55or *) (p->next ) ) p->KeyDown(keys); }
 
 void Drop( string name ) {
  FOREACHN(Proce55or,p,{
   if ( p->name == name ) {
    Remove( p );
    p->OnDrop();
    delete p;
   }
  });
 }

 bool Drop( Proce55or *drop ) {
  FOREACHN(Proce55or,p,{
   if ( p == drop ) {
    Remove( p );
    p->OnDrop();
    delete p;
    return true;
   }
  });
  return false;
 }

 void Drop( Proce55orHandles *ps );

 void DropDone() {
  EACHN(first,Proce55or,p,{
   if ( p->Done() || p->dropMe ) {
    Remove(p);
    delete p;
   }
  });
 }

 void Debug() {
  FOREACH(Proce55or,p) {
   Cartesian rect;
   rect.x=p->x;
   rect.y=p->y;
   rect.x2=p->x2;
   rect.y2=p->y2;
   rect.w=p->w;
   rect.h=p->h;
   Rectangle(crayons.Pick(blue),&rect);
   string classname=typeid(*p).name();
   string discard;
   const char *q=string_argument(classname.c_str(),&discard); // discards the words "class or struct"
   Crayon drawing; 
   drawing.Pick(black); glColor4fv(drawing.floats);
   Text(q, (double) (rect.x+rect.w/2+1), (double) (rect.y+rect.h/2+1), 10.0, 8.0, true);
   drawing.Pick(alabaster); glColor4fv(drawing.floats);
   Text((char *)classname.c_str(), (double) (rect.x+rect.w/2), (double) (rect.y+rect.h/2), 10.0, 8.0, true);
   char buf[80];
   int mx,my;
   p->MousePosition(&mx,&my);
   FORMAT(buf,80,"mx,y: %d,%d",(int)mx,(int)my);
   Text(buf, (double) (rect.x+rect.w/2), (double) (rect.y+rect.h/2)+12.0, 10.0, 8.0, true);
   FORMAT(buf,80,"x,y,x2,y2,wxh: %d,%d,%d,%d,%dx%d",
    (int)p->x,(int)p->y,(int)p->x2,(int)p->y2,(int)p->w,(int)p->h);
   Text(buf, (double) (rect.x+rect.w/2), (double) (rect.y+rect.h/2)+24.0, 10.0, 8.0, true);
  }
 }
 void FitWithin( int startx, int starty, int maxw, int maxh ) {
  FOREACH(Proce55or,widg) widg->SetExtents(widg->x,widg->y,widg->w,widg->h);
  RectPack rectPack;
  rectPack.inputs.Clear();
  rectPack.Import(this);
  rectPack.packSize.Set(maxw,maxh);
  rectPack.growLimit.Set(maxw,maxh);
  rectPack.growth.Set(0,0);
  rectPack.Grow();
  rectPack.Pack();
  rectPack.Export(startx,starty,this);
 }
 void FindWithin( Proce55orHandles *out, int x, int y, int w, int h ) {
  Cartesian bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( bounds.within(p->x,p->y) ) out->Add(p);
  }
 }
 void FindWithin( having inclusive, Proce55orHandles *out, int x, int y, int w, int h ) {
  Cartesian bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( bounds.withinclusive(p->x,p->y) ) out->Add(p);
  }
 }
 void FindOutside( Proce55orHandles *out, int x, int y, int w, int h ) {
  Cartesian bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( !bounds.within(p->x,p->y) ) out->Add(p);
  }
 }
 void FindOutside( having inclusive, Proce55orHandles *out, int x, int y, int w, int h ) {
  Cartesian bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( !bounds.withinclusive(p->x,p->y) ) out->Add(p);
  }
 }
 void FindWithin( Proce55orHandles *out, double x, double y, double w, double h ) {
  Cartesiand bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( bounds.within(p->x,p->y) ) out->Add(p);
  }
 }
 void FindOutside( Proce55orHandles *out, double x, double y, double w, double h ) {
  Cartesiand bounds(x,y,w,h);
  FOREACH(Proce55or,p) {
   if ( !bounds.within(p->x,p->y) ) out->Add(p);
  }
 }
 void FindWithin( Proce55orHandles *out, Cartesian *c ) { FindWithin(out,c->x,c->y,c->w,c->h); }
 void FindWithin( having inclusive, Proce55orHandles *out, Cartesian *c ) { FindWithin(true,out,c->x,c->y,c->w,c->h); }
 void FindOutside( Proce55orHandles *out, Cartesian *c ) { FindOutside(out,c->x,c->y,c->w,c->h); }
 void FindOutside( having inclusive, Proce55orHandles *out, Cartesian *c ) { FindOutside(true,out,c->x,c->y,c->w,c->h); }
 void FindWithin( Proce55orHandles *out, Cartesiand *c ) { FindWithin(out,c->x,c->y,c->w,c->h); }
 void FindOutside( Proce55orHandles *out, Cartesiand *c ) { FindOutside(out,c->x,c->y,c->w,c->h); }
DONE(Proce55or);


HANDLED(Proce55ors,Proce55orsHandle,Proce55orsHandles,Proce55orsHandlesHandle,Proce55orsHandlesHandles);

class Proce55orDebugger : public Proce55or {
public:
 virtual void Debug() {
  Cartesian extents; extents.SetRect(x,y,w,h);
  Crayon drawing;
  drawing.Pick(green);
  drawing.ScaleByAlphaf(0.25f);
  glColor4fv(drawing.floats);
  Blending(transparency);
  Area(drawing,x,y,w,h);
  drawing.Any();
  Rectangle(drawing,&extents);
  drawing.Pick(alabaster);
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue),(double)x,(double)y,(double)x2,(double)y2);
  DrawLine2Color(crayons.Pick(yellow),crayons.Pick(green),(double)x,(double)y2,(double)x2,(double)y);
  char buf[80];
  FORMAT(buf,80,"%d,%d",(int)x,(int)y);
  Text(buf,(double) (x+w/2), (double) (y+h/2), 10.0, 8.0, true );
 }
 void Render() { Debug(); }
 void Render( GLWindow *t ) { Debug(); }
};

class MovingProce55orDebugger : public Proce55or {
public:
 virtual void Debug() {
  Cartesian extents; extents.SetRect(x,y,w,h);
  Crayon drawing;
  drawing.Pick(green);
  drawing.ScaleByAlphaf(0.25f);
  glColor4fv(drawing.floats);
  Blending(transparency);
  Area(drawing,x,y,w,h);
  drawing.Any();
  Rectangle(drawing,&extents);
  drawing.Pick(alabaster);
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue),(double)x,(double)y,(double)x2,(double)y2);
  DrawLine2Color(crayons.Pick(yellow),crayons.Pick(green),(double)x,(double)y2,(double)x2,(double)y);
  char buf[80];
  FORMAT(buf,80,"%d,%d",(int)x,(int)y);
  Text(buf,(double) (x+w/2), (double) (y+h/2), 10.0, 8.0, true );
 }
 void Between() {
  this->MoveBy( 1-upto(3), 1-upto(3) );
 }
 void Render() { Debug(); }
 void Render( GLWindow *t ) { Debug(); }
};

enum Proce55orTypes {
 p5_CoolButton=0,
 p5_CoolButtonToggle=1,
 p5_CoolToggle=2,
 p5_DirectButton=3,
 p5_TexFontCoolButton=4,
 p5_UglyButton=5,
 p5_UglyCoolButton=6,
 p5_CircleMenu=7, // Not implemented?
 p5_Film=8,
 p5_FilmIsotrapf=9,
 p5_FilmLineFader=10,
 p5_FilmLoop=11,
 p5_FilmLoopFader=12,
 p5_MultiFilm=13,
 p5_Glow=14,
 p5_Glow2=15,
 p5_Gloe3=16,
 p5_IconSelect=17,
 p5_Lightning=18,
 p5_LightningField=19,
 p5_LightningBolts=20,
 p5_LineGraph=21,
 p5_List=22, // Not implemented?
 p5_Box=23,
 p5_Circle=24,
 p5_FadeIn=25,
 p5_FadeOut=26,
 p5_MatrixText=27,
 p5_tText=28,
 p5_WriteText=29,
 p5_EasyModel=30,
 p5_Model=31,
 p5_ModelInspector=32,
 p5_ModelRenderTypes=33, // Not implemented?
 p5_MultiPointSprite=34,
 p5_FloatSelect=35,
 p5_IntegerSelect=36,
 p5_CoolRGBAPal4f=37, // Not implemented?
 p5_CoolRGBAPal3f=38, // Not implemented?
 p5_SimpleRGBPal3f=39, // Not implemented?
 p5_UglyRGBPal=40,
 p5_RotaryMenu=41,
 p5_SectionalPercentages=42,
 p5_Shader=43,
 p5_Shockwave=44,
 p5_CoordPlane=45,
 p5_LabeledSlider=46,
 p5_RangeSelector=47,
 p5_ReadOnlySlider=48,
 p5_UglySlider=49,
 p5_VerticalSlider=50,
 p5_FBOSwitcher=51, // Not implemented?
 p5_pBufferSwitcher=52, // Not implemented?
 p5_TexFont=53,
 p5_TexFontBox=54,
 p5_TexFontBoxString=55,
 p5_TexFontFader=56,
 p5_TexNumberToggle=57,
 p5_ToggleBox=58,
 p5_ToggleIcon=59,
 p5_UglyToggleBox=60,
 p5_Knob=61,
 p5_Presentation=62,
 p5_Scroller=63, // Not implemented?
 p5_Slide=64, // Not implemented?
 p5_Slides=65, // Not implemented?
 p5_TabView=66, // Not implemented?
 p5_VScroller=67, // Not implemented?
 p5_XFade=68,
 p5_XYPad=69,
 p5_TweenSelect=70,
 p5_UglyText=71,
 p5_UglyTextBox=72,
 p5_VBOView=73,
 p5_Aid=74,
 p5_DriftingRotatingVisualAid=75,
 p5_DriftingStretchingRotatingVisualAid=76,
 p5_DriftingStretchingVisualAid=77,
 p5_MultiVisualAid=78,
 p5_Visual=79,
 p5_VisualAid=80,
 p5_VisualAid2=81,
 p5_VisualAidAdditive=82,
 p5_VisualAids=83,
 p5_VisualRotated=84,
 p5_VisualSkewed=85,
 p5_WanderingVisualAid=86,
 p5_WobblingVisualAid=87
};



ONE(FarWorldArea,{})
 Proce55ors stored;
 Cartesian ofs,index;
 void Restore( Proce55ors *out, Cartesiand *moved );
MANY(FarWorldArea,FarWorldAreaHandle,FarWorldAreaHandles,"",FarWorldAreas,{})
DONE(FarWorldArea)

ONE(FarWorldStorage,{
})
 Cartesian cellSize,worldSize,centerCellOffset;
 ZIndexed<FarWorldArea> farMap;
 Zbool recenter; // If you turn this on, i,j is determined by center of map rather than top left corner.  This changes the way StoreOrRetrieve works.
 // In the default mode, this is used for a world that has a top left mark meaning 0,0
 // In recenter mode, we assume cellSize is larger than a screen.
 void Create( int tilesw, int tilesh, int cellsizew, int cellsizeh, bool centerOrigin=true ) {
  recenter=centerOrigin;
  cellSize.SetRect(cellsizew,cellsizeh,cellsizew,cellsizeh);
  worldSize.SetRect(0,0,tilesw*cellsizew,tilesh*cellsizeh);
  farMap.Size((size_t)tilesw,(size_t)tilesh);
  for ( int i=0; i<tilesw; i++ ) for ( int j=0; j<tilesh; j++ ) {
   farMap[i+j*farMap.w].ofs.Set((cellsizew*i+worldSize.x), (cellsizeh*j+worldSize.y));
   farMap[i+j*farMap.w].index.Set(i,j);
  }
  if ( centerOrigin ) {
   centerCellOffset.Set( (int)(cellSize.w*(farMap.w/2)),(int)(cellSize.h*(farMap.h/2)) );
   worldSize.MoveBy(-centerCellOffset.w,-centerCellOffset.h);
  }
 }
 FarWorldArea *Get( int i, int j ) {
  return &farMap[i+j*farMap.w];
 }
 void Audit( Proce55ors *inout, Cartesiand *moved );  // Unstores everything.
 bool Store( Proce55or *p, int i, int j, Cartesiand *moved );
 void StoreOrRetrieve_recentered( Proce55ors *inout, Cartesiand * moved );
 void StoreOrRetrieve( Proce55ors *inout, int iStart, int jStart, int iEnd, int jEnd, Cartesiand * moved );
 void Remove( Proce55or *p ) {
  if ( !p->farStorage && !p->storedFar ) return;
  unsigned int xy=(unsigned int)((int)p->storedI+(int)p->storedJ*(int)farMap.w);
  if ( farMap[xy].stored.ListIteminList(p) )
  farMap[xy].stored.Remove(p);
  else OUTPUT("FarWorldStorage:Remove Warning: Could not remove from farMap!\n");
 }
 void Remove( Proce55or *p, Cartesian *tileLocation ) {
  if ( !p->farStorage && !p->storedFar ) return;
  if ( tileLocation ) tileLocation->Set(p->storedI,p->storedJ);
  unsigned int xy=(unsigned int)((int)p->storedI+(int)p->storedJ*(int)farMap.w);
  if ( farMap[xy].stored.ListIteminList(p) )
  farMap[xy].stored.Remove(p);
  else OUTPUT("FarWorldStorage:Remove Warning: Could not remove from farMap!\n");
 }
MANY(FarWorldStorage,FarWorldStorageHandle,FarWorldStorageHandles,"",FarWorldStorages,{})
DONE(FarWorldStorage)