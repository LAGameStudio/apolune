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

#include <vector>
using namespace std;

#include <Windows.h>
#include <stdio.h>
#include <string.h>

#include "macros.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"

#include "GLImage.h"
#include "ImageProcess.h"
#include "pBuffer.h"

class GLSetup;

GLint magMipAndWrapOptions( string k );
string magMipAndWrapOptionsString( GLint op );

ONE(LoadedTexture,{
 filename=string("");
 magFilter=wrapS=wrapT=0;
 minFilter=GL_LINEAR;
 assertedNoMipmaps=false;
})
 Zdis<GLImage> image;
 GLint magFilter, minFilter, wrapS, wrapT;
 string filename;
 bool assertedNoMipmaps;
 SKEYSTART
  SKEY("file",      { SKEYPAIR; filename=key; })
  SKEY("mag",       { SKEYPAIR; magFilter=magMipAndWrapOptions(key); })
  SKEY("min",       { SKEYPAIR; minFilter=magMipAndWrapOptions(key); })
  SKEY("wrap_s",    { SKEYPAIR; wrapS=magMipAndWrapOptions(key); })
  SKEY("wrap_t",    { SKEYPAIR; wrapT=magMipAndWrapOptions(key); })
  SKEY("nomipmaps", { SKEYPAIR; assertedNoMipmaps=true; })
 SKEYEND("LoadedTexture");
 string toString() {
  string out=string("");
  char buf[2048];
  FORMAT(buf,2048,"file {%s}%s", filename.c_str(), assertedNoMipmaps?" nomipmaps":"");
  out+=string(buf);
  if ( !assertedNoMipmaps ) {
   if ( magFilter != 0 ) {
    out+=string(" mag ")+magMipAndWrapOptionsString(magFilter);
   }
   if ( minFilter != GL_LINEAR ) {
    out+=string(" min ")+magMipAndWrapOptionsString(minFilter);
   }
  } else {
   out+=string(" nomipmaps");
  }
  if ( wrapS != 0 ) out+=string(" ")+magMipAndWrapOptionsString(wrapS);
  if ( wrapT != 0 ) out+=string(" ")+magMipAndWrapOptionsString(wrapT);
  return out;
 }
MANY(LoadedTexture,LoadedTextureHandle,LoadedTextureHandles,"texture",LoadedTextures,{})
 GLImage *find( const char *s, GLint mag, GLint min, GLint wrapS, GLint wrapT ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( !str_cmp(s,x->filename.c_str())
    && x->magFilter==mag
    && x->minFilter==min 
    && x->wrapS==wrapS 
    && x->wrapT==wrapT ) return x->image;
  return null;
 }
 GLImage *find( const char *s ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( !str_cmp(s,x->filename.c_str()) ) return x->image;
  return null;
 }
 GLImage *find( GLImage *g ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( x->image == g ) return x->image;
  return null;
 }
 LoadedTexture *findSettings( const char *s, GLint mag, GLint min, GLint wrapS, GLint wrapT ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( !str_cmp(s,x->filename.c_str())
    && x->magFilter==mag
    && x->minFilter==min 
    && x->wrapS==wrapS 
    && x->wrapT==wrapT ) return x;
  return null;
 }
 LoadedTexture *findSettings( const char *s ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( !str_cmp(s,x->filename.c_str()) ) return x;
  return null;
 }
 LoadedTexture *findSettings( GLImage *g ) {
  for ( LoadedTexture *x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( x->image == g ) return x;
  return null;
 }
 LoadedTexture *findSettings( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT, bool NoMipmaps  ) {
  LoadedTexture *x;
  for ( x=(LoadedTexture *)first; x; x=(LoadedTexture *)x->next )
   if ( !str_cmp(s,x->filename.c_str())
    && x->magFilter==magFilter 
    && x->minFilter==minFilter 
    && x->wrapS==wrapS 
    && x->wrapT==wrapT
    && x->assertedNoMipmaps == NoMipmaps ) return x;
  return null;
 }
DONE(LoadedTexture);

extern Zp<GLImage> invis,invisiblePNG;

enum TextureLibraryLoadTextureOptions {
 tllto_Default=0,                    // Mips Clip
 tllto_Nearest=1,                    // Clip
 tllto_Tiling=2,                     // Mips Tile
 tllto_Mirrored=3,                   // Mips Mirrored
 tllto_Tiling_Nearest=4,             
 tllto_Mirrored_Nearest=5,
 tllto_TilingX_Nearest=6,
 tllto_TilingY_Nearest=7,
 tllto_MirroredX_Nearest=8,
 tllto_MirroredY_Nearest=9,
 tllto_TilingX=10,                   // Mips
 tllto_TilingY=11,                   // Mips
 tllto_MirroredX=12,                 // Mips
 tllto_MirroredY=13,                 // Mips
 tllto_TilingX_MirroredY=14,         // Mips
 tllto_TilingY_MirroredX=15,         // Mips
 tllto_MirroredX_TilingY=16,         // Mips
 tllto_MirroredY_TilingX=17,         // Mips
 tllto_TilingX_MirroredY_Nearest=18,
 tllto_TilingY_MirroredX_Nearest=19,
 tllto_MirroredX_TilingY_Nearest=20,
 tllto_MirroredY_TilingX_Nearest=21,
};

void GetLoadTextureOpts( TextureLibraryLoadTextureOptions opts, GLint *mag, GLint *min, GLint *wrapS, GLint *wrapT );

#define TL_TABLE_SIZE 100
ONE(TextureLibrary,{ Init(); })
 vector<LoadedTextures> table;
 Zp<GLSetup> glSetup;
 Zbool hasStreaming;
 void Init();
 int StringToKey( const char *fn ) {
  int sum=0,count=0;
  while ( *fn != '\0' ) { count++; sum+=(int)(*fn); fn++; }
  return (sum+count)%TL_TABLE_SIZE;
 }
 LoadedTextures *pick( const char *s ) {
  int x=StringToKey(s);
  return &table[x];
 }
 GLImage *search( const char *s, GLint mag, GLint min, GLint wrapS, GLint wrapT ) {
  LoadedTextures *lt=pick(s);
  return lt->find(s,mag,min,wrapS,wrapT);
 }
 GLImage *search( const char *s ) {
  LoadedTextures *lt=pick(s);
  return lt->find(s);
 }
 GLImage *search( GLImage *s ) {
  LoadedTextures *lt=pick(s->filename.c_str());
  return lt->find(s);
 }
 LoadedTexture *searchSettings( const char *s, GLint mag, GLint min, GLint wrapS, GLint wrapT ) {
  LoadedTextures *lt=pick(s);
  return lt->findSettings(s,mag,min,wrapS,wrapT);
 }
 LoadedTexture *searchSettings( const char *s ) {
  LoadedTextures *lt=pick(s);
  return lt->findSettings(s);
 }
 LoadedTexture *searchSettings( GLImage *s ) {
  LoadedTextures *lt=pick(s->filename.c_str());
  return lt->findSettings(s);
 }
 LoadedTexture *searchSettings( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT, bool NoMipmaps  ) {
  LoadedTextures *lt=pick(s);
  return lt->findSettings(s,magFilter,minFilter,wrapS,wrapT,NoMipmaps);
 }
 void Append( LoadedTexture *t ) {
  LoadedTextures *lt=pick(t->filename.c_str());
  lt->Append(t);
 }
 // Completely deletes a library member.  Note that the image itself is not deleted directly but is not gauranteed valid after this call.
 void Remove( GLImage *i ) {
  LoadedTextures *lt=pick(i->filename.c_str());
  LoadedTexture *s=lt->findSettings(i);
  if ( s ) { 
   lt->Remove(s);
   delete s;
  }
 }
 SKEYSTART
  SKEY("image", { SKEYPAIR; LoadedTexture *t=new LoadedTexture; t->fromString(key.c_str()); LoadFromSettings(t); })
  SKEY("texture", { SKEYPAIR; LoadedTexture *t=new LoadedTexture; t->fromString(key.c_str()); LoadFromSettings(t); })
 SKEYEND("TextureLibrary");
 // Texture Streaming via PBOs
 // Called after glSetup is provided, inits the streaming texture PBOs.
 GLuint pbo[2];
 void InitStreaming() {
    if(glSetup->hasExtension("GL_ARB_pixel_buffer_object")) hasStreaming=true;
 }
 // Create "size" W X H X Channels PBOs
 void CreatePBOs( GLImage *i ) {
  int size=i->width*i->height*4;
  // create 2 pixel buffer objects, you need to delete them when program exits.
  // glBufferDataARB with NULL pointer reserves only memory space.
  glGenBuffersARB(2, pbo);
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[0]);
  glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, size, 0, GL_STREAM_DRAW_ARB);
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[1]);
  glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, size, 0, GL_STREAM_DRAW_ARB);
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
  // bind the texture and PBO
  glBindTexture(GL_TEXTURE_2D, i->texture);
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[0]);
  // copy pixels from PBO to texture object
  // Use offset instead of ponter.
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, i->width, i->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  // bind PBO to update pixel values
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[1]);
  // Map the buffer object into client's memory; note that glMapBufferARB() causes sync issue.
  // If GPU is working with this buffer, glMapBufferARB() will wait(stall) for GPU to finish its job.
  // To avoid waiting (stall), you can call first glBufferDataARB() with NULL pointer before glMapBufferARB().
  // If you do that, the previous data in PBO will be discarded and glMapBufferARB() returns a new allocated
  // pointer immediately even if GPU is still working with the previous data.
  glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, size, 0, GL_STREAM_DRAW_ARB);
  GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
  if(ptr) {
   if (!ptr) OUTPUT( "TextureLibrary::CreatePBOs: Error, glMapBufferARB returned null ptr.\n" );
   // copy bytes from GLImage->image to texture, then release pixels and the mapping buffer
   glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
  }
  // It is good idea to release PBOs with ID 0 after use.
  // Once bound with 0, all pixel operations behave normal ways.
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
  glDeleteBuffersARB(2, pbo);
 }
 // Used by SKEY*
 // Loads if 2D texture with exactly the same settings it not already loaded.
 // Queries developer for mipmapping and edge related options.
 GLImage *LoadFromSettings( LoadedTexture *y ) {
  bool mipmaps=y->magFilter != 0 || !y->assertedNoMipmaps;
  GLImage *result=search(y->filename.c_str(),y->magFilter,y->minFilter,y->wrapS,y->wrapT);
  if ( result ) {
   delete y;
   return result;
  }
  y->image=new GLImage(y->filename);
  y->magFilter=y->assertedNoMipmaps ? GL_LINEAR : (y->magFilter == 0 ? GL_LINEAR_MIPMAP_LINEAR : y->magFilter);
  y->image->Upload(glSetup,y->magFilter,y->minFilter,y->wrapS,y->wrapT);
  Append(y);
  return y->image;
 }
 // Loads if 2D texture with exactly the same settings it not already loaded.
 // Uses default settings of clamp-to-edge and mipmap.
 GLImage *Load( const char *s, TextureLibraryLoadTextureOptions opts=tllto_Default, bool keep=false, bool uploadIt=true ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  GLImage *result=search(s,mag,min,wrapS,wrapT);
  if ( result ) return result;
  LoadedTexture *x=new LoadedTexture; 
  x->filename=string(s);
  x->image=new GLImage(s);
  x->magFilter=mag;
  x->minFilter=min;
  x->wrapS=wrapS;
  x->wrapT=wrapT;
  x->image->deleteAfterUpload=!keep;
  if ( uploadIt ) x->image->Upload(glSetup,mag,min,wrapS,wrapT);
  Append(x);
  return x->image;
 }
 // Loads if 2D texture with exactly the same settings it not already loaded.
 // Uses default settings of clamp-to-edge and mipmap.
 GLuint *LoadGLuint( const char *s, TextureLibraryLoadTextureOptions opts=tllto_Default, bool keep=false, bool uploadIt=true ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  GLImage *result=search(s,mag,min,wrapS,wrapT);
  if ( result ) return &result->texture;
  LoadedTexture *x=new LoadedTexture; 
  x->filename=string(s);
  x->image=new GLImage(s);
  x->magFilter=mag;
  x->minFilter=min;
  x->wrapS=wrapS;
  x->wrapT=wrapT;
  x->image->deleteAfterUpload=!keep;
  if ( uploadIt ) x->image->Upload(glSetup,mag,min,wrapS,wrapT);
  Append(x);
  return &x->image->texture;
 }
 // Treats the incoming CPUImage as a new image not currently in the library.
 // Uses default settings of clamp-to-edge and mipmap.
 GLImage *Load( const char *name, CPUImage *in, TextureLibraryLoadTextureOptions opts, bool keep=true ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  LoadedTexture *x=new LoadedTexture; 
  x->filename=string(name);
  x->image=new GLImage(name,in);
  x->magFilter=mag;
  x->minFilter=min;
  x->wrapS=wrapS;
  x->wrapT=wrapT;
  x->image->deleteAfterUpload=!keep;
  x->image->Upload(glSetup,x->magFilter,x->minFilter,x->wrapS,x->wrapT);
  Append(x);
  return x->image;
 } 
 // Loads if 2D texture with exactly the same settings it not already loaded.
 // Queries developer for mipmapping parameters and edge related options.
 GLImage *Load( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT ) {
  GLImage *result=search(s,magFilter,minFilter,wrapS,wrapT);
  if ( result ) return result;
  LoadedTexture *x=new LoadedTexture;
  x->filename=string(s);
  x->image=new GLImage(s);
  x->magFilter=magFilter;
  x->minFilter=minFilter;
  x->wrapS=wrapS;
  x->wrapT=wrapT;
  x->image->Upload(glSetup,x->magFilter,x->minFilter,x->wrapS,x->wrapT);
  Append(x);
  return x->image;
 }
 // Loads, duplicating the settings of, an existing image and preprocesses it with the provided class.
 // Checks if the old process has been run already, and returns that one to avoid process duplication.
 GLImage *Load( GLImage *i, ImageProcess *ip, bool keep ) {
  LoadedTexture *old=findSettings(i);
  LoadedTexture *x=null;
  if ( old ) {
   string keyword=string(i->filename)+ip->postfix;
   GLImage *result=search(keyword.c_str(),old->magFilter,old->minFilter,old->wrapS,old->wrapT);
   if ( result ) return result;
   x=new LoadedTexture; 
   x->image=ip->Process(i);
   x->filename=x->image->filename;
   x->image->deleteAfterUpload=!keep;
   x->magFilter=old->magFilter;
   x->minFilter=old->minFilter;
   x->wrapS=old->wrapS;
   x->wrapT=old->wrapT;
   x->image->Upload(glSetup,x->magFilter,x->minFilter,x->wrapS,x->wrapT);
   Append(x);
  } else {
   x=new LoadedTexture; 
   x->image=ip->Process(i);
   x->filename=x->image->filename;
   x->image->deleteAfterUpload=!keep;
   x->magFilter=GL_LINEAR;
   x->minFilter=GL_LINEAR_MIPMAP_LINEAR;
   x->wrapS=GL_CLAMP_TO_EDGE;
   x->wrapT=GL_CLAMP_TO_EDGE;
   x->image->Upload(glSetup,x->magFilter,x->minFilter,x->wrapS,x->wrapT);
   Append(x);
  }
  return x->image;
 }
 GLImage *Load( float *rgb_buffer, int w, int h, const char *name, TextureLibraryLoadTextureOptions opts=tllto_Default ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  LoadedTexture *old=findSettings(name);
  if ( old ) {
   OUTPUT("TextureLibrary::Load found an identically named RGB/RGBA buffer image, no action taken, request ignored!\n");
   return old->image;
  } else {
   LoadedTexture *x=new LoadedTexture; 
   x->image.Recycle(new GLImage(rgb_buffer,w,h,name,mag,min,wrapS,wrapT));
   x->filename=name;
   x->image->deleteAfterUpload=true;
   x->magFilter=GL_LINEAR;
   x->minFilter=GL_LINEAR_MIPMAP_LINEAR;
   x->wrapS=GL_CLAMP_TO_EDGE;
   x->wrapT=GL_CLAMP_TO_EDGE;
   Append(x);
   return x->image;
  }
 }
 GLImage *Load( having alpha, float *rgba_buffer, int w, int h, const char *name, TextureLibraryLoadTextureOptions opts=tllto_Default, bool genMips=false ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  LoadedTexture *old=findSettings(name);
  if ( old ) {
   OUTPUT("TextureLibrary::Load found an identically named RGB/RGBA buffer image, no action taken, request ignored!\n");
   return old->image;
  } else {
   LoadedTexture *x=new LoadedTexture; 
   x->image.Recycle(new GLImage(true,rgba_buffer,w,h,name,mag,min,wrapS,wrapT,!genMips));
   x->filename=name;
   x->image->deleteAfterUpload=true;
   x->magFilter=GL_LINEAR;
   x->minFilter=GL_LINEAR_MIPMAP_LINEAR;
   x->wrapS=GL_CLAMP_TO_EDGE;
   x->wrapT=GL_CLAMP_TO_EDGE;
   Append(x);
   return x->image;
  }
 }

 void Unload( GLImage *i ) {
  OUTPUT("Unloading GLImage(`%s`) and removing from library...",i->filename.c_str());
  Remove(i);
  OUTPUT("Image unload complete.\n");
 } 

 // Creates a new image in memory by using ProceduralImage
  // Loads, duplicating the settings of, an existing image and preprocesses it with the provided class.
 // Checks if the old process has been run already, and returns that one to avoid process duplication.
 GLImage *Generate( int w, int h, const char *filename, ProceduralImage *pi, TextureLibraryLoadTextureOptions opts, bool keep=false, bool save=false ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  GL_Assert_Param("GLImage::Generate: %s",filename);
  glReportError(glGetError());
  string keyword=string(filename)+pi->postfix;
  LoadedTexture *x=new LoadedTexture; 
  x->image=pi->Process( filename, w, h, save );
  x->filename=string(filename);
  x->magFilter=mag;
  x->minFilter=min;
  x->wrapS=wrapS;
  x->wrapT=wrapT;
  x->image->deleteAfterUpload=!keep;
  x->image->Upload(glSetup,x->magFilter,x->minFilter,x->wrapS,x->wrapT);
  Append(x);
  glReportError(glGetError());
  GL_Assert("Leaving GLImage::Generate");
  return x->image; 
 }

 // Finds by name (usually the filename/path provided when loaded) in the library.
 GLImage *find( const char *s ) {
  GLImage *result=search(s,GL_LINEAR,GL_LINEAR_MIPMAP_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
  if ( result ) return result;
  return Load(s);
 }
 // Finds by name and settings, (usually the filename/path provided when loaded) in the library.
 GLImage *find( const char *s, TextureLibraryLoadTextureOptions opts ) {
  GLint mag,min,wrapS,wrapT; GetLoadTextureOpts(opts,&mag,&min,&wrapS,&wrapT);
  GLImage *result=search(s,mag,min,wrapS,wrapT);
  if ( result ) return result;
  return Load(s);
 }
 // Finds by settings and name (usually the path provided when loaded) when multiple versions
 // of the same 2D texture have been established.
 GLImage *find( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT  ) {
  GLImage *result=search(s,magFilter,minFilter,wrapS,wrapT);
  if ( result ) return result;
  return Load(s,magFilter,minFilter,wrapS,wrapT);
 }
 // Finds by settings and name (usually the path provided when loaded) when multiple versions
 // of the same 2D texture have been established.
 GLImage *find( const char *s, bool mipmaps, GLint wrapS, GLint wrapT  ) {
  GLImage *result=search(s,GL_LINEAR,mipmaps?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR,wrapS,wrapT);
  if ( result ) return result;
  return Load(s,GL_LINEAR,mipmaps?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR,wrapS,wrapT);
 }
 // Finds by name (usually the filename/path provided when loaded) in the library.
 LoadedTexture *findSettings( GLImage *s ) {
  return searchSettings(s->filename.c_str());
 }
 // Finds by name (usually the filename/path provided when loaded) in the library.
 LoadedTexture *findSettings( const char *s ) {
  return searchSettings(s,GL_LINEAR,GL_LINEAR_MIPMAP_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
 }
 // Finds by settings and name (usually the path provided when loaded) when multiple versions
 // of the same 2D texture have been established.
 LoadedTexture *findSettings( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT  ) {
  return searchSettings(s,magFilter,minFilter,wrapS,wrapT);
 }
 // Finds by settings and name (usually the path provided when loaded) when multiple versions
 // of the same 2D texture have been established.
 LoadedTexture *findSettings( const char *s, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT, bool NoMipmaps  ) {
  return searchSettings(s,magFilter,minFilter,wrapS,wrapT,NoMipmaps);
 }
 // Finds by settings and name (usually the path provided when loaded) when multiple versions
 // of the same 2D texture have been established.
 LoadedTexture *findSettings( LoadedTexture *t ) {
  return searchSettings(t->filename.c_str(),t->magFilter,t->minFilter,t->wrapS,t->wrapT,t->assertedNoMipmaps);
 }
 // Returns a random 2D texture.
 GLImage *random() {
  LoadedTextures *lt=null;
  while ( !lt || lt->count == 0 ) lt=&table[upto(TL_TABLE_SIZE)];
  int x=upto(lt->count);
  LoadedTexture *i=(LoadedTexture *)lt->Element(x);
  return i->image;
 }
 // "Grabs" by adding an ImageHandle.
 ImageHandle *grab( const char *s ) {
  ImageHandle *ih=new ImageHandle;
  ih->i=find(s);
  return ih;
 }
};

extern TextureLibrary library;
 
#undef TL_TABLE_SIZE