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
#include "Art.h"
#include "Crayon.h"
#include "Proce55or.h"
#include "fx_FireAndForget.h"
#include "Shape.h"
#include "fx_Shader2d.h"

class SpilledLiquid2d : public FAFProce55or {
public:
 Crayon tint;
 Blends blend;
 float delta, expired, scale, spread, maxSpread, spreadRate, existing, evaporationDelay, evaporated, evaporation;
 bool evaporating;
 SpilledLiquid2d() : FAFProce55or() {
  spreadRate=0.8f;
  spread=0.0f;
  maxSpread=100.0f;
  evaporating=false;
  evaporated=0.0f;
  evaporation=20.0f;
  evaporationDelay=10.0f;
  delta=TWO_PIf/64.0f;
  scale=1.0f;
  expired=0.0f;
  tint.Float(0.5f,0.0f,0.0f,1.0f);
  blend=additive;
  existing=0.0f;
 }
 void Setup() {
 }
 bool Done() {
  return (evaporated>evaporation);
 }
 void Between() {
  if ( !evaporating ) {
   if ( spread < maxSpread ) {
    spread+=spreadRate;
    evaporating= ( spread >= maxSpread );
   } 
  } else {
   if ( existing < evaporationDelay ) {
    existing+=FRAMETIME;
   } else {
    evaporated+=FRAMETIME;
    spread=maxSpread*(1.0f-evaporated/evaporation);
   }
  }
 }
 void Render() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef((float)x,(float)y,0.0f);
  glScalef(spread/maxSpread*scale,spread/maxSpread*scale,1.0f);
  Blending(blend);
  //Crayon drawing(tint); // for some reason makes more sense without
  //drawing.ScaleByAlphaf( 1.0f-evaporated/evaporation );
  //glColor4fv(drawing.floats);
  glColor4fv(tint.floats);
  dlcircle2d.Render();
  glPopMatrix();
 }
 void Render( GLWindow *surface ) {
  Render();
 }
};

class ShadedSpilledLiquid2d : public SpilledLiquid2d {
public:
 Blends overlayBlend;
 fx_Shader2d shader; // Limit use of glasscave and other complex shaders with this one.
 Shader2d selected;
 bool whenmoving,overlayColor;
 void Setup() {
  shader.Select(selected);
  shader.Setup();
  shader.Between();
  whenmoving=true;
  overlayColor=true;
  overlayBlend=transparency;
 }
 void Between() {
  if ( !evaporating ) {
   if ( spread < maxSpread ) {
    spread+=spreadRate;
    evaporating= ( spread >= maxSpread );
   } 
  } else {
   if ( existing < evaporationDelay ) {
    existing+=FRAMETIME;
   } else {
    evaporated+=FRAMETIME;
    spread=maxSpread*(1.0f-evaporated/evaporation);
   }
  }
  if ( !whenmoving ) shader.Between();
 }
 void WhenMoved() {
  if ( whenmoving ) shader.Between();
 }
 void Render() {
  shader.Update(); // Put here to ensure settings are set right before render. Calls "each"
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef((float)x,(float)y,0.0f);
  glScalef(spread/maxSpread*scale,spread/maxSpread*scale,1.0f);
  Blending(blend);
  switch ( selected ) {
  case monjori:
    monjoriShader.position[0]=spread/maxSpread*scale;
    monjoriShader.position[1]=1.0f/spread/maxSpread*scale;
   break;
  case nautilus:
    nautilusShader.position[0]=spread/maxSpread*scale;
    nautilusShader.position[1]=1.0f/spread/maxSpread*scale;
   break;
  case plasmas:
    plasmaShader.position[0]=spread/maxSpread;
    plasmaShader.position[1]=1.0f/spread/maxSpread;
   break;
  case clod:
    clodShader.position[0]=spread/maxSpread*scale;
    clodShader.position[1]=1.0f/spread/maxSpread*scale;
   break;
  }
  shader.active->UpdateUniforms();
  dlcircle2d.Render();
  shader.active->disableShader();
  if ( overlayColor ) {
   Blending(overlayBlend);
   if ( overlayBlend == transparency ) {
    Crayon drawing(tint);
    drawing.Alpha(0.6f);
    glColor4fv(drawing.floats);
   } else
   glColor4fv(tint.floats);
   dlcircle2d.Render();
  }
  glPopMatrix();
  glPopAttrib();
 }
 void Render( GLWindow *t ) { Render(); }
};

class SpilledLiquidBubbler : public FAFProce55or {
public:
 int created,max;
 float counting,interval;
 void Setup() {
  counting=interval=0.25f;
  duration=5.0f;
  created=0;
  max=5;
 }
 virtual void Bubble() {
  int quantity=2+upto(3);
  for ( int i=0; i<quantity; i++ ) {
   ShadedSpilledLiquid2d *liq=new ShadedSpilledLiquid2d;
   float a=float_range(0.0f,TWO_PIf);
   liq->x2=liq->x=x+(int)((float)(128-upto(256))*cosf(a));
   liq->y2=liq->y=y+(int)((float)(128-upto(256))*sinf(a));
   liq->scale=float_range(1.0f/32.0f,1.0f/4.0f);
   liq->maxSpread=32;
   liq->evaporationDelay=0.0f;
   liq->evaporation=0.2f;
   liq->selected=monjori;
   liq->shader.resolution.Set(float_range(0.0f,256.0f),float_range(0.0f,256.0f));//rx,ry);
   liq->shader.position.Set(float_range(0.0f,256.0f),float_range(0.0f,256.0f));//rx,ry);
   liq->tint.Float(1.0f,0.0f,0.0f,1.0f);
   proce55ors->Add(liq);
   liq->Setup();
   liq->whenmoving=false;
   liq->overlayColor=true;
  }
 }
 void Between() {
  expired+=FRAMETIME;
  counting+=FRAMETIME;
  if ( counting > interval ) {
   counting=0.0f;
   if ( created < max ) { Bubble(); created++; }
  }
 }
};