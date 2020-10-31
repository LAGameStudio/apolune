#pragma once
#include "Art.h"
#include "ParallaxShader.h"
#include "TextureLibrary.h"
#include "Tweens.h"

ONE(Sunlamps,{
 dayElapsed=0.5f;
 timeScale=0.01f; // speed of rotation / length of day
 combined=1.0f;
 a.Set(0.0f,0.5f,2.0f);
 b.Set(-3.0f,0.5f,2.0f);
 c.Set(3.0f,0.5f,2.0f);
 whiteTex.pointer=library.Load("data/images/elements/white.png",true,GL_REPEAT,GL_REPEAT);
})
 Zfloat elapsed,dayElapsed,timeScale;
 Vertex a,b,c;
 Crayon A,B,C;
 Zp<GLImage> whiteTex;
 Zp<FBOColor> heightNormal,waterSurfaceColormap;
 Zbool dynamic;
 Zp<Tween> cA,cB,cC;
 Zfloat combined;
 WORDKEYS({
  WORDKEY("name",name)
  NUMBKEY("dayElapsed",dayElapsed)
  NUMBKEY("timeScale",timeScale)
  SSUBKEY("va",a)
  SSUBKEY("vb",b)
  SSUBKEY("vc",c)
  TINTKEY("a",A)
  TINTKEY("b",B)
  TINTKEY("c",C)
  TAGKEY("dynamic",dynamic)
  TWEENKEY("ta",cA)
  TWEENKEY("tb",cB)
  TWEENKEY("tc",cC)
  NUMBKEY("combined",combined)
 })
 KEYWORDS({
  TXTWORD("name",name)
  else NUMWORD("dayElapsed",dayElapsed)
  else NUMWORD("timeScale",timeScale)
  else SSUBWORD("va",a)
  else SSUBWORD("vb",b)
  else SSUBWORD("vc",c)
  else TINTWORD("a",A)
  else TINTWORD("b",B)
  else TINTWORD("c",C)
  else TAGWORD("dynamic",dynamic)
  else TWEENWORD("ta",cA)
  else TWEENWORD("tb",cB)
  else TWEENWORD("tc",cC)
  else NUMWORD("combined",combined)
 })
 void Set( Sunlamps *in ) {
  elapsed=in->elapsed;
  dayElapsed=in->dayElapsed;
  timeScale=in->timeScale;
  a.Set(&in->a);
  b.Set(&in->b);
  c.Set(&in->c);
  A.fromCrayon(in->A);
  B.fromCrayon(in->B);
  C.fromCrayon(in->C);
  dynamic=in->dynamic;
 }
 void Render( FBOColor *target, FBOColor *colorMap, FBOColor *normalMap, FBOColor *lightMap ) {
  elapsed+=FRAMETIME;
  dayElapsed+=FRAMETIME*timeScale;
  if ( dayElapsed > 1.0f ) dayElapsed=0.0f;
  target->background.Pick(black);
  target->Bind();
  // Normal map our surface based on our heightNormal and solar positions
  glPush();
 	// Enable depth test
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT); // <- This had to be done for it to work.
 	// Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);//QUAL); //ESS); 
//  glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);
  Blending(transparency);
  normalmappingalpha3lightalmShader.lightColor1.fromCrayon(A);
  normalmappingalpha3lightalmShader.lightColor2.fromCrayon(B);
  normalmappingalpha3lightalmShader.lightColor3.fromCrayon(C);
  if ( dynamic ) {
   if ( cA ) normalmappingalpha3lightalmShader.lightColor1.ScaleByAlphaf(cA->tweenf(elapsed.value,true));
   if ( cB ) normalmappingalpha3lightalmShader.lightColor2.ScaleByAlphaf(cB->tweenf(elapsed.value,true));
   if ( cC ) normalmappingalpha3lightalmShader.lightColor3.ScaleByAlphaf(cC->tweenf(elapsed.value,true));
  }
  float solarOffset=dayElapsed*35.0f-15.0f;
  combined=absf(solarOffset)/15.0f;
  normalmappingalpha3lightalmShader.lightPosition1.Set(&a);
  normalmappingalpha3lightalmShader.lightPosition1.x+=solarOffset;
  normalmappingalpha3lightalmShader.lightPosition2.Set(&b);
  normalmappingalpha3lightalmShader.lightPosition2.x+=solarOffset;
  normalmappingalpha3lightalmShader.lightPosition3.Set(&c);
  normalmappingalpha3lightalmShader.lightPosition3.x+=solarOffset;
  normalmappingalpha3lightalmShader.camera.scale=glm::vec3(display.aspect,1.0f,1.0f);
  normalmappingalpha3lightalmShader.vbo=&vboQuadVNTTanBi;
  normalmappingalpha3lightalmShader.camera.MouseChangeOrientation(FRAMETIME);
  normalmappingalpha3lightalmShader.camera.position.x=0.5f*display.aspect;
  normalmappingalpha3lightalmShader.camera.position.y=0.5f;
  normalmappingalpha3lightalmShader.camera.position.z=1.0f+(display.aspect-1.0f)/2.0f;
  normalmappingalpha3lightalmShader.camera.direction.x=0.0f;
  normalmappingalpha3lightalmShader.camera.direction.y=0.0f;
  normalmappingalpha3lightalmShader.camera.direction.z=-1.0f;
  normalmappingalpha3lightalmShader.camera.right.x=1.0f;
  normalmappingalpha3lightalmShader.camera.right.y=0.0f;
  normalmappingalpha3lightalmShader.camera.right.z=0.0f;
  normalmappingalpha3lightalmShader.camera.up.x=0.0f;
  normalmappingalpha3lightalmShader.camera.up.y=1.0f;
  normalmappingalpha3lightalmShader.camera.up.z=0.0f;
  normalmappingalpha3lightalmShader.camera.UpdateMatrices();
  normalmappingalpha3lightalmShader.diffuse=&colorMap->texture;
  normalmappingalpha3lightalmShader.normal=&normalMap->texture;
  normalmappingalpha3lightalmShader.specular=&whiteTex->texture;
  normalmappingalpha3lightalmShader.alm=&lightMap->texture;
  normalmappingalpha3lightalmShader.UpdateUniforms();
  normalmappingalpha3lightalmShader.Disable();
  glPop();
  target->Unbind();
 }
 void DebugInfo() {
  MultilineText(
   FORMAT(buf,127,
    "light0: %f,%f,%f\n"
    "light1: %f,%f,%f\n"
    "light2: %f,%f,%f\n"
    "combined: %f\n"
    ,
    (float) normalmappingalpha3lightalmShader.lightPosition1.x,
    (float) normalmappingalpha3lightalmShader.lightPosition1.y,
    (float) normalmappingalpha3lightalmShader.lightPosition1.z,
    (float) normalmappingalpha3lightalmShader.lightPosition2.x,
    (float) normalmappingalpha3lightalmShader.lightPosition2.y,
    (float) normalmappingalpha3lightalmShader.lightPosition2.z,
    (float) normalmappingalpha3lightalmShader.lightPosition3.x,
    (float) normalmappingalpha3lightalmShader.lightPosition3.y,
    (float) normalmappingalpha3lightalmShader.lightPosition3.z,
    (float) combined
   ),128,10,10,10,2,false);
  Area(C,display.wd-10.0,display.hd-10.0,display.wd,     display.hd);
  Area(B,display.wd-20.0,display.hd-10.0,display.wd-10.0,display.hd);
  Area(A,display.wd-30.0,display.hd-10.0,display.wd-20.0,display.hd);
 }
 void DebugKeys();
MANY(Sunlamps,SunlampsHandle,SunlampsHandles,"Sunlamps",Sunlampses,{})
// unused //
DONE(Sunlamps);