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

#include "GLSetup.h"
#include "GLImage.h"
#include "Crayon.h"
#include "Art.h"
#include "TextureLibrary.h"
#include "Strings.h"
#include "Numbers.h"
#include "CompositeImage.h" // included for enum
#include "Atlas.h"
#include "Art2d.h"

#include <stdio.h>
#include <stdarg.h>

extern GLSetup gl;

class FilmLibrary;
class Film : public ListItem {
public:
 Zdis<Atlas> atlas;
 Zstring title;
 Images frames;
 Zfloat duration;
 Zfloat expired;
 Zbool loop,wrap;
 float quadratic[3];
 Zfloat threshold,minSize,maxSize;
 Crayon tint;
 Blends blend;
 Zint x, y, w, h,lastframe;

 void Constructor() {
  title=string("");
  duration=1.0f;
  expired=0.0f;
  quadratic[0]=1.0f;
  quadratic[1]=0.0f;
  quadratic[2]=0.01f;
  loop=true;
  threshold=60.0f;
  minSize=1.0f;
  maxSize=gl.glMaxSize > 100.0f ? 100.0f : gl.glMaxSize;
  x=0;y=0;w=0;h=0;lastframe=0;
  blend=none;
 }

 Film() {
  Constructor();
 }

 Film( GLImage *a, ... ) {
  ImageHandle *ih=null;
  Constructor();
  if (a) {
   ih=new ImageHandle;
   ih->i=a;
   frames.Append(ih);
  }
  va_list args;  
  va_start ( args, a );
  while ( a != NULL ) {
   a=va_arg( args, GLImage * ); 
   if (a) {
    ih=new ImageHandle;
    ih->i=a;
    frames.Append(ih);
   }
  }
  va_end (args);
 }

 Film( const char *a, ... ) {
  ImageHandle *ih=null;
  GLImage *i=null;
  Constructor();
  if ( i=library.find(a) ) {
   ih=new ImageHandle;
   ih->i=i;
   frames.Append(ih);
   va_list args;  
   va_start ( args, a );
   while ( a != NULL ) {
    a=va_arg( args, char * ); 
    if (i=library.find(a)) {
     ih=new ImageHandle;
     ih->i=i;
     frames.Append(ih);
    }
   }
   va_end (args);
  }
 }

 ~Film(void) {}

 FilmLibrary *GetLibrary();

 void Rewind() {
  expired=0.0f;
 }

 // Update time
 void Between( float dt ) {
  expired+=dt;
  if ( loop && expired>duration ) {
  }
 }

 // Render at x,y crossfaded nearest neighbors to simulate motion
 Images *xfade( GLWindow *surface ) {
  Crayon t;
  float c=duration/expired;
  int ci=(int) c;
  ImageHandle *ih=(ImageHandle *) frames.Element(ci%frames.count);
  ImageHandle *n=(ImageHandle *)(ih->next);
  if ( !n ) { if ( loop ) n=(ImageHandle *) frames.first; }
  c=c-(float)ci;
  t.fromCrayon(tint);
  t.ScaleByAlphaf( 1.0f-c );
  StretchiNoAlpha( surface, ih->i, t, blend, x, y, w, h ); 
  if ( loop && n ) {
   t.fromCrayon(tint);
   t.ScaleByAlpha( c );
   StretchiNoAlpha( surface, n->i, t, blend, x, y, w, h ); 
  }
 }

 // This is how will our point sprite's size will be modified by 
 // distance from the viewer

 void setFrameAsTexture() {
  GLImage *f=frame();
  glBindTexture( GL_TEXTURE_2D, f->texture );
 }

 // Draws a cross-faded point sprite centered at x,y,z in the current view
 void xfadePointSprite( float x, float y, float z ) {
 }
 
 // Calculate the duration based on the number of desired FPS
 void findfps( int frames_per_second ) {
  duration=(float) frames.count / (float) frames_per_second;
 }

 // Advance by FPS
 void advance() {
   expired+=( 1.0f/FPS );
   if ( expired > duration ) expired-=duration;
 }

 // Return the GLImage of the current frame
 GLImage *frame() {
  float current=expired/duration;
  lastframe=(int) (current* (float) (frames.count-1));
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }
 
 GLImage *frame( int f ) {
  lastframe=f;
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }

 GLImage *frame( float f ) {
  lastframe=(int) (f* (float) (frames.count-1));
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }

 GLImage *frame( double f ) {
  lastframe=(int) (f* (double) (frames.count-1));
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }
 
 GLImage *frame( double f, double d ) {
  lastframe=(int) ( (fmod(f,d)/d) * (double) frames.count);
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }
 
 GLImage *frame( double f, double d, bool reverse ) {
  if ( reverse ) lastframe=(int) ( (1.0-fmod(f,d)/d) * (double) (frames.count-1));
  else            lastframe=(int) ( (fmod(f,d)/d) * (double) (frames.count-1));
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }
  
 GLImage *frame( having noloop, double f, double d, bool reverse ) {
  if ( reverse ) lastframe=(int) ( (1.0-fmod(f,d)/d) * (double) (frames.count-1));
  else           lastframe=(int) ( (fmod(f,d)/d) * (double) (frames.count-1));
  if ( reverse && lastframe < 1 ) lastframe=0;
  else if ( !reverse && lastframe >= frames.count ) lastframe=frames.count-1;
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }

 GLImage *frame( double f, double d, bool reverse, bool *ping_or_pong ) {
  if ( *ping_or_pong ) lastframe=(int) ( (1.0-fmod(f,d)/d) * (double) (frames.count-1));
  else            lastframe=(int) ( (fmod(f,d)/d) * (double) (frames.count-1));
  if ( lastframe == 0 || lastframe >= frames.count-1 ) (*ping_or_pong) = !(*ping_or_pong);
  ImageHandle *h=(ImageHandle *) frames.Element(lastframe%frames.count);
  return h->i;
 }
 
 GLImage *GetFirstFrame() {
  if ( frames.count == 0 ) return null;
  ImageHandle *h=(ImageHandle *) frames.Element(0);
  return h->i;
 }
 
 GLImage *GetLastFrame() {
  if ( frames.count == 0 ) return null;
  ImageHandle *h=(ImageHandle *) frames.Element(frames.count-1);
  return h->i;
 }

 void PointSprite( float X, float Y, float Z ) {
 
  setFrameAsTexture();
 
  glEnable(GL_BLEND);
    switch ( blend ) {
     case _solarize: glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO); break;
         case _mask: glBlendFunc(GL_DST_COLOR,GL_ZERO); break;
     case _screened: glBlendFunc(GL_ONE,GL_ONE); break;
     case _additive: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
     case _multiply: glBlendFunc(GL_SRC_COLOR, GL_ONE); break;
      case _overlay: glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
 case _transparency: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
     case _saturate: glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); break;
        case _mask1: glBlendFunc(GL_DST_COLOR,GL_ONE); break;
      default: break;
    }
 
  glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
  glPointSize( maxSize );
 
     // The alpha of a point is calculated to allow the fading of points 
     // instead of shrinking them past a defined threshold size. The threshold 
     // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
     // the minimum and maximum point sizes.
  glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, threshold );
  glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, minSize );
  glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
  
     // Specify point sprite texture coordinate replacement mode for each 
     // texture unit
  glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
 
     //
    	// Render point sprite...
  glEnable( GL_POINT_SPRITE_ARB );
 	glBegin( GL_POINTS );
  glColor4f( tint.rf, tint.gf, tint.bf, tint.af );
  glVertex3f(X,Y,Z);
 	glEnd();
 	glDisable( GL_POINT_SPRITE_ARB );
 }

 void Image( GLImage *i ) {
  ImageHandle *h=new ImageHandle;
  h->i=i;
  frames.Append(h);
 }

 // Appends an image list to the film
 void fromImages( Images *i ) {
  ImageHandle *h;
  for ( h=(ImageHandle *)i; h; h=(ImageHandle *) h->next ) {
   ImageHandle *n=new ImageHandle;
   n->i=h->i;
   frames.Append(n);
  }
 }
 // Loads/appends a list of files in a string delineated by space or newline 
 // (use quotes for paths with spaces) ie "this that/this.png" those.png
 void fromString( const char *s ) {
  char buf[1024];
  const char *p= s;
  while ( p && *p != '\0' ) {
   p=one_argcase(p,buf);
   ImageHandle *h=new ImageHandle;
   h->i=library.find(buf);
   frames.Append(h);
  }
 }
 // Loads/appends a list of files in the form prefix%d.ext until a number is skipped, min starting frame 0, max 1000
 void fromPrefix( const char *prefix, const char *ext ) {
  FILE *f;
  char buf[1024];
  int j=0;
  FORMAT( buf,1024, "%s%d.%s", prefix, (int) j, ext );
  fopen_s(&f,buf,"r");
  while ( !f && j<1000) {
   j++;
   FORMAT( buf,1024, "%s%d.%s", prefix, (int) j, ext );
  }
  while (f) {
   ImageHandle *h=new ImageHandle;
   fclose(f);
   h->i=library.find(buf);
   frames.Append(h);
   if ( ++j==1000 ) break;
   FORMAT( buf,1024, "%s%d.%s", prefix, (int) j, ext );
   fopen_s(&f,buf,"r");
  }
 }
};

class Films : public LinkedList {
public:
 Film *find(string p) {
  if ( p.length() == 0 ) return null;
  FOREACH(Film,f) if ( !str_cmp(f->title.c_str(), p.c_str()) ) return f;
  FOREACH(Film,f) if ( !str_prefix(f->title.c_str(), p.c_str()) ) return f;
  OUTPUT("Films: Could not find the film `%s`\n", p.c_str());
  return null;
 }
 Strings *asStringList() {
  Strings *L=new Strings;
  FOREACH(Film,f) L->Add(f->title.c_str());
  return L;
 }
 CLEARLISTRESET(Film);
 ~Films() { Clear(); }
};

class CompositeFilm : public ListItem {
public:
 Zp<Film> film;
 Zp<Film> mask;
 Cartesian position;
 Zp<Cartesian> offset;
 Zp<GLWindow> target;
 Blends blend;
 Crayon tint,masking;
 PROGRAMMABLE(CompositeFilm,RenderPtr,render,Render);
 CompositeFilm() : ListItem() {
  render=&CompositeFilm::Default;
  film=null;
  mask=null;
  target=null;
  masking.Pick(alabaster);
  offset=null;
  blend=none;
 }
 CompositeFilm( Film *I ) : ListItem() {
  render=&CompositeFilm::Overlay;
  film=I;
  mask=null;
  target=null;
  masking.Pick(alabaster);
  blend=additive;
  offset=null;
  blend=none;
 }
 CompositeFilm( Film *o, Crayon t ) : ListItem() {
  render=&CompositeFilm::Overlay;
  t.fromCrayon(t);
  film=o;
  offset=null;
  blend=none;
  target=null;
 }
 CompositeFilm( Film *I, Film *M ) : ListItem() {
  render=&CompositeFilm::Masked;
  film=I;
  mask=M;
  target=null;
  masking.Pick(alabaster);
  blend=additive;
  offset=null;
 }
 CompositeFilm( Film *o, Film *M, Crayon t, Crayon m ) : ListItem() {
  render=&CompositeFilm::Masked;
  film=o;
  mask=M;
  masking.fromCrayon(m);
  tint.fromCrayon(t);
  offset=null;
  blend=additive;
  target=null;
 }
 void BinaryRead( BinaryFile *open )  {
 }
 void BinaryWrite( BinaryFile *open ) {
 }
 void RenderTo( GLWindow *T ) { target=T; }
 void Rewind() { film->Rewind(); mask->Rewind(); }
 void Between() { film->advance(); }
 void Overlay( Film *o ) { render=&CompositeFilm::Overlay; film=o; }
 void Overlay( Film *o, Crayon t ) { render=&CompositeFilm::Overlay; t.fromCrayon(t); film=o; }
 void Mask( Film *M ) { render=&CompositeFilm::Overlay; mask=M; masking.Pick(alabaster); }
 void OverlayMasked( Film *o, Film *M ) { render=&CompositeFilm::Masked; film=o; mask=M; masking.Pick(alabaster); }
 void OverlayMasked( Film *o, Film *M, Crayon t, Crayon m ) { render=&CompositeFilm::Masked; film=o; mask=M; masking.fromCrayon(m); tint.fromCrayon(t);  }
 void Masked() {
  Blending(mask1);
  StretchiNoAlpha(target,mask->frame(),masking,blend,position.x,position.y,position.w,position.h);
  Blending(blend);
  StretchiNoAlpha(target,film->frame(),tint,blend,position.x,position.y,position.w,position.h);
 }
 void Overlay() {
  Blending(blend);
  StretchiNoAlpha(target,film->frame(),tint,blend,position.x,position.y,position.w,position.h);
 }
 void RenderOfs( GLWindow *surface, int x, int y, int w, int h ) {
  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask->frame(),masking,blend,x+position.x,y+position.y,w,h);
   Blending(blend);
   StretchiNoAlpha(surface,film->frame(),tint,blend,x+position.x,y+position.y,w,h);
  } else {
   Blending(blend);
   StretchiNoAlpha(surface,film->frame(),tint,blend,x+position.x,y+position.y,w,h);
  }
 }
 void RenderOn( GLWindow *surface, int x, int y, int w, int h ) {
  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask->frame(),masking,blend,x,y,w,h);
   Blending(blend);
   StretchiNoAlpha(surface,film->frame(),tint,blend,x,y,w,h);
  } else {
   Blending(blend);
   StretchiNoAlpha(surface,film->frame(),tint,blend,x,y,w,h);
  }
 }
 void RenderOn( GLWindow *surface, double x, double y, int w, int h ) {
  if ( mask ) {
   Blending(mask1);
   StretchdNoAlpha(surface,mask->frame(),masking,blend,x,y,w,h);
   Blending(blend);
   StretchdNoAlpha(surface,film->frame(),tint,blend,x,y,w,h);
  } else {
   Blending(blend);
   StretchdNoAlpha(surface,film->frame(),tint,blend,x,y,w,h);
  }
 }
 void Default() {
 }
 void Duplicate( CompositeFilm *d ) {
  film=d->film;
  mask=d->mask;
  position.Duplicate(&d->position);
  offset=d->offset;
  target=d->target;
  blend=d->blend;
  tint.fromCrayon(d->tint);
  masking.fromCrayon(d->masking);
  render=d->render;
 }
};

class CompositeFilms : public LinkedList {
public:
 void Rewind() { FOREACH(CompositeFilm,c) { c->film->Rewind(); c->mask->Rewind(); } }
 void MoveBy( int x, int y ) { FOREACH(CompositeFilm,c) { c->position.x+=x; c->position.y+=y; } }
 void RenderTo( GLWindow *surface ) { FOREACH(CompositeFilm,c) c->RenderTo(surface); }
 void Render() { FOREACH(CompositeFilm,c) c->Render(); }
 void Between() { FOREACH(CompositeFilm,c) c->Between(); }
 CLEARLISTRESET(CompositeFilm);
 ~CompositeFilms() { Clear(); }
};

class FilmStrip : public ListItem {
public:
 string title;
 Film *film;
 FilmStrip() : ListItem() { film=null; }
};

// Could make this class do less checks for null
class FilmStrips : public LinkedList {
public:
 FilmStrip *current;
 string name;
 PROGRAMMABLE(FilmStrips,PlayPtr,play,Play);
 PlayPtr oldMode;
 Integers order;
 Integer *cued;
 float limit,expired;
 Cartesian position;
 Crayon tint;
 Blends blend;
 float direction,duration,firstFrame,lastFrame,startSegment,endSegment;
 bool frozen;
 FilmStrips() : LinkedList() {
  current=null;
  cued=null;
  oldMode=play=&FilmStrips::PlayerEmpty;
  limit=expired=0.0f;
  direction=FPS;
  firstFrame=lastFrame=0.0f;
  startSegment=-1.0f;
  endSegment=-1.0f;
  frozen=false;
  duration=0.0f;
  blend=none;
 }
 FilmStrip *find( const char *match ) {
  FOREACH(FilmStrip,s) if ( !str_cmp( match, (char *) s->title.c_str() ) ) return s;
  return null;
 }
 void Add( Film *f ) {
  FilmStrip *fs=new FilmStrip;
  duration+=f->duration;
  fs->film=f;
  Append(fs);
 }
 void Add( Film *f, string n ) {
  FilmStrip *fs=new FilmStrip;
  duration+=f->duration;
  fs->film=f;
  fs->title=n;
  Append(fs);
 }
 virtual CLEARLIST(FilmStrip);
 ~FilmStrips() { Clear(); }
 // Default Play() member
 void PlayerEmpty() { }
 // Return active frame, or null
 GLImage *Frame() { if ( current ) return current->film->frame(); return null; }
 // Reverse play direction and amplitude (FPS = 1.0f)
 void Reverse() {
  direction=-direction;
 }
 // Render 2D with global tint and blend
 void Render2D( GLWindow *surface ) {
  if ( !current ) return;
  GLImage *frame=current->film->frame();
  if ( !frame ) return;
  StretchiNoAlpha(surface,frame,tint,blend,position.x,position.y,position.w,position.h);
 }
 // Render 2D with local (film level) tint and blend
 void Render2DLocal( GLWindow *surface ) {
  if ( !current ) return;
  GLImage *frame=current->film->frame();
  if ( !frame ) return;
  StretchiNoAlpha(surface,frame,current->film->tint,current->film->blend,position.x,position.y,position.w,position.h);
 }
 // Rewind current strip
 void Rewind() { current->film->Rewind(); frozen=false; }
 // Rewind all strips
 void RewindAll() {
  while( current ) { current->film->Rewind(); current=(FilmStrip *) current->next; }
  frozen=false; 
 }
 // Rewind all strips and cue first strip
 void RewindAllAndReset() {
  while( current ) { current->film->Rewind(); current=(FilmStrip *) current->next; }
  current=(FilmStrip *) first; 
  frozen=false;
 }
 // Rewind alls trips and cue first strip, resetting queue
 void RewindAllAndResetQueue() {
  while( current ) { current->film->Rewind(); current=(FilmStrip *) current->next; }
  current=order.first==null ? null : (FilmStrip *) Element( (cued=(Integer *) order.first)->i );
  frozen=false;
 }
 // Take keywords as input and generate the order queue by filmstrip names
 void StringsToQueue( Strings *names ) {
  EACH(names->first,String,s) order.Add(this->IndexOf(find((char *)s->s.c_str())));
 }
 // Advance to the next cue
 void AdvanceToNextInQueue() {
  if ( cued ) cued=(Integer *) cued->next;
  if ( cued ) current=(FilmStrip *) Element(cued->i);
 }
 // Advance to the next cue
 void AdvanceToNextInQueueLoop() {
  if ( cued ) cued=(Integer *) cued->next;
  if ( !cued ) cued=(Integer *) order.first;
  if ( cued ) current=(FilmStrip *) Element(cued->i);
 }
 // Advance to global position
 void AdvanceToGlobal( float seek ) {
 }
 // Play options

 // Advances a frame through each segment
 void PlayEndtoEndLooped() {
  if ( !current ) { current=(FilmStrip *) first; RewindAll(); } 
  if ( !current ) return;
  if ( (current->film->expired+=direction) >= current->film->duration ) {
   static float d=current->film->duration-current->film->expired;
   current=(FilmStrip *) current->next;
   if ( !current ) current=(FilmStrip *) first;
   if ( !current ) return;
   current->film->duration=d;
  }
 }
 // For PlayEndtoEnd
 void AdvanceNextAccumulatingDuration() {
  current->film->expired+=direction;
  if ( current->film->expired >= current->film->duration ) {
   static float d=current->film->duration-current->film->expired;
   current=(FilmStrip *) current->next;
   if ( current ) current->film->expired=d;
  }
 }
 // Plays each film end to end
 void PlayEndtoEnd() {
  if ( current ) AdvanceNextAccumulatingDuration();
 }
 // Plays in order specified by Integers order
 void PlayInOrder() {
  if ( order.count == 0 || !cued ) return;
  Film *element=(Film *) Element(cued->i);
  if ( !current ) return;
  if ( current->film->expired >= current->film->duration ) {
   static float d=current->film->duration-current->film->expired;
   cued=(Integer *) cued->next;
   if ( cued ) current = (FilmStrip * ) Element(cued->i);
   if ( current ) current->film->duration=d;
  }
 }
 // Plays in order specified by Integers order
 void PlayInOrderLooped() {
  if ( order.count == 0 ) return;
  if ( !cued ) {
   cued=(Integer *) order.first;
   if ( cued ) current = (FilmStrip * ) Element(cued->i);
   else return;
  } 
  Film *element=(Film *) Element(cued->i);
  if ( !current ) return;
  if ( current->film->expired >= current->film->duration ) {
   static float d=current->film->duration-current->film->expired;
   cued=(Integer *) cued->next;
   if ( cued ) current = (FilmStrip * ) Element(cued->i);
   if ( current ) current->film->duration=d;
  }
 }
 // Plays in random order
 void PlayRandomOrderLoop() {
  if ( !current ) current = (FilmStrip * ) Element(number_range(0,count));
  if ( !current ) return;
  current->film->expired+=direction;
  if ( current->film->expired >= current->film->duration ) {
   current = (FilmStrip * ) Element(number_range(0,count));
   if ( current ) current->film->Rewind();
  }
 }
 // Plays in random order and limits the segment played to a duration
 void PlayRandomOrderLoopLimited() {    
  if ( !current || current->film->expired >= current->film->duration ) {
   current=(FilmStrip *) this->Element(number_range(0,count));
   current->film->Rewind();
  } else {
  }
 }
 // Play current film once
 void PlayCurrent() {
  if ( current ) {
   current->film->expired+=direction;
   if ( current->film->expired >= current->film->duration ) current=null;
  }
 }
 // Play current film looped
 void PlayCurrentLooped() {
  if ( current ) {
   current->film->expired+=direction;
   if ( current->film->expired >= current->film->duration ) current->film->expired-=current->film->duration;
  }
 }
 // Calculates the last frame of the currently selected film (for LastFrame members)
 void CurrentLastFrame() {
  lastFrame=current->film->duration-abs(direction);
 }
 // Play current film until last frame then pause at last frame
 void PlayCurrentFreezeLastFrame() {
  if ( !frozen && current ) {
   if ( current->film->expired < lastFrame ) current->film->expired+=direction;
   if ( current->film->expired >= lastFrame ) {
    current->film->expired=lastFrame;
    frozen=true; //
   }
  }
 }
 // Plays all films end-to-end until last frame then pause at last frame
 void PlayAllFreezeLastFrame() {
  if ( !frozen && current ) {
   current->film->expired+=direction;
   if ( current->next != null && current->film->expired >= current->film->duration ) {
    static float d=current->film->duration-current->film->expired;
    current = (FilmStrip *) current->next;
    current->film->Rewind();
   } else
   if ( current->next == null && current->film->expired >= lastFrame ) {
    current->film->expired=lastFrame;
    frozen=true; //
   }
  }
 }
 // Play random frames from any film
 void PlayRandomFrames() {
  if ( count == 0 ) return;
  float delta=float_range(0,duration);
  int c=0;
  current = (FilmStrip *) first;
  while ( delta > current->film->duration ) {
   c++;
   delta-=current->film->duration;
   current= (FilmStrip *) current->next;
   if ( current == null ) current = (FilmStrip *) first;
  }
  current->film->expired=delta;
 }
 // Play random length ending segments from any film
 void PlayRandomEndingSegments() {
  if ( !current ) {
   current = (FilmStrip *) Element(number_range(0,count));
   if ( current ) current->film->expired=float_range(0.0f,current->film->duration*0.75f);
  } else {
   current->film->expired+=direction;
   if ( current->film->expired >= current->film->duration ) {
    current = (FilmStrip *) Element(number_range(0,count));
    if ( current ) current->film->expired=float_range(0.0f,current->film->duration*0.75f);
   }
  }
 }
 // Play random segments from any film
 void PlayRandomSegments() {
  if ( startSegment == -1 || endSegment == -1 ) {
   startSegment=float_range(0.0f,duration);
   endSegment=float_range(0.0f,duration);
   if ( endSegment > startSegment ) { float temp=endSegment; startSegment=endSegment; endSegment=temp; }
  }
 }
 void PlayRandomCircular() {
  if ( startSegment == -1 || endSegment == -1 ) {
   startSegment=float_range(0.0f,duration);
   endSegment=float_range(0.0f,duration);
   if ( endSegment > startSegment ) { float temp=endSegment; startSegment=endSegment; endSegment=temp; }
  }
 }
 void PlayRandomLoop() {
  if ( startSegment == -1 || endSegment == -1 ) {
   startSegment=float_range(0.0f,duration);
   endSegment=float_range(0.0f,duration);
   if ( endSegment > startSegment ) { float temp=endSegment; startSegment=endSegment; endSegment=temp; }
  }
 }
 // Ping pong the entire set of films
 void PlayPingPongLoop() {
 }
 // Ping pong the entire set of films
 void PlayPingPongOrderedLoop() {
  if ( !current ) {
   cued=(Integer *) order.first;
   if ( cued ) current = (FilmStrip *) Element(cued->i);
   direction=abs(direction);
  } else {
   if ( cued && cued == order.last ) {
    direction=-abs(direction);
   }
  }
 }
 // Randomly ping-pong random segments randomly
 void PlayPingPongRandom() {
 }
 // Randomly one-way ping-pong random segments randomly
 void PlayOneWayPingPongRandom() {
 }
 // Randomly change direction, but always play from one end to another on any film
 void PlayPingPongRandomSequences() {
 }
 // Randomly change direction (essentially brownian motion)
 void PlayRandomDirectionEndless() {
 }
 void PlayEndtoEndDirectionTween() {
 }
 // Plays a segment of the films
 void PlaySegment() {
 }
 // Plays in random order and limits the segment played to a duration
 void PlaySegmentLoop() {    
  if ( !current || current->film->expired >= current->film->duration ) {
   current=(FilmStrip *) this->Element(number_range(0,count));
   current->film->Rewind();
  } else {
  }
 }
 // Advances all of the films concurrently
 void PlayAdvanceAllFilmsConcurrently() {
  FOREACH(FilmStrip,strip) { strip->film->expired+=direction; }
 }
};

class FilmLibrary : public ListItem {
public:
 string title;
 FilmStrips filmstrips;
 Films films;
 CompositeFilms compositeFilms;
 Film *Film( string c ) {
  return films.find(c);
 }
 FilmLibrary() : ListItem() {
  title=string("");
 }
 FilmLibrary( string n ) : ListItem() {
  title=n;
 }
 Strings *asFilmStringList() {
  return films.asStringList();
 }
};

class FilmLibraries : public LinkedList {
public:
 FilmLibrary *find( const char *match ) {
  FOREACH(FilmLibrary,l) if ( !str_cmp( match, (char *) l->title.c_str() ) ) return l;
  return null;
 }
 Film *Any( const char *match ) {
  FilmLibrary *found=find(match);
  if ( !found ) return null;
  return (Film *) found->films.Any();
 }
 Film *Any( const char *match, int seed ) {
  FilmLibrary *found=find(match);
  if ( !found ) return null;
  return (Film *) found->films.Any(seed);
 }
 Film *find( const char *library, const char *film ) {
  FilmLibrary *lib=find(library);
  if ( !lib ) {
   if ( strlen(library) > 0 ) OUTPUT("FilmLibraries: Cannot find library `%s`\n", library);
#if defined(DIV_BY_ZERO_REPORTING)
   else OUTPUT("FilmLibraries: Cannot find library (empty string provided)\n");
#endif
   return null;
  }
  return lib->Film(film);
 }
 FilmLibrary *Add( string name ) {
  FilmLibrary *lib=new FilmLibrary;
  lib->title = name;
  Append(lib);
  return lib;
 }
 FilmLibrary *findOrAdd( const char *match ) {
  FilmLibrary *f=find(match);
  if ( f ) return f;
  return Add(string(match));
 }
 Strings *asStringList() {
  Strings *L=new Strings;
  FOREACH(FilmLibrary,f) L->Add(f->title);
  return L;
 }
 Strings *asStringList( const char *match ) {
  FilmLibrary *f=find(match);
  if ( f ) return f->asFilmStringList();
  return null;
 }
 void Load( const char *filename );
};

extern FilmLibraries vault;