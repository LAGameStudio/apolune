/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "macros.h"
#include "moremath.h"
#include "GLSetup.h"
#include "GLScissor.h"
#include "Crayon.h"
#include "Cartesian.h"
#include "ScreenPosition.h"

class Line;
class Vertexd;
class CPUImage;
class GLImage;
class Quad;
class GLSLShader;

/* To-do: This FBO setup is not thread-safe. */
#include "pThread.h"

extern int fbo_pixel_count;

struct DepthStencilBuffer {
 GLuint depth;
 GLfloat depthf;
 GLubyte stencil;
};

enum FBOTargets {
 gl_color0=GL_COLOR_ATTACHMENT0,
 gl_depth=GL_DEPTH_ATTACHMENT,
 gl_stencil=GL_STENCIL_ATTACHMENT,
 gl_depthstencil=GL_DEPTH_STENCIL_ATTACHMENT
};

enum FBOFormats {
 gl_color_index=GL_COLOR_INDEX,
 gl_stencil_index=GL_STENCIL_INDEX,
 gl_depth_component=GL_DEPTH_COMPONENT,
 gl_red=GL_RED,
 gl_green=GL_GREEN,
 gl_blue=GL_BLUE,
 gl_alpha=GL_ALPHA,
 gl_rgb=GL_RGB,
 gl_bgr=GL_BGR,
 gl_rgba=GL_RGBA,
 gl_bgra=GL_BGRA,
 gl_luminance=GL_LUMINANCE,
 gl_luminance_alpha=GL_LUMINANCE_ALPHA
};

#define DEPTH_FAR 16777215
#define DEPTH_FARf 16777215.0f
#define DEPTH_FARd 16777215.0

class FBO;
class FBOColor;
class FBOColorDepth;
class FBOColorDepthStencil;
class FBOBound : public ListItem {
public:
 bool *wasStenciling;
 FBO *bound;
 FBOBound( FBO *f );
 FBOBound( FBOColorDepthStencil *f );
};
class FBOBindings : public LinkedList {
public:
 void Push( FBO *f );
 void Push( FBO *f, bool clear );
 void Push( FBOColorDepthStencil *f );
 void Push( FBOColorDepthStencil *f, bool clear );
 void Pop();
 CLEARLISTRESET(FBOBound);
};

extern FBOBindings fboBindingStack;

class FBO {
public:
 GLuint id,texture,dsTexture;
 Blends blend;
 Crayon tint;
 bool created,error,multisample;
 int w,h,w2,h2,samples;
 GLenum status;
 GLint oldDrawBufferMode;
 bool texturesNotRBOs;
 GLenum s,t;
 GLfloat min,mag; // settings for color attachment
 bool depth, stencil, pixelate;
 GLuint rboDepth,rboDepthStencil;
 
 GLubyte ub_bgra[4];
 GLubyte r_stencil;
 GLuint r_depth;
 GLfloat f_depth;
 
 FBO(void) {
  w=h=512;
  w2=h2=256;
  created=false;
  error=false;
  multisample=(gl.ARB_FBO_multisample || gl.ARB_FBO_blit);
  id=0;
  texture=dsTexture=0;
  blend=none;
  tint.x_White();
  samples=0;
  depth=stencil=true;
  status=0;
  texturesNotRBOs=true;
  s=GL_REPEAT;
  t=GL_REPEAT;
  min=GL_LINEAR_MIPMAP_LINEAR;
  mag=GL_LINEAR;
  rboDepth=rboDepthStencil=0;
  ub_bgra[0]=0;
  ub_bgra[1]=1;
  ub_bgra[2]=2;
  ub_bgra[3]=3;
  r_stencil=0;
  r_depth=0;
  f_depth=0.0f;
  pixelate=false;
 }

 CPUImage *Snapshot();
 CPUImage *Snapshot( int x, int y, int w, int h );
 void Save( const char *pngfile );

 void Blit( FBO *from );
 void BlitStretch( FBO *from, double Xscale=1.0, double Yscale=1.0) {  
  this->Bind();
  from->RenderUpsideDown(transparency,0.0,0.0,(double)this->w*Xscale,(double)this->h*Yscale);
  this->Unbind();
 }
 void BlitStretch( FBO *from, double X, double Y, double W, double H) {  
  this->Bind();
  from->RenderUpsideDown(transparency,X,Y,X+W,Y+H);
  this->Unbind();
 }

 // Legacy, don't use.
 void Blending( const Blends& blend ) { Blending(blend.blend); }
 void Blending( Blendings blend ) {
  glEnable(GL_BLEND);
  switch ( blend ) {
    case _solarize: glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO); break;
        case _mask: glBlendFunc(GL_DST_COLOR,GL_ZERO); break;
    case _screened: glBlendFunc(GL_ONE,GL_ONE); break;
    case _additive: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
 case _subtractive: glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE); break;
    case _multiply: glBlendFunc(GL_ZERO, GL_SRC_COLOR); break; // GL_ONE, GL_SRC_COLOR
     case _overlay: glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;
case _transparency: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
    case _saturate: glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); break;
       case _mask1: glBlendFunc(GL_DST_COLOR,GL_ONE); break;
        case _none: glBlendFunc(GL_ONE,GL_ZERO); break;
case _translucency: glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR); break;
          default: break;
  }
 }

 void Attach() {
  glGenTextures(1, &texture);
  glBindTexture(gl.arbMultisampleSupported ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, pixelate ? GL_FALSE : GL_TRUE); // automatic mipmap

  if ( gl.arbMultisampleSupported ) glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_RGBA8, w, h, false);
  else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); // GL_TEXTURE_2D_MULTISAMPLE?
  if ( texturesNotRBOs ) {
   if ( depth && stencil ) {
    glGenTextures(1, &dsTexture);
    glBindTexture(GL_TEXTURE_2D, dsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
    if ( pixelate ) {
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
//    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, dsTexture, 0);
   } else if ( depth ) {
    glGenTextures(1, &dsTexture);
    glBindTexture(GL_TEXTURE_2D, dsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
    if ( pixelate ) {
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dsTexture, 0);
   }
  } else {
   if ( depth && stencil ) {
    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
   } else if ( depth ) {
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
   }
  }
 }

 void Detach() {
  if ( texturesNotRBOs ) {
   if ( depth && stencil ) {
   } else if ( depth ) {
   }
  } else {
   if ( depth && stencil ) {
   } else if ( depth ) {
   }
  }
 }

 // Determines the maximum possible dimension of a square fBO
 int GetMaxDimension() {
  int f=display.h;
  if ( display.w<display.h ) f=display.w;
  if ( f > 1024 && f < 2048 ) return 1024;
  if ( f > 512 && f < 1024 ) return 512;
  if ( f > 256 && f < 512 ) return 256;
  if ( f > 128 && f < 256 ) return 128;
  return 64;
 }

 // Finds the maximum possible dimension of the supported fBO format
 // May be rectangular if supported.
 void GetMaxDimensions( int *x, int *y ) {
  if ( (gl.EXT_FBO_rect && gl.EXT_FBO_npot) || (gl.ARB_FBO_npot && gl.ARB_FBO_rect) ) {
   *x=display.w;
   *y=display.h;
   return;
  }
  *x=*y=GetMaxDimension();
 }

 void Dispose() {
  Release();
  created=false;
 }

 void Create() {
#if defined(USE_PTHREADS)
  pthreads.Lock();
#endif
  if ( created ) Dispose();
  glGenFramebuffers(1, &id);
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  Attach();
  glReportError( glGetError() );
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  glReportError( glGetError() );
  if ( status != GL_FRAMEBUFFER_COMPLETE) OUTPUT("Error: GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");
  else OUTPUT("FBO:Create() says GL_FRAMEBUFFER_COMPLETE OK, using FBO id=%d\n",id);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  created=true;
  fbo_pixel_count+=w*h;
  GL_Report("Framebuffer::Create() Post" );
  if ( !GetInfoAndStatus() ) {
   created=false;
   error=true;
   glReportError( glGetError() );
  }
#if defined(USE_PTHREADS)
  pthreads.Unlock();
#endif
 }

 void Create( int ww, int hh ) {
  w=ww;
  h=hh;
  w2=ww/2;
  h2=hh/2;
  Create();
 }
 void Create( int ww, int hh, bool depth, bool stencil ) {
  w=ww;
  h=hh;
  w2=ww/2;
  h2=hh/2;
  this->depth=depth;
  this->stencil=stencil;
  Create();
 }

 // Provide fbo->color, par example
 virtual void Bind( bool clear=true ) {
  fboBindingStack.Push(this,clear);
 }
 virtual void BindWithoutClearing() {
  fboBindingStack.Push(this,false);
 }
 virtual void Activate() {
  glGetIntegerv(GL_DRAW_BUFFER,&oldDrawBufferMode);
	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
 }
 virtual void ActivateWithoutClearing() {
  glGetIntegerv(GL_DRAW_BUFFER,&oldDrawBufferMode);
	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
 }
 /*
 After this point you may bind the ColorTexture as source for drawing into the visible framebuffer,
 but be aware that it is still attached as target to the created FBO. That is only a problem if
 the FBO is bound again and the texture is used at the same time for being a FBO attachment target
 and the source of a texturing operation. This will cause feedback effects and is most likely not
 what you intended.
 */
 virtual void Unbind() {
  fboBindingStack.Pop();
 }
 virtual void Deactivate() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL_Assert("Framebuffer::Deactivate() glBindFramebuffer" );
  glReportError( glGetError() );
  glDrawBuffer( (GLenum) oldDrawBufferMode );
  glViewport(0,0,display.w,display.h); // Is this necessary?
 }

 void SetRenderTarget( FBOTargets target ) {
  glDrawBuffer( (GLenum) target );
 }

 // ub_bgra is populated - If the number returned is ~"30,30,30,255" this means you have forgotten to bind before reading.
 void ReadPixel( FBOTargets type, int x, int y ) {
  GL_Assert("FBO:ReadPixel");
  switch ( type ) {
   case gl_color0:  glReadPixels(x,y,1,1,GL_BGRA,GL_UNSIGNED_BYTE,ub_bgra);
    break;
   case gl_stencil: glReadPixels(x,y,1,1,GL_STENCIL_INDEX,GL_UNSIGNED_BYTE,ub_bgra);
    break;
   case gl_depth:   glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE, ub_bgra);
    break;
   case gl_depthstencil: 
    {
     GLuint g;
     glReadPixels(x,y,1,1, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, &g);   //GL 3.0
     r_stencil = (byte) (g & 0x000000FF);
     r_depth = (unsigned int) (g >> 8);
     f_depth = (float) r_depth / DEPTH_FARf;
    }
    break;
  }
  glReportError( glGetError() );
 }
 
 // Test N points along the line (one method, there are others..)
 // Segment must be FBO-translated from screen space.  Output point must be re-translated back to screen space.
 bool Strike( FBOTargets type, Line *segment, Vertexd *out, double drawW, double drawH, int precision );

 DepthStencilBuffer *ReadDepthStencilRegion( int x, int y, int read_w, int read_h ) {
  DepthStencilBuffer *data=new DepthStencilBuffer[read_w*read_h];
  Indexed<GLuint> g;
  g.Size(read_w*read_h);
  glReadPixels(x,y,read_w,read_h,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,g.list);   //GL 3.0
  for ( int i=0; i<read_w*read_h; i++ ) {
   data[i].stencil = (GLubyte) (g.list[i] & 0x000000FF);
   data[i].depth = (GLuint) (g.list[i] >> 8);
   data[i].depthf = (GLfloat) data[i].depth / DEPTH_FARf;
  }
  return data;
 }

 GLubyte *ReadPixels( FBOTargets target, int x, int y, int read_w, int read_h ) {
  GLubyte *data=new GLubyte[read_w*read_h*4];
  GL_Assert("FBO:ReadPixel");
  switch ( target ) {
  default: OUTPUT("FBO:ReadPixels called on target, use ReadDepthStencilRegion instead.\n"); break;
   case gl_color0:  glReadPixels(x,y,read_w,read_h,GL_BGRA,GL_UNSIGNED_BYTE,data);
    break;
   case gl_stencil: glReadPixels(x,y,read_w,read_h,GL_STENCIL_INDEX,GL_UNSIGNED_BYTE,data);
    break;
   case gl_depth:   glReadPixels(x,y,read_w,read_h,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE, data);
    break;
  }
  GL_Assert("FBO:ReadPixel");
  glReportError( glGetError() );
  return data;
 } 

 GLuint *TextureIDPtr() {
  return &texture;
 }
 GLuint TextureID() {
  return texture;
 }

 void ActiveTexture( GLuint t) {
  glActiveTexture(t);
  glBindTexture(GL_TEXTURE_2D, texture);
 }

 void RenderCenteredRotated( float x, float y, float px, float py, float h2, float w2, double a ) {
  GL_Assert("Framebuffer::RenderCenteredRotated() Pre" );
  glReportError( glGetError() );
  glPushAttrib(GL_COLOR_BUFFER_BIT);
  Blending(blend);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glMatrixMode(GL_MODELVIEW); //assumed
  glPushMatrix();
  glTranslatef(x,y,0.0f);
  glRotated(a,0.0f,0.0f,1.0f);
  glColor4fv(tint.floats);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);  glVertex2f( w2+px, -h2+py);
  glTexCoord2f(1.0f, 1.0f);  glVertex2f(-w2+px, -h2+py);
  glTexCoord2f(1.0f, 0.0f);  glVertex2f(-w2+px,  h2+py);
  glTexCoord2f(0.0f, 0.0f);  glVertex2f( w2+px,  h2+py);
  glEnd();
  glPopMatrix();
  glPopAttrib();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }
 
 void Render( Zint x, Zint y, Zint x2, Zint y2 ) { Render((int)x,(int)y,(int)x2,(int)y2); }
 void Render( int x, int y, int x2, int y2 );
  
 void RenderSection( float xStart, float xEnd, float yStart, float yEnd, Blends b, Zint x, Zint y, Zint x2, Zint y2 ) { RenderSection(xStart,xEnd,yStart,yEnd,b,(int)x,(int)y,(int)x2,(int)y2); }
 void RenderSection( float xStart, float xEnd, float yStart, float yEnd, Blends b, int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::RenderSection() Pre" );
  glReportError( glGetError() );
  Blending(b);
  glColor4fv(tint.floats);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(xStart,yEnd);   glVertex2i(x,y);
  glTexCoord2f(xEnd,yEnd);     glVertex2i(x2,y);
  glTexCoord2f(xEnd,yStart);   glVertex2i(x2,y2);
  glTexCoord2f(xStart,yStart); glVertex2i(x,y2);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  GL_Assert("Framebuffer::RenderSection() Post" );
  glReportError( glGetError() );
 }

 void RenderReversed( Zint x, Zint y, Zint x2, Zint y2 ) { RenderReversed((int)x,(int)y,(int)x2,(int)y2); }
 void RenderReversed( int x, int y, int x2, int y2 );

 void RenderUpsideDown( Zint x, Zint y, Zint x2, Zint y2 ) { RenderUpsideDown((int)x,(int)y,(int)x2,(int)y2); }
 void RenderUpsideDown( int x, int y, int x2, int y2 );

 void RenderUpsideDown( Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { RenderUpsideDown((double)x,(double)y,(double)x2,(double)y2); }
 void RenderUpsideDown( double x, double y, double x2, double y2 );

 void RenderUpsideDown( Crayon t, Zint x, Zint y, Zint x2, Zint y2 ) { RenderUpsideDown(t,(int)x,(int)y,(int)x2,(int)y2); }
 void RenderUpsideDown( Crayon t, int x, int y, int x2, int y2 );

 void RenderUpsideDown( Crayon t, double x, double y, double x2, double y2 );

 void RenderUpsideDown( Blends b, Zint x, Zint y, Zint x2, Zint y2 ) { RenderUpsideDown(b,(int)x,(int)y,(int)x2,(int)y2); }
 void RenderUpsideDown( Blends b, int x, int y, int x2, int y2 );
 void RenderUpsideDown( Blends b, double x, double y, double x2, double y2 );

 void RenderUpsideDown( Blends b, Crayon t, Zint x, Zint y, Zint x2, Zint y2 ) { RenderUpsideDown(b,t,(int)x,(int)y,(int)x2,(int)y2); }
 void RenderUpsideDown( Blends b, Crayon t, int x, int y, int x2, int y2 );
 void RenderUpsideDownReversed( Zint x, Zint y, Zint x2, Zint y2 ) { RenderUpsideDownReversed((int)x,(int)y,(int)x2,(int)y2); }
 void RenderUpsideDownReversed( int x, int y, int x2, int y2 );

 void RenderFullScreen();
 void Render( Cartesian *extents );
 
 void Render( ScreenPosition *extents );

 void Render( Cartesiand *extents );

 void Renderf( Zfloat x, Zfloat y, Zfloat x2, Zfloat y2 ) { Renderf((float)x,(float)y,(float)x2,(float)y2); }
 void Renderf( float x, float y, float x2, float y2 );
 
 void Render( Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { Render((double)x,(double)y,(double)x2,(double)y2); }
 void Render( double x, double y, double x2, double y2 );
 void RenderQuad( Quad *q );
 void RenderQuadUpsideDown( Quad *q );

 // Rotates at angle a, blended textured rectangle around x,y that is h long and w2*2 wide
 void RenderRotatedRect(double x, double y, double h, double w2, double a ) {
  GL_Assert("Framebuffer::RenderRotatedRect() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 

  glPushMatrix();
  glTranslated(x,y,0.0);
  glRotated(a,0.0,0.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2i(0,1);  glVertex2d(w2, 0.0); 
  glTexCoord2i(1,1);  glVertex2d(-w2, 0.0);
  glTexCoord2i(1,0);  glVertex2d(-w2, h);
  glTexCoord2i(0,0);  glVertex2d(w2, h); 
  glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
 
 }
 
 void RenderRotatedRect(Crayon t, double x, double y, double h, double w2, double a ) {
  GL_Assert("Framebuffer::RenderRotatedRect() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(t.floats); 

  glPushMatrix();
  glTranslated(x,y,0.0);
  glRotated(a,0.0,0.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2i(0,1);  glVertex2d(w2, 0.0); 
  glTexCoord2i(1,1);  glVertex2d(-w2, 0.0);
  glTexCoord2i(1,0);  glVertex2d(-w2, h);
  glTexCoord2i(0,0);  glVertex2d(w2, h); 
  glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
 
 }
 void RenderLineRect(double x1, double y1, double x2, double y2, double w2 ) {
  double d=ddistance(x1,y1,x2,y2);
  double X=x2-x1;
  double Y=y2-y1;
  if ( X==Y && X == 0 ) return;
  double a=atan2(Y,X)*180.0/PI+270.0;

  GL_Assert("Framebuffer::RenderLineRect() Pre" ); 		
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 

  glPushMatrix();
  glTranslated(x1,y1,0.0);
  glRotated(a,0.0,0.0,1.0); 
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);  glVertex2d(w2, 0.0); 
  glTexCoord2d(1.0, 1.0);  glVertex2d(-w2, 0.0);
  glTexCoord2d(1.0, 0.0);  glVertex2d(-w2, d);   
  glTexCoord2d(0.0, 0.0);  glVertex2d(w2, d);    
  glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D); 
 }

 // Used for isometric tile effects
 void Swab( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1);  glVertex2d(x1, y1);
  glTexCoord2i(1, 1);  glVertex2d(x2, y2);
  glTexCoord2i(1, 0);  glVertex2d(x3, y3);
  glTexCoord2i(0, 0);  glVertex2d(x4, y4);
  glEnd();
  glDisable(GL_TEXTURE_2D);
 }
 
 // Used for isometric tile effects
 void SwabYInvert( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 ) {
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0);  glVertex2d(x1, y1);
  glTexCoord2i(1, 0);  glVertex2d(x2, y2);
  glTexCoord2i(1, 1);  glVertex2d(x3, y3);
  glTexCoord2i(0, 1);  glVertex2d(x4, y4);
  glEnd();
  glDisable(GL_TEXTURE_2D);
 }
 
 // Pivots a rectangle around axis of rotation px,py at position x,y 
 // rotated by angle a, h2*2 by w2*2 are rectangular dimensions
 void RenderPivotedRotatedRect( double x, double y, double px, double py, double h2, double w2, double a ) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 

  glPushMatrix();
  glTranslated(x,y,0.0);
  glRotated(a,0.0,0.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);  glVertex2d( w2+px, -h2+py); 
  glTexCoord2d(1.0, 1.0);  glVertex2d(-w2+px, -h2+py);
  glTexCoord2d(1.0, 0.0);  glVertex2d(-w2+px,  h2+py);   
  glTexCoord2d(0.0, 0.0);  glVertex2d( w2+px,  h2+py);    
  glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
 }
 
 
 // Can't use quad due to perspective correction, so we use a series of 8 interlocking triangles of points,
 //     D ____h___ C
 //      \        /
 //      f+  e.  +g
 //        \____/
 //        A  i  B
 //glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);? no
 //This is the best I can do, it looks good if you use the swab.png and wanted to do a lamp/lightsource.
 void RenderRotatedIsotrap( double x, double y, double h, double w1, double w2, double a ) {
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 

  glPushMatrix();
  glTranslated(x,y,0.0);
  glRotated(a,0.0,0.0,1.0);
 
  // Interlocking triangle method
  double 
   Ax=-w1,              Ay=0.0f,
   Bx=w1,               By=0.0f,
   Cx=w2,               Cy=h,
   Dx=-w2,              Dy=h,
   ex=0.0f,             ey=h/2.0f,
   gx(w1+(w2-w1)/2),    gy=ey,
   fx=-gx,              fy=ey,
   hx=0.0f,             hy=h,
   ix=0.0f,             iy=0.0f;
 
  // combined 1+2
  glBegin(GL_TRIANGLES);
  glTexCoord2d(1.0,1.0); glVertex2d(Ax,Ay);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glTexCoord2d(0.0,1.0); glVertex2d(Bx,By);
  glEnd();
 
  //3
  glBegin(GL_TRIANGLES);
  glTexCoord2d(0.0,1.0); glVertex2d(Bx,By);
  glTexCoord2d(0.0,0.5); glVertex2d(gx,gy);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glEnd();
 
  //4
  glBegin(GL_TRIANGLES);
  glTexCoord2d(0.0,0.5); glVertex2d(gx,gy);
  glTexCoord2d(0.0,0.0); glVertex2d(Cx,Cy);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glEnd();
 
  //5
  glBegin(GL_TRIANGLES);
  glTexCoord2d(0.0,0.0); glVertex2d(Cx,Cy);
  glTexCoord2d(0.5,0.0); glVertex2d(hx,hy);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glEnd();
 
  //6
  glBegin(GL_TRIANGLES);
  glTexCoord2d(0.5,0.0); glVertex2d(hx,hy);
  glTexCoord2d(1.0,0.0); glVertex2d(Dx,Dy);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glEnd();
 
  //7
  glBegin(GL_TRIANGLES);
  glTexCoord2d(1.0,0.5); glVertex2d(fx,fy);
  glTexCoord2d(1.0,1.0); glVertex2d(Ax,Ay);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glEnd();
 
  //8
  glBegin(GL_TRIANGLES);
  glTexCoord2d(0.5,0.5); glVertex2d(ex,ey);
  glTexCoord2d(1.0,0.5); glVertex2d(fx,fy);
  glTexCoord2d(1.0,0.0); glVertex2d(Dx,Dy);
  glEnd();
  
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
 }
 
 void RenderRotatedTri( double x, double y, double h, double w2, double a ) {
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  Blending(blend);
  glColor4fv(tint.floats); 

  glPushMatrix();
  glTranslated(x,y,0.0);
  glRotated(a,0.0,0.0,1.0); 
  glBegin(GL_TRIANGLES);
  glTexCoord2i(1,1); glVertex2d(-w2,h);
  glTexCoord2i(0,0); glVertex2d(0,0);
  glTexCoord2i(0,1); glVertex2d(w2,h);
  glEnd();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
 }


 void RenderfUpsideDown( Zfloat x, Zfloat y, Zfloat x2, Zfloat y2 ) { RenderfUpsideDown((float)x,(float)y,(float)x2,(float)y2); }
 void RenderfUpsideDown( float x, float y, float x2, float y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,1.0f);     glVertex2f(x,y);
  glTexCoord2f(1.0f,1.0f);     glVertex2f(x2,y);
  glTexCoord2f(1.0f,0.0f);     glVertex2f(x2,y2);
  glTexCoord2f(0.0f,0.0f);     glVertex2f(x,y2);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }
 
 void RenderfUpsideDownReversed( Zfloat x, Zfloat y, Zfloat x2, Zfloat y2 ) { RenderfUpsideDownReversed((float)x,(float)y,(float)x2,(float)y2); }
 void RenderfUpsideDownReversed( float x, float y, float x2, float y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f,1.0f);     glVertex2f(x,y);
  glTexCoord2f(0.0f,1.0f);     glVertex2f(x2,y);
  glTexCoord2f(0.0f,0.0f);     glVertex2f(x2,y2);
  glTexCoord2f(1.0f,0.0f);     glVertex2f(x,y2);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }
 
 void Renderf( ScreenPosition *extents ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,0.0f);     glVertex2f(extents->fx, extents->fy);
  glTexCoord2f(1.0f,0.0f);     glVertex2f(extents->fx2,extents->fy);
  glTexCoord2f(1.0f,1.0f);     glVertex2f(extents->fx2,extents->fy2);
  glTexCoord2f(0.0f,1.0f);     glVertex2f(extents->fx, extents->fy2);
  glEnd();   
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }

 void Renderf( Crayon c, Blends b, Zfloat x, Zfloat y, Zfloat x2, Zfloat y2 ) { Renderf(c,b,(float)x,(float)y,(float)x2,(float)y2); }
 void Renderf( Crayon c, Blends b, float x, float y, float x2, float y2 ) {
  Crayon old; old.fromCrayon(tint);
  tint.fromCrayon(c);
  Blends cache=blend;
  blend=b;
  Renderf(x,y,x2,y2);
  blend=cache;
  tint.fromCrayon(old);
 }

 void Renderf( Blends b, Zfloat x, Zfloat y, Zfloat x2, Zfloat y2 ) { Renderf(b,(float)x,(float)y,(float)x2,(float)y2); }
 void Renderf( Blends b, float x, float y, float x2, float y2 ) {
  Blends cache=blend;
  blend=b;
  Renderf(x,y,x2,y2);
  blend=cache;
 }
 
 void Renderd( ScreenPosition *extents ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,0.0f);     glVertex2d(extents->dx, extents->dy);
  glTexCoord2f(1.0f,0.0f);     glVertex2d(extents->dx2,extents->dy);
  glTexCoord2f(1.0f,1.0f);     glVertex2d(extents->dx2,extents->dy2);
  glTexCoord2f(0.0f,1.0f);     glVertex2d(extents->dx, extents->dy2);
  glEnd();   
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }
 
 void Render( Crayon c, Blends b, Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { Render(c,b,(double)x,(double)y,(double)x2,(double)y2); }
 void Render( Crayon c, Blends b, double x, double y, double x2, double y2 ) {
  Crayon old; old.fromCrayon(tint);
  tint.fromCrayon(c);
  Blends cache=blend;
  blend=b;
  Render(x,y,x2,y2);
  blend=cache;
  tint.fromCrayon(old);
 }

 void Render( Blends b, Zdouble x, Zdouble y, Zdouble x2, Zdouble y2 ) { Render(b,(double)x,(double)y,(double)x2,(double)y2); }
 void Render( Blends b, double x, double y, double x2, double y2 ) {
  Blends cache=blend;
  blend=b;
  Render(x,y,x2,y2);
  blend=cache;
 }

 void Render( Blends b, int x, int y, Zint x2, Zint y2 ) { Render(b,x,y,(int)x2,(int)y2); }
 void Render( Blends b, Zint x, Zint y, Zint x2, Zint y2 ) { Render(b,(int)x,(int)y,(int)x2,(int)y2); }
 void Render( Blends b, int x, int y, int x2, int y2 ) {
  Blends cache=blend;
  blend=b;
  Render(x,y,x2,y2);
  blend=cache;
 }

 // Used by Multisampling
 void CopyFrameBufferToTexture() {
  glBindTexture(GL_TEXTURE_2D, texture);
  glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,w,h);
 }

 // utility
 ///////////////////////////////////////////////////////////////////////////////
 // check FBO completeness, print status info
 ///////////////////////////////////////////////////////////////////////////////
 bool GetInfoAndStatus()
 {
  bool result=false;
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(status) {
                       case GL_FRAMEBUFFER_COMPLETE_EXT: OUTPUT("FBO is `complete`.\n" ); return true;
          case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: OUTPUT("FBO incomplete: Attachment is `incomplete`.\n" ); return false;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: OUTPUT("FBO incomplete: No image is attached to FBO.\n" ); return false;
          case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: OUTPUT("FBO incomplete: Attached images have different dimensions.\n" ); return false;
             case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: OUTPUT("FBO incomplete: Color attached images have different internal formats.\n" ); return false;
         case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: OUTPUT("FBO incomplete: Draw buffer.\n" ); return false;
         case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: OUTPUT("FBO incomplete: Read buffer.\n" ); return false;
                    case GL_FRAMEBUFFER_UNSUPPORTED_EXT: OUTPUT("FBO reports an error of `unsupported` by FBO implementation.\n"); return false;
                                                default: OUTPUT("[ERROR] Unknown error\n"); return false;
  }
 }

 ///////////////////////////////////////////////////////////////////////////////
 // set camera position and lookat direction
 ///////////////////////////////////////////////////////////////////////////////
 void Camera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
 }

 /*
  * When an FBO is larger than the front or back buffers (screen size), you must
  * use a special scissor test to properly anchor the screen.  The Y value becomes
  * negative.  This may impact other gl* (Ortho,Viewport,Scissor,Matrixf) calls
  * CORRECTION: You cannot use an FBO larger than the Framebuffer, which is display sized.
  * Also, fBOs do not seem to have the logical clipping coordinates.
  */
 void OrthoThis2d() {
  GL_Report("Framebuffer::RenderTestSceneOnFBO() Pre" );
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(GLdouble) w,(GLdouble) h,0,0.0,1.0);
  GL_Report("Framebuffer::RenderTestSceneOnFBO() OrthoThis" );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 }

 // Renders a test pattern on the fBO
 void RenderTestSceneOnFBO() {
  Blending(none);
  glColor4f(0.0f,0.0f,0.0f,1.0f);
  glBegin(GL_QUADS); // Draws in the lower left
  glVertex2i(-10,-10);
  glVertex2i(-10,10);
  glVertex2i(10,10);
  glVertex2i(10,-10);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(0,0);
  glVertex2i(w,h);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(w,0);
  glVertex2i(0,h);
  glVertex2i(-w,-h);
  glEnd();
  glColor4f(0.0f,1.0f,0.0f,1.0f);
  GL_Report("Framebuffer::RenderTestSceneOnFBO() glColor4f #2" );
  float r=0.0f;
  int dx=w/10;
  int dy=h/10;
  for ( int i=0; i<10; i++ )
   for ( int j=0; j<10; j++ ) {
    r+=FRAMETIME;
    glColor4f(r,1.0f,0.0f,1.0f);
    if ( r > 1.0f ) r=0.0f;
    int
     X=i*dx*(i%2),
     Y=j*dy*(j%2);
    glRecti(X,Y,X+dx,Y+dy);
   }
  GL_Report("Framebuffer::RenderTestSceneOnFBO() About to scissor" );
 }

 void RenderStencil( int x, int y, int x2, int y2 ) {  
  GL_Assert("Framebuffer::RenderStencil() Pre" );
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  Blending(blend);
  glColor4fv(tint.floats);
  glBindTexture(GL_TEXTURE_2D, dsTexture);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0,0.0);     glVertex2i(x,y);
  glTexCoord2d(1.0,0.0);     glVertex2i(x2,y);
  glTexCoord2d(1.0,1.0);     glVertex2i(x2,y2);
  glTexCoord2d(0.0,1.0);     glVertex2i(x,y2);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
 }

 void Release() {
  if ( !created ) return;
  fbo_pixel_count-=w*h;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &id);
  Detach();
 }

 void quad();
 void QuadXInverted();
 void QuadYInverted();
 void QuadXYInverted();

 void quad( double stretchX, double stretchY );
 void QuadXInverted( double stretchX, double stretchY );
 void QuadYInverted( double stretchX, double stretchY );
 void QuadXYInverted( double stretchX, double stretchY );

 void quad( GLuint *draw );
 void QuadXInverted( GLuint *draw );
 void QuadYInverted( GLuint *draw );
 void QuadXYInverted( GLuint *draw );

 void quad( FBO *draw );
 void QuadXInverted( FBO *draw );
 void QuadYInverted( FBO *draw );
 void QuadXYInverted( FBO *draw );

 void quad( FBO *draw, double stretchX, double stretchY );
 void QuadXInverted( FBO *draw, double stretchX, double stretchY );
 void QuadYInverted( FBO *draw, double stretchX, double stretchY ); 
 void QuadXYInverted( FBO *draw, double stretchX, double stretchY );

 void quad( GLImage *draw ); 
 void QuadXInverted( GLImage *draw ); 
 void QuadYInverted( GLImage *draw ); 
 void QuadXYInverted( GLImage *draw );
 
 void quad( GLImage *draw, double stretchX, double stretchY ); 
 void QuadXInverted( GLImage *draw, double stretchX, double stretchY ); 
 void QuadYInverted( GLImage *draw, double stretchX, double stretchY ); 
 void QuadXYInverted( GLImage *draw, double stretchX, double stretchY );
 
 void quad( GLSLShader *in, int x=0, int y=0 );
 void QuadXInverted( GLSLShader *in, int x=0, int y=0  );
 void QuadYInverted( GLSLShader *in, int x=0, int y=0  );
 void QuadXYInverted( GLSLShader *in, int x=0, int y=0  );


 ~FBO() {
  Release();
 }
};

class FBOColor : public FBO {
public:
 Crayon background;
 bool scissorStateAtBinding;
 Cartesian oldState;
 FBOColor() : FBO() {
  depth=false;
  stencil=false;
  scissorStateAtBinding=false;
 }
 virtual void Activate() {
  GL_Report("Framebuffer::Activate() Pre" );
  	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  SetRenderTarget(gl_color0);
  glPush();
  glViewport(0,0,w,h);
  scissorStateAtBinding=scissors.enabled;
  gl.GetScissorBox(&oldState);
  if ( scissorStateAtBinding ) scissors.Disable();
  glClearColor((GLclampf) background.rf,(GLclampf) background.gf,(GLclampf) background.bf,(GLclampf) background.af);
  glClear(GL_COLOR_BUFFER_BIT);
  this->OrthoThis2d();
  glReportError( glGetError() );
  GL_Assert("Framebuffer::Activate() Post" );
 }

 virtual void ActivateWithoutClearing() {
  GL_Report("Framebuffer::ActivateWithoutClearing() Pre" );
  	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  SetRenderTarget(gl_color0);
  glPush();
  glViewport(0,0,w,h);
  scissorStateAtBinding=scissors.enabled;
  gl.GetScissorBox(&oldState);
  if ( scissorStateAtBinding ) scissors.Disable();
  this->OrthoThis2d();
  glReportError( glGetError() );
  GL_Assert("Framebuffer::ActivateWithoutClearing() Post" );
 }
 
 virtual void Deactivate() {
  glPop();
  GL_Report("Framebuffer::Deactivate() Pre" );
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL_Report("Framebuffer::Deactivate() glBindFramebuffer(0)" );

  glEnable(GL_TEXTURE_2D);
  //**** Now that we rendered to level 0 of the texture, we must generate the mipmaps.
  //This should be quick since it is done on the GPU.
  glBindTexture(GL_TEXTURE_2D, texture);
  glGenerateMipmap(GL_TEXTURE_2D);
  GL_Report("Framebuffer::Deactivate() glGenerateMipmap" );
  glBindTexture(GL_TEXTURE_2D,0);
 
  if (samples > 0 && multisample) {
   // Multisample rendering so copy the pixel data in the multisample
   // color render buffer image to the FBO containing the offscreen
   // texture image.
   glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texture); // is color->texture here?  or is it another ?  was g_fboResolveTarget
   glBlitFramebuffer(0, 0, w, h,
                     0, 0, w, h,
                     GL_COLOR_BUFFER_BIT, GL_NEAREST);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  glDisable(GL_TEXTURE_2D);
  
  GL_Report("Framebuffer::Deactivate() Post" );
  glScissor(oldState.x,oldState.y,oldState.w,oldState.h);
  glViewport(0,0,display.w,display.h); // Is this necessary with the glPop?
  if ( scissorStateAtBinding ) scissors.Enable();
  else scissors.Disable();
  GL_Report("Framebuffer::Deactivate() Post glScissor" );
  glFinish();
 }
};

class FBOColorDepth : public FBOColor {
public:
 FBOColorDepth() : FBOColor() {
  depth=true;
  stencil=false;
 }
 virtual void Activate() {
  glFinish();
  GL_Report("Framebuffer::Activate() Pre" );
  	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  SetRenderTarget(gl_color0);
  glPush();
  glViewport(0,0,w,h);
  scissorStateAtBinding=scissors.enabled;
  gl.GetScissorBox(&oldState);
  if ( scissorStateAtBinding ) scissors.Disable();
  glClearColor((GLclampf) background.rf,(GLclampf) background.gf,(GLclampf) background.bf,(GLclampf) background.af);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->OrthoThis2d();
  glReportError( glGetError() );
  GL_Assert("Framebuffer::Activate() Post" );
 }
};


class FBOColorDepthStencil : public FBOColorDepth {
public:
 bool wasStenciling,stenciling;
 FBOColorDepthStencil() : FBOColorDepth() {
  stencil=true;
  wasStenciling=stenciling=false;
 }
 Cartesian oldState;
 void Activate() {
  wasStenciling=false;
  GL_Report("Framebuffer::Activate() Pre" );
  	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  SetRenderTarget(gl_color0);
  glPush();
  glViewport(0,0,w,h);
  scissorStateAtBinding=scissors.enabled;
  gl.GetScissorBox(&oldState);
  if ( scissorStateAtBinding ) scissors.Disable();
  glClearColor((GLclampf) background.rf,(GLclampf) background.gf,(GLclampf) background.bf,(GLclampf) background.af);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  this->OrthoThis2d();
  glReportError( glGetError() );
  GL_Assert("Framebuffer::Activate() Post" );
 }

 void StartStenciling() {
  stenciling=true;
  GL_Assert("Start Stenciling");
  glReportError(glGetError());
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0x1, 0x1);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  glReportError(glGetError());
  GL_Assert("Leaving StartStenciling");
 }

 void StopStenciling() {
  GL_Assert("Stop Stenciling");
  glReportError(glGetError());
  glDisable(GL_STENCIL_TEST);
  stenciling=false;
  glReportError(glGetError());
  GL_Assert("Leaving StopStenciling");
 }

 void ActivateWithoutClearing() {
  wasStenciling=false;
  GL_Report("Framebuffer::ActivateWithoutClearing() Pre" );
  glReportError( glGetError() );
  	// If there is a FBO, rendering to offscreen buffer via FrameBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  SetRenderTarget(gl_color0);
/* GLPUSH */ glMatrixMode(GL_PROJECTION);
/* GLPUSH */ glPushMatrix();
/* GLPUSH */ glLoadIdentity();
/* GLPUSH */ glMatrixMode(GL_MODELVIEW);
/* GLPUSH */ glPushMatrix();
/* GLPUSH */ glLoadIdentity();
/* GLPUSH */ glPushAttrib(GL_ALL_ATTRIB_BITS);
  glViewport(0,0,w,h);
  scissorStateAtBinding=scissors.enabled;
  gl.GetScissorBox(&oldState);
  if ( scissorStateAtBinding ) scissors.Disable();
  this->OrthoThis2d();
  glReportError( glGetError() );
  GL_Assert("Framebuffer::ActivateWithoutClearing() Post" );
 }
 
 void Deactivate() {
  if ( wasStenciling=stenciling ) StopStenciling();
  GL_Report("Framebuffer::Deactivate() Pre" );

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GL_Report("Framebuffer::Deactivate() glBindFramebuffer(0)" );

  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  //**** Now that we rendered to level 0 of the texture, we must generate the mipmaps.
  //This should be quick since it is done on the GPU.
  glBindTexture(GL_TEXTURE_2D, texture);
  glGenerateMipmap(GL_TEXTURE_2D);
  GL_Report("Framebuffer::Deactivate() glGenerateMipmap" );
  glBindTexture(GL_TEXTURE_2D,0);
 
  if (samples > 0 && multisample) {
   // Multisample rendering so copy the pixel data in the multisample
   // color render buffer image to the FBO containing the offscreen
   // texture image.
   glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texture); // is color->texture here?  or is it another ?  was g_fboResolveTarget
   glBlitFramebuffer(0, 0, w, h,
                     0, 0, w, h,
                     GL_COLOR_BUFFER_BIT, GL_NEAREST);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glDisable(GL_TEXTURE_2D);
  }
  
  GL_Report("Framebuffer::Deactivate() Post" );
  glScissor(oldState.x,oldState.y,oldState.w,oldState.h);
  if ( scissorStateAtBinding ) scissors.Enable();
  else scissors.Disable();
  GL_Report("Framebuffer::Deactivate() Post glScissor" );
  /* GLPOP */
#if defined(GL_DEBUG)
 GLenum err=glGetError();
 switch ( err ) {
 case GL_NO_ERROR: break;                       
 case GL_INVALID_ENUM:      OUTPUT("glError:glPop() [pre] GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument, offending command is ignored.\n" ); break;
 case GL_INVALID_VALUE:     OUTPUT("glError:glPop() [pre] GL_INVALID_VALUE: A numeric argument is out of range, offending command is ignored.\n" ); break;                       
 case GL_INVALID_OPERATION: OUTPUT("glError:glPop() [pre] GL_INVALID_OPERATION: The specified operation is not allowed in the current state, offending command is ignored.\n" ); break;
 case GL_STACK_OVERFLOW:    OUTPUT("glError:glPop() [pre] GL_STACK_OVERFLOW: A command would cause a stack overflow, the offending command is ignored.\n" ); break;
 case GL_STACK_UNDERFLOW:   OUTPUT("glError:glPop() [pre] GL_STACK_UNDERFLOW: A command would cause a stack underflow, the offending command is ignored.\n" ); break;
 case GL_OUT_OF_MEMORY:     OUTPUT("glError:glPop() [pre] GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined,\n except for the state of the error flags, after this error is recorded.\n" ); break;                       
 case GL_TABLE_TOO_LARGE:   OUTPUT("glError:glPop() [pre] GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored.\n" ); break;
 default: OUTPUT("glError:glPop() unknown error!\n"); break;
 }
#endif
 glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(GL_MODELVIEW);
 glPopMatrix();
 glPopAttrib();
#if defined(GL_DEBUG) && defined(REPORT_ALL_ERRORS)
 err=glGetError();
 switch ( err ) {
 case GL_NO_ERROR: break;                       
 case GL_INVALID_ENUM:      OUTPUT("glError:glPop() [post]GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument, offending command is ignored.\n" ); break;
 case GL_INVALID_VALUE:     OUTPUT("glError:glPop() [post]GL_INVALID_VALUE: A numeric argument is out of range, offending command is ignored.\n" ); break;                       
 case GL_INVALID_OPERATION: OUTPUT("glError:glPop() [post]GL_INVALID_OPERATION: The specified operation is not allowed in the current state, offending command is ignored.\n" ); break;
 case GL_STACK_OVERFLOW:    OUTPUT("glError:glPop() [post]GL_STACK_OVERFLOW: A command would cause a stack overflow, the offending command is ignored.\n" ); break;
 case GL_STACK_UNDERFLOW:   OUTPUT("glError:glPop() [post]GL_STACK_UNDERFLOW: A command would cause a stack underflow, the offending command is ignored.\n" ); break;
 case GL_OUT_OF_MEMORY:     OUTPUT("glError:glPop() [post]GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined,\n except for the state of the error flags, after this error is recorded.\n" ); break;                       
 case GL_TABLE_TOO_LARGE:   OUTPUT("glError:glPop() [post]GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored.\n" ); break;
 default: OUTPUT("glError:glPop() unknown error!\n"); break;
 }
#else
 glGetError();
#endif
  /* GLPOP */
 }
 /*
 void DebugMouse() {
  int mx,my;
  MousePosition(&mx,&my);
  if ( input->left ) {
   if ( WITHIN(mx,my,5,5,5+512,5+512) ) {
    BindWithoutClearing();
    ReadPixel(gl_color0,mx-5,my-5);
    Unbind();
   } else
   if ( WITHIN(mx,my,5+512+5,5,5+512+5+512,5+512) ) {
    BindWithoutClearing();
    ReadPixel(gl_depthstencil,mx-(5+512+5),my-5);
    Unbind();
   }
  }
 }
 void Debug() {
  Render(5,5,5+512,5+512);
  RenderStencil(5+512+5,5,5+512+5+512,5+512);
  Blending(none);
  Area(crayons.jami(0,ub_bgra[2],ub_bgra[1],ub_bgra[0],ub_bgra[3]),5+512+32,5+512+32,5+512+32+32,5+512+32+32);
  char buf[128];
  glColor3d(0.0,1.0,0.0);
  FORMAT(buf,128,"r_Stencil: %d, r_Depth: %d f_Depth: %f", (int) r_stencil, (int) r_depth, f_depth );
  Text(buf,5.0,700.0,5,7,false);
 }
 */

};

extern FBOColorDepth //fbo64,fbo128,fbo256,
 fbo512,
 fbo512a,fbo512b,fbo512c,fbo512d,
 //fbo256a,fbo256b,fbo256c,fbo256d,
 //fbo128a,fbo128b,fbo128c,fbo128d,
 fboScreen,
 fboScreen1,fboScreen2,fboScreen3,fboScreen4,fboScreen5,
 fboScreen6,fboScreen7,fboScreen8,fboScreen9,fboScreen10,
 fboScreen11,fboScreen12//,fboScreen13,fboScreen14,
// fboScreen15,fboScreen16,
 // A list of 8 additional screens
// fboScreen1a, fboScreen2a, fboScreen3a, fboScreen4a,
// fboScreen5a, fboScreen6a, fboScreen7a, fboScreen8a
 ;
extern FBOColor fboC512, fboCScreen;
extern FBOColorDepthStencil //fboCDS64, fboCDS128, fboCDS256, 
 fboCDS512, fboCDS512a, fboCDS512b, 
 fboCDSScreen;//, fboCDSScreen2, fboCDSScreen3;
// 1k and 2k FBOs
extern FBOColorDepthStencil fboCDS1024, fboCDS1024b, fboCDS1024c, fboCDS2048;

// Special HD-TV sizes
extern FBOColorDepthStencil fbo1080p,fbo1080p2,fbo1080p3,fbo1080p4,fbo1080p5,fbo1080p6;
//extern FBOColorDepthStencil fbo2160p,fbo2160p2,fbo2160p3,fbo2160p4;

extern FBOColorDepthStencil fboScreenWindow;

extern Zbool fbo4k;

void InitDefaultFBOs();
void ResetDefaultFBOs();

void FBOBlit( FBO *from, FBO *to );

#if defined(DEBUG)
void FBODrawDebug();
void FBODebugKeys();
void FBODebug();
#endif

FBO *FBOfromString( const char *w );
FBOColor *FBOColorfromString( const char *w );
FBOColorDepth *FBOColorDepthfromString( const char *w );
FBOColorDepthStencil *FBOColorDepthStencilfromString( const char *w );