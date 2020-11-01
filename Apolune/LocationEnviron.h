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
/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "StarNestShader.h"
#include "SpaceShaders.h"
#include "Interface.h"

#include "GameOptions.h"

extern Interface input;
//#include "ClassicPlanetSurface.h"

enum StarNestShaderTypes {
 sns_Simple=0,
 sns_Simple2=1,
 sns_Normal=2,
 sns_Normal2=3
};

ONE(LocationEnviron,{
 starNestType=sns_Simple;
 starNestSampler=library.Load("data/images/noise2.png",tllto_Tiling);
 electroTimeScale=1000.0;
 wandgBlend=additive;
 ecpBlend=additive;
 elcpBlend=additive;
 classicBlend=additive;
 starNestBlend=additive;
 wandgColor.Pick(alabaster);
 ecpColor.Pick(alabaster);
 elcpColor.Pick(alabaster);
 classicColor.Pick(alabaster);
 starNestColor.Pick(alabaster);
 bg.Pick(black);
})
 Zint surfaceSeed;
 Zbool planetary, classicPlanetSurface, starNest, emissionClouds, electricClouds, cloudsOnPlanet, rains, snows;
 WaterAndGroundProfile wandg;
 EmissionCloudsProfile ecp;
 ElectronCloudsProfile elcp;
 //NebulazProfile np;
 ClassicPlanetProfile classic;
 StarNestShaderTypes starNestType;
 Zp<GLImage> starNestSampler;
 Zdouble electroTimeScale;
 Crayon wandgColor,ecpColor,elcpColor,classicColor,starNestColor,bg;
 Blends wandgBlend,ecpBlend,elcpBlend,classicBlend,starNestBlend;
 ///
 Cartesiand snp;
 Zbool raining,snowing;
 Zdouble time;
// Zdis<ClassicPlanetSurface> surfacePreview; 
 WORDKEYS({
  SSUBKEY("wg",wandg)
  SSUBKEY("ecp",ecp)
  SSUBKEY("elcp",elcp)
  SSUBKEY("cpp",classic)
  TAGKEY("planetary",planetary)
  TAGKEY("classic",classicPlanetSurface)
  TAGKEY("starnest",starNest)
  TAGKEY("emission",emissionClouds)
  TAGKEY("electric",electricClouds)
  TAGKEY("clouds",cloudsOnPlanet)
  TAGKEY("rains",rains)
  TAGKEY("snows",snows)
  ENUMKEY("snt",starNestType)
  NUMBKEY("ets",electroTimeScale)
  TINTKEY("bg",bg)
  TINTKEY("wcol",wandgColor)
  TINTKEY("ecpcol",ecpColor)
  TINTKEY("elcpcol",elcpColor)
  TINTKEY("cpcol",classicColor)
  TINTKEY("sncol",starNestColor)
  BLENDKEY("wbl",wandgBlend)
  BLENDKEY("ecpbl",ecpBlend)
  BLENDKEY("elcpbl",elcpBlend)
  BLENDKEY("cpbl",classicBlend)
  BLENDKEY("snbl",starNestBlend)
 })
 KEYWORDS({
  SUBWORD("wg",wandg)
  else SUBWORD("ecp",ecp)
  else SUBWORD("elcp",elcp)
  else SUBWORD("cpp",classic)
  else TAGWORD("planetary",planetary)
  else TAGWORD("classic",classicPlanetSurface)
  else TAGWORD("starnest",starNest)
  else TAGWORD("emission",emissionClouds)
  else TAGWORD("electric",electricClouds)
  else TAGWORD("clouds",cloudsOnPlanet)
  else TAGWORD("rains",rains)
  else TAGWORD("snows",snows)
  else KEYWORD("snt",{starNestType=(StarNestShaderTypes)atoi(w);})
  else NUMWORD("ets",electroTimeScale)
  else TINTWORD("bg",bg)
  else TINTWORD("wcol",wandgColor)
  else TINTWORD("ecpcol",ecpColor)
  else TINTWORD("elcpcol",elcpColor)
  else TINTWORD("cpcol",classicColor)
  else TINTWORD("sncol",starNestColor)
  else BLENDWORD("wbl",wandgBlend)
  else BLENDWORD("ecpbl",ecpBlend)
  else BLENDWORD("elcpbl",elcpBlend)
  else BLENDWORD("cpbl",classicBlend)
  else BLENDWORD("snbl",starNestBlend)
 })
 void Set( LocationEnviron *in ) {
  planetary=in->planetary;
  classicPlanetSurface=in->classicPlanetSurface;
  starNest=in->starNest;
  emissionClouds=in->emissionClouds;
  electricClouds=in->electricClouds;
  cloudsOnPlanet=in->cloudsOnPlanet;
  rains=in->rains;
  snows=in->snows;
  wandg.Set(&in->wandg);
  ecp.Set(&in->ecp);
  elcp.Set(&in->elcp);
///  classic.Set(&in->classic);///
  raining=in->raining;
  snowing=in->snowing;
  electroTimeScale=in->electroTimeScale;
  wandgColor.fromCrayon(in->wandgColor);
  ecpColor.fromCrayon(in->ecpColor);
  elcpColor.fromCrayon(in->elcpColor);
  classicColor.fromCrayon(in->classicColor);
  starNestColor.fromCrayon(in->starNestColor);
  wandgBlend=in->wandgBlend;
  ecpBlend=in->ecpBlend;
  elcpBlend=in->elcpBlend;
  classicBlend=in->classicBlend;
  starNestBlend=in->starNestBlend;
 }
 void SetPosition( Cartesiand *pos ) {
  classic.pos2d.Set(pos);
  classic.pos2d.x/=display.wd;
  classic.pos2d.y/=display.hd;
  classic.pos2d.y=-classic.pos2d.y;
  wandg.pos2d.Set(pos);
//  wandg.pos2d.x=-wandg.pos2d.x;
  wandg.pos2d.y=-wandg.pos2d.y;
  wandg.pos2d.x/=(display.wd*10.0);
  wandg.pos2d.y/=(display.hd*10.0);
  snp.Set(pos);
  snp.Set((float)-pos->x,(float)-pos->y);
  snp.x/=display.wd;
  snp.y/=display.hd;
  elcp.pos2d.Set(pos);
  elcp.pos2d.y=-elcp.pos2d.y;
  elcp.pos2d.x/=display.wd*10.0;
  elcp.pos2d.y/=display.hd*10.0;
  ecp.pos2d.Set(pos);
  ecp.pos2d.y=-ecp.pos2d.y;
  ecp.pos2d.x/=display.wd/2.0;
  ecp.pos2d.y/=display.hd/2.0;
 }
 // Output is split into "fore" and "back" for layering behind and above the sprite layer.
 void RenderPreview( Cartesiand *position, int x, int y, int w, int h ) {
  time+=FRAMETIME;
  FBOColor *stack=&fbo512a;
  stack->background.Pick(clear);
  stack->tint.Pick(alabaster);
  stack->blend=none;
  FBOColor *out=&fbo512;
  out->background.fromCrayon(bg);
  out->tint.Pick(alabaster);
  out->blend=none;
  out->Bind(); out->Unbind(); // Clear the FBO
  if ( planetary ) { //wandg
   wandg.time+=FRAMETIME; //1.0+FRAMETIME; // no movement.
   ShadedWaterAndGround(stack,&wandg);
   stack->tint.fromCrayon(wandgColor); 
   out->BindWithoutClearing(); stack->RenderUpsideDown(wandgBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( classicPlanetSurface ) {
   stack->Bind();
   ShadedClassicPlanet(stack,&classic);
   stack->Unbind();
   stack->tint.fromCrayon(classicColor);
   out->BindWithoutClearing(); stack->RenderUpsideDown(classicBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( starNest && starNestSampler ) {
   stack->tint.fromCrayon(starNestColor);
   ShadedStarNestSimple2(FRAMETIME,(float)snp.x,(float)snp.y,1.0f,1.0f,stack);
   out->BindWithoutClearing(); stack->RenderUpsideDown(starNestBlend,0,0,out->w,out->h); out->Unbind();
  }
  if ( emissionClouds ) {
   ecp.time+=FRAMETIME;
   stack->tint.fromCrayon(ecpColor);
   ShadedEmissionClouds(stack,&ecp);
   out->BindWithoutClearing(); stack->RenderUpsideDown(ecpBlend,0,0,out->w,out->h); out->Unbind();
  }
  if ( electricClouds ) {
   elcp.time+=FRAMETIME/electroTimeScale;
   stack->tint.fromCrayon(elcpColor);
   ShadedElectronClouds(stack,&elcp);
   stack->blend=additive;
   out->BindWithoutClearing(); stack->RenderUpsideDown(elcpBlend,0,0,out->w,out->h); out->Unbind();
  }
  out->RenderUpsideDown(x,y,x+w,y+h);
 }
 // Output is split into "fore" and "back" for layering behind and above the sprite layer.
 void RenderPreview( Blends blend, Crayon tint, Cartesiand *position, int x, int y, int w, int h ) {
  glReportError(glGetError());
  time+=FRAMETIME;
  FBOColor *stack=&fbo512a;
  stack->background.Pick(clear);
  stack->tint.Pick(alabaster);
  stack->blend=none;
  FBOColor *out=&fbo512;
  out->background.fromCrayon(bg);
  out->tint.Pick(alabaster);
  out->blend=none;
  out->Bind(); out->Unbind(); // Clear the FBO
  if ( planetary ) { //wandg
   wandg.time+=FRAMETIME; //1.0+FRAMETIME; // no movement.
   ShadedWaterAndGround(stack,&wandg);
   stack->tint.fromCrayon(wandgColor); 
   out->BindWithoutClearing(); stack->RenderUpsideDown(wandgBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( classicPlanetSurface ) {
   stack->Bind();
   ShadedClassicPlanet(stack,&classic);
   stack->Unbind();
   stack->tint.fromCrayon(classicColor);
   out->BindWithoutClearing(); stack->RenderUpsideDown(classicBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( starNest && starNestSampler ) {
   stack->tint.fromCrayon(starNestColor);
   ShadedStarNestSimple2(FRAMETIME,(float)snp.x,(float)snp.y,1.0f,1.0f,stack);
   out->BindWithoutClearing(); stack->RenderUpsideDown(starNestBlend,0,0,out->w,out->h); out->Unbind();
  }
  if ( emissionClouds ) {
   ecp.time+=FRAMETIME;
   stack->tint.fromCrayon(ecpColor);
   ShadedEmissionClouds(stack,&ecp);
   out->BindWithoutClearing(); stack->RenderUpsideDown(ecpBlend,0,0,out->w,out->h); out->Unbind();
  }
  if ( electricClouds ) {
   elcp.time+=FRAMETIME/electroTimeScale;
   stack->tint.fromCrayon(elcpColor);
   ShadedElectronClouds(stack,&elcp);
   stack->blend=additive;
   out->BindWithoutClearing(); stack->RenderUpsideDown(elcpBlend,0,0,out->w,out->h); out->Unbind();
  }
  out->RenderUpsideDown(blend,tint,x,y,x+w,y+h);
 }
 FBOColor *RenderGround( bool includeNebula ) {
  time+=FRAMETIME;
  FBOColor *stack=/* !options.hiresShaders ? &fboCDS1024b :*/ &fbo1080p3; //&fbo512a;
  stack->background.Pick(clear);
  stack->tint.Pick(alabaster);
  stack->blend=none;
  FBOColor *out=/* !options.hiresShaders ? &fboCDS1024 :*/ &fbo1080p4; // &fbo512;
  out->background.fromCrayon(bg);
  out->tint.Pick(alabaster);
  out->blend=none;
  out->Bind(); out->Unbind(); // Clear the FBO
  if ( planetary ) { //wandg
   wandg.time+=FRAMETIME; //1.0+FRAMETIME; // no movement.
   ShadedWaterAndGround(stack,&wandg);
   stack->tint.fromCrayon(wandgColor); 
   out->BindWithoutClearing(); stack->RenderUpsideDown(wandgBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( classicPlanetSurface ) {
   stack->Bind();
   ShadedClassicPlanet(stack,&classic);
   stack->Unbind();
   stack->tint.fromCrayon(classicColor);
   out->BindWithoutClearing(); stack->RenderUpsideDown(classicBlend,0,0,out->w,out->h); out->Unbind();
  } else if ( starNest && starNestSampler ) {
   stack->tint.fromCrayon(starNestColor);
   ShadedStarNest(FRAMETIME,(float)snp.x,(float)snp.y,1.0f,1.0f,stack);
   out->BindWithoutClearing(); stack->RenderUpsideDown(starNestBlend,0,0,out->w,out->h); out->Unbind();
  }
  if ( !includeNebula ) {
   if ( emissionClouds ) {
    ecp.time+=FRAMETIME;
    stack->tint.fromCrayon(ecpColor);
    ShadedEmissionClouds(stack,&ecp);
    out->BindWithoutClearing(); stack->RenderUpsideDown(ecpBlend,0,0,out->w,out->h); out->Unbind();
   }
   if ( electricClouds ) {
    elcp.time+=FRAMETIME/electroTimeScale;
    stack->tint.fromCrayon(elcpColor);
    ShadedElectronClouds(stack,&elcp);
    stack->blend=additive;
    out->BindWithoutClearing(); stack->RenderUpsideDown(elcpBlend,0,0,out->w,out->h); out->Unbind();
   }
  }
  return out;
 }
 FBOColor *RenderSky( bool includeNebula ) {
  time+=FRAMETIME;
  FBOColor *stack= !options.hiresShaders ? &fboCDS1024b : &fbo1080p3; //&fbo512a;
  stack->background.Pick(clear);
  stack->tint.Pick(alabaster);
  stack->blend=transparency;
  FBOColor *out= !options.hiresShaders ? &fboCDS1024 : &fbo1080p2; // &fbo512;
  out->background.Pick(clear);
  out->tint.Pick(alabaster);
  out->blend=additive;
  out->Bind(); out->Unbind(); // Clear the FBO
  if ( includeNebula ) {
   if ( emissionClouds ) {
    ecp.time+=FRAMETIME;
    stack->tint.fromCrayon(ecpColor);
    ShadedEmissionClouds(stack,&ecp);
    out->BindWithoutClearing(); stack->RenderUpsideDown(ecpBlend,0,0,out->w,out->h); out->Unbind();
   }
   if ( electricClouds ) {
    elcp.time+=FRAMETIME/electroTimeScale;
    stack->tint.fromCrayon(elcpColor);
    ShadedElectronClouds(stack,&elcp);
    stack->blend=additive;
    out->BindWithoutClearing(); stack->RenderUpsideDown(elcpBlend,0,0,out->w,out->h); out->Unbind();
   }
  }
  return out;
 }
MANY(LocationEnviron,LocationEnvironHandle,LocationEnvironHandles,"le",LocationEnvirons,{})
 KEYSWORDSGroups("le",LocationEnviron)
DONE(LocationEnviron);