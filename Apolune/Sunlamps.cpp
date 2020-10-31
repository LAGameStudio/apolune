#include "Sunlamps.h"

void Sunlamps::DebugKeys() {
  // Move the lightsources
 if ( input.KeyDown(DX_1) ) {
  if ( input.KeyDown(DX_J) ) {
   a.x-=FRAMETIME;
  }
  if ( input.KeyDown(DX_L) ) {
   a.x+=FRAMETIME;
  }
  if ( input.KeyDown(DX_K) ) {
   a.y-=FRAMETIME;
  }
  if ( input.KeyDown(DX_I) ) {
   a.y+=FRAMETIME;
  }
  if ( input.KeyDown(DX_O) ) {
   a.z-=FRAMETIME;
  }
  if ( input.KeyDown(DX_P) ) {
   a.z+=FRAMETIME;
  }
  if ( input.KeyDown(DX_U) ) {
   normalmappingalpha3lightShader.lightPower1+=FRAMETIME;
  }
  if ( input.KeyDown(DX_H) ) {
   normalmappingalpha3lightShader.lightPower1-=FRAMETIME;
  }
  if ( input.KeyDown(DX_Y) ) {
   normalmappingalpha3lightShader.lightColor1.Any();
  }
  if ( input.KeyDown(DX_SPACE) ) { normalmapping3lightShader.lightPosition1.Set(0.0f,0.0f,0.0f); }
 }

 if ( input.KeyDown(DX_2) ) {
  if ( input.KeyDown(DX_J) ) {
   b.x-=FRAMETIME;
  }
  if ( input.KeyDown(DX_L) ) {
   b.x+=FRAMETIME;
  }
  if ( input.KeyDown(DX_K) ) {
   b.y-=FRAMETIME;
  }
  if ( input.KeyDown(DX_I) ) {
   b.y+=FRAMETIME;
  }
  if ( input.KeyDown(DX_O) ) {
   b.z-=FRAMETIME;
  }
  if ( input.KeyDown(DX_P) ) {
   b.z+=FRAMETIME;
  }
  if ( input.KeyDown(DX_U) ) {
   normalmappingalpha3lightShader.lightPower2+=FRAMETIME;
  }
  if ( input.KeyDown(DX_H) ) {
   normalmappingalpha3lightShader.lightPower2-=FRAMETIME;
  }
  if ( input.KeyDown(DX_Y) ) {
   normalmappingalpha3lightShader.lightColor2.Any();
  }
 }

 if ( input.KeyDown(DX_3) ) {
  if ( input.KeyDown(DX_J) ) {
   c.x-=FRAMETIME;
  }
  if ( input.KeyDown(DX_L) ) {
   c.x+=FRAMETIME;
  }
  if ( input.KeyDown(DX_K) ) {
   c.y-=FRAMETIME;
  }
  if ( input.KeyDown(DX_I) ) {
   c.y+=FRAMETIME;
  }
  if ( input.KeyDown(DX_O) ) {
   c.z-=FRAMETIME;
  }
  if ( input.KeyDown(DX_P) ) {
   c.z+=FRAMETIME;
  }
  if ( input.KeyDown(DX_U) ) {
   normalmappingalpha3lightShader.lightPower3+=FRAMETIME;
  }
  if ( input.KeyDown(DX_H) ) {
   normalmappingalpha3lightShader.lightPower3-=FRAMETIME;
  }
  if ( input.KeyDown(DX_Y) ) {
   normalmappingalpha3lightShader.lightColor3.Any();
  }
 }
}