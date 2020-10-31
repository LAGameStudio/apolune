#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "FBO.h"
#include "PixelateShader.h"
#include "ConvolverShader.h"
#include "TVShader.h"
#include "BlendShader.h"
#include "BloomShader.h"
#include "GammaShader.h"
#include "ParallaxShader.h"

class PixelatedFBOTest : public GLWindow {
public:
 Zp<GLImage> pImg,pImg2;
 Zdouble time;
 void OnLoad() {
  Fullscreen();
  background.Pick(hotPink);
  pImg=library.Load("data/images/pixeltest.png");
  pImg2=library.Load("data/images/astro.png");
 }
 void Between() {
  time+=FRAMETIME;
 } 
 void Render() {
  fbo1080p.tint.Double(1.0,1.0,1.0,1.0);
  fbo1080p.Bind();
  Stretchi(pImg,crayons.Pick(alabaster),1.0f,none,0,0,pImg->width,pImg->height);
  Stretchi(pImg2,crayons.Pick(alabaster),1.0f,none,pImg->width,0,pImg2->width,pImg2->height);
  fbo1080p.Unbind();
  if ( input->KeyDown(DX_1) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen3.tint.Pick(alabaster);
   fboScreen3.background.Pick(clear);
   fboScreen3.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen2.tint.Double(1.0,1.0,1.0,0.2);
   fboScreen2.RenderUpsideDown(multiply,0,0,display->w,display->h);
   fboScreen2.tint.Double(1.0,1.0,1.0,0.3);
   fboScreen2.RenderUpsideDown(additive,0,0,display->w,display->h);
   fboScreen3.Unbind();
   ShadedGamma(&fboScreen3.texture,1.5,&fboScreen4);
   fboScreen4.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_2) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   OldTVProfile tvProfile;
   tvProfile.tex=&fboScreen.texture;
   tvProfile.time=time;
   ShadedOldTV(&fboScreen3,&tvProfile);
   fboScreen3.RenderUpsideDown(transparency,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_3) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen3.tint.Pick(alabaster);
   fboScreen3.background.Pick(clear);
   fboScreen3.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen2.RenderUpsideDown(multiply,0,0,display->w,display->h);
   fboScreen3.Unbind();
   ShadedBloom(&fboScreen3.texture,&fboScreen4);
   ShadedGamma(&fboScreen4.texture,1.5f,&fboScreen3);
   fboScreen3.tint.Pick(alabaster);
   fboScreen3.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_4) ) {
   Rectangle(crayons.Pick(black),0,0,display->w,display->h);
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   fboScreen2.background.Pick(clear);
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen3.Bind();
   fboScreen2.tint.Pick(alabaster);
   fboScreen2.RenderUpsideDown(none,0,0,display->w,display->h);
   fbo1080p.tint.Double(1.0,1.0,1.0,0.1);
   fbo1080p.RenderUpsideDown(additive,0,0,display->w*2,display->h*2);
   fboScreen3.Unbind();
   ShadedGamma(&fboScreen3.texture,0.75f,&fboScreen4);
   fboScreen4.tint.Pick(alabaster);
   fboScreen4.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_5) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen2.tint.Pick(alabaster);
   fboScreen2.background.Pick(clear);
   ShadedEmbossPreserveA(&fboScreen2.texture,&fboScreen3);
   fboScreen3.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_6) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen3.tint.Pick(alabaster);
   fboScreen3.background.Pick(clear);
   ShadedBlendAlphaMult(&fbo1080p.texture,&fboScreen2.texture,&fboScreen3);
   fboScreen3.background.Pick(clear);
   fboScreen3.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_7) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen3.tint.Pick(alabaster);
   fboScreen3.background.Pick(clear);
   fboScreen3.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen2.tint.Double(1.0,1.0,1.0,0.154);
   fboScreen2.RenderUpsideDown(additive,0,0,display->w,display->h);
   fboScreen3.Unbind();
   fboScreen3.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_8) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedSharpen(&fboScreen.texture,&fboScreen2);
   fboScreen2.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_9) ) {
   fboScreen.Bind();
   fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
   fboScreen.Unbind();
   ShadedBloom(&fboScreen.texture,&fboScreen2);
   fboScreen2.RenderUpsideDown(none,0,0,display->w,display->h);
  } else if ( input->KeyDown(DX_0) ) {
   ShadedHQ4X(&fbo1080p,&fboScreen);
   fboScreen.RenderUpsideDown(none,0,0,display->w,display->h);
  } else fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);

//  ShadedPixelate(&fboScreen,&fbo1080p.texture,input->mxd,input->myd); //fbo1080p.w,fbo1080p.h,(float)input->mxd*1920.0f*10.0f,(float)input->myd*1080.0f*10.0f);
//  fbo1080p.RenderUpsideDown(transparency,0,0,display->w*2,display->h*2);
 }
 void LinearNearest() {
  int dx=0;
  int dy=0;
  int dx2=input->mxi*10;
  int dy2=input->myi*10;
  glReportError( glGetError() );
  glEnable(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  Blending(transparency);
  glColor4fv(fbo1080p.tint.floats);
  glBindTexture(GL_TEXTURE_2D, fbo1080p.texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f,1.0f);     glVertex2i(dx,dy);
  glTexCoord2f(1.0f,1.0f);     glVertex2i(dx2,dy);
  glTexCoord2f(1.0f,0.0f);     glVertex2i(dx2,dy2);
  glTexCoord2f(0.0f,0.0f);     glVertex2i(dx,dy2);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glReportError( glGetError() );
 }
};

