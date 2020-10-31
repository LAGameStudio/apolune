
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <stdio.h>

#include "macros.h"
#include "GLWindow.h"
#include "Proce55or.h"
#include "LinkedList.h"
#include "Art.h"
#include "Slide.h"
#include "Presentation.h"

#define NO_DEBUG 1

extern Display display;

void Presentation::Constructor(void)
{
 fbo=&fboScreen3;
 this->classDescription="Presentation";
 this->InitGLWindow();
 this->slides.Recycle(new Slides);
 this->current=null;
 this->paused=false;
 this->letterbox=false;
 this->active=false;
 this->timescale=1.0;
 this->parent=null;
 this->loop=false;
 this->next_presentation=null;
 this->doNotPauseWhenNotVisible=false;
 slides->parent=this;
 audio=null;
}

void Presentation::InitDisplay(void) {
 this->px=100;
 this->py=display->h-100;
 this->px2=110;
 this->py2=display->h-150;
 this->ppx=130;
 this->ppy=display->h-100;
 this->ppx2=140;
 this->ppy2=display->h-150;
}

void Presentation::Init( bool start_paused ) {
 EACH(slides->first,Slide,s) {
  s->fx->Go();
  s->fullscreen_fx->Go();
 }
 Rewind(start_paused);
}

Presentation::Presentation(Display *d)
{
 this->display = d;
 this->Constructor();
 this->InitDisplay(); 
}

Presentation::Presentation(Display *d, Interface *i, Audio *a)
{
 this->display = d;
 this->input = i;
 this->Constructor();
 this->InitDisplay();
 this->audio=a;
}

Presentation::Presentation(int x, int y, int w, int h)
{
 this->Constructor();
 this->x=x;
 this->y=y;
 this->w=w;
 this->h=h;
}

Presentation::Presentation(Display *d, int x, int y, int w, int h)
{
 this->Constructor();
 display=d;
 this->InitDisplay();
 this->x=x;
 this->y=y;
 this->w=w;
 this->h=h;
}

//

void Presentation::Present(void)
{
 this->Constructor();
}

void Presentation::Present(Display *d)
{
 this->display = d;
 this->Constructor();
 this->InitDisplay(); 
}

void Presentation::Present(Display *d, Interface *i, Audio *a)
{
 this->display = d;
 this->input = i;
 this->Constructor();
 this->InitDisplay(); 
 this->audio=a;
}

void Presentation::Present(int x, int y, int w, int h)
{
 this->Constructor();
 this->x=x;
 this->y=y;
 this->w=w;
 this->h=h;
}

void Presentation::Present(Display *d, int x, int y, int w, int h)
{
 this->Constructor();
 display=d;
 this->InitDisplay();
 this->x=x;
 this->y=y;
 this->w=w;
 this->h=h;
}

//
Presentation::~Presentation(void)
{
 this->DeinitGLWindow();
// if ( current && current.sounds && current.sounds.events ) current.sounds.Stop();
// next_presentation.Delete();
}

void Presentation::Deinit() {
 this->DeinitGLWindow();
}

void Presentation::BetweenFrames(void)
{
 if ( paused || !current || ( !visible && !doNotPauseWhenNotVisible) ) return; // recently added "!visible"
 expired+=FRAMETIME;
 remaining-=FRAMETIME; //printf("%f\n",remaining);
 if ( remaining < 0.0 && loop ) {
  if ( freezeLast ) {
   paused=true;
   return;
  }
  remaining=duration;
  expired=0.0;
  Rewind(false);
  looped++;
  return;
 }
 if ( current ) {
  current->remaining-=FRAMETIME;
  if ( current->remaining < 0.0 ) {
   if ( !current->OnSlideComplete() ) { 
    current->BetweenFrames(); 
    return; 
   }
   current=(Slide *) (current->next);
   expired=0.0;
   if ( current ) current->Reset(paused); 
   else { OnComplete(); return; } 
  }
  current->BetweenFrames();
 }
}

void Presentation::OnRedraw(void)
{
 if ( !visible ) return;
 if ( fbo ) fbo->Bind();
 PreProcessing();
 if ( !current ) {
  if ( loop && !paused ) { 
   if ( OnComplete() ) current=(Slide *) slides->first;
   if ( fbo ) {
    fbo->Unbind();
    fbo->RenderUpsideDown((int)x,(int)y,(int)(x+w),(int)(y+h));
   }
   return;
  } else {
   if ( fbo ) {
    fbo->Unbind();
    fbo->RenderUpsideDown((int)x,(int)y,(int)(x+w),(int)(y+h));
   }
   return;
  }
 }
 if ( paused ) {
  RenderPaused();
 } else {
  current->Render( );
  if ( letterbox ) { Area(this,background,0,0,w,y1); Area(this,background,0,y2,w,h); }
 }
 PostProcessing();
#if !defined(NO_DEBUG)
 if ( debugmode == 1 || debugmode==3 || debugmode==4 || debugmode==5 || ( debugmode == 2 && paused ) ) {//
//  VisualAid v;
//  Proce55or *p;
  char b[1024];
  int oy;
  // write info text
//  surface.TextFont(font);
  sprintf_s( b, "Presentation: Duration: %1.2f Remaining: %1.2f FPS=%f", duration, remaining, FPS ); 
//  surface.foreground=white;
//  surface.WriteText(10,10,b,strlen(b));
  if ( current != null && debugmode != 3 ) {
   sprintf_s( b, "Current slide: %1.2f wait: %1.2f", current->duration, current->remaining ); 
//   surface.WriteText(10,20,b,strlen(b));
   oy=20; 
/*   if ( debugmode !=4 )
   for ( v=current.aids.first; v!=null; v=v.next ) {
    oy+=13;
    if ( !v.image || !v.image.bitmap ) 
    sprintf( b, "No image on visual aid!" );
    else
    sprintf( b, "va: %2.2f:%2.2f : wait=%2.2f / x%2.2f at %d,%d moving from %d,%d by %d,%d, scaled from %dx%d, to %dx%d, from %dx%d -> %dx%d %s at opacity %d (@%1.2f,%d->%d,@%1.2f->%d) fn:%s", 
                v.duration, v.delay, v.ofs, v.expired, v.x, v.y, v.sx, v.sy, v.dx, v.dy, v.image.bitmap.width, v.image.bitmap.height, v.width, v.height, v.w, v.h, v.w2, v.h2, (v.visible?"vis":"invis"), v.opacity, v.in, v.start, v.mid, v.out, v.end, v.image.fileName ); 
    surface.background= ColorAlpha { v.opacity, { 255,255,255} };  surface.Area(20,5+oy,30,oy+8); surface.foreground=orange;   surface.Rectangle(20,5+oy,30,oy+8); 
    surface.background= v.actual.asColorAlpha();  surface.Area(40,5+oy,50,oy+8); surface.foreground=orange;   surface.Rectangle(40,5+oy,50,oy+8); 
    surface.WriteText(65,oy,b,strlen(b));
   } 
   if ( debugmode !=5 )
   for ( p=(Proce55or *) current->fx->first; p!=null; p=(Proce55or *)p->next ) {
    fx_Glow u;
    oy+=13;
    sprintf( b, "fx: %2.2f:%2.2f : ofs=%2.2f / x%2.2f %s | %dx%d x,y=%d,%d {%s} Ii[%2.2f,%d] d[%d,%2.2ff] %s%s", 
                p.duration, p.delay, p.ofs, p.expired, p.name ? p.name : "none", p.w,p.h,p.x,p.y, 
                p.debugString,
                p.controller.instant, p.controller.increment, p.controller.delta, p.controller.deltaf, 
                p.controller.state ? "sT" : "sF", p.controller.pingpong ? " pong" : "" ); 
    u=(fx_Glow) p;
    surface.background= u.c; surface.Area(40,5+oy,50,oy+8); surface.foreground=orange;   surface.Rectangle(40,5+oy,50,oy+8); 
    surface.WriteText(65,oy,b,strlen(b));
   }
*/
  }
 }//
#endif
 if ( fbo ) {
  fbo->Unbind();
  fbo->RenderUpsideDown((int)x,(int)y,(int)(x+w),(int)(y+h));
 }
}

void Presentation::Pause(void)
{
 if ( paused ) { 
  if ( current ) current->sounds.Resume(); 
  paused=false; 
 } 
 else { 
  if ( current ) current->sounds.Pause();
  paused=true;
  paused_fader=0;
 } 
}

void Presentation::LetterBox(void)
{
 if ( letterbox ) { letterbox=false; return; } 
 letterbox=true; 
 y1=display->h/7; 
 y2=display->h-y1;
}

void Presentation::Rewind( bool start_paused )
{
 paused=start_paused;
 paused_fader=0;
 duration=0.0;
 EACH(slides->first,Slide,s) {
  s->Reset(paused); 
  duration += s->duration; 
 }
 current = (Slide *) (slides->first);
 remaining=duration;
 expired=0.0;
 BetweenFrames();
 Between();
}

void Presentation::Seek( int n )
{
 double new_duration=0.0;
 Slide *s=(Slide *) (slides->first);
 Rewind(paused); // reset everything
 while ( s != null && --n > 0 ) { new_duration+=s->duration; s=(Slide *) (s->next); } // advance to slide
 current=s; 
 if ( s ) { s->Reset(paused); remaining=new_duration; expired=0.0; } 
}

void Presentation::Rollback( float delta )
{
 remaining+=-delta; 
 if ( current ) current->remaining += delta;
}

void Presentation::Move( int movebyx, int movebyy, Slide *slideornullforall, bool updateSounds )
{
 MovedX+=movebyx; MovedY+=movebyy;
 if ( slideornullforall ) {
  EACH(slideornullforall->fx->first,Proce55or,p) p->MoveBy(movebyx,movebyy);
 } else {
  EACH(slides->first,Slide,s) EACH(s->fx->first,Proce55or,p) p->MoveBy(movebyx,movebyy);
 }
 if ( updateSounds ) {  }
}

bool Presentation::OnComplete(void)
{
 if ( current ) current->sounds.Stop(); 
 current=null; 
 return false;
}