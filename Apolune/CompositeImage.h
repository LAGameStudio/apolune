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
#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Cartesian.h"
#include "Art.h"
#include "Strings.h"
#include "TextureLibrary.h"

enum CompositeModes {
 _CompositeDefault=0,
 _CompositeMask=1,
 _CompositeOverlay=2,
 _CompositeOverlayMask=3
};

CompositeModes CompositeModeOptions( string k );

class CompositeImage : public ImageHandle {
public:
 CompositeModes mode;
 Cartesian position;
 Cartesian *offset;
 GLWindow *target;
 GLImage *mask;
 Blends blend;
 Crayon tint,masking;
 PROGRAMMABLE(CompositeImage,RenderPtr,render,Render);

 CompositeImage() : ImageHandle() {
  blend=additive;
  target=null;
  offset=null;
  render=&CompositeImage::RenderDefault;
  mode=_CompositeDefault;
  i=null;
  mask=null;
  tint.Pick(alabaster);
  masking.Pick(alabaster);
 }
 CompositeImage( GLImage *I ) : ImageHandle() {
  blend=additive;
  offset=null;
  target=null;
  render=&CompositeImage::Overlay;
  mode=_CompositeOverlay;
  i=I;
  mask=null;
  tint.Pick(alabaster);
  masking.Pick(alabaster);
 }
 CompositeImage( GLImage *o, Crayon t ) : ImageHandle() {
  blend=additive;
  offset=null;
  target=null;
  render=&CompositeImage::Overlay;
  mode=_CompositeOverlay;
  tint.fromCrayon(t);
  i=o;
  mask=null;
  masking.Pick(alabaster);
 }
 CompositeImage( GLImage *I, GLImage *M ) : ImageHandle() {
  blend=additive; 
  offset=null;
  target=null;
  render=&CompositeImage::Masked;
  mode=_CompositeMask;
  i=I;
  mask=M;
  tint.Pick(alabaster);
  masking.Pick(alabaster);
 }
 CompositeImage( GLImage *o, GLImage *M, Crayon t, Crayon m ) : ImageHandle() {
  blend=additive;
  offset=null;
  target=null;
  render=&CompositeImage::Masked;
  mode=_CompositeOverlayMask;
  i=o;
  mask=M;
  masking.fromCrayon(m);
  tint.fromCrayon(t);
 }
 void SetMode( CompositeModes m ) {
  mode=m; 
  switch (mode) {
       case _CompositeDefault: render=&CompositeImage::RenderDefault; break;
          case _CompositeMask: render=&CompositeImage::Masked;  break;
       case _CompositeOverlay: render=&CompositeImage::Overlay; break;
   case _CompositeOverlayMask: render=&CompositeImage::Masked;  break;
  }  
 }
 void BinaryWrite( BinaryFile *open ) {
  int temp;
  temp=(int)mode; open->write(&temp);
  position.BinaryWrite(open);
  if ( i ) {
   temp=1;
   open->write(&temp);
   string im;
   im=string(i->filename);
   open->scrawl(im);
  } else {
   temp=0;
   open->write(&temp);
  }
  if ( mask ) {
   temp=1;
   open->write(&temp);
   string im;
   im=string(mask->filename);
   open->scrawl(im);
  } else {
   temp=0;
   open->write(&temp);
  }
  temp=(int)blend; open->write(&temp);
  tint.BinaryWrite(open);
  masking.BinaryWrite(open);
 }
 void BinaryRead( BinaryFile *open ) {
  int temp;
  temp=(int)mode; open->read(&temp); mode=(CompositeModes)temp;
  position.BinaryRead(open);
  open->read(&temp);
  if ( temp == 1 ) {
   string im;
   open->read(&im);
   i=library.find((char *) im.c_str());
  }
  open->read(&temp);
  if ( temp == 1 ) {
   string im;
   open->read(&im);
   mask=library.find((char *) im.c_str());
  }
  open->read(&temp); blend=temp;
  tint.BinaryRead(open);
  masking.BinaryRead(open);
  if ( mode==_CompositeDefault ) {
   if ( mask ) { if ( i ) mode=_CompositeOverlayMask; else mode=_CompositeMask; }
   else if ( i ) { mode=_CompositeOverlay; }
  }
  SetMode(mode);
 }
 SKEYSTART
  SKEY("mode", { SKEYPAIR; mode=CompositeModeOptions(key); })
  SKEY("position", { SKEYPAIR; position.fromString((char *) key.c_str()); })
  // offset not saved, set by context
  // target not saved, set by context
  SKEY("image",   { SKEYPAIR; i=library.find((char *) key.c_str());
   if ( mode==_CompositeDefault ) { if ( mask ) mode=_CompositeOverlayMask; mode=_CompositeOverlay;}
  })
  SKEY("i",       { SKEYPAIR; i=library.find((char *) key.c_str());
   if ( mode==_CompositeDefault ) { if ( mask ) mode=_CompositeOverlayMask; mode=_CompositeOverlay;}
  })
  SKEY("mask",    { SKEYPAIR; mask=library.find((char *) key.c_str()); 
   if ( mode==_CompositeDefault ) { if ( i ) mode=_CompositeOverlayMask; else mode=_CompositeMask; }
  })
  SKEY("blend",   { SKEYPAIR; blend=key; })
  SKEY("tint",    { SKEYPAIR; tint.fromString((char *) key.c_str()); })
  SKEY("masking", { SKEYPAIR; masking.fromString((char *) key.c_str()); })
 SKEYENDING("CompositeImage")
  SetMode(mode);
 SKEYENDER
 string toString() {
  string out=string("");
  char buf[80];
  incdent();
  FORMAT(buf,80,"mode %d\n",(int) mode);
  out+=indention+string(buf);
  if ( !masking.Equals(0.0f,0.0f,0.0f,0.0f) ) out+=indention+string("masking {")+masking.toString()+string("}\n");
  if ( !tint.Equals(0.0f,0.0f,0.0f,0.0f) )    out+=indention+string("tint {")+tint.toString()+string("}\n");
  if ( i ) {
   out+=indention+string("image {")+string(i->filename)+string("}\n");
  }
  if ( mask ) {
   out+=indention+string("mask {")+string(mask->filename)+string("}\n");
  }
  out+=indention+string("blend {")+blend.toString()+string("}\n");
  decdent();
  return out;
 }
 void RenderTo( GLWindow *T ) { target=T; }
 void Overlay( GLImage *o ) { render=&CompositeImage::Overlay; mode=_CompositeOverlay; i=o; }
 void Overlay( GLImage *o, Crayon t ) { render=&CompositeImage::Overlay; mode=_CompositeOverlay; t.fromCrayon(t); i=o; }
 void Mask( GLImage *M ) { render=&CompositeImage::Overlay; mask=M; mode=_CompositeOverlay; masking.Pick(alabaster); }
 void OverlayMasked( GLImage *o, GLImage *M ) { render=&CompositeImage::Masked; mode=_CompositeOverlayMask; i=o; mask=M; masking.Pick(alabaster); }
 void OverlayMasked( GLImage *o, GLImage *M, Crayon t, Crayon m ) { render=&CompositeImage::Masked; mode=_CompositeOverlayMask; i=o; mask=M; masking.fromCrayon(m); tint.fromCrayon(t);  }
 void Masked() {
  Blending(mask1);
  DrawImage(mask,masking,blend,position.x,position.y,position.w,position.h);
  Blending(blend);
  DrawImage(i,tint,blend,position.x,position.y,position.w,position.h);
 }
 void Overlay() {
  Blending(blend);
  DrawImage(i,tint,blend,position.x,position.y,position.w,position.h);
 }
 void RenderDefault() {
/*  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask,masking,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
  } else {*/
   Blending(blend);
   FBOTexturedQuad(i,tint,blend,position.x,position.y,position.w,position.h);
/*  }*/
 }
 void RenderIn( GLWindow *surface ) {
/*  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask,masking,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
  } else {*/
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
/*  }*/
 }
 void RenderAt( GLWindow *surface ) {
/*  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask,masking,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,surface->x+position.x,surface->y+position.y,position.w,position.h);
  } else {*/
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,position.x,position.y,position.w,position.h);
/*  }*/
 }
 void RenderOfs( GLWindow *surface, int x, int y, int w, int h ) {
  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask,masking,blend,x+position.x,y+position.y,w,h);
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,x+position.x,y+position.y,w,h);
  } else {
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,x+position.x,y+position.y,w,h);
  }
 }
 void RenderOn( GLWindow *surface, int x, int y, int w, int h ) {
  if ( mask ) {
   Blending(mask1);
   StretchiNoAlpha(surface,mask,masking,blend,x,y,w,h);
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,x,y,w,h);
  } else {
   Blending(blend);
   StretchiNoAlpha(surface,i,tint,blend,x,y,w,h);
  }
 }
 void BlendOn( GLWindow *surface, int x, int y, int w, int h ) {
  Blending(blend);
  StretchiNoAlpha(surface,i,tint,blend,x,y,w,h);
 }
 void BlendOn( GLWindow *surface, double x, double y, int w, int h ) {
  Blending(blend);
  StretchdNoAlpha(surface,i,tint,blend,x,y,w,h);
 }
 void DrawImage( GLImage * source, Crayon tint, Blends blend, int x, int y, int w, int h ) {
  int OX=(int) target->x;
  int OY=(int) target->y;

#if defined(GL_DEBUG)
  GL_Assert( "[entering CompoundImageHandle.StretchiNoAlpha()]" );
  glReportError( glGetError() );
  GL_Assert( "CompoundImageHandle::StretchiNoAlpha()" );
#endif
 
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, source->texture);

  //printf( "%f,%f,%f,%f\n", t[0], t[1], t[2], t[3] );
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);
  if(h < 0)
  {
   glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+OX,   y+OY);
   glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w+OX, y+OY);
   glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+OX, y-h+OY);
   glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+OX,   y-h+OY);
  }
  else
  {
   glTexCoord2f(0.0f, 0.0f);  glVertex2i(x+OX,   y+OY);
   glTexCoord2f(1.0f, 0.0f);  glVertex2i(x+w+OX, y+OY);
   glTexCoord2f(1.0f, 1.0f);  glVertex2i(x+w+OX, y+h+OY);
   glTexCoord2f(0.0f, 1.0f);  glVertex2i(x+OX,   y+h+OY);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();

#if defined(GL_DEBUG)
  GL_Assert( "[exiting StretchiNoAlpha()]" );
  glReportError( glGetError() );
#endif
 }
 void Duplicate( CompositeImage *d ) {
  position.Duplicate(&(d->position));
  offset=d->offset;
  target=d->target;
  mask=d->mask;
  blend=d->blend;
  tint.fromCrayon(d->tint);
  masking.fromCrayon(d->masking);
  render=d->render;
 }
};

class CompositeImages : public LinkedList {
public:
 void Add( GLImage *i, int x, int y ) {
  CompositeImage *c=new CompositeImage;
  c->i=i;
  c->blend=transparency;
  c->position.SetRect(x,y,i->width,i->height);
  Append(c);
 }
 void Add( GLImage *i, int x, int y, int w, int h ) {
  CompositeImage *c=new CompositeImage;
  c->i=i;
  c->blend=transparency;
  c->position.SetRect(x,y,w,h);
  Append(c);
 }
 void MoveBy( int x, int y ) { FOREACH(CompositeImage,c) { c->position.x+=x; c->position.y+=y; } }
 void RenderTo( GLWindow *surface ) { FOREACH(CompositeImage,c) c->RenderTo(surface); }
 void RenderIn( GLWindow *surface ) { FOREACH(CompositeImage,c) c->RenderIn(surface); }
 void RenderAt( GLWindow *surface ) { FOREACH(CompositeImage,c) c->RenderAt(surface); }
 void Render() { FOREACH(CompositeImage,c) c->Render(); }
 CLEARLISTRESET(CompositeImage);
 ~CompositeImages() { Clear(); }
};