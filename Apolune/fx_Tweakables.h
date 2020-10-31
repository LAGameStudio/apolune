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
#include "Cartesian.h"
#include "Audio.h"

#include "Presentation.h"
#include "Load.h"

extern Interface input;

// Crossfades 2 images
class fx_XFade : public Proce55or {
};


class fx_XYPad : public Proce55or {
};

// Interactive knob
class fx_Knob : public Proce55or {
};

// Classic tab viewer 
class fx_TabView : public Proce55or {
public:
 Zpointer<Presentation> presentations;
 Zpointer<Presentation> displayed;
 Strings labels;
 Cartesians rects;
 Zbool centered;
 Zpointer<GLWindowManager> remove;
 Zint count, active, hovering, clicking;
 Crayon foreground,background,selected,text,highlight,hover,click;

 fx_TabView() : Proce55or() {
  foreground.Int( 255, 127, 127, 255 );
  background.Int( 127,  64,  64, 255 );
  selected.Int( 160, 127, 127, 255 );
  text.Int( 196, 196, 196, 255 );
  hover     = crayons.Pick(brown);
  highlight = crayons.Pick(alabaster);
  click     = crayons.Pick(red255);
  clicking  = -1;
 }

 void Add( Presentation *p, char *label, bool create, bool rewind ) {
  labels.Push( label );
  if ( create ) p->Init(false);
  else if ( rewind ) p->Rewind(false);
  if ( !presentations ) { p->visible=true; displayed=p; } else p->visible=false;
  p->next_presentation=presentations;
  presentations=p;  
 }

 void Activate( const char *label ) {
  String *L=labels.find( label );
  int idx=labels.IndexOf(L);
  for ( int i=0; i<labels.count; i++ ) {
   if ( i==idx ) {
    Presentation *p=presentations;
    active=i;
    for ( int j=0; j<i && p; j++ ) p=p->next_presentation;
    if ( displayed ) displayed->visible=false;
    p->visible=true;
    displayed=p;
    return;
   }
  }
 }

 void Activatei( int a ) {
  Presentation *p=presentations;
  if ( displayed ) displayed->visible=false;
  for ( int i=0; i<a && p; i++ ) p=p->next_presentation; 
  p->visible=true; 
  displayed=p;
 }

 void Setup() { 
  int k=0,j;
  rects.Clear();
  for ( String *l=(String *) (labels.first); l; l=(String *) (l->next) ) {
   Cartesian *descarte=new Cartesian;
   descarte->x=this->x+k+1;
   descarte->x2=this->x+(k+=(j=(l->integer+3)*10));
   descarte->y=this->y;
   descarte->y2=this->y+20; /*, w=j, h=20,*/
   rects.Append( descarte );
  }
  x2=x+w; y2=y+h; 
 }

 void Draw(GLWindow *surface) {
  String *l=(String *) (labels.first);
  int i=0;
  for ( Cartesian *rect=(Cartesian *) (rects.first); rect; rect=(Cartesian *) (rect->next) ) {
   Area( surface, ( active==i ? selected  : (hovering ==i ? hover : (clicking ==i ? click : background)) ), rect->x, rect->y, rect->x2, rect->y2 );
   if ( active==i ) Rectangle( surface, foreground, rect->x, rect->y, rect->x2, rect->y2 );
//   WriteText( surface, ( active==i ? highlight : text ), rect.x+20, rect.y+2, l.string, l.integer );
   l=(String *) (l->next);
   i++;
  }
 }

 void MouseMove() {
  int i=0;
  for ( Cartesian *rect=(Cartesian *) (rects.first); rect; rect=(Cartesian *) (rect->next) ) {
   if ( WITHIN( input.mxi,input.myi, rect->x, rect->y, rect->x2, rect->y2 ) ) { 
    hovering=i;
    break;
   }
   i++;
  }
 }

 void MouseLeave(Modifiers mods) { hovering=-1; }

 void LeftDown(int mx, int my, Modifiers mods) {
  int c=0;
  Presentation *p=presentations;
//  MouseMove(mx,my,mods);
  for ( ; c < count; c++ ) { 
   if ( c==hovering ) { 
    if ( displayed) displayed->visible=false;
    p->visible=true;
    displayed=p;
    active=hovering;
    clicking=hovering;
    break;
   }
   p=p->next_presentation;
  }
 }

 void LeftUp(int mx, int my, Modifiers mods) { clicking=-1; }

 void RightDown(int mx, int my, Modifiers mods) { LeftDown(mx,my,mods); }
 void RightUp  (int mx, int my, Modifiers mods) { LeftUp  (mx,my,mods); }

 void Clean() {
  if ( remove ) {
   Presentation *p,*n;
   for( p=presentations; p; p=n ) {
    n=(Presentation *) (p->next_presentation);
    remove->Remove(p);
    delete p;
   }
  }
 }

};

/*
 * Scroll slider object that moves another presentation (or can be customized)
 * p_max = dimension of the presentation in pixels
 */

class fx_Scroller : public Proce55or {
public:
 Zpointer<Presentation> p; // convenient hook
 Zint p_max, last; // the max x of the presentation horiz or vert
 Zint sx,sy,sx2,sy2;
 Zbool dragging;
 Zbool horizontal;
 Crayon tab, backing, hover, normal, click;
 Zpointer<GLImage> slider,knob;
 Zint width,height,maxheight;
 Zfloat perc;
 Zint opacity;
 Blends blend;

 fx_Scroller() : Proce55or() {
  blend=additive;
  backing=crayons.Pick(red255);
  hover=crayons.Pick(cyan);
  normal=crayons.Pick(blue);
  click=crayons.Pick(red255);
  opacity=255;
  knob=library.find( "data/images/elements/slider.png" );
  if ( horizontal ) slider=library.find( "data/images/elements/horizslider.png" );
  else              slider=library.find( "data/images/elements/vertslider.png" );
 }

 void Setup()
 {
  maxheight = horizontal ? w-width : h-height;
  sx=(int) (maxheight * perc);
  sy=0;
  sx2=sx+width;
  sy2=sy+height;
  sx2=sx+width;
  sy2=sy+height;
  tab=normal;
  x2=x+w;
  y2=y+h;
 } 

 void SetTo( float f ) {
  perc=f; if ( perc > 1.0f ) perc=1.0f; if ( perc < 0.0f ) perc=0.0f;
  sx=0;
  sy=(int) (maxheight * perc);
  sx2=sx+width;
  sy2=sy+height;
  OnChange();
 }

 void MouseOver(int mx, int my, Modifiers mods)  { if ( WITHIN(mx,my,sx,sy,sx2,sy2) ) tab=hover; }
 void MouseMove (int mx, int my, Modifiers mods) { if ( WITHIN(mx,my,sx,sy,sx2,sy2) ) tab=hover; else tab=normal; }
 void MouseLeave(Modifiers mods)                 {  dragging=false; tab=normal; }
 void LeftDown(int mx, int my, Modifiers mods)   {  
  dragging=true;  tab=click; 
  if ( horizontal ) {
   perc=(float) mx/ (float)maxheight; if ( perc > 1.0f ) perc=1.0f;
   sx=(int) (maxheight * perc);
   sy=0;
  } else {
   perc=(float) my/ (float)maxheight; if ( perc > 1.0f ) perc=1.0f;
   sx=0;
   sy=(int) (maxheight * perc);
  }
  sx2=sx+width;
  sy2=sy+height;
  OnChange();
 }
 void LeftUp(int mx, int my, Modifiers mods)     {  dragging=false; tab=normal; }
 void RightDown(int mx, int my, Modifiers mods)  {  LeftDown(mx,my,mods);  }
 void RightUp(int mx, int my, Modifiers mods)    {  dragging=false; tab=normal; }

 Crayon c,d;

 void Between() {
  c=backing;
  c.a=opacity;
  c.af=((float) opacity/255.0f);
  d=tab;
  d.a=opacity;
  d.af=((float) opacity/255.0f);
 }

 void Draw(GLWindow *surface) {
  Blend(surface,slider,blend,sx,sy,0,0,w,h,slider->width,slider->height);
  Blend(surface,knob,blend,sx,sy,0,0,width,height,knob->width,knob->height);
 }

 virtual void OnChange() {
  if ( !p ) return;
  //
  if ( horizontal ) {
   p->Move(-last,0,null,false);
   p->Move(last=(int)(perc*p_max),0,null,false);
  } else {
   p->Move(0,-last,null,false);
   p->Move(0,last=(int)(perc*p_max),null,false);
  }
 }
};

/*
 * Optimized as a vertical scroll slider control.
 * p_max = dimension of the presentation in pixels
 */
class fx_VScroller : public Proce55or {
public:
 Zpointer<Presentation> p; // convenient hook
 Zint p_max, last; // the max x of the presentation horiz or vert
 Zint sx,sy,sx2,sy2;
 Zbool dragging;
 Crayon tab, backing, hover, normal,click;
 Zpointer<GLImage> slider,knob;
 Zint width,height,maxheight;
 Zfloat perc;
 Zbyte opacity;
 Blends blend;

 fx_VScroller() : Proce55or() {}

 void Init() {
  blend=additive;
  backing=crayons.Pick(red255);
  hover=crayons.Pick(cyan);
  normal=crayons.Pick(blue);
  click=crayons.Pick(red255);
  opacity=255;
  knob=library.find( "data/images/elements/slider.png" );
//  if ( hortizontal ) slider=library.load( "data/images/elements/horizslider.png" );
  slider=library.find( "data/images/elements/vertslider.png" );
 }

 void Setup()
 {
  if ( !width && !height ) { width=w; height=w*2+1; }
  maxheight = h-height;
  sx=x;
  sy=y;
  last=0;
  sx2=x+width;
  sy2=y+height;
  tab=normal;
  x2=x+w;
  y2=y+h;
 } 

 void SetTo( float f ) {
  perc=f; if ( perc > 1.0f ) perc=1.0f; if ( perc < 0.0f ) perc=0.0f;
  sx=x;//+0;
  sy=y+(int) ((float) maxheight * perc);
  sx2=sx+width;
  sy2=sy+height;
  OnChange();
 }

 void MouseOver(int mx, int my, Modifiers mods) {
  if ( WITHIN(mx,my,x,y,x2,y2) ) tab=hover; 
 }
 void MouseLeave(Modifiers mods)                 {  dragging=false; tab=normal; }
 void MouseMove (int mx, int my, Modifiers mods) { 
  if ( WITHIN(mx,my,sx,sy,sx2,sy2) ) tab=hover; else tab=normal; 
  if ( dragging ) {
   perc=(float) (my-y)/ (float) maxheight; if ( perc > 1.0f ) perc=1.0f; if ( perc < 0.0f ) perc=0.0f;
   sy=y+(int) (maxheight * perc);
   sy2=sy+height;
   tab=click;  
   OnChange();
  }
 }
 void LeftDown(int mx, int my, Modifiers mods)   {
  if ( !WITHIN(mx,my,x,y,x2,y2) ) return;
  dragging=true;
 }
 void LeftUp(int mx, int my, Modifiers mods)     {  dragging=false; tab=normal; }
 void RightDown(int mx, int my, Modifiers mods)  {  LeftDown(mx,my,mods);  }
 void RightUp(int mx, int my, Modifiers mods)    {  dragging=false; tab=normal; }

 Crayon c,d;

 void Between() {
  c=backing;
  c.a=opacity;
  c.af=((float) opacity/255.0f);
  d=tab;
  d.a=opacity;
  d.af=(opacity/255.0f);
 }

 void Draw(GLWindow *surface) {
  Blend(surface,slider,blend,x,y,0,0,w,h,slider->width,slider->height);
  Blend(surface,knob,blend,sx,sy,0,0,width,height,knob->width,knob->height);
 }

 virtual void OnChange() {
  if ( p ) {
   p->Move(0,last,null,false);
   p->Move(0,-(last=(int)(perc*p_max)),null,false);
  }
 }
};


#if defined(NEVER)
class fx_Slide : public Proce55or {
public:
 Proce55ors fx;
 Proce55ors fullscreen_fx;
 float Duration, remaining;
 SoundSequencer sounds;

 void Clean() {
 }

 void Between() {
  sounds.Update(1.0f/FPS);
  fx.BetweenFrames();
  if ( fullscreen_fx )   fullscreen_fx.BetweenFrames();
 }

 void Reset(bool paused) {
  if ( sounds ) sounds.Reset();
  if ( aids ) aids.Reset(paused);
  if ( fx ) fx.Reset();
  if ( fullscreen_fx ) fullscreen_fx.Reset();
  Duration=remaining;
 }

 void Redraw( Surface target ) {
  if ( aids ) aids.Redraw( target ); 
  if ( fx )   fx.Redraw( target );
  if ( fullscreen_fx )   fullscreen_fx.Redraw( target );
 }

 void Draw( Surface target ) {
  if ( aids ) aids.Render( target ); 
  if ( fx )   fx.Render( target );
  if ( fullscreen_fx )   fullscreen_fx.Render( target );
 }

 void Drop( const char *name ) {
  Proce55or  p, pnext;
  VisualAid va,vanext;
  for ( p=fx.first; p; p=pnext ) { pnext=p.next; if ( p.name.inList(name) ) fx.Delete( (void *) p ); }
  for ( p=fullscreen_fx.first; p; p=pnext ) { pnext=p.next; if ( p.name.inList(name) ) fullscreen_fx.Delete( (void *) p ); }
  for ( va=aids.first; va; va=vanext ) { vanext=va.next; if ( va.name.inList(name) ) aids.Delete((void *)va); }
 }

 virtual bool OnComplete( fx_Presentation p ) { return true; }

 void      Resize(int w, int h)                    { if ( fullscreen_fx ) fullscreen_fx._Resize(w,h);              if ( fx ) fx._Resize(w,h);               }
 void   MouseMove(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._MouseMove(mx,my,mods);    if ( fx ) fx._MouseMove(mx,my,mods);     }
 void   MouseOver(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._MouseOver(mx,my,mods);    if ( fx ) fx._MouseOver(mx,my,mods);     }
 void    LeftDown(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._LeftDown(mx,my,mods);     if ( fx ) fx._LeftDown(mx,my,mods);      }
 void   RightDown(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._RightDown(mx,my,mods);    if ( fx ) fx._RightDown(mx,my,mods);     }
 void      LeftUp(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._LeftUp(mx,my,mods);       if ( fx ) fx._LeftUp(mx,my,mods);        }
 void     RightUp(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._RightUp(mx,my,mods);      if ( fx ) fx._RightUp(mx,my,mods);       }
 void  MouseLeave( Modifiers mods )                { if ( fullscreen_fx ) fullscreen_fx._MouseLeave(mods);         if ( fx ) fx._MouseLeave(mods);          }
 void    MiddleUp(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._MiddleUp(mx,my,mods);     if ( fx ) fx._MiddleUp(mx,my,mods);      }
 void  MiddleDown(int mx, int my, Modifiers mods)  { if ( fullscreen_fx ) fullscreen_fx._MiddleDown(mx,my,mods);   if ( fx ) fx._MiddleDown(mx,my,mods);    }
 void MiddleDouble(int mx, int my, Modifiers mods) { if ( fullscreen_fx ) fullscreen_fx._MiddleDouble(mx,my,mods); if ( fx ) fx._MiddleDouble(mx,my,mods);  }
 void   MouseLost()                                { if ( fullscreen_fx ) fullscreen_fx._MouseLost();              if ( fx ) fx._MouseLost();               }
 void      KeyHit( Key key, unichar ch )           { if ( fullscreen_fx ) fullscreen_fx._KeyHit(key,ch);           if ( fx ) fx._KeyHit(key,ch);            }
 void       KeyUp( Key key, unichar ch )           { if ( fullscreen_fx ) fullscreen_fx._KeyUp(key,ch);            if ( fx ) fx._KeyUp(key,ch);             }
 void     KeyDown( Key key, unichar ch )           { if ( fullscreen_fx ) fullscreen_fx._KeyDown(key,ch);          if ( fx ) fx._KeyDown(key,ch);           }  

};

class fx_Slides : LinkList<fx_Slide, link = fx_Slide::link>  {
 void Render( Surface target ) { Slide s; for ( s : this ) s.Draw(target);  }
 void Redraw( Surface target ) { Slide s; for ( s : this ) s.Redraw(target);  }
 void Reset( bool paused )     { Slide s; for ( s : this ) s.Reset(paused);   }
 void BetweenFrames()          { Slide s; for ( s : this ) s.Between(); }
};


/*
 * Presentation wrapped inside a Proce55or
 */
class fx_Presentation : public Proce55or {

 fx_Slides slides {};
 fx_Slide current;
 float Duration,remaining;
 bool paused;
 int y1,y2;
 bool active;
 float timescale;

 int lx,ly; // for storing the last mousex, mousey

 void Between() {
  if ( paused || !current ) return; 
  this.remaining-=1.0f/FPS; //printf("%f\n",remaining);
  if ( this.remaining < 0 ) {
   this.remaining=Duration; 
  }
  if ( current ) {
   current.remaining-=1.0f/FPS;
   if ( current.remaining < 0 ) {
    if ( !current.OnComplete(this) ) { 
     current.Between(); 
     return;
    }
    current=current.next; 
    if ( current ) current.Reset(paused); 
    else { this.OnComplete(); return; } 
   }
   current.Between();
  }
 }

 void Setup()
 {
  timescale=1.0f; active=false;
 }

 void Clean() {
  if ( current && current.sounds && current.sounds.events ) current.sounds.Stop();
  delete slides;
 }

// void Append( Slide s ) { Slide x; for ( x : slides ) if ( x.next == null ) break; x.next = s; }

 // return true when you want it to rewind/replay
 virtual bool OnComplete() { if ( current ) current.sounds.Stop(); current=null; return false; }

 void OnRedraw(Surface surface) {
  if ( !current ) { if ( this.OnComplete() ) current=slides.first; return; }
  PreProcessing( surface );
  if ( paused ) { current.Redraw( surface ); } else { current.Draw( surface ); }
  PostProcessing( surface );
 }
 
 void Pause() { if ( paused ) { current.sounds.Resume(); paused=false; } else { current.sounds.Pause();  paused=true; } }

 void Rewind( bool start_paused ) {
  fx_Slide s; 
  paused=start_paused;
  Duration=0;
  for ( s=slides.first; s!=null; s=s.next ) { s.Reset(paused); Duration += s.Duration; }
  current=slides.first;
  remaining=Duration;
 }

 void Seek( int n ) {
  float new_Duration=0.0f;
  fx_Slide s=slides.first;
  this.Rewind(paused); // reset everything
  while ( s != null && --n > 0 ) { new_Duration+=s.Duration; s=s.next; } // advance to slide
  current=s; 
  if ( s ) { s.Reset(paused); remaining=new_Duration; } 
 }

 // Rolls back the presentation clock.
 void Rollback( float delta ) { remaining+=delta; if ( current ) current.remaining += delta; }

 virtual void PreProcessing ( Surface surface ) { }
 virtual void PostProcessing( Surface surface ) { }

 void Resize(int w, int h)                              { if ( current ) current.Resize(w,h);            active=true;   }
 void OnKeyHit(Key key, unichar ch)                     { if ( current ) current.KeyHit(key,ch);         active=true;   }
 void OnKeyDown(Key key, unichar ch)                    { if ( current ) current.KeyDown(key,ch);        active=true;   }
 void OnKeyUp(Key key, unichar ch)                      { if ( current ) current.KeyUp(key,ch);          active=true;   }
 void OnMouseMove(int x, int y, Modifiers mods)         { if ( current ) current.MouseMove(x,y,mods);    active=true;   } /**/
 void OnMouseOver(int x, int y, Modifiers mods)         { if ( current ) current.MouseOver(x,y,mods);    active=true;   }
 void OnLeftButtonDown(int x, int y, Modifiers mods)    { if ( current ) current.LeftDown(x,y,mods);     active=true;   }
 void OnRightButtonDown(int x, int y, Modifiers mods)   { if ( current ) current.RightDown(x,y,mods);    active=true;   }
 void OnLeftButtonUp(int x, int y, Modifiers mods)      { if ( current ) current.LeftUp(x,y,mods);       active=false;  }
 void OnRightButtonUp(int x, int y, Modifiers mods)     { if ( current ) current.RightUp(x,y,mods);      active=false;  }
 void OnMiddleButtonUp(int x, int y, Modifiers mods)    { if ( current ) current.MiddleUp(x,y,mods);     active=true;   }
 void OnMiddleButtonDown(int x, int y, Modifiers mods)  { if ( current ) current.MiddleDown(x,y,mods);   active=true;   }
 void OnMiddleDoubleClick(int x, int y, Modifiers mods) { if ( current ) current.MiddleDouble(x,y,mods); active=true;   }
 void OnMouseLeave(Modifiers mods)                      { if ( current ) current.MouseLeave(mods);       active=false;  }
 void OnMouseCaptureLost()                              { if ( current ) current.MouseLost();            active=false;  }
};
#endif