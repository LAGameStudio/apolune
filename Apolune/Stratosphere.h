#pragma once
#include "Art.h"
#include "GLWindow.h"
#include "TextureLibrary.h"
#include "ForeverWrappingQuadLayers.h"
#include "LightningSystem2d.h"
#include "Audio.h"
#include "Globals.h"

class Stratosphere : public ListItem {
 Crayon cloudColor;
 ForeverWrappingQuadLayers fwql;
 Zp<FBOColor> clouds;
};


enum StormPhases {
 storm_sunny=0,
 storm_partly_cloudy=1,
 storm_light_rain=2,
 storm_thunder=3,
 storm_rain_and_snow=4,
 storm_snow=5,
 storm_blizzard=6,
 storm_deep_freeze=7
};

#define MAX_STORM 6

class Magnetosphere : public ListItem {
public:
 Zbool striking;
 Zdouble magnetics;
 GyratingLightningTree lightningTree;
 MovingPoints2d curved;
 LightningSystem2d lightning;
 MovingLightningSystem2d movinglightning;
 Zp<LightningCircuit2d> L;
 Zdouble time,dischargeRate,discharging;
 Zint seed;
 Crayon tint;
 void Seed( int seed ) {
  this->seed=seed;
  tint.Brights(seed+145);
  magnetics=randoms.Unique(seed,seed/2);
  dischargeRate=0.1+2.0*randoms.Unique(seed,seed/3);
  lightning.sq1=30.0*2.0*randoms.Unique(seed,seed+1);
  lightning.sq3=30.0*2.0*randoms.Unique(seed,seed+2);
  lightning.sh=30.0*2.0*randoms.Unique(seed,seed+3);
  lightning.circuit.diffuseA.Any(seed);
  lightning.circuit.diffuseB.Any(seed+1);
  lightning.circuit.A.Brights(seed+2);
  lightning.circuit.B.Brights(seed+3);
  lightning.circuit.tintA.Any(seed+5);
  lightning.circuit.tintB.Any(seed+6);
  lightning.circuit.core=randoms.Range(1,4,seed+30,seed+40);
  lightning.circuit.diffusion=randoms.Range(6,64,seed+10,seed+20);
  movinglightning.lightning.sq1=30.0*2.0*randoms.Unique(seed,seed+1);
  movinglightning.lightning.sq3=30.0*2.0*randoms.Unique(seed,seed+2);
  movinglightning.lightning.sh=30.0*2.0*randoms.Unique(seed,seed+3);
  movinglightning.lightning.circuit.diffuseA.Any(seed);
  movinglightning.lightning.circuit.diffuseB.Any(seed+1);
  movinglightning.lightning.circuit.A.Brights(seed+2);
  movinglightning.lightning.circuit.B.Brights(seed+3);
  movinglightning.lightning.circuit.tintA.Any(seed+5);
  movinglightning.lightning.circuit.tintB.Any(seed+6);
  movinglightning.lightning.circuit.core=randoms.Range(1,4,seed+30,seed+40);
  movinglightning.lightning.circuit.diffusion=randoms.Range(6,64,seed+10,seed+20);
 }
 void Between() {
  time+=FRAMETIME;
  if ( discharging > dischargeRate ) {
   Clear();
   discharging=0.0;
  } else discharging+=FRAMETIME;
  curved.Between();
  if ( striking ) lightningTree.Between();
  lightning.Between();
  movinglightning.Between();
//  DebugKeys();
 }
 void Clear() {
  lightning.Clear();
  movinglightning.Clear();
  curved.Clear();
 }
 void Impact( int type, double x, double y ) {
  audio.manager.CueAndPlayLimited(thunder_claps.AnyChar(),1,(float)uniform()+0.5f,0.8f,false,true);
  switch ( type ) {
  case 0:
   if ( !L ) {
    L=new LightningCircuit2d;
    L->ra.x=x;
    L->ra.y=y;
   } else {
    L->rb.x=x;
    L->rb.y=y;
    movinglightning.Clear();
    movinglightning.Create(L->ra.x,L->ra.y,L->rb.x,L->rb.y);
    delete L;
    L=null;
   }
  break;
  case 1:
  case 2:
   if ( !L ) {
    L=new LightningCircuit2d;
    L->ra.x=x;
    L->ra.y=y;
   } else {
    L->rb.x=x;
    L->rb.y=y;
    Line line(L->ra.x,L->ra.y,L->rb.x,L->rb.y);
    movinglightning.Clear();
    movinglightning.Create(&line,
     line.LineAngle()+HALF_PI,
     randoms.Unique(seed,seed+1)*HALF_PI,
     randoms.Unique(seed+2,seed)*512.0,
     randoms.Range(1,6,seed+321,seed+123),
     randoms.Unique(seed+2,seed+4)*0.75+0.15,
     0.5+randoms.Unique(seed+5,seed+4)*0.4,
     randoms.Range(2,7,seed+3,seed+6)
    );
    delete L;
    L=null;
   }
  break;
  case 3:
   if ( !L ) {
    L=new LightningCircuit2d;
    L->ra.x=x;
    L->ra.y=y;
   } else {
    L->rb.x=x;
    L->rb.y=y;
    lightning.Add(5,L->ra.x,L->ra.y,L->rb.x,L->rb.y);
    delete L;
    L=null;
   }
   break;
  case 4:
   {
    curved.Clear();
    Line segment(x,y,(uniform()*display.wd)-x,(uniform()*display.hd)-y);
    curved.Add( type%2==1 ? additive : transparency,
     3+(int)(magnetics*20.0),
     &segment, crayons.Any(seed), crayons.Any(seed) );
   }
   break;
  case 5: case 6:
//    lightningTree.segment.Set(x-1,y,x+1,y);
//   break;
  case 7: 
   lightning.Add(true,
    (magnetics*10.0)*10.0,
    x/((magnetics*10.0)*10.0),y/((magnetics*10.0)*10.0),
    x/((magnetics*10.0)*10.0),(y*2.0)/((magnetics*10.0)*10.0)
   );
   break;
  case 8: lightning.Add(5, x,y, 100.0, 100.0, x, y-75.0 );   break;
  case 9: lightning.Add(10, x, y, 100.0 );   break;
 case 10: lightning.Add(10, x, y, x, y-75.0, 100.0 ); break;
 default: lightning.Add(5, x,y, 
           64.0+magnetics*512.0*randoms.Unique(seed,seed),
           64.0+magnetics*512.0*randoms.Unique(seed+1,seed+1), x, y-randoms.Unique(seed+2,seed+2) );
   break;
  }
 }
 Zdouble angle;
 void Render() {
  fboScreen3.background.Pick(clear);
  fboScreen3.Bind();
  if ( striking ) lightningTree.Render();
  lightning.Render();
  movinglightning.Render();
  curved.Render();
  fboScreen3.Unbind();

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  dreamblurrgbShader.pTex0=&fboScreen3.texture;
  dreamblurrgbShader.UpdateUniforms();
  glPushMatrix();
  FullscreenQuadYInverted();
  glPopMatrix();
  dreamblurrgbShader.Disable();
  fboScreen5.Unbind();  
  
  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();  
  bloomShader.a=0.0004f;
  bloomShader.b=0.12f;
  bloomShader.c=0.09f;
  bloomShader.d=0.075f;
  bloomShader.e=0.3f;
  bloomShader.f=0.5f;
  bloomShader.g=0.125f;
  bloomShader.kernel1=4;
  bloomShader.kernel2=3;
  bloomShader.pTex0=&fboScreen5.texture;
  bloomShader.UpdateUniforms();
  FullscreenQuadYInverted();
  bloomShader.Disable(); 
  fboScreen1.Unbind();

  fboScreen1.blend=additive;
  fboScreen1.RenderUpsideDown(0,0,display.w,display.h);

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  sigmaV9gaussianShader.pTex0=&fboScreen3.texture;
  sigmaV9gaussianShader.UpdateUniforms();
  FullscreenQuadYInverted();
  sigmaV9gaussianShader.Disable();
  fboScreen5.Unbind();

  fboScreen4.background.Pick(clear);
  fboScreen4.Bind();
  sigmaH9gaussianShader.pTex0=&fboScreen3.texture;
  sigmaH9gaussianShader.UpdateUniforms();
  FullscreenQuadYInverted();
  sigmaH9gaussianShader.Disable();
  fboScreen4.Unbind();

  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();
  Blending(additive);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,fboScreen5.texture);
  FullscreenQuadYInverted();
  glBindTexture(GL_TEXTURE_2D,fboScreen4.texture);
  FullscreenQuadYInverted();
  glDisable(GL_TEXTURE_2D);
  fboScreen2.Unbind();

  fboScreen1.background.Pick(clear);
  fboScreen1.Bind();  
  bloomShader.a=0.0004f;
  bloomShader.b=0.12f;
  bloomShader.c=0.09f;
  bloomShader.d=0.075f;
  bloomShader.e=0.3f;
  bloomShader.f=0.5f;
  bloomShader.g=0.125f;
  bloomShader.kernel1=4;
  bloomShader.kernel2=3;
  bloomShader.pTex0=&fboScreen2.texture;
  bloomShader.UpdateUniforms();
  FullscreenQuadYInverted();
  bloomShader.Disable(); 
  fboScreen1.Unbind();

  fboScreen5.background.Pick(clear);
  fboScreen5.Bind();
  sigmaVgaussianShader.pTex0=&fboScreen1.texture;
  sigmaVgaussianShader.UpdateUniforms();
  FullscreenQuadYInverted();
  sigmaVgaussianShader.Disable();
  fboScreen5.Unbind();

  fboScreen4.background.Pick(clear);
  fboScreen4.Bind();
  sigmaHgaussianShader.pTex0=&fboScreen1.texture;
  sigmaHgaussianShader.UpdateUniforms();
  FullscreenQuadYInverted();
  sigmaH9gaussianShader.Disable();
  fboScreen4.Unbind();

  fboScreen2.background.Pick(clear);
  fboScreen2.Bind();
  Blending(additive);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,fboScreen5.texture);
  FullscreenQuadYInverted();
  glBindTexture(GL_TEXTURE_2D,fboScreen4.texture);
  FullscreenQuadYInverted();
  glDisable(GL_TEXTURE_2D);
  fboScreen2.Unbind();

  fboScreen1.blend=additive;
  fboScreen1.tint.fromCrayon(tint);
  fboScreen1.RenderUpsideDown(0,0,display.w,display.h);
 }
 void MoveBy( double x, double y ) {
  lightning.MoveBy(x,y);
  curved.MoveBy(x,y);
  movinglightning.MoveBy(x,y);
 }
};