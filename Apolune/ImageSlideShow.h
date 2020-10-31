/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2015 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "Art.h"
#include "GLImage.h"
#include "Proce55or.h"
#include "Strings.h"
#include "Delay.h"
#include "TextureLibrary.h"
#include "FastGUI.h"
#include "Tweens.h"

class ImageSlide : public ListItem {
public:
 Zp<GLImage> image,image2,image3;
 Zdouble duration,fadeIn;
 Crayon tint;
 Zstring sound;
 Delay soundDelay;
 Zbool triggeredSound;
 Zbool parallaxZoomIn,parallaxZoomOut,kenBurnsIn,kenBurnsOut,kenBurnsSlideRight,kenBurnsSlideLeft,kenBurnsSlideUp,kenBurnsSlideDown;
 ImageSlide() : ListItem() {
  duration=10.0;
  fadeIn=0.5;
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i ) {
  image=i;
  duration=10.0;
  fadeIn=0.5;
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i, double duration ) {
  image=i;
  this->duration=duration;
  fadeIn=0.5;
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i, float duration ) {
  image=i;
  this->duration=(double)duration;
  fadeIn=0.5;
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i, Crayon c ) {
  image=i;
  duration=10.0;
  fadeIn=0.5;
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i, Crayon c, double duration ) {
  image=i;
  this->duration=duration;
  fadeIn=0.5;
  this->tint.fromCrayon(c);
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 ImageSlide( GLImage *i, Crayon c, float duration ) {
  image=i;
  this->duration=(double)duration;
  fadeIn=0.5;
  this->tint.fromCrayon(c);
  tint.Pick(alabaster);
  triggeredSound=false;
 }
 void Render( float alpha, Blends b, float expiredRatio, int x, int y, int w, int h ) {
  if ( parallaxZoomIn ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   double zoomRatio=1.0;
   Cartesiand rect;
   if ( this->image ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w,(double)fboScreen2.h);
    Stretchd(this->image,tint,alpha,transparency,&rect);
   }
   zoomRatio=((expiredRatio)*0.5)+1.0;
   if ( this->image2 ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w*zoomRatio,(double)fboScreen2.h*zoomRatio);
//    rect.MoveBy(-((rect.w-(double)fboScreen2.w)/2.0),-((rect.h-(double)fboScreen2.h)/2.0));
    Stretchd(this->image2,tint,alpha,transparency,&rect);
   }
   zoomRatio*=1.75;
   if ( this->image3 ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w*zoomRatio,(double)fboScreen2.h*zoomRatio);
    rect.MoveBy(-((rect.w-(double)fboScreen2.w)/2.0),-((rect.h-(double)fboScreen2.h)/2.0));
    Stretchd(this->image3,tint,alpha,transparency,&rect);
   }
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( parallaxZoomOut ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   double zoomRatio=1.0;
   Cartesiand rect;
   if ( this->image ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w,(double)fboScreen2.h);
    Stretchd(this->image,tint,alpha,transparency,&rect);
   }
    zoomRatio=((1.0-expiredRatio)*0.5)+1.0;
   if ( this->image2 ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w*zoomRatio,(double)fboScreen2.h*zoomRatio);
//    rect.MoveBy(-((rect.w-(double)fboScreen2.w)/2.0),-((rect.h-(double)fboScreen2.h)/2.0));
    Stretchd(this->image2,tint,alpha,transparency,&rect);
   }
   zoomRatio*=1.75;
   if ( this->image3 ) {
    rect.SetRect(0.0,0.0,(double)fboScreen2.w*zoomRatio,(double)fboScreen2.h*zoomRatio);
    rect.MoveBy(-((rect.w-(double)fboScreen2.w)/2.0),-((rect.h-(double)fboScreen2.h)/2.0));
    Stretchd(this->image3,tint,alpha,transparency,&rect);
   }
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsIn ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   double zoomRatio=(double)expiredRatio+1.0;
   Cartesiand rect;
   rect.SetRect((double)x*zoomRatio,(double)y*zoomRatio,(double)w*zoomRatio,(double)h*zoomRatio);
   rect.MoveBy(-((rect.w-(double)w)/2.0),-((rect.h-(double)h)/2.0));
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsOut ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   double zoomRatio=(1.0-(double)expiredRatio)+1.0;
   Cartesiand rect;
   rect.SetRect((double)x*zoomRatio,(double)y*zoomRatio,(double)w*zoomRatio,(double)h*zoomRatio);
   rect.MoveBy(-((rect.w-(double)w)/2.0),-((rect.h-(double)h)/2.0));
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsSlideRight ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   Cartesiand rect;
   double r=(double)expiredRatio;
   double remainder=(double)(image->width-w);
   double offset=-r*remainder;
   rect.SetRect((double)x+offset,(double)y,(double)image->width,(double)image->height);
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsSlideLeft ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   Cartesiand rect;
   double r=(1.0-(double)expiredRatio);
   double remainder=(double)(image->width-w);
   double offset=-r*remainder;
   rect.SetRect((double)x+offset,(double)y,(double)image->width,(double)image->height);
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsSlideUp ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   Cartesiand rect;
   double r=(1.0-(double)expiredRatio);
   double remainder=(double)(image->height-h);
   double offset=-r*remainder;
   rect.SetRect((double)x,(double)y+offset,(double)image->width,(double)image->height);
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else if ( kenBurnsSlideDown ) {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   Cartesiand rect;
   double r=expiredRatio;
   double remainder=(double)(image->height-h);
   double offset=-r*remainder;
   rect.SetRect((double)x,(double)y+offset,(double)image->width,(double)image->height);
   if ( this->image ) Stretchd(this->image,tint,alpha,transparency,&rect);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  } else {
   fboScreen2.background.Pick(clear);
   fboScreen2.Bind();
   if ( this->image ) Stretchi(this->image,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image2 ) Stretchi(this->image2,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   if ( this->image3 ) Stretchi(this->image3,tint,alpha,transparency,0,0,fboScreen2.w,fboScreen2.h);
   fboScreen2.Unbind();
   fboScreen2.blend=b;
   fboScreen2.tint=crayons.Pick(alabaster);
   fboScreen2.RenderUpsideDown(0,0,w,h);
  }
 }
};

class ImageSlides : public LinkedList {
public:
 CLEARLISTRESET(ImageSlide);
 ImageSlide *Add( const char *s ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( const char *s, float duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  i->duration=(double)duration;
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( const char *s, double duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  i->duration=duration;
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( const char *s, Crayon c ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( const char *s, Crayon c, float duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  i->duration=(double)duration;
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( const char *s, Crayon c, double duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=library.Load(s);
  i->duration=duration;
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s, float duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  i->duration=(double)duration;
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s, double duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  i->duration=duration;
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s, Crayon c ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s, Crayon c, float duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  i->duration=(double)duration;
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 ImageSlide *Add( GLImage *s, Crayon c, double duration ) {
  ImageSlide *i=new ImageSlide;
  i->image=s;
  i->duration=duration;
  i->tint.fromCrayon(c);
  if ( !i->image ) { delete i; return null; }
  Append(i);
  return i;
 }
 double TotalDuration( double fadeLength ) {
  double total=0.0;
  FOREACH(ImageSlide,i) total+=i->duration;
  return total+fadeLength*(double)count;
 }
};

class ImageSlideShow : public Proce55or {
public:
 Zp<Tween> pulse;
 ImageSlides slides;
 Zdouble fadeLength,totalLength;
 Zp<ImageSlide> current,previous;
 Zdouble expired;
 Zbool crossfading, paused;
 Zbool userAdvance;
 Delay nextFade,fading,inTo,outOf;
 Crayon background,tint,color,activated,deactivated,highlight;
 Zp<GLImage> circle;
 Zstring sound;
 Delay soundDelay;
 Zbool triggeredSound;
 ImageSlideShow() : Proce55or() {
  nextFade.Set(10.0);
  background.Pick(clear);
  tint.fromCrayon(fast.tint);
  color.fromCrayon(fast.color);
  activated.fromCrayon(fast.activated);
  deactivated.fromCrayon(fast.deactivated);
  highlight.fromCrayon(fast.highlight);
  fadeLength=2.0;
  circle=library.Load("data/images/elements/circle-white.png");
  pulse=tweens.find("Humanized In Out Slow");
 }
 ImageSlide *Add( GLImage *i ) { return slides.Add(i); }
 ImageSlide *Add( GLImage *i, float duration ) { return slides.Add(i,duration); }
 ImageSlide *Add( GLImage *i, double duration ) { return slides.Add(i,duration); }
 ImageSlide *Add( GLImage *i, Crayon c ) { return slides.Add(i,c); }
 ImageSlide *Add( GLImage *i, Crayon c, float duration ) { return slides.Add(i,c,duration); }
 ImageSlide *Add( GLImage *i, Crayon c, double duration ) { return slides.Add(i,c,duration); }
 ImageSlide *Add( const char *i ) { return slides.Add(i); }
 ImageSlide *Add( const char *i, float duration ) { return slides.Add(i,duration); }
 ImageSlide *Add( const char *i, double duration ) { return slides.Add(i,duration); }
 ImageSlide *Add( const char *i, Crayon c ) { return slides.Add(i,c); }
 ImageSlide *Add( const char *i, Crayon c, float duration ) { return slides.Add(i,c,duration); }
 ImageSlide *Add( const char *i, Crayon c, double duration ) { return slides.Add(i,c,duration); }
 void Load( const char *s ) {
  if ( !file_exists(s) ) return;
  Zstring file=file_as_string(s);
  const char *w=file.Next();
  Strings out;
  while ( *w != '\0' ) {
   if ( !file_exists(w) ) {
    OUTPUT("ImageSlideShow::Load(`%s`) skipped unreadable file: %s\n", s, w );
    w=file.Next(); // duration...
    continue;
   }
   string s=string(w);
   w=file.Next();
   float d=(float)atof(w);
   if ( d > 0.0f && s.length() > 0 ) {
    String *f=new String;
    f->s=s;
    f->floating=d;
    out.Append(f);
   }
   w=file.Next();
  }
  AddVariableDurations(&out);
  Rewind();
 }
 void AddVariableDurations( Strings *in ) {
  EACH(in->first,String,s) slides.Add(s->s.c_str(),s->floating);
  totalLength=slides.TotalDuration(fadeLength);
 }
 void Add( Strings *in ) {
  EACH(in->first,String,s) slides.Add(s->s.c_str());
  totalLength=slides.TotalDuration(fadeLength);
 }
 void Rewind() {
  SetSlide((ImageSlide *)slides.first);
  previous=(ImageSlide *)slides.last;
  crossfading=false;
  nextFade.Set((float)(fadeLength+(current?current->duration:0.0f)));
  inTo.Set((float)(fadeLength+current->duration+fadeLength));
  inTo.Reset();
  outOf.Set((float)fadeLength);
  outOf.Reset();
  nextFade.Reset();
  soundDelay.Reset();
  triggeredSound=false;
 }
 ImageSlide *GetNextSlide( ImageSlide *c, bool *looped=null ) {
  if ( c ) {
   if ( c->next ) return (ImageSlide *) c->next;
   else {
    if ( looped ) (*looped)=true;
    return (ImageSlide *) slides.first;
   }
  } else return (ImageSlide *) slides.first;
 }
 void SetSlide( ImageSlide *slide ) {
  previous=current;
  current=slide;
  crossfading=false;
  triggeredSound=false;
  nextFade.Set((float)(fadeLength+(current?current->duration:0.0f)));
  nextFade.Reset();
  if ( !slide ) return;
  ImageSlide *nextSlide=GetNextSlide(slide);
  fadeLength=slide->fadeIn+(nextSlide?nextSlide->fadeIn:slide->fadeIn);
  slide->soundDelay.Reset();
  slide->triggeredSound=false;
 }
 bool Next() {
  if ( !current ) { Rewind(); return false; }
  SetSlide( (ImageSlide *)current->next ? (ImageSlide *)current->next : (ImageSlide *)slides.first );
  return ( current == slides.first );
 }
 void Previous() {
  if ( !current ) { Rewind(); return; }
  SetSlide( (ImageSlide *)current->prev ? (ImageSlide *)current->prev : (ImageSlide *)slides.last );
 }
 void Setup() {
  Rewind();
 }
 Zbool looped;
 Zfloat previousFadeLength,nextFadeLength;
 void Between() {
  looped=false;
  if ( !hidden ) {
   if ( slides.count < 1 ) return;
   if ( input.leftReleased() ) paused=false;
   if ( paused ) return;
   expired+=FRAMETIME;
   nextFade.Between();
   if (outOf) outOf.Between();
   inTo.Between();
   if ( !nextFade ) { // Begin Fade Out
    if ( current ) {
     ImageSlide *nextSlide=GetNextSlide(current,&looped.value);
     ImageSlide *nextNextSlide=GetNextSlide(current);
     if ( !nextSlide || !nextNextSlide ) return;
     outOf.Set((float)nextSlide->fadeIn);
     outOf.instant=inTo.instant;
     inTo.Set((float)(nextSlide->fadeIn+nextSlide->duration+nextNextSlide->fadeIn));
     inTo.Reset();
     if ( current->next ) SetSlide((ImageSlide *) current->next);
     else {
      looped=true;
      SetSlide((ImageSlide *) slides.first);
     }
    }
    if ( !current ) Rewind(); 
    fading.Set((float)(current?current->fadeIn:fadeLength));
    fading.Reset();
    crossfading=true;
   } else {
    if ( soundDelay ) soundDelay.Between();
    if ( sound.length > 0 && !soundDelay ) {
     if ( !triggeredSound ) {
      triggeredSound=true;
      audio.manager.CueAndPlayIfNotPlaying(sound.c_str(),1.0f,1.0f,false,true);
     }
    }
    if ( current && current->sound.length > 0 ) {
     current->soundDelay.Between();
     if ( !current->soundDelay && !current->triggeredSound ) {
      current->triggeredSound=true;
      audio.manager.CueAndPlayIfNotPlaying(current->sound.c_str(),1.0f,1.0f,false,true);
     }
    }
   }
   if ( crossfading ) {
    fading.Between();
    if ( fading ) {
    } else {
     crossfading=false;
     if ( userAdvance ) paused=true;
    }
   }
  }
 }
 void RenderSlide() {
  if ( current ) {
   current->Render(1.0f,overlay,inTo.Ratio(),x,y,w,h);
  }
 }
 void RenderCrossfade() {
  float mixAB=fading.Ratio();//    float mixBA=1.0f-mixAB;
  if ( previous ) previous->Render(mixAB,overlay,outOf.Ratio(),x,y,w,h);
  if ( current ) current->Render(1.0f-mixAB,additive,inTo.Ratio(),x,y,w,h);
 }
 void Render() {
  if ( !hidden ) {
   if ( slides.count < 1 ) return;
   if ( slides.count == 1 ) {
    ImageSlide *i=((ImageSlide *)slides.first);
    Stretchi(i->image,i->tint,1.0f,none,(int)x,(int)y,(int)w,(int)h);
    return;
   }
//   Blending(none);
//   Area(background,(int)x,(int)y,(int)w,(int)h);
   if ( crossfading && current ) {
    float mixAB=fading.Ratio();//    float mixBA=1.0f-mixAB;
    Crayon mixedA;
    mixedA.ColorInterp(current->tint,crayons.Pick(clear),mixAB);
    Crayon mixedB;
    mixedB.ColorInterp(crayons.Pick(clear),current->tint,mixAB);
    RenderCrossfade();
   } else RenderSlide();
   if ( userAdvance && paused ) { // Draw play icon
    DrawNext();
   }
  }
 }
 Zdouble delta;
 void DrawNext() {
  Crayon color,pulsing;
  color.fromCrayon(tint);
  // Circle
  pulsing.fromCrayon(highlight);
  delta+=FRAMETIME;
//    if ( delta > 1.0f ) delta=0.0f;
  pulsing.Alpha(pulse->tweend(delta.value));
  if ( fast.button(fast.rightArrow,circle, display.w-128, display.h-128, 64,64,
   color,activated,deactivated, false,transparency) ) {
   paused=false;
  } 
 }
};