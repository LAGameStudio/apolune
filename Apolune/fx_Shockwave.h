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
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "ShockwaveShader.h"
#include "fx_FireAndForget.h"

class Shockwaver : public Proce55or {
public:
 Zp<FBOColorDepth> source,destination;
 Zfloat expired,radius,curveA,curveB;
 Shockwaver( ) : Proce55or() { Reset(); }
 void Reset() {
  this->source=&fboScreen10;
  this->destination=&fboScreen11;
  radius=64;
  w=(int)(radius*2.0f);
  curveA=0.1f;
  curveB=0.2f;
  expired=0.0f;
  duration=8.0f;
  this->x=x;
  this->y=source->h-y;
 }
 Shockwaver( FBOColorDepth *from, FBOColorDepth *to, int x, int y, float r ) : Proce55or() {
  Reset();
  this->source=from;
  this->destination=to;
  radius=r;
  w=(int)(r*2.0f);
  curveA=0.1f;
  curveB=0.2f;
  expired=0.0f;
  duration=8.0f;
  this->x=x;
  this->y=source->h-y;
 }
 bool Done() {
  return (expired>duration);
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Render() {
  shockwaveShader.time=expired/(float)duration;
  shockwaveShader.shock[2]=radius;
  shockwaveShader.shock[1]=curveA;
  shockwaveShader.shock[2]=curveB;
  shockwaveShader.center[0]=iratiof(x,source->w);
  shockwaveShader.center[1]=iratiof(y,source->h);
  shockwaveShader.pTex0=&(source->texture);
  destination->Bind();
  destination->QuadYInverted(&shockwaveShader);
  destination->Unbind();
//  source->BindWithoutClearing();
  destination->Render(0,0,display.w,display.h);
 }
 void Render( GLWindow *surface ) {
  Render();
 }
};

extern Shockwaver shockwaver;

class fx_Shockwave : public FAFProce55or {
public:
 Zp<FBOColorDepth> buffer;
 Zfloat expired,radius,curveA,curveB;
 fx_Shockwave( ) : FAFProce55or() {
  this->buffer=&fboScreen10;
  radius=64;
  w=(int)(radius*2.0f);
  curveA=0.1f;
  curveB=0.2f;
  expired=0.0f;
  duration=8.0f;
  this->x=x;
  this->y=buffer->h-y;
 }
 fx_Shockwave( FBOColorDepth *buffer, int x, int y, float r ) : FAFProce55or() {
  this->buffer=buffer;
  radius=r;
  w=(int)(r*2.0f);
  curveA=0.1f;
  curveB=0.2f;
  expired=0.0f;
  duration=8.0f;
  this->x=x;
  this->y=buffer->h-y;
 }
 bool Done() {
  return (expired>duration);
 }
 void Between() {
  expired+=FRAMETIME;
 }
 void Render() {
  shockwaveShader.time=expired;
  shockwaveShader.shock[2]=radius;
  shockwaveShader.shock[1]=curveA;
  shockwaveShader.shock[2]=curveB;
  shockwaveShader.center[0]=iratiof(x,buffer->w);
  shockwaveShader.center[1]=iratiof(y,buffer->h);
  shockwaveShader.pTex0=&(buffer->texture);
  buffer->Unbind();
  fboScreen2.Bind();
  fboScreen2.QuadYInverted(&shockwaveShader);
  fboScreen2.Unbind();
  buffer->BindWithoutClearing();
  fboScreen2.Render(0,0,display.w,display.h);
 }
 void Render( GLWindow *surface ) {
  Render();
 }
};

class ShockwaveTest : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 GLImage *bg;
 float keyDelay;
 void OnLoad() {
  buffer.Create(display->w,display->h);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  bg=library.Load("data/images/encyclopedia/PIA03048.jpg");
  keyDelay=0.0f;
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  if ( input->KeyDown(DX_SPACE) && keyDelay<=0.0f ) {
   Add(new fx_Shockwave(&buffer,mx,my,64));
   keyDelay=0.1f;
  } else if (keyDelay>0.0f) keyDelay-=FRAMETIME;
 }
 void PreProcessing() {
  QuadiNoAlpha(bg,crayons.Pick(alabaster),none,0,0,w,h);
 }
};