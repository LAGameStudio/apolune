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

#include <iostream>
#include <math.h>

#include "string_functions.h"
#include "Art.h"
#include "Crayon.h"
#include "FANSI.h"

//#define USE_CORONA 1

#if defined(USE_CORONA)
#include <corona.h>
#else
#include "ImageLoader.h"
#endif

struct FansiPixel {
 unsigned int hi,lo;
 FansiPixel() { hi=lo=0; }
 void fansiColor( Crayon c, unsigned int yd2, unsigned int ym2 ) {
  switch ( ym2 ) {
  case 0:
    hi=FANSI.Near(c);
    if ( hi == 255 ) hi=15; // Reserve 255 as "clear" color
    if ( c.rf < 0.0f ) hi=255;
   break;
  default:
  case 1:
    lo=FANSI.Near(c);
    if ( lo == 255 ) lo=15;
    if ( c.rf < 0.0f ) lo=255;
   break;
  }
 }
};

struct FansiCompressedPixel {
public:
 int count;
 unsigned int hi,lo;
 FansiCompressedPixel() { count=-1; hi=lo=0; }
 string Output() {
  if ( count == -1 ) return string("");
  return 
   (count > 1
    ? FORMAT(buf2,128,"[color(%d,%d,c(223,%d))]",hi,lo,count)
    : FORMAT(buf2,128,"[color(%d,%d,c(223))]",hi,lo )
   )
  ;
 }
};

struct FansiCompressedImage {
public:
 Indexed<FansiCompressedPixel> pixels;
 void FromFansiPixels( Indexed<FansiPixel> *in ) {
  pixels.Size(in->w,in->h);
  for ( unsigned int y=0; y<in->h; y++ ) {
   /*
   for ( unsigned int x=0; x<in->w; x++ ) {
    pixels(x,y).hi=(*in)(x,y).hi;
    pixels(x,y).lo=(*in)(x,y).lo;
    pixels(x,y).count=1;
   }
   */
   unsigned int x=0;
   unsigned int hi=0,lo=0;
   unsigned int cx=0;
   while ( x < in->w ) {
    hi=(*in)(x,y).hi;
    lo=(*in)(x,y).lo;
    int count=0;
    bool matches=true;
    for ( unsigned int lx=x; lx<in->w && matches; lx++ ) {
     matches =((*in)(lx+count,y).lo == lo && (*in)(lx+count,y).hi == hi);
     if ( matches ) count++;
    }
    pixels(cx,y).hi=hi;
    pixels(cx,y).lo=lo;
    pixels(cx,y).count=count;
    cx++;
    x+=(count);
   }
  }
 }
 string Output() {
  string out=string("");
  for ( unsigned int y=0; y<pixels.h; y++ ) {
   for ( unsigned int x=0; x<pixels.w; x++ ) {
    if ( pixels(x,y).count == -1 ) break;
    out+=pixels(x,y).Output();
   }
   out+=string("%r");
  }
  return out;
 }
};

class Image2Fansi : public ListItem {
public:
 string filename;
 byte *pixels;
#if defined(USE_CORONA)
 corona::Image *image;
 corona::PixelFormat format;
#else
 CPUImage *image;
#endif
 int pixelSize,bytesSize;
 int width, height;
 Zstring result;
 Zbool fansi_simplify,fansi_transparency;
 char trans_char;
 Zstring output;
 Image2Fansi() : ListItem() {
  trans_char='@';
  width=height=0;
  pixelSize=bytesSize=0;
  image=null;
  pixels=null;
  filename=string("");
 }
 Image2Fansi( const char *filename ) : ListItem() {
  Load(filename);
 }
 void Load( const char *filename ) {
  this->filename=string(filename);
#if defined(USE_CORONA)
  try {
   image=corona::OpenImage(filename, corona::PF_B8G8R8A8);
  } catch (bad_alloc& ba) {
   OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
  }
  if ( !image ) { OUTPUT("Image file '%s' missing\n",filename); return; }
  width=image->getWidth();
  height=image->getHeight();
  pixels=(byte*)image->getPixels();
  format=image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  image=imageLoader.Load(filename);
  if ( !image ) {
   OUTPUT("Image file '%s' missing\n",filename);
  } else if ( image->error ) {
   OUTPUT("Image file '%s' was corrupt, or an unsupported format.\n");
  } else {
   if ( ( pixels=image->getPixels() ) == null ) {
    OUTPUT("Image was blank (null pixels)\n");
   }
   width=image->getWidth();
   height=image->getHeight();
   pixels=image->getPixels();
   pixelSize=4;
   bytesSize=width*height*pixelSize;
#endif
   Process();
   delete image;
   image=null;
   pixels=null;
  }
 }
 void Process() {
  unsigned int hd2=height/2;
  Indexed<FansiPixel> fpixels;
  FansiCompressedImage fci;
  fpixels.Size((size_t)width,(size_t)hd2);
  for ( unsigned int x=0; x<(unsigned int)width; x++ ) {
   for ( unsigned int y=0; y<(unsigned int)height; y+=2 ) {
    if ( y+1 >= (unsigned int) height ) break;
    byte *bgra1=(byte *) &pixels[(x+y*width)*pixelSize];
    byte *bgra2=(byte *) &pixels[(x+(y+1)*width)*pixelSize];
    unsigned int yd2=(unsigned int)y/2;
    Crayon hi,lo;
    if ( bgra1[3] == 255 ) {
     hi.Int((int)bgra1[2],(int)bgra1[1],(int)bgra1[0],255);
     fpixels(x,yd2).hi=FANSI.Near(hi);
     if ( fpixels(x,yd2).hi == 255 ) fpixels(x,yd2).hi=15; // Reserve 255 as "clear" color
    } else fpixels(x,yd2).hi=255;
    if ( bgra2[3] == 255 ) {
     lo.Int((int)bgra2[2],(int)bgra2[1],(int)bgra2[0],255);
     fpixels(x,yd2).lo=FANSI.Near(lo);
     if ( fpixels(x,yd2).lo == 255 ) fpixels(x,yd2).lo=15; // Reserve 255 as "clear" color
    } else fpixels(x,yd2).lo=255;
   }
  }
  fci.FromFansiPixels(&fpixels);
  output=fci.Output();
 }
};

class Image2Fansis : public LinkedList {
public:
 Image2Fansi *Load( const char *filename ) {
  FOREACH(Image2Fansi,i) if ( contains(filename,i->filename.c_str()) ) return null;
  Image2Fansi *loaded;
  Append(loaded=new Image2Fansi(filename));
  return loaded;
 }
 Image2Fansi *Find( const char *filename ) {
  FOREACH(Image2Fansi,i) if ( !str_cmp(filename,i->filename.c_str()) ) return i;
  return null;
 }
 void Load( Strings *in ) {
  EACH(in->first,String,s) if ( s->s.length() > 0 && !Find(s->s.c_str()) ) Append(new Image2Fansi(s->s.c_str()));
 }
 CLEARLISTRESET(Image2Fansi);
 ~Image2Fansis() {
  Clear();
 }
};

class Sheet2Fansi : public ListItem {
public:
 string filename;
 byte *pixels;
#if defined(USE_CORONA)
 corona::Image *image;
 corona::PixelFormat format;
#else
 CPUImage *image;
#endif
 int pixelSize,bytesSize;
 int width, height;
 unsigned int tilesx, tilesy, tilew,tileh;
 Zstring result;
 Zbool fansi_simplify,fansi_transparency;
 char trans_char;
 Indexed<Zstring> output;
 Sheet2Fansi() : ListItem() {
  trans_char='@';
  width=height=0;
  pixelSize=bytesSize=0;
  image=null;
  pixels=null;
  filename=string("");
  tilesx=-1;
  tilesy=-1;
 }
 Sheet2Fansi( const char *filename, int tw, int th ) : ListItem() {
  tilesx=-1;
  tilesy=-1;
  tilew=tw;
  tileh=th;
  Load(filename);
 }
 Sheet2Fansi( const char *filename, int xcount, int ycount, int tw, int th ) : ListItem() {
  tilesx=xcount;
  tilesy=ycount;
  tilew=tw;
  tileh=th;
  Load(filename);
 }
 void Load( const char *filename ) {
  this->filename=string(filename);
#if defined(USE_CORONA)
  try {
   image=corona::OpenImage(filename, corona::PF_B8G8R8A8);
  } catch (bad_alloc& ba) {
   OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
  }
  if ( !image ) { OUTPUT("Image file '%s' missing\n",filename); return; }
  width=image->getWidth();
  height=image->getHeight();
  pixels=(byte*)image->getPixels();
  format=image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  image=imageLoader.Load(filename);
  if ( !image ) {
   OUTPUT("Image file '%s' missing\n",filename);
  } else if ( image->error ) {
   OUTPUT("Image file '%s' was corrupt, or an unsupported format.\n");
  } else {
   if ( ( pixels=image->getPixels() ) == null ) {
    OUTPUT("Image was blank (null pixels)\n");
   }
   width=image->getWidth();
   height=image->getHeight();
   pixels=image->getPixels();
   pixelSize=4;
   bytesSize=width*height*pixelSize;
#endif
   Process();
   delete image;
   image=null;
   pixels=null;
  }
 }
 void Process() {
  if ( tilesx == -1 ) { tilesx=(unsigned int)width/tilew; }
  if ( tilesy == -1 ) { tilesy=(unsigned int)height/tileh; }
  Crayon hi,lo;
  output.Size((size_t)tilesx,(size_t)tilesy);
  for ( unsigned int i=0; i<tilesx; i++ ) {
   for ( unsigned int j=0; j<tilesy; j++ ) {
    unsigned int hd2=tileh/2;
    Indexed<FansiPixel> fpixels;
    FansiCompressedImage fci;
    fpixels.Size((size_t)tilew,(size_t)hd2);
    for ( unsigned int x=0; x<tilew; x++ ) {
     for ( unsigned int y=0; y<tileh; y+=2 ) {
      unsigned int tx=x+tilew*i;
      unsigned int ty=y+tileh*j;
      if ( ty+1 >= (unsigned int) height ) break;
      byte *bgra1=(byte *) &pixels[(tx+ty*width)*pixelSize];
      byte *bgra2=(byte *) &pixels[(tx+(ty+1)*width)*pixelSize];
      unsigned int yd2=(unsigned int)y/2;
      if ( bgra1[3] == 255 ) {
       hi.Int((int)bgra1[2],(int)bgra1[1],(int)bgra1[0],255);
       fpixels(x,yd2).hi=FANSI.Near(hi);
       if ( fpixels(x,yd2).hi == 255 ) fpixels(x,yd2).hi=15; // Reserve 255 as "clear" color
      } else fpixels(x,yd2).hi=255;
      if ( bgra2[3] == 255 ) {
       lo.Int((int)bgra2[2],(int)bgra2[1],(int)bgra2[0],255);
       fpixels(x,yd2).lo=FANSI.Near(lo);
       if ( fpixels(x,yd2).lo == 255 ) fpixels(x,yd2).lo=15; // Reserve 255 as "clear" color
      } else fpixels(x,yd2).lo=255;
     }
    }
    fci.FromFansiPixels(&fpixels);
    output(i,j)=fci.Output();
   }
  }
 }
};

class Sheets2Fansis : public LinkedList {
public:
 Sheet2Fansi *Load( const char *filename, int tilesx, int tilesy, int tilew, int tileh ) {
  FOREACH(Sheet2Fansi,i) if ( contains(filename,i->filename.c_str()) ) return null;
  Sheet2Fansi *loaded=new Sheet2Fansi(filename, tilesx, tilesy, tilew, tileh);
  Append(loaded);
  return loaded;
 }
 Sheet2Fansi *Load( const char *filename, int tilew, int tileh ) {
  FOREACH(Sheet2Fansi,i) if ( contains(filename,i->filename.c_str()) ) return null;
  Sheet2Fansi *loaded=new Sheet2Fansi(filename, tilew, tileh);
  Append(loaded);
  return loaded;
 }
 Sheet2Fansi *Find( const char *filename ) {
  FOREACH(Sheet2Fansi,i) if ( !str_cmp(filename,i->filename.c_str()) ) return i;
  return null;
 }
 void Load( Strings *in, int tilew, int tileh ) {
  EACH(in->first,String,s) if ( s->s.length() > 0 && !Find(s->s.c_str()) )
   Append(new Sheet2Fansi(s->s.c_str(), tilew, tileh));
 }
 void Load( Strings *in, int tilesx, int tilesy, int tilew, int tileh ) {
  EACH(in->first,String,s) if ( s->s.length() > 0 && !Find(s->s.c_str()) )
   Append(new Sheet2Fansi(s->s.c_str(), tilesx, tilesy, tilew, tileh));
 }
 CLEARLISTRESET(Sheet2Fansi);
 ~Sheets2Fansis() {
  Clear();
 }
};

#include "GLWindow.h"
#include "TextureLibrary.h"

class ConvertFANSIs : public GLWindow {
public:
 Image2Fansis fansis;
 Sheets2Fansis fansisheets;
 void OnLoad() {
  Strings in,sheets;
  in.Load("data/fansis.txt");
  fansis.Load(&in);
  sheets.Load("data/fansisheets24.txt");
  fansisheets.Load(&sheets,16,16);
  Zstring out,out2;
  EACH(fansis.first,Image2Fansi,f) {
   out+="\n"+f->filename+":\n"+f->output+string("\n");
  }
  string_as_file(out.c_str(),"data/outfansi.txt");
  EACH(fansisheets.first,Sheet2Fansi,sf) {
   for ( unsigned int i=0; i<sf->tilesx; i++ )
    for ( unsigned int j=0; j<sf->tilesy; j++ )
     out2+="\n"+sf->filename+i+","+j+":\n"+sf->output(i,j);
  }
  string_as_file(out2.c_str(),"data/outfansisheets.txt");
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
 }
 void Render() {
  int x=10;
  int y=10;
  EACH(fansis.first,Image2Fansi,f) {
   QuadiNoAlpha(library.Load(f->filename.c_str()),crayons.Pick(alabaster),transparency,x,y,f->width,f->height);
   x+=f->width;
   if ( x > display->w-10 ) { x=10; y+=f->height; }
  }
 }
};