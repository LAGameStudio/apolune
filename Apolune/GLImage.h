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

#include "moremath.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "GLBlend.h"
#include "GLWindow.h"
#include "Strings.h"
#include "Cartesian.h"

//#define USE_CORONA 1

#if defined(USE_CORONA)
#include <corona.h>
#else
#include "ImageLoader.h"
#endif

class GLImage : public ListItem {
 void Init(void);
public:
#if defined(USE_CORONA)
 corona::Image *image;
 void *pixels;
// corona::Image *mipmap;
#else
 Zp<CPUImage> image;
 GLubyte *pixels;
// CPUImage *mipmap;
#endif
 Zint width, height, yBottom;
 string filename;
 Zint integer;
 Zfloat floating; 
 Zbool deleteAfterUpload; // Delete after upload?
 GLuint texture;
 Zbool uploaded;

 GLImage(void);
 GLImage( const char *name, int w, int h );
 GLImage( const char * filename );
 GLImage( string filename );
 GLImage( CPUImage *in );
 GLImage( const char *name, CPUImage *in );
 GLImage( float *rgb_buffer, int w, int h, const char *name, GLint mag, GLint min, GLint wrapS, GLint wrapT, bool mipmaps=true );
 GLImage( having alpha, float *rgba_buffer, int w, int h, const char *name, GLint mag, GLint min, GLint wrapS, GLint wrapT, bool mipmaps=true );
 
// GLImage( GLGenTextureSettings *t );
 ~GLImage(void);

 double GetScale( double r ) { return UMIN( r/(double)width, r/(double)height ); }
 double GetScale( Cartesiand *fitIn ) {
  double r=UMIN(fitIn->w,fitIn->h);
  return UMIN( r/(double)width, r/(double)height );
 }

 SKEYSTART
  SKEY("file",        { SKEYPAIR; filename=key; })
  SKEY("integer",     { SPARAM(integer,int,atoi);     })
  SKEY("floating",    { SPARAM(floating,float,atof);  })
  SKEY("retain",      { deleteAfterUpload=false;      })
 SKEYENDING("GLImage")
#if defined(USE_CORONA)
  try {
   image=corona::OpenImage(filename.c_str(), corona::PF_B8G8R8A8);
  } catch (bad_alloc& ba) {
   OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
  }
  if ( !image ) { OUTPUT("Image file '%s' missing\n",filename); return; }
  width=image->getWidth();
  height=image->getHeight();
  pixels=image->getPixels();
#else
  image=imageLoader.Load(filename);
  if ( !image ) {
   OUTPUT("Image file '%s' missing\n",filename);
  } else if ( image->error ) {
   OUTPUT("Image file '%s' was corrupt, or an unsupported format.\n");
  }
  if ( ( pixels=image->getPixels() ) == null ) {
   OUTPUT("Image was blank (null pixels)\n");
  }
  width=image->getWidth();
  height=image->getHeight();
#endif
 SKEYENDER

 string toString() {
  string out=string("");
  char buf[512];
  FORMAT(buf,512,"file {%s}",filename.c_str() ); out+=string(buf);
  if ( integer!=-6666666 ) {
   FORMAT(buf,512," integer %d", (int) integer); out+=string(buf);
  }
  if ( floating!=-6666666.6666666f ) {
   FORMAT(buf,512," floating %f", (float) floating); out+=string(buf);
  }
  if ( !deleteAfterUpload ) out+=string(" retain");
  return out;
 }

// void Save( char *filename );
 void Upload( GLSetup *glSetup, bool mipmaps );
 void Upload( GLSetup *glSetup, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT );
 void Unload(void);
 void Use() { glBindTexture(GL_TEXTURE_2D, texture); }
 void Disable() { glBindTexture(GL_TEXTURE_2D,0); }
 void DiscardCPUData() {
  if ( image ) {
   OUTPUT("Deleting CPU image...\n");
   delete image;
   image=null;
   pixels=null;
  }
 }
 bool matches( const char *match ) {
  return ( !str_infix(match,filename.c_str()) );
 }
};

/*
 * Graphics primitives
 */

class ImageHandle : public ListItem {
public:
 GLImage *i;
 ImageHandle() : ListItem() { i=null; }
 ImageHandle(GLImage *I) : ListItem() { i=I; }
};

class Images : public LinkedList {
public:
 // Bitmap lists
 inline void Push( ImageHandle *i ) { Prepend(i); }

 CLEARLISTRESET(ImageHandle);
 ~Images() { Clear(); }

 GLImage *element( int i ) {
  ImageHandle *iH=(ImageHandle *)Element(i);
  return iH->i;
 }
 GLImage *Frame( int i ) {
  ImageHandle *iH=(ImageHandle *) Element(i);
  return iH?iH->i:null;
 }
 GLImage *FrameMod( int i ) {
  ImageHandle *iH=(ImageHandle *) Element(i%count.value);
  return iH->i;
 }
 GLImage *Frame( double d ) {
  int i=(int)(d*(double)count.value)%count.value;
  return Frame(i);
 }
 GLImage *FrameMod( double d ) {
  int i=(int)(d*(double)count.value)%count.value;
  return FrameMod(i);
 }
 GLImage *random() {
  if ( count==1 ) return ((ImageHandle *)first)->i;
  ImageHandle *iH=(ImageHandle *) Element(upto(100000+count)%count);
  return iH->i;
 }

 bool inList( ImageHandle *i ) {
  ImageHandle *iH;
  for ( iH=(ImageHandle *) first; iH; iH=(ImageHandle *) iH->next )
   if ( iH == i || !str_cmp( iH->i->filename.c_str(), i->i->filename.c_str() ) ) return true; 
  return false;
 }

 bool inList( GLImage *i ) {
  ImageHandle *iH;
  for ( iH=(ImageHandle *) first; iH; iH=(ImageHandle *) iH->next )
   if ( !str_cmp( iH->i->filename.c_str(), i->filename.c_str() ) ) return true; 
  return false;
 }

 bool addIfUnique( ImageHandle *i ) {
  if ( this->inList(i) ) return false;
  this->Push(i); 
  return true;
 }

 bool addIfUnique( GLImage *i ) {
  if ( this->inList(i) ) return false;
  this->Push(new ImageHandle(i)); 
  return true;
 }

 void Add( GLImage *i ) {
  this->Push(new ImageHandle(i)); 
 }

 void AddAtEnd( GLImage *i ) {
  this->Append((ListItem *) new ImageHandle(i));
 }

 /*
 Images *Copy() {
  Images new_list= new Images;
  Images *iL;
  for ( iL=(ImageHandle *) first; iL!=null; iL=(ImageHandle *) iL->next ) { new_list->Push(iL); 
  return new_list;
 }*/

};

class ImageSet : public ListItem {
public:
 Images images;
 string name;
 void Add( GLImage *i ) {
  images.Add(i);
 }
 void addIfUnique( GLImage *i ) {
  images.addIfUnique(i);
 }
};

class ImageSets : public LinkedList {
public:
 ImageSet *Add( string name ) {
  ImageSet *n=new ImageSet;
  n->name=name;
  Append(n);
 }
 ImageSet *find( string name ) {
  FOREACH(ImageSet,i) 
   if ( !str_infix( name.c_str(), i->name.c_str() ) ) 
    return i;
  return null;
 }
 // Finds the set containing a matching image
 ImageSet *find( string name, string image ) {
  ImageSet *i=find(name);
  if ( !i ) return null;
  EACH(i->images.first,ImageHandle,ih) if ( ih->i->matches( image.c_str()) ) return i;
  return null;
 }
 // Finds the set containing a matching image pointer
 ImageSet *find( string name, GLImage *image ) {
  ImageSet *i=find(name);
  if ( !i ) return null;
  EACH(i->images.first,ImageHandle,ih) if ( ih->i == image ) return i;
  return null;
 }
 ImageSet *Add( string name, GLImage *i ) {
  ImageSet *n=find(name);
  if ( !n ) {
   n=new ImageSet;
   n->name=name;
   Append(n);
  }
  n->Add(i);
  return n;
 }
 ImageSet *addIfUnique( string name, GLImage *i ) {
  ImageSet *n=find(name);
  if ( !n ) {
   n=new ImageSet;
   n->name=name;
   Append(n);
  }
  n->addIfUnique(i);
  return n;
 }
 // Finds the set containing a matching image pointer
 ImageSet *findOrAdd( string name, GLImage *image ) {
  ImageSet *i=find(name);
  if ( !i ) {
   i=new ImageSet;
   i->name=name;
   Append(i);
   i->Add(image);
   return i;
  }
  EACH(i->images.first,ImageHandle,ih) if ( ih->i == image ) return i;
  i->Add(image);
  return i;
 }
 // Finds the set containing a matching image pointer
 ImageSet *findOrAddIfUnique( string name, GLImage *image ) {
  ImageSet *i=find(name);
  if ( !i ) {
   i=new ImageSet;
   i->name=name;
   Append(i);
   i->addIfUnique(image);
   return i;
  }
  EACH(i->images.first,ImageHandle,ih) if ( ih->i == image ) return i;
  i->addIfUnique(image);
  return i;
 }
 CLEARLISTRESET(ImageSet);
 ~ImageSets() { Clear(); }
};