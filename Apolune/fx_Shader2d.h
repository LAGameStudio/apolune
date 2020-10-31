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

#include "GLWindow.h"
#include "GLImage.h"
#include "GLSL2Shader.h"
#include "Crayon.h"
#include "Art.h"
#include "Proce55or.h"
#include "Tween.h"
#include "Cartesian.h"

#include "ClodShader.h"
#include "DeformerShader.h"
#include "FlyingShader.h"
#include "GlasscaveShader.h"
#include "LuxShader.h"
#include "MonjoriShader.h"
#include "MotionShader.h"
#include "NautilusShader.h"
#include "PlasmaShader.h"
#include "PulseShader.h"
#include "RadialShader.h"
#include "TVShader.h"
#include "TunnelShader.h"
#include "VortexShader.h"
#include "Shadows2DShader.h"
#include "SpherizeShader.h"
#include "WireframeShader.h"
#include "TweenBeam.h"
#include "LayerCombiner.h"
#include "StencilShader.h"
#include "Art2d.h"

/*
 * I observed: Sadly, this method is not usable.  It causes the keyboard to lock up.  We're not sure why.
 * It would have been convenient, but alas it is not possible.
 *
 * NOTE: Restarting Microsoft Visual C++ seemed to fix the issue.
 */

enum Shader2d {
 clod,
 deformer,
 glasscave,
 gliding,
 //lux,// does not seem to work properly
 monjori,
 motionblur,
 nautilus,
 plasmas,
 pulse,
 radial,
 television,
 tunnel,
 vortex,
 shadows2d,
 light2d,
 spherize
};

class fx_Shader2d : public Proce55or {
public:
 PROGRAMMABLE(fx_Shader2d,UpdatePtr,each,EachFrame);
 PROGRAMMABLE(fx_Shader2d,ActivatePtr,activate,Activate);
 PROGRAMMABLE(fx_Shader2d,DeactivatePtr,deactivate,Deactivate);
 PROGRAMMABLE(fx_Shader2d,RenderFramePtr,renderFrame,RenderFrame);
 GLuint *pTex0,*pTex1,*pTex2,*pTex3;
 Blends blend;
 Variable *tween_resolution, *tween_panning, *tween_time, *tween_rgba;
 Crayon tint,tint1,tint2,tint3;
 Cartesian position,offset,resolution,undulation;
 Zfloat size;
 Zfloat expired, delta; // updated elsewhere, set to presentation if you wish, use Tweak to change
 PROGRAMMABLE(fx_Shader2d,TimeEquationPtr,time,Time);
 Zfloat a,b;
 Zp<GLSLShader> active;
 fx_Shader2d() : Proce55or() {
  time=&fx_Shader2d::March;
  pTex0=pTex1=pTex2=pTex3=null;
  blend=additive;
  tween_resolution=tween_panning=tween_time=tween_rgba=null;
  delta=1.0f/FPS;
  b=1.0f;
 }
 void NoChange() {}
 void OscillateAB() {
  expired+=delta;
  if ( expired < a || expired > b ) delta=-delta;
 }
 void March() {
  expired+=delta;
 }
 void Oscillate( float A, float Start, float B, float d ) {
  expired=Start;
  A=a;
  B=b;
  delta=d;
  time=&fx_Shader2d::OscillateAB;
 }
 void Select( Shader2d s ) {
  tint.Pick(alabaster);
  position.Set(0.0f,0.0f);
  offset.Set(0.0f,0.0f);
  resolution.Set(512.0f,512.0f);
  undulation.Set(0.1f,0.05f);
  undulation.fz=0.1f;
  switch ( s ) {
   case clod:
     undulation.fx=0.1f;
     undulation.fy=0.05f;
     undulation.fz=0.1f;
     active=&clodShader;
     each=&fx_Shader2d::EachClod;
     activate=&fx_Shader2d::ClodActivate;
     deactivate=&fx_Shader2d::ClodDeactivate;
    break;
   case deformer:
     undulation.fx=0.0f;
     undulation.fy=0.0f;
     active=&deformerShader;
     each=&fx_Shader2d::EachDeformer;
     activate=&fx_Shader2d::DeformerActivate;
     deactivate=&fx_Shader2d::DeformerDeactivate;
    break;
   case glasscave:
     active=&glasscaveShader;
     each=&fx_Shader2d::EachGlasscave;
     activate=&fx_Shader2d::GlasscaveActivate;
     deactivate=&fx_Shader2d::GlasscaveDeactivate;
    break;
   case gliding:
     active=&flyingShader;
     each=&fx_Shader2d::EachGliding;
     activate=&fx_Shader2d::GlidingActivate;
     deactivate=&fx_Shader2d::GlidingDeactivate;
    break;
//   case lux: Turned off until position can be manipulated.
//     init=&fx_Shader2d::InitLux;
//     each=&fx_Shader2d::EachLux;
//    break;
   case motionblur:
     active=&motionShader;
     each=&fx_Shader2d::EachMotionblur;
     activate=&fx_Shader2d::MotionblurActivate;
     deactivate=&fx_Shader2d::MotionblurDeactivate;
    break;
   case monjori:
     renderFrame=&fx_Shader2d::RenderToFBO;
     tint1.Float(1.6f,2.0f,1.0f,1.0f);
     tint2.Float(1.3f,2.0f,1.0f,1.0f);
     tint3.Float(13.0f,8.0f,18.0f,1.0f);
     active=&monjoriShader;
     each=&fx_Shader2d::EachMonjori;
     activate=&fx_Shader2d::MonjoriActivate;
     deactivate=&fx_Shader2d::MonjoriDeactivate;
    break;
   case nautilus:
     active=&nautilusShader;
     each=&fx_Shader2d::EachNautilus;
     activate=&fx_Shader2d::NautilusActivate;
     deactivate=&fx_Shader2d::NautilusDeactivate;
    break;
   case plasmas:
     active=&plasmaShader;
     each=&fx_Shader2d::EachPlasma;
     activate=&fx_Shader2d::PlasmaActivate;
     deactivate=&fx_Shader2d::PlasmaDeactivate;
    break;
   case pulse:
     active=&pulseShader;
     each=&fx_Shader2d::EachPulse;
     activate=&fx_Shader2d::PulseActivate;
     deactivate=&fx_Shader2d::PulseDeactivate;
    break;
   case radial:
     active=&radialShader;
     each=&fx_Shader2d::EachRadial;
     activate=&fx_Shader2d::RadialActivate;
     deactivate=&fx_Shader2d::RadialDeactivate;
    break;
   case television:
     active=&tvShader;
     each=&fx_Shader2d::EachTelevision;
     activate=&fx_Shader2d::TelevisionActivate;
     deactivate=&fx_Shader2d::TelevisionDeactivate;
    break;
   case tunnel:
     active=&tunnelShader;
     each=&fx_Shader2d::EachTunnel;
     activate=&fx_Shader2d::TunnelActivate;
     deactivate=&fx_Shader2d::TunnelDeactivate;
    break;
   case vortex:
     active=&vortexShader;
     each=&fx_Shader2d::EachVortex;
     activate=&fx_Shader2d::VortexActivate;
     deactivate=&fx_Shader2d::VortexDeactivate;
    break;
   case shadows2d:
     active=&shadows2dShader;
     each=&fx_Shader2d::EachShadows2d;
     activate=&fx_Shader2d::Shadows2dActivate;
     deactivate=&fx_Shader2d::Shadows2dDeactivate;
    break;
   case light2d:
     active=&light2dShader;
     each=&fx_Shader2d::EachLight2d;
     activate=&fx_Shader2d::Light2dActivate;
     deactivate=&fx_Shader2d::Light2dDeactivate;
    break;
   case spherize:
     active=&spherizeShader;
     each=&fx_Shader2d::EachSpherize;
     activate=&fx_Shader2d::SpherizeActivate;
     deactivate=&fx_Shader2d::SpherizeDeactivate;
    break;
  }
 }
 void Setup() {
  x2=x+w;
  y2=y+h;
  Tweak();
  renderFrame=&fx_Shader2d::RenderFrameDefault;
 }
 virtual void Tweak() {}
 void EachClod() {
  clodShader.time=expired;
  clodShader.position[0]=position.fx;
  clodShader.position[1]=position.fy;
  clodShader.resolution[0]=resolution.fx;
  clodShader.resolution[1]=resolution.fy;
  clodShader.undulation[0]=undulation.fx;
  clodShader.undulation[1]=undulation.fy;
  clodShader.undulation[2]=undulation.fz;
 }
 void ClodActivate() { art.Stretch(&clodShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void ClodDeactivate() {}
 void EachDeformer() {
  if ( abs(position.fx) > 3000 && abs(position.fy) > 3000 ) {
   renderFrame=&fx_Shader2d::RenderNothing;
   return;
  }
  renderFrame=&fx_Shader2d::RenderFrameDefault;
  deformerShader.time=expired;
  deformerShader.rgbFactor[0]=tint.rf;
  deformerShader.rgbFactor[1]=tint.gf;
  deformerShader.rgbFactor[2]=tint.bf;
  deformerShader.position[0]=position.fx;
  deformerShader.position[1]=position.fy;
  deformerShader.resolution[0]=resolution.fx;
  deformerShader.resolution[1]=resolution.fy;
  deformerShader.mouse[0]=offset.fx;
  deformerShader.mouse[1]=offset.fy;
  deformerShader.pTex0=pTex0;
  deformerShader.textureOffset[0]=undulation.fx;
  deformerShader.textureOffset[1]=undulation.fy;
 }
 void DeformerActivate() { art.Stretch(&deformerShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void DeformerDeactivate() { }
 void EachGlasscave() {
  glasscaveShader.time=expired;
  glasscaveShader.position[0]=position.fx;
  glasscaveShader.position[1]=position.fy;
  glasscaveShader.position[2]=position.fz;
  glasscaveShader.resolution[0]=resolution.fx;
  glasscaveShader.resolution[1]=resolution.fy;
 }
 void GlasscaveActivate() { art.Stretch(&glasscaveShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void GlasscaveDeactivate() {}
 void EachGliding() {
  flyingShader.time=expired;
  flyingShader.position[0]=position.fx;
  flyingShader.position[1]=position.fy;
  flyingShader.resolution[0]=resolution.fx;
  flyingShader.resolution[1]=resolution.fy;
  flyingShader.pTex0=pTex0;
  flyingShader.pTex1=pTex1;
 }
 void GlidingActivate() { art.Stretch(&flyingShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void GlidingDeactivate() {}
 void EachMotionblur() {
  motionShader.time=expired;
  motionShader.position[0]=position.fx;
  motionShader.position[1]=position.fy;
  motionShader.resolution[0]=resolution.fx;
  motionShader.resolution[1]=resolution.fy;
  motionShader.mouse[0]=offset.fx;
  motionShader.mouse[1]=offset.fy;
  motionShader.pTex0=pTex0;
  motionShader.pTex1=pTex1;
 }
 void MotionblurActivate() { art.Stretch(&motionShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void MotionblurDeactivate() {}
 void EachMonjori() {
  monjoriShader.rgbFactor1[0]=tint1.rf;
  monjoriShader.rgbFactor1[1]=tint1.gf;
  monjoriShader.rgbFactor1[2]=tint1.bf;
  monjoriShader.rgbFactor2[0]=tint2.rf;
  monjoriShader.rgbFactor2[1]=tint2.gf;
  monjoriShader.rgbFactor2[2]=tint2.bf;
  monjoriShader.rgbFactor3[0]=tint3.rf;
  monjoriShader.rgbFactor3[1]=tint3.gf;
  monjoriShader.rgbFactor3[2]=tint3.bf;
  monjoriShader.time=expired;
  monjoriShader.position[0]=position.fx;
  monjoriShader.position[1]=position.fy;
  monjoriShader.resolution[0]=resolution.fx;
  monjoriShader.resolution[1]=resolution.fy;
 }
 void MonjoriActivate() { art.Stretch(&monjoriShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void MonjoriDeactivate() {}
 void InitNautilus() {}
 void EachNautilus() {
  nautilusShader.time=expired;
  nautilusShader.position[0]=position.fx;
  nautilusShader.position[1]=position.fy;
  nautilusShader.resolution[0]=resolution.fx;
  nautilusShader.resolution[1]=resolution.fy;
  nautilusShader.mouse[0]=offset.fx;
  nautilusShader.mouse[1]=offset.fy;
 }
 void NautilusActivate() { art.Stretch(&nautilusShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void NautilusDeactivate() {}
 void InitPlasma() {}
 void EachPlasma() {
  plasmaShader.time=expired;
  plasmaShader.position[0]=position.fx;
  plasmaShader.position[1]=position.fy;
  plasmaShader.resolution[0]=resolution.fx;
  plasmaShader.resolution[1]=resolution.fy;
 }
 void PlasmaActivate() { art.Stretch(&plasmaShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void PlasmaDeactivate() {}
 void InitPulse() {}
 void EachPulse() {
  pulseShader.pTex0=pTex0;
  pulseShader.time=expired;
  pulseShader.position[0]=position.fx;
  pulseShader.position[1]=position.fy;
  pulseShader.resolution[0]=resolution.fx;
  pulseShader.resolution[1]=resolution.fy;
 }
 void PulseActivate() { art.Stretch(&pulseShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void PulseDeactivate() {}
 void InitRadial() {}
 void EachRadial() {
  radialShader.rgbFactor[0]=tint.rf;
  radialShader.rgbFactor[1]=tint.gf;
  radialShader.rgbFactor[2]=tint.bf;
  radialShader.time=expired;
  radialShader.position[0]=position.fx;
  radialShader.position[1]=position.fy;
  radialShader.resolution[0]=resolution.fx;
  radialShader.resolution[1]=resolution.fy;
  radialShader.mouse[0]=offset.fx;
  radialShader.mouse[1]=offset.fy;
  radialShader.pTex0=pTex0;
 }
 void RadialActivate() { art.Stretch(&radialShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void RadialDeactivate() {}
 void InitTelevision() {
  resolution.fx=(float) w;
  resolution.fy=(float) h;
  position.fx=0.0f;
  position.fy=(float) -h;
 }
 void EachTelevision() {
  tvShader.time=expired;
  tvShader.pTex0=pTex0;
 }
 void TelevisionActivate() { art.Stretch(&tvShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void TelevisionDeactivate() {}
 void EachTunnel() {
  tunnelShader.time=expired;
  tunnelShader.position[0]=position.fx;
  tunnelShader.position[1]=position.fy;
  tunnelShader.resolution[0]=resolution.fx;
  tunnelShader.resolution[1]=resolution.fy;
  tunnelShader.pTex0=pTex0;
  tunnelShader.pTex1=pTex1;
 }
 void TunnelActivate() { art.Stretch(&tunnelShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void TunnelDeactivate() {}
 void InitVortex() {}
 void EachVortex() {
  vortexShader.time=expired;
  vortexShader.position[0]=position.fx;
  vortexShader.position[1]=position.fy;
  vortexShader.resolution[0]=resolution.fx;
  vortexShader.resolution[1]=resolution.fy;
  vortexShader.pTex0=pTex0;
  vortexShader.pTex1=pTex1;
 }
 void VortexActivate() { art.Stretch(&vortexShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void VortexDeactivate() {}
 void InitShadows2d() {}
 void EachShadows2d() {
  shadows2dShader.color.fromCrayon(tint);
  shadows2dShader.position[0]=position.fx;
  shadows2dShader.position[1]=position.fy;
  shadows2dShader.obstructions=pTex0;
 }
 void Shadows2dActivate() { art.Stretch(&shadows2dShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void Shadows2dDeactivate() {}
 void InitLight2d() {}
 void EachLight2d() {
  light2dShader.color.fromCrayon(tint);
  light2dShader.position[0]=position.fx;
  light2dShader.position[1]=position.fy;
  light2dShader.radius=a;
 }
 void Light2dActivate() { art.Stretch(&light2dShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void Light2dDeactivate() {}
 void InitSpherize() {}
 void EachSpherize() {
  spherizeShader.time=expired;
  spherizeShader.resolution[0]=resolution.fx;
  spherizeShader.resolution[1]=resolution.fy;
  spherizeShader.pTex0=pTex0;
 }
 void SpherizeActivate() { art.Stretch(&radialShader,(double)x,(double)y,(double)w,(double)h,false,false); }
 void SpherizeDeactivate() {}
 virtual void Update(){}
 void Between() {
  Update();
  Time();
  controller.Updatef();
  // use the tweens here
 }
 void Render() {
  EachFrame(); // Put here to ensure settings are set right before render. Calls "each"
  Blending(blend);
  art.Stretch(active,(double)x,(double)y,(double)w,(double)h);
#if defined(GL_DEBUG)
  GL_Assert( "fx_Shader2d::Render" );
  glReportError( glGetError() );
#endif
 }
 void RenderFrameDefault() {
#if defined(GL_DEBUG)
   GL_Assert( "fx_Shader2d::Render (entering)" );
   glReportError( glGetError() );
#endif
   Blending(blend);
   Activate();
#if defined(GL_DEBUG)
   GL_Assert( "fx_Shader2d::Render" );
   glReportError( glGetError() );
#endif
 } 
 void RenderToFBO() {
  EachFrame(); // Put here to ensure settings are set right before render. Calls "each"
#if defined(GL_DEBUG)
  GL_Assert( "fx_Shader2d::Render (entering)" );
  glReportError( glGetError() );
#endif
  fboScreen2.Bind();
  Blending(blend);
  art.Stretch(active,(double)x,(double)y,(double)w,(double)h);
  fboScreen2.Unbind();
  fboScreen2.blend=blend;
  fboScreen2.RenderUpsideDown(0,0,display.w,display.h);
#if defined(GL_DEBUG)
  GL_Assert( "fx_Shader2d::Render" );
  glReportError( glGetError() );
#endif
 }
 void RenderNothing() {}
 void Render( GLWindow *surface ) {
  EachFrame(); // Put here to ensure settings are set right before render. Calls "each"
  RenderFrame();
 }
};

void InitShaders();