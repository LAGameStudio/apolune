#include "FBO.h"

#include "pThread.h"
#include "Trigger.h"
#include "Vertexd.h"
#include "GLImage.h"

#include "VBO.h"

#include "CommandLine.h"

#include "Art2d.h"

#include "TexShader.h"

int fbo_pixel_count=0;

FBOBindings fboBindingStack;

FBOColorDepth //fbo64,fbo128,fbo256,
 fbo512,
 fbo512a,fbo512b,fbo512c,fbo512d,
 //fbo256a,fbo256b,fbo256c,fbo256d,
 //fbo128a,fbo128b,fbo128c,fbo128d,
 fboScreen,
 fboScreen1,fboScreen2,fboScreen3,fboScreen4,fboScreen5,
 fboScreen6,fboScreen7,fboScreen8,fboScreen9,fboScreen10,
 fboScreen11,fboScreen12//,fboScreen13,fboScreen14,
// fboScreen15,fboScreen16,
// fboScreen1a, fboScreen2a, fboScreen3a, fboScreen4a,
// fboScreen5a, fboScreen6a, fboScreen7a, fboScreen8a
 ;

FBOColor fboC512, fboCScreen;
FBOColorDepthStencil //fboCDS64,fboCDS128,fboCDS256,
 fboCDS512, fboCDS512a, fboCDS512b,
 fboCDSScreen;//, fboCDSScreen2, fboCDSScreen3;

FBOColorDepthStencil fboCDS1024, fboCDS1024b, fboCDS1024c, fboCDS2048;

FBOColorDepthStencil fbo1080p,fbo1080p2,fbo1080p3,fbo1080p4,fbo1080p5,fbo1080p6;
// fbo2160p,fbo2160p2,fbo2160p3,fbo2160p4;

FBOColorDepthStencil fboScreenWindow; // Used for the screen as the accumulation buffer.

Zbool fbo4k;

 CPUImage *FBO::Snapshot() {
  CPUImage *c=new CPUImage(w,h);
  glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, c->pixels.list);
  return c;
 }

 CPUImage *FBO::Snapshot( int sx, int sy, int ww, int hh ) {
  CPUImage *c=new CPUImage(ww,hh);
  glReadPixels(sx, sy, ww, hh, GL_RGBA, GL_UNSIGNED_BYTE, c->pixels.list);
  return c;
 }

 void FBO::Save( const char *pngfile ) {
  CPUImage *c=Snapshot();
  c->Save(pngfile);
  delete c;
 }

#include "Vertexd.h"
 


FBOBound::FBOBound( FBO *f ) {
 bound=f;
 wasStenciling=null;
}

FBOBound::FBOBound( FBOColorDepthStencil *f ) {
 bound=f;
 wasStenciling=&f->wasStenciling;
}

void FBOBindings::Push( FBO *f ) {
 if ( last ) {
  FBOBound *b=(FBOBound *) last;
  b->bound->Deactivate();
 }
 Append(new FBOBound(f));
 f->Activate();
}

void FBOBindings::Push( FBO *f, bool clear ) {
 if ( last ) {
  FBOBound *b=(FBOBound *) last;
  b->bound->Deactivate();
 }
 Append(new FBOBound(f));
 if ( clear ) f->Activate();
 else f->ActivateWithoutClearing();
}

void FBOBindings::Push( FBOColorDepthStencil *f ) {
 if ( last ) {
  FBOBound *b=(FBOBound *) last;
  b->bound->Deactivate();
 }
 Append(new FBOBound(f));
 f->Activate();
}

void FBOBindings::Push( FBOColorDepthStencil *f, bool clear ) {
 if ( last ) {
  FBOBound *b=(FBOBound *) last;
  b->bound->Deactivate();
 }
 Append(new FBOBound(f));
 if ( clear ) f->Activate();
 else f->ActivateWithoutClearing();
}

void FBOBindings::Pop() {
 if ( !last ) {
  OUTPUT("FBOBindings::Pop() requested, but nothing was available to pop.\n");
  return;
 }
 FBOBound *b=(FBOBound *) last;
 b->bound->Deactivate();
 Remove(b);
 delete b;
 if ( last ) {
  b=(FBOBound *) last;
  if ( b->wasStenciling && *b->wasStenciling ) {
   FBOColorDepthStencil *f=(FBOColorDepthStencil *) b->bound;
   f->ActivateWithoutClearing();
   f->StartStenciling();
  } else {
   b->bound->ActivateWithoutClearing();
  }
 }
}

bool FBO::Strike( FBOTargets type, Line *segment, Vertexd *out, double drawW, double drawH, int precision ) {
 Cartesian pixel;
 bool struck=false;
 double delta=iratiod(1,precision);
 Integers list; // Randomize the order
 for ( int i=0; i<precision; i++ ) {
  list.Add(i);
 }
 list.RandomizeOrder();
 EACH(list.first,Integer,I) {
  double percent=(double)I->i * delta;
  segment->Point(percent,out);
  pixel.Set(
   (int)((double)w*(out->x/drawW)),
   (int)((double)h*(out->y/drawH))
  );
  if ( pixel.x < 0 || pixel.x >= w
    || pixel.y < 0 || pixel.y >= h ) continue;
  ReadPixel(type,pixel.x,pixel.y);
  if ( !( ub_bgra[0]==0 && ub_bgra[1]==0 && ub_bgra[2]==0 && ub_bgra[3]==0 ) ) {
   struck=true;
   break;
  }
 }
 return struck;
}

void InitDefaultFBOs() {
 //OUTPUT("fbo64:");
 ///fbo64.background.Pick(clear);
 ///fbo64.blend=transparency;
 ///fbo64.Create(64,64);
 //OUTPUT("fboCDS64:");
 ///fboCDS64.background.Pick(clear);
 ///fboCDS64.blend=transparency;
 ///fboCDS64.Create(64,64);
 //OUTPUT("fbo128:");
 ///fbo128.background.Pick(clear);
 ///fbo128.blend=transparency;
 ///fbo128.Create(128,128);
 //OUTPUT("fboCDS128:");
 ///fboCDS128.background.Pick(clear);
 ///fboCDS128.blend=transparency;
 ///fboCDS128.Create(128,128);
 //OUTPUT("fbo128a:");
 ///fbo128a.background.Pick(clear);
 ///fbo128a.blend=transparency;
 ///fbo128a.Create(128,128);
 //OUTPUT("fbo128b:");
 ///fbo128b.background.Pick(clear);
 ///fbo128b.blend=transparency;
 ///fbo128b.Create(128,128);
 //OUTPUT("fbo128c:");
 ///fbo128c.background.Pick(clear);
 ///fbo128c.blend=transparency;
 ///fbo128c.Create(128,128);
 //OUTPUT("fbo128d:");
 ///fbo128d.background.Pick(clear);
 ///fbo128d.blend=transparency;
 ///fbo128d.Create(128,128);
 //OUTPUT("fbo256:");
 ///fbo256.background.Pick(clear);
 ///fbo256.blend=transparency;
 ///fbo256.Create(256,256);
 //OUTPUT("fboCDS256:");
 ///fboCDS256.background.Pick(clear);
 ///fboCDS256.blend=transparency;
 ///fboCDS256.Create(256,256);
 //OUTPUT("fbo256a:");
 ///fbo256a.background.Pick(clear);
 ///fbo256a.blend=transparency;
 ///fbo256a.Create(256,256);
 //OUTPUT("fbo256b:");
 ///fbo256b.background.Pick(clear);
 ///fbo256b.blend=transparency;
 ///fbo256b.Create(256,256);
 //OUTPUT("fbo256c:");
 ///fbo256c.background.Pick(clear);
 ///fbo256c.blend=transparency;
 ///fbo256c.Create(256,256);
 //OUTPUT("fbo256d:");
 ///fbo256d.background.Pick(clear);
 ///fbo256d.blend=transparency;
 ///fbo256d.Create(256,256);
 OUTPUT("fbo512:");
 fbo512.background.Pick(clear);
 fbo512.blend=transparency;
 fbo512.Create(512,512);
 OUTPUT("fbo512a:");
 fbo512a.background.Pick(clear);
 fbo512a.blend=transparency;
 fbo512a.Create(512,512);
 OUTPUT("fbo512b:");
 fbo512b.background.Pick(clear);
 fbo512b.blend=transparency;
 fbo512b.Create(512,512);
 OUTPUT("fbo512c:");
 fbo512c.background.Pick(clear);
 fbo512c.blend=transparency;
 fbo512c.Create(512,512);
 OUTPUT("fbo512d:");
 fbo512d.background.Pick(clear);
 fbo512d.blend=transparency;
 fbo512d.Create(512,512);
 OUTPUT("fboScreen:");
 fboScreen.blend=none;
 fboScreen.background.Pick(clear);
 fboScreen.Create(display.w,display.h);
 OUTPUT("fboScreen1:");
 fboScreen1.blend=transparency;
 fboScreen1.background.Pick(clear);
 fboScreen1.Create(display.w,display.h);
 OUTPUT("fboScreen2:");
 fboScreen2.blend=transparency;
 fboScreen2.background.Pick(clear);
 fboScreen2.Create(display.w,display.h);
 OUTPUT("fboScreen3:");
 fboScreen3.blend=transparency;
 fboScreen3.background.Pick(clear);
 fboScreen3.Create(display.w,display.h);
 OUTPUT("fboScreen4:");
 fboScreen4.blend=transparency;
 fboScreen4.background.Pick(clear);
 fboScreen4.Create(display.w,display.h);
 OUTPUT("fboScreen5:");
 fboScreen5.blend=transparency;
 fboScreen5.background.Pick(clear);
 fboScreen5.Create(display.w,display.h);
 OUTPUT("fboScreen6:");
 fboScreen6.blend=transparency;
 fboScreen6.background.Pick(clear);
 fboScreen6.Create(display.w,display.h);
 OUTPUT("fboScreen7:");
 fboScreen7.blend=transparency;
 fboScreen7.background.Pick(clear);
 fboScreen7.Create(display.w,display.h);
 OUTPUT("fboScreen8:");
 fboScreen8.blend=transparency;
 fboScreen8.background.Pick(clear);
 fboScreen8.Create(display.w,display.h);
 OUTPUT("fboScreen9:");
 fboScreen9.blend=transparency;
 fboScreen9.background.Pick(clear);
 fboScreen9.Create(display.w,display.h);
 OUTPUT("fboScreen10:");
 fboScreen10.blend=transparency;
 fboScreen10.background.Pick(clear);
 fboScreen10.Create(display.w,display.h);
 OUTPUT("fboScreen11:");
 fboScreen11.blend=transparency;
 fboScreen11.background.Pick(clear);
 fboScreen11.Create(display.w,display.h);
 OUTPUT("fboScreen12:");
 fboScreen12.blend=transparency;
 fboScreen12.background.Pick(clear);
 fboScreen12.Create(display.w,display.h);
 //OUTPUT("fboScreen13:");
 //fboScreen13.blend=transparency;
 //fboScreen13.background.Pick(clear);
 //fboScreen13.Create(display.w,display.h);
 //OUTPUT("fboScreen14:");
 //fboScreen14.blend=transparency;
 //fboScreen14.background.Pick(clear);
 //fboScreen14.Create(display.w,display.h);
 //OUTPUT("fboScreen15:");
 //fboScreen15.blend=transparency;
 //fboScreen15.background.Pick(clear);
 //fboScreen15.Create(display.w,display.h);
 //OUTPUT("fboScreen16:");
 //fboScreen16.blend=transparency;
 //fboScreen16.background.Pick(clear);
 //fboScreen16.Create(display.w,display.h);
 //OUTPUT("fboScreen1a:");
 //fboScreen1a.blend=transparency;
 //fboScreen1a.background.Pick(clear);
 //fboScreen1a.Create(display.w,display.h);
 //OUTPUT("fboScreen2a:");
 //fboScreen2a.blend=transparency;
 //fboScreen2a.background.Pick(clear);
 //fboScreen2a.Create(display.w,display.h);
 //OUTPUT("fboScreen3a:");
 //fboScreen3a.blend=transparency;
 //fboScreen3a.background.Pick(clear);
 //fboScreen3a.Create(display.w,display.h);
 //OUTPUT("fboScreen4a:");
 //fboScreen4a.blend=transparency;
 //fboScreen4a.background.Pick(clear);
 //fboScreen4a.Create(display.w,display.h);
 //OUTPUT("fboScreen5a:");
 //fboScreen5a.blend=transparency;
 //fboScreen5a.background.Pick(clear);
 //fboScreen5a.Create(display.w,display.h);
 //OUTPUT("fboScreen6a:");
 //fboScreen6a.blend=transparency;
 //fboScreen6a.background.Pick(clear);
 //fboScreen6a.Create(display.w,display.h);
 //OUTPUT("fboScreen7a:");
 //fboScreen7a.blend=transparency;
 //fboScreen7a.background.Pick(clear);
 //fboScreen7a.Create(display.w,display.h);
 //OUTPUT("fboScreen8a:");
 //fboScreen8a.blend=transparency;
 //fboScreen8a.background.Pick(clear);
 //fboScreen8a.Create(display.w,display.h);
 OUTPUT("fboC512:");
 fboC512.blend=transparency;
 fboC512.background.Pick(clear);
 fboC512.Create(512,512);
 OUTPUT("fboCScreen:");
 fboCScreen.blend=transparency;
 fboCScreen.background.Pick(clear);
 fboCScreen.Create(display.w,display.h);
 OUTPUT("fboCDS512:");
 fboCDS512.blend=transparency;
 fboCDS512.background.Pick(clear);
 fboCDS512.Create(512,512);
 OUTPUT("fboCDS512a:");
 fboCDS512a.blend=transparency;
 fboCDS512a.background.Pick(clear);
 fboCDS512a.Create(512,512);
 OUTPUT("fboCDS512b:");
 fboCDS512b.blend=transparency;
 fboCDS512b.background.Pick(clear);
 fboCDS512b.Create(512,512);
 OUTPUT("fboCDSScreen:");
 fboCDSScreen.blend=transparency;
 fboCDSScreen.background.Pick(clear);
 fboCDSScreen.Create(display.w,display.h);
 //OUTPUT("fboCDSScreen2:");
 //fboCDSScreen2.blend=transparency;
 //fboCDSScreen2.background.Pick(clear);
 //fboCDSScreen2.Create(display.w,display.h);
 //OUTPUT("fboCDSScreen3:");
 //fboCDSScreen3.blend=transparency;
 //fboCDSScreen3.background.Pick(clear);
 //fboCDSScreen3.Create(display.w,display.h);
 OUTPUT("fboCDS1024:");
 fboCDS1024.blend=transparency;
 fboCDS1024.background.Pick(clear);
 fboCDS1024.Create(1024,1024);
 OUTPUT("fboCDS1024b:");
 fboCDS1024b.blend=transparency;
 fboCDS1024b.background.Pick(clear);
 fboCDS1024b.Create(1024,1024);
 OUTPUT("fboCDS1024c:");
 fboCDS1024c.blend=transparency;
 fboCDS1024c.background.Pick(clear);
 fboCDS1024c.Create(1024,1024);
// OUTPUT("fboCDS2k:");
/// fboCDS2048.blend=transparency;
/// fboCDS2048.background.Pick(clear);
/// fboCDS1024.Create(2048,2048);
 OUTPUT("fbo1080p:");
 fbo1080p.blend=transparency;
 fbo1080p.background.Pick(clear);
 fbo1080p.Create(1920,1080);
 OUTPUT("fbo1080p2:");
 fbo1080p2.blend=transparency;
 fbo1080p2.background.Pick(clear);
 fbo1080p2.Create(1920,1080);
 OUTPUT("fbo1080p3:");
 fbo1080p3.blend=transparency;
 fbo1080p3.background.Pick(clear);
 fbo1080p3.Create(1920,1080);
 OUTPUT("fbo1080p4:");
 fbo1080p4.blend=transparency;
 fbo1080p4.background.Pick(clear);
 fbo1080p4.Create(1920,1080);
 OUTPUT("fbo1080p5:");
 fbo1080p5.blend=transparency;
 fbo1080p5.background.Pick(clear);
 fbo1080p5.Create(1920,1080);
 OUTPUT("fbo1080p6:");
 fbo1080p6.blend=transparency;
 fbo1080p6.background.Pick(clear);
 fbo1080p6.Create(1920,1080);
// if ( display.w > 1920 && display.h > 1200 && !CmdLine.Option("-force1080p") ) {
//  fbo4k=true; // to indicate the following 4 FBOs were created.
//  OUTPUT("fbo2160p:");
///  fbo2160p.blend=transparency;
///  fbo2160p.background.Pick(clear);
///  fbo2160p.Create(3840,2160);
//  OUTPUT("fbo2160p2:");
///  fbo2160p2.blend=transparency;
///  fbo2160p2.background.Pick(clear);
///  fbo2160p2.Create(3840,2160);
//  OUTPUT("fbo2160p3:");
///  fbo2160p3.blend=transparency;
///  fbo2160p3.background.Pick(clear);
///  fbo2160p3.Create(3840,2160);
//  OUTPUT("fbo2160p3:");
///  fbo2160p4.blend=transparency;
///  fbo2160p4.background.Pick(clear);
///  fbo2160p4.Create(3840,2160);
// }
 OUTPUT("FBO's have been initialized, total fbo_pixel_count: %d\n--------------------\n",(int)fbo_pixel_count);
}


void ResetDefaultFBOs() {
//OUTPUT("fbo64:");
/// fbo64.background.Pick(clear);
/// fbo64.blend=transparency;
// OUTPUT("fbo128:");
/// fbo128.background.Pick(clear);
/// fbo128.blend=transparency;
// OUTPUT("fbo128a:");
/// fbo128a.background.Pick(clear);
/// fbo128a.blend=transparency;
// OUTPUT("fbo128b:");
/// fbo128b.background.Pick(clear);
/// fbo128b.blend=transparency;
// OUTPUT("fbo128c:");
/// fbo128c.background.Pick(clear);
/// fbo128c.blend=transparency;
// OUTPUT("fbo128d:");
/// fbo128d.background.Pick(clear);
/// fbo128d.blend=transparency;
// OUTPUT("fbo256:");
/// fbo256.background.Pick(clear);
/// fbo256.blend=transparency;
// OUTPUT("fbo256a:");
/// fbo256a.background.Pick(clear);
/// fbo256a.blend=transparency;
// OUTPUT("fbo256b:");
/// fbo256b.background.Pick(clear);
/// fbo256b.blend=transparency;
// OUTPUT("fbo256c:");
/// fbo256c.background.Pick(clear);
/// fbo256c.blend=transparency;
// OUTPUT("fbo256d:");
/// fbo256d.background.Pick(clear);
/// fbo256d.blend=transparency;
// OUTPUT("fbo512:");
 fbo512.background.Pick(clear);
 fbo512.blend=transparency;
// OUTPUT("fbo512a:");
 fbo512a.background.Pick(clear);
 fbo512a.blend=transparency;
// OUTPUT("fbo512b:");
 fbo512b.background.Pick(clear);
 fbo512b.blend=transparency;
// OUTPUT("fbo512c:");
 fbo512c.background.Pick(clear);
 fbo512c.blend=transparency;
// OUTPUT("fbo512d:");
 fbo512d.background.Pick(clear);
 fbo512d.blend=transparency;
// OUTPUT("fboScreen:");
 fboScreen.blend=none;
 fboScreen.background.Pick(clear);
// OUTPUT("fboScreen1:");
 fboScreen1.blend=transparency;
 fboScreen1.background.Pick(clear);
// OUTPUT("fboScreen2:");
 fboScreen2.blend=transparency;
 fboScreen2.background.Pick(clear);
// OUTPUT("fboScreen3:");
 fboScreen3.blend=transparency;
 fboScreen3.background.Pick(clear);
// OUTPUT("fboC512:");
 fboC512.blend=transparency;
 fboC512.background.Pick(clear);
// OUTPUT("fboCScreen:");
 fboCScreen.blend=transparency;
 fboCScreen.background.Pick(clear);
// OUTPUT("fboCDS512:");
 fboCDS512.blend=transparency;
 fboCDS512.background.Pick(clear);
// OUTPUT("fboCDSa512:");
 fboCDS512a.blend=transparency;
 fboCDS512a.background.Pick(clear);
// OUTPUT("fboCDSb512:");
 fboCDS512b.blend=transparency;
 fboCDS512b.background.Pick(clear);
// OUTPUT("fboCDSScreen:");
 fboCDSScreen.blend=transparency;
 fboCDSScreen.background.Pick(clear);
// OUTPUT("fboCDSScreen2:");
 ///fboCDSScreen2.blend=transparency;
 ///fboCDSScreen2.background.Pick(clear);
// OUTPUT("fboCDS 1k, 2k:");
 fboCDS1024.blend=transparency;
 fboCDS1024.background.Pick(clear);
 fboCDS1024b.blend=transparency;
 fboCDS1024b.background.Pick(clear);
 fboCDS1024c.blend=transparency;
 fboCDS1024c.background.Pick(clear);
 fboCDS2048.blend=transparency;
 fboCDS2048.background.Pick(clear);
}


void FBO::Blit( FBO *from ) { FBOBlit(from,this); }


 void FBO::Render( int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderReversed( int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect_FlipX,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDown( int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDown( Crayon t, int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,t,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDown( Crayon t, double x, double y, double x2, double y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,t,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDown( Blends b, int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(b);
  art.Element(texture,tint,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }
 
 void FBO::RenderUpsideDown( Blends b, double x, double y, double x2, double y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(b);
  art.Element(texture,tint,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDown( Blends b, Crayon t, int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(b);
  art.Element(texture,t,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 } 

 void FBO::RenderUpsideDown( double x, double y, double x2, double y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect_FlipY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderUpsideDownReversed( int x, int y, int x2, int y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect_FlipXY,x,y,x2-x,y2-y);
  GL_Assert("Framebuffer::Render() Post" );
  glReportError( glGetError() );
 }

 void FBO::RenderFullScreen() {
  GL_Assert("Framebuffer::RenderFullScreen() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,0.0,0.0,display.wd,display.hd);
 }
 
 void FBO::Render( Cartesian *extents ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,extents->x,extents->y,extents->w,extents->h);
 }
 
 void FBO::Render( ScreenPosition *extents ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,extents->x,extents->y,extents->w,extents->h);
 }

 void FBO::Render( Cartesiand *extents ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,extents->x,extents->y,extents->w,extents->h);
 }

 void FBO::Renderf( float x, float y, float x2, float y2 ) { Render((double)x,(double)y,(double)x2,(double)y2); }
 
 void FBO::Render( double x, double y, double x2, double y2 ) {
  GL_Assert("Framebuffer::Render() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,x,y,x2-x,y2-y);
 }


 void FBO::RenderQuad( Quad *q ) {
  GL_Assert("Framebuffer::RenderQuad() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect,q->TL.x,q->TL.y,q->BR.x-q->TL.x,q->BR.y-q->TL.y);
 }

 void FBO::RenderQuadUpsideDown( Quad *q ) {
  GL_Assert("Framebuffer::RenderQuadUpsideDown() Pre" );
  glReportError( glGetError() );
  Blending(blend);
  art.Element(texture,tint,art.stash.filledRect_FlipY,q->TL.x,q->TL.y,q->BR.x-q->TL.x,q->BR.y-q->TL.y);
 }

 
 void FBO::quad() {
  art.Element(texture,tint,art.stash.filledRect,0,0,w,h);
 }
 
 void FBO::QuadXInverted() {
  art.Element(texture,tint,art.stash.filledRect_FlipX,0,0,w,h);
 }
 
 void FBO::QuadYInverted() {
  art.Element(texture,tint,art.stash.filledRect_FlipY,0,0,w,h);
 }
 
 void FBO::QuadXYInverted() {
  art.Element(texture,tint,art.stash.filledRect_FlipXY,0,0,w,h);
 }
 
 void FBO::quad( double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(texture,tint,art.stash.filledRect,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadXInverted( double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(texture,tint,art.stash.filledRect_FlipX,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadYInverted( double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(texture,tint,art.stash.filledRect_FlipY,0,0,xMag,yMag);
 }
 
 void FBO::QuadXYInverted( double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(texture,tint,art.stash.filledRect_FlipXY,-xMag,-yMag,w,h);
 }

 void FBO::quad( GLuint *draw ) {
  art.Element(*draw,tint,art.stash.filledRect,0,0,w,h);
 }
 
 void FBO::QuadXInverted( GLuint *draw ) {
  art.Element(*draw,tint,art.stash.filledRect_FlipX,0,0,w,h);
 }
 
 void FBO::QuadYInverted( GLuint *draw ) {
  art.Element(*draw,tint,art.stash.filledRect_FlipY,0,0,w,h);
 }
 
 void FBO::QuadXYInverted( GLuint *draw ) {
  art.Element(*draw,tint,art.stash.filledRect_FlipXY,0,0,w,h);
 }

 void FBO::quad( FBO *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect,0,0,w,h);
 }
 
 void FBO::QuadXInverted( FBO *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipY,0,0,w,h);
 }
 
 void FBO::QuadYInverted( FBO *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipY,0,0,w,h);
 }
 
 void FBO::QuadXYInverted( FBO *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipXY,0,0,w,h);
 }
 
 void FBO::quad( FBO *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadXInverted( FBO *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipX,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadYInverted( FBO *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipY,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadXYInverted( FBO *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipXY,-xMag,-yMag,w,h);
 }



 void FBO::quad( GLImage *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect,0,0,w,h);
 }
 
 void FBO::QuadXInverted( GLImage *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipX,0,0,w,h);
 }
 
 void FBO::QuadYInverted( GLImage *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipY,0,0,w,h);
 }
 
 void FBO::QuadXYInverted( GLImage *draw ) {
  art.Element(draw->texture,tint,art.stash.filledRect_FlipXY,0,0,w,h);
 }

 
 void FBO::quad( GLImage *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipX,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadXInverted( GLImage *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipX,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadYInverted( GLImage *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipY,-xMag,-yMag,w,h);
 }
 
 void FBO::QuadXYInverted( GLImage *draw, double stretchX, double stretchY ) {
  int xMag=(int)(stretchX*(double)w);
  int yMag=(int)(stretchY*(double)h);
  art.Element(draw->texture,tint,art.stash.filledRect_FlipXY,-xMag,-yMag,w,h);
 }
 
#include "GLSL2Shader.h"

 // These are used to draw a shaded element on itself.

 void FBO::quad( GLSLShader *in, int x, int y ) {
  art.PushViewport(this);
  TwoDeeElement *rrs=art.stash.rrs_filledRect;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = art.viewport->ortho * trans;
  in->UpdateUniforms(trans);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  in->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  art.PopViewport();
 }
 
 void FBO::QuadXInverted( GLSLShader *in, int x, int y ) {
  art.PushViewport(this);
  TwoDeeElement *rrs=art.stash.rrs_filledRect_FlipX;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = art.viewport->ortho * trans;
  in->UpdateUniforms(trans);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  in->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  art.PopViewport();
 }
 
 void FBO::QuadYInverted( GLSLShader *in, int x, int y ) {
  art.PushViewport(this);
  TwoDeeElement *rrs=art.stash.rrs_filledRect_FlipY;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = art.viewport->ortho * trans;
  in->UpdateUniforms(trans);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  in->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  art.PopViewport();
 }
 
 void FBO::QuadXYInverted( GLSLShader *in, int x, int y ) {
  art.PushViewport(this);
  TwoDeeElement *rrs=art.stash.rrs_filledRect_FlipXY;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rrs->ebo);
  glBindBuffer(GL_ARRAY_BUFFER, rrs->vbo);
  glm::mat4 trans;
  trans = glm::translate(trans,glm::vec3(x,y,0.0));
  trans = glm::scale(trans,glm::vec3(w,h,1.0));
  trans = art.viewport->ortho * trans;
  in->UpdateUniforms(trans);
  glDrawElements( GL_TRIANGLES, (GLsizei) rrs->elements.value, GL_UNSIGNED_INT, 0);
  in->Disable();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  art.PopViewport();
 }

 void FBOBlit( FBO *a, FBO *b ) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER,a->id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,b->id);
  glBlitFramebuffer(0,0,a->w,a->h,0,0,b->w,b->h, GL_COLOR_BUFFER_BIT,GL_LINEAR);
  glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
 }

#if defined(DEBUG)
#include "Crayon.h"
#include "Art.h"
#include "UglyFont.h"

#define FBODRAWDEBUG(fbo,charstar,b,x,y) { Area(crayons.Pick(green255),(x),(y), (x)+64,(y)+64 ); crayons.Pick(black).gl(); (fbo).blend=(b); (fbo).RenderUpsideDown((x),(y),(x)+64,(y)+64 ); Rectangle(crayons.Pick(black),(x),(y),(x)+64,(y)+64);  Text((charstar), (x)+80-1,(y)+8+1, 8,10,true); crayons.Pick(alabaster).gl(); Text((charstar), (x)+80,(y)+8, 8,10,true); }

void FBODrawDebug() {
 FBODRAWDEBUG(fboScreen,  "0", transparency,10, 74 );
 FBODRAWDEBUG(fboScreen1, "1", transparency,10, 138);
 FBODRAWDEBUG(fboScreen2, "2", transparency,10, 202);
 FBODRAWDEBUG(fboScreen3, "3", transparency,10, 268);
 FBODRAWDEBUG(fboScreen4, "4", transparency,10, 332);
 FBODRAWDEBUG(fboScreen5, "5", transparency,10, 400);
 FBODRAWDEBUG(fboScreen6, "6", transparency,10, 464);
 FBODRAWDEBUG(fboScreen7, "7", transparency,10, 536);
 FBODRAWDEBUG(fboScreen8, "8", transparency,10, 600);
 FBODRAWDEBUG(fboScreen9, "9", transparency,10, 666);
 FBODRAWDEBUG(fboScreen10,"10",transparency,10, 730);
 FBODRAWDEBUG(fboScreen11,"11",transparency,128,74 );
 FBODRAWDEBUG(fboScreen12,"12",transparency,128,138);
 //FBODRAWDEBUG(fboScreen13,"13",transparency,128,202);
 //FBODRAWDEBUG(fboScreen14,"14",transparency,128,268);
 //FBODRAWDEBUG(fboScreen15,"15",transparency,128,332);
 //FBODRAWDEBUG(fboScreen16,"16",transparency,128,400);

 //FBODRAWDEBUG(fboScreen1a, "1a", transparency,1024, 138);
 //FBODRAWDEBUG(fboScreen2a, "2a", transparency,1024, 202);
 //FBODRAWDEBUG(fboScreen3a, "3a", transparency,1024, 268);
 //FBODRAWDEBUG(fboScreen4a, "4a", transparency,1024, 332);
 //FBODRAWDEBUG(fboScreen5a, "5a", transparency,1024, 400);
 //FBODRAWDEBUG(fboScreen6a, "6a", transparency,1024, 464);
 //FBODRAWDEBUG(fboScreen7a, "7a", transparency,1024, 536);
 //FBODRAWDEBUG(fboScreen8a, "8a", transparency,1024, 600);

 FBODRAWDEBUG(fbo512,  "512",  transparency, 128,465);
 FBODRAWDEBUG(fbo512a, "512a", transparency, 128,536);
 FBODRAWDEBUG(fbo512b, "512b", transparency, 128,600);
 FBODRAWDEBUG(fbo512c, "512c", transparency, 128,666);
 FBODRAWDEBUG(fbo512d, "512d", transparency, 128,730);

 //FBODRAWDEBUG(fbo256,  "256",  transparency, 256,465);
 //FBODRAWDEBUG(fbo256a, "256a", transparency, 256,536);
 //FBODRAWDEBUG(fbo256b, "256b", transparency, 256,600);
 //FBODRAWDEBUG(fbo256c, "256c", transparency, 256,666);
 //FBODRAWDEBUG(fbo256d, "256d", transparency, 256,730);
 //
 FBODRAWDEBUG(fbo1080p,  "1080p",  transparency, 256+128,465);
 FBODRAWDEBUG(fbo1080p2, "1080p2", transparency, 256+128,536);
 FBODRAWDEBUG(fbo1080p3, "1080p3", transparency, 256+128,600);
 FBODRAWDEBUG(fbo1080p4, "1080p4", transparency, 256+128,666);
 FBODRAWDEBUG(fbo1080p5, "1080p5", transparency, 256+128,730);
 FBODRAWDEBUG(fbo1080p6, "1080p6", transparency, 256+128,900);
 //
 FBODRAWDEBUG(fboC512,       "C512", transparency,512,74 );
 FBODRAWDEBUG(fboCDSScreen,  "CDSScreen", transparency,512,138);
 //FBODRAWDEBUG(fboCDSScreen3, "CDS3", transparency,512,202);
 //FBODRAWDEBUG(fboCDSScreen2, "CDS2", transparency,512,268);
 //FBODRAWDEBUG(fbo64,         "64",   transparency,512,465);

 FBODRAWDEBUG(fboCDS512,  "CDS12",  transparency,128+512,74 );
 FBODRAWDEBUG(fboCDS512a, "CDS5a", transparency,128+512,138);
 FBODRAWDEBUG(fboCDS512b, "CDS5b", transparency,128+512,202);
 FBODRAWDEBUG(fboCScreen, "Cs",    transparency,128+512,268);

 FBODRAWDEBUG(fboCDS1024,  "CDS1k",    transparency,256+512,74);
 FBODRAWDEBUG(fboCDS1024b, "CDS1kb",   transparency,256+512,138);
 FBODRAWDEBUG(fboCDS1024c, "CDS1kc",   transparency,256+512,202);
 FBODRAWDEBUG(fboCDS2048,  "CDS2k",    transparency,256+512,268);

 FBODRAWDEBUG(fboCDS2048, "screen",    transparency,256+512,332);
}

#include "Display.h"
#include "Interface.h"
extern Display display;
extern Interface input;

#define FBODEBUG(fbo,b,x,y) { if ( WITHIN(input.mxi,input.myi,(x),(y), (x)+64,(y)+64 ) ) { Area(crayons.Pick(green255),display.w-512,display.h-512,display.w,display.h); DrawBar( crayons.Pick(green255), 0.5f, display.w-512+128,display.h-512+128,display.w-128,display.h-128, 8 ); (fbo).blend=(b); (fbo).RenderUpsideDown(display.w-512,display.h-512,display.w,display.h); } }

void FBODebugKeys() { 
 FBODEBUG(fboScreen,  transparency,10, 74 );
 FBODEBUG(fboScreen1, transparency,10, 138);
 FBODEBUG(fboScreen2, transparency,10, 202);
 FBODEBUG(fboScreen3, transparency,10, 268);
 FBODEBUG(fboScreen4, transparency,10, 332);
 FBODEBUG(fboScreen5, transparency,10, 400);
 FBODEBUG(fboScreen6, transparency,10, 464);
 FBODEBUG(fboScreen7, transparency,10, 536);
 FBODEBUG(fboScreen8, transparency,10, 600);
 FBODEBUG(fboScreen9, transparency,10, 666);
 FBODEBUG(fboScreen10,transparency,10, 730);
 FBODEBUG(fboScreen11,transparency,128,74 );
 FBODEBUG(fboScreen12,transparency,128,138);
 //FBODEBUG(fboScreen13,transparency,128,202);
 //FBODEBUG(fboScreen14,transparency,128,268);
 //FBODEBUG(fboScreen15,transparency,128,332);
 //FBODEBUG(fboScreen16,transparency,128,400);

 //FBODEBUG(fboScreen1a, transparency,1024, 138);
 //FBODEBUG(fboScreen2a, transparency,1024, 202);
 //FBODEBUG(fboScreen3a, transparency,1024, 268);
 //FBODEBUG(fboScreen4a, transparency,1024, 332);
 //FBODEBUG(fboScreen5a, transparency,1024, 400);
 //FBODEBUG(fboScreen6a, transparency,1024, 464);
 //FBODEBUG(fboScreen7a, transparency,1024, 536);
 //FBODEBUG(fboScreen8a, transparency,1024, 600);

 FBODEBUG(fbo512,  transparency, 128,465);
 FBODEBUG(fbo512a, transparency, 128,536);
 FBODEBUG(fbo512b, transparency, 128,600);
 FBODEBUG(fbo512c, transparency, 128,666);
 FBODEBUG(fbo512d, transparency, 128,730);

 //FBODEBUG(fbo256,  transparency, 256,465);
 //FBODEBUG(fbo256a, transparency, 256,536);
 //FBODEBUG(fbo256b, transparency, 256,600);
 //FBODEBUG(fbo256c, transparency, 256,666);
 //FBODEBUG(fbo256d, transparency, 256,730);
 //
 //FBODEBUG(fbo128,  transparency, 256+128,465);
 //FBODEBUG(fbo128a, transparency, 256+128,536);
 //FBODEBUG(fbo128b, transparency, 256+128,600);
 //FBODEBUG(fbo128c, transparency, 256+128,666);
 //FBODEBUG(fbo128d, transparency, 256+128,730);
 
 FBODEBUG(fbo1080p,  transparency, 256+128,465);
 FBODEBUG(fbo1080p2, transparency, 256+128,536);
 FBODEBUG(fbo1080p3, transparency, 256+128,600);
 FBODEBUG(fbo1080p4, transparency, 256+128,666);
 FBODEBUG(fbo1080p5, transparency, 256+128,730);
 FBODEBUG(fbo1080p6, transparency, 256+128,900);

 FBODEBUG(fboC512,       transparency,512,74 );
 FBODEBUG(fboCDSScreen,  transparency,512,138);
 //FBODEBUG(fboCDSScreen3, transparency,512,202);
 //FBODEBUG(fboCDSScreen2, transparency,512,268);
 //FBODEBUG(fboScreen4,    transparency,512,332);
 //FBODEBUG(fbo64,         transparency,512,465);

 FBODEBUG(fboCDS512,  transparency,128+512,74 );
 FBODEBUG(fboCDS512a, transparency,128+512,138);
 FBODEBUG(fboCDS512b, transparency,128+512,202);
 FBODEBUG(fboCScreen, transparency,128+512,268);
 
 FBODEBUG(fboCDS1024, transparency,256+512,202);
 FBODEBUG(fboCDS2048, transparency,256+512,268);

 FBODEBUG(fboScreenWindow,  transparency,256+512,332);

}

void FBODebug() {
 FBODrawDebug();
 FBODebugKeys();
}
#endif


FBO *FBOfromString( const char *w ) {
 FBO *result=(FBO *) FBOColorfromString(w);
 if ( !result ) result=(FBO *) FBOColorDepthfromString(w);
 if ( !result ) result=(FBO *) FBOColorDepthStencilfromString(w);
 return result;
}

FBOColor *FBOColorfromString( const char *w ) {
 if ( !str_cmp(w,"C512") ) return &fboC512;
 if ( !str_cmp(w,"CScreen" ) ) return &fboCScreen;
 return null;
}

FBOColorDepth *FBOColorDepthfromString( const char *w ) {
 //if ( !str_cmp(w,"64") ) return &fbo64;
 //if ( !str_cmp(w,"128") ) return &fbo128;
 //if ( !str_cmp(w,"256") ) return &fbo256;
 if ( !str_cmp(w,"512") ) return &fbo512;
 if ( !str_cmp(w,"512a") ) return &fbo512a;
 if ( !str_cmp(w,"512b") ) return &fbo512b;
 if ( !str_cmp(w,"512c") ) return &fbo512c;
 if ( !str_cmp(w,"512d") ) return &fbo512d;
 //if ( !str_cmp(w,"256a") ) return &fbo256a;
 //if ( !str_cmp(w,"256b") ) return &fbo256b;
 //if ( !str_cmp(w,"256c") ) return &fbo256c;
 //if ( !str_cmp(w,"256d") ) return &fbo256d;
 //if ( !str_cmp(w,"128a") ) return &fbo128a;
 //if ( !str_cmp(w,"128b") ) return &fbo128b;
 //if ( !str_cmp(w,"128c") ) return &fbo128c;
 //if ( !str_cmp(w,"128d") ) return &fbo128d;
 if ( !str_cmp(w,"Screen") ) return &fboScreen;
 if ( !str_cmp(w,"Screen1") ) return &fboScreen1;
 if ( !str_cmp(w,"Screen2") ) return &fboScreen2;
 if ( !str_cmp(w,"Screen3") ) return &fboScreen3;
 if ( !str_cmp(w,"Screen4") ) return &fboScreen4;
 if ( !str_cmp(w,"Screen5") ) return &fboScreen5;
 if ( !str_cmp(w,"Screen6") ) return &fboScreen6;
 if ( !str_cmp(w,"Screen7") ) return &fboScreen7;
 if ( !str_cmp(w,"Screen8") ) return &fboScreen8;
 if ( !str_cmp(w,"Screen9") ) return &fboScreen9;
 if ( !str_cmp(w,"Screen10") ) return &fboScreen10;
 if ( !str_cmp(w,"Screen11") ) return &fboScreen11;
 if ( !str_cmp(w,"Screen12") ) return &fboScreen12;
 //if ( !str_cmp(w,"Screen13") ) return &fboScreen13;
 //if ( !str_cmp(w,"Screen14") ) return &fboScreen14;
 //if ( !str_cmp(w,"Screen15") ) return &fboScreen15;
 //if ( !str_cmp(w,"Screen16") ) return &fboScreen16;
 //if ( !str_cmp(w,"Screen1a") ) return &fboScreen1a;
 //if ( !str_cmp(w,"Screen2a") ) return &fboScreen2a;
 //if ( !str_cmp(w,"Screen3a") ) return &fboScreen3a;
 //if ( !str_cmp(w,"Screen4a") ) return &fboScreen4a;
 //if ( !str_cmp(w,"Screen5a") ) return &fboScreen5a;
 //if ( !str_cmp(w,"Screen6a") ) return &fboScreen6a;
 //if ( !str_cmp(w,"Screen7a") ) return &fboScreen7a;
 //if ( !str_cmp(w,"Screen8a") ) return &fboScreen8a;
 return null;
}

FBOColorDepthStencil *FBOColorDepthStencilfromString( const char *w ) {
 if ( !str_cmp(w,"CDS512") ) return &fboCDS512;
 if ( !str_cmp(w,"CDS512a") ) return &fboCDS512a;
 if ( !str_cmp(w,"CDS512b") ) return &fboCDS512b;
 if ( !str_cmp(w,"CDSScreen") ) return &fboCDSScreen;
 //if ( !str_cmp(w,"CDSScreen2") ) return &fboCDSScreen2;
 //if ( !str_cmp(w,"CDSScreen3") ) return &fboCDSScreen3;
 if ( !str_cmp(w,"CDS1024") ) return &fboCDS1024;
 if ( !str_cmp(w,"CDS1024b") ) return &fboCDS1024b;
 if ( !str_cmp(w,"CDS1024c") ) return &fboCDS1024c;
 if ( !str_cmp(w,"CDS2048") ) return &fboCDS2048;
 return null;
}



