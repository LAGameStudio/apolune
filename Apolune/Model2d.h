/***********************************************************************************************
 * OpenGL Window and Animation Library (formerly the Open Art Engine)                          *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once
#include "GLWindow.h"
#include "Cartesian.h"
#include "Vertex.h"
#include "LookAtPerspective.h"

#include "ModelSelector.h"

#include "fx_TexFont.h"
#include "fx_Object3d.h"
#include "fx_Buttons.h"
#include "fx_RGBAPal.h"

#include "Technology.h"

#include "UglyFont.h"

#include "fx_IconSelect.h"

class TechColorChanger : public fx_CoolRGBAPal4f {
public:
};

class TechTypeSelect : public fx_IconSelect { // used in ModelEdit, here.
public:
 bool addingTech;
 TechTypeSelect( int *binds, COMPONENT *c ) {
  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //
  duration=20.0f;
  addingTech=false;
//  items.Push("data/images/modeledit/joint.png",     (int)_JOINT);   
  switch ( (ComponentTypes) ((int) c->type) ) {
   case _HARDPOINT_GUN:
     items.Push("data/images/modeledit/emitter.png",   (int)_EMITTER);
     Select((int)_EMITTER);
    break;
   case _HARDPOINT_LAUNCHER:
     items.Push("data/images/modeledit/emitter.png",   (int)_EMITTER);
     Select((int)_EMITTER);
    break;
   case _HULL:
     items.Push("data/images/modeledit/hardpoint.png", (int)_HARDPOINT);
     items.Push("data/images/modeledit/coupler.png",   (int)_COUPLER);
     items.Push("data/images/modeledit/lightbulb.png", (int)_LIGHT);
     Select((int)_COUPLER);
    break;
   case _FOUNDATION:
     items.Push("data/images/modeledit/emitter.png",   (int)_EMITTER);
     items.Push("data/images/modeledit/hardpoint.png", (int)_HARDPOINT);
     items.Push("data/images/modeledit/lightbulb.png", (int)_LIGHT);
     Select((int)_LIGHT);
    break;
   case _HARDPOINT_TRACTOR_BEAM:
     items.Push("data/images/modeledit/emitter.png",   (int)_EMITTER);
     Select((int)_EMITTER);
    break;
   case _COMMODITY:
     items.Push("data/images/modeledit/lightbulb.png", (int)_LIGHT);
     Select((int)_LIGHT);
    break;
   case _LOCOMOTION:
     items.Push("data/images/modeledit/emitter.png",   (int)_EMITTER);
     Select((int)_EMITTER);
    break;
   default:
     items.Push("data/images/modeledit/emitter.png",   (int)_LIGHT);
     Select((int)_LIGHT);
     break;
  }
  Bind(binds);
  click.Pick(red255);
  hover.Pick(blue);
  normal.Int(26,13,7,0);
  t_normal.Pick(alabaster);
  f_normal.Pick(chocolate);
  click.Pick(red255);
  t_click.Pick(alabaster);
  f_click.Pick(red255);
  hover.Pick(green);
  t_hover.Pick(alabaster);
  f_hover.Pick(green); 
  backing = library.find( "data/images/elements/rounder1.png");
  glow_c  = library.find( "data/images/elements/rounder4.png");
  glow_h  = library.find( "data/images/elements/rounder4.png");
  glow_n  = library.find( "data/images/elements/rounder0.png");
  controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                 // OMFG ASCII Space ship!
  controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
  controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
  controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
  controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
  delay=0.0f;
  duration=20.0f;
 }
 void WhenClicked() {
  if ( !addingTech ) {
   addingTech=true;
   SetPrior();
  }
  else
  switch ( *binding ) {
  case _NONE:
   break;
  case _COUPLER:
   break;
  case _HARDPOINT:
   break;
  case _JOINT:
   break;
  case _LIGHT:
   break;
  case _EMITTER:
   break;
  }
 }
};

class LightTypeSelect : public fx_CoolToggle {
 void Customize() {
  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //
  icon =  library.find("data/images/modeledit/spot.png");
  ledOn=  library.find("data/images/elements/rounder1.png");
  ledOff= library.find("data/images/elements/rounder_box.png");
  click.Pick(red255);
  hover.Pick(blue);
  normal.Int(26,13,7,0);
  t_normal.Pick(alabaster);
  f_normal.Pick(chocolate);
  click.Pick(red255);
  t_click.Pick(alabaster);
  f_click.Pick(red255);
  hover.Pick(green);
  t_hover.Pick(alabaster);
  f_hover.Pick(green); 
  led.Pick(orangeRed);
  backing = library.find( "data/images/elements/rounder1.png");
  glow_c  = library.find( "data/images/elements/rounder4.png");
  glow_h  = library.find( "data/images/elements/rounder4.png");
  glow_n  = library.find( "data/images/elements/rounder0.png");
  controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                 // OMFG ASCII Space ship!
  controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
  controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
  controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
  controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
  delay=0.0f;
  duration=20.0f;
 }
};

class VerticalJointToggle : public fx_CoolToggle {
 void Customize() {
  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //
  icon =  library.find("data/images/modeledit/vert.png");
  ledOn=  library.find("data/images/elements/rounder1.png");
  ledOff= library.find("data/images/elements/rounder_box.png");
  click.Pick(red255);
  hover.Pick(blue);
  normal.Int(26,13,7,0);
  t_normal.Pick(alabaster);
  f_normal.Pick(chocolate);
  click.Pick(red255);
  t_click.Pick(alabaster);
  f_click.Pick(red255);
  hover.Pick(green);
  t_hover.Pick(alabaster);
  f_hover.Pick(green); 
  led.Pick(orangeRed);
  backing = library.find( "data/images/elements/rounder1.png");
  glow_c  = library.find( "data/images/elements/rounder4.png");
  glow_h  = library.find( "data/images/elements/rounder4.png");
  glow_n  = library.find( "data/images/elements/rounder0.png");
  controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                 // OMFG ASCII Space ship!
  controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
  controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
  controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
  controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
  delay=0.0f;
  duration=20.0f;
 }
};
class HorizJointToggle : public fx_CoolToggle {
 void Customize() {
  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //
  icon =  library.find("data/images/modeledit/horiz.png");
  ledOn=  library.find("data/images/elements/rounder1.png");
  ledOff= library.find("data/images/elements/rounder_box.png");
  click.Pick(red255);
  hover.Pick(blue);
  normal.Int(26,13,7,0);
  t_normal.Pick(alabaster);
  f_normal.Pick(chocolate);
  click.Pick(red255);
  t_click.Pick(alabaster);
  f_click.Pick(red255);
  hover.Pick(green);
  t_hover.Pick(alabaster);
  f_hover.Pick(green); 
  led.Pick(orangeRed);
  backing = library.find( "data/images/elements/rounder1.png");
  glow_c  = library.find( "data/images/elements/rounder4.png");
  glow_h  = library.find( "data/images/elements/rounder4.png");
  glow_n  = library.find( "data/images/elements/rounder0.png");
  controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                 // OMFG ASCII Space ship!
  controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
  controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
  controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
  controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
  delay=0.0f;
  duration=20.0f;
 }
};

class ArticulatingJointToggle : public fx_CoolToggle {
 void Customize() {
  Tween
   *pulsing=tweens.find("Humanized Gradual Rise"),                                        //
   *gyrating=tweens.find("Humanized In Out Slow"),                       ///   <Capn        //
   *flashing=tweens.find("Humanized Wobble Rise");                     //////                 //
  icon =  library.find("data/images/modeledit/articulating.png");
  ledOn=  library.find("data/images/elements/rounder1.png");
  ledOff= library.find("data/images/elements/rounder_box.png");
  click.Pick(red255);
  hover.Pick(blue);
  normal.Int(26,13,7,0);
  t_normal.Pick(alabaster);
  f_normal.Pick(chocolate);
  click.Pick(red255);
  t_click.Pick(alabaster);
  f_click.Pick(red255);
  hover.Pick(green);
  t_hover.Pick(alabaster);
  f_hover.Pick(green); 
  led.Pick(orangeRed);
  backing = library.find( "data/images/elements/rounder1.png");
  glow_c  = library.find( "data/images/elements/rounder4.png");
  glow_h  = library.find( "data/images/elements/rounder4.png");
  glow_n  = library.find( "data/images/elements/rounder0.png");
  controller.variables.NewfT("chi",  0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("clo",  0.0f, 255.0f, 0.0625f,flashing);                 // OMFG ASCII Space ship!
  controller.variables.NewfT("ct",   0.0f, 255.0f, 0.0625f,flashing);
  controller.variables.NewfT("hhi",127.0f, 255.0f, 0.0625f,pulsing); 
  controller.variables.NewfT("hlo",  0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("ht",   0.0f, 255.0f, 1.0f,gyrating);
  controller.variables.NewfT("hi",   0.0f, 255.0f, 0.0f,null); //////////////////////
  controller.variables.NewfT("lo",   0.0f, 255.0f, 1.0f,null); 
  controller.variables.NewfT("t",    0.0f, 255.0f, 0.0f,null); ///////////////////
  delay=0.0f;
  duration=20.0f;
 }
};

class Model2d : public GLWindow {
public:
 TechTypeSelect *techtypes;
// LightTypeSelect *lighttypes;
// ArticulatingJointToggle *articulating;
// HorizJointToggle *horiz;
// VerticalJointToggle *vert;
 Cartesian center;
 ModelSelector *ms;
 Model2d *cousin;
 Crayon grid,grid2,lines,select,hover,money;
 Matters *matters;
 Techs *techs;
 Matter *hovering, *selected;
 Tech *tech_selected, *tech_hovering, *moving_tech;
 GLImage *arrowhead;
 ViewModes viewing; // top or side
 int mx,my;
 int couplerCount,lightCount,emitterCount,jointCount,hardpointCount;
 bool dragging,ALT,CTRL,SHIFT;
 float zoom;
 float leftUpDelay,rightUpDelay;
 bool ignoreLeftUp;
 bool clicked;
 LookAtPerspective camera;
// ImmediateLights lights;

 Model2d() : GLWindow() {
  techtypes=null;
  ms=null;
  cousin=null;
  matters=null;
  techs=null;
  ResetGUI();
 }

 void ResetGUI() {
  if ( techtypes ) techtypes->addingTech=false;
  moving_tech=null;
  selected=null;
  hovering=null;
  tech_hovering=null;
  tech_selected=null;
 }

 void OnLoad() {
  background.Int(8,8,64,196);
  grid.Int(32,32,96,32);  
  grid2.fromCrayon(grid); grid2.ScaleByAlphaf(0.5f);
  lines.Int(255,255,255,127);
  select.Pick(cyan);
  hover.Pick(orange);
  money.Pick(green);
  cursor=false;
  hovering=selected=null;
  moving_tech=tech_selected=tech_hovering=null;
  zoom=0.1f;
  leftUpDelay=0.0f;
  rightUpDelay=0.0f;
  ignoreLeftUp=true;
  clicked=false;
  ALT=CTRL=SHIFT=false;
  arrowhead=library.find("data/images/elements/arrowhead.png");
  camera.Near=0.0001;
  camera.Far=60.0;
  camera.halfFOV=55.0;
  camera.eye.Set( 0, 0, 10 );
  camera.center.Set( 0, 0, -50 );
  camera.ortho=true;
  camera.extents.SetCorners(-5.0,5.0,-5.0,5.0);
 }

 void Recenter() {
  x2=x+w;
  y2=y+h;
  center.Set(x+w/2,y+h/2);
 }

 // Find out where we are in normalized space (-0.5f to 0.5f)
 float fx,fy,fyinv,fxinv;
 Cartesian zoomed;
 void Translate() {
  MousePosition(&mx,&my);
     fx=iratiof(mx,w)-0.5f,
     fy=iratiof(my,h)-0.5f,
  fyinv=iratiof(h-my,h)-0.5f;
  fxinv=iratiof(w-mx,w)-0.5f;
  zoomed.fx=fx/zoom;
  zoomed.fy=fy/zoom;
  zoomed.fh=fyinv/zoom;
  zoomed.x=(int)(fx*(float)w2);
  zoomed.y=(int)(fy*(float)h2);
  zoomed.h=(int)(fyinv*(float)h2);
 }

 int mtx,mty;
 void DetectHoveringMatter() {
  mtx=(int)((float)fboCDSScreen.w*iratiof(input->mxi-x,w));
  mty=(int)((float)fboCDSScreen.h*iratiof(input->myi-y,h));
  mtx=fboCDSScreen.w-mtx;
  Vertexd rotation; 
  switch ( viewing ) {
   case LEFT: rotation.x=90.0; break;
   case TOP: break;
  }
  if ( mtx > 0 && mtx < fboCDSScreen.w && mty > 0 && mty < fboCDSScreen.h )
   hovering=matters->RenderColorCodedTest(&camera,null,&rotation,mtx,mty,&fboCDSScreen);
  else hovering=null;
 }

 void Between() {
  if ( !visible ) return;
  ALT=input->KeyDown(DX_LALT) || input->KeyDown(DX_RALT);
  CTRL=input->KeyDown(DX_LCONTROL) || input->KeyDown(DX_RCONTROL);
  SHIFT=input->KeyDown(DX_LSHIFT) || input->KeyDown(DX_RSHIFT);
  if ( leftUpDelay > 0.0f )  leftUpDelay-=FRAMETIME;
  if ( rightUpDelay > 0.0f ) rightUpDelay-=FRAMETIME;
  Translate();
  DetectHoveringMatter();
 }
 
 Crayon drawing;

 void DrawPrimitives() {
  fboScreen.Bind();
  camera.Apply();
  glMatrixMode(GL_MODELVIEW);
  switch ( viewing ) {
   case LEFT:
     glRotated(90.0,1.0,0.0,0.0);
    break;
   case TOP: /* do nothing */ break;
  }
  matters->Render();
  if ( !tech_selected ) {
   if ( hovering || (cousin&&cousin->hovering) ) {
    glDisable(GL_TEXTURE_2D);
    Blending(none);
    glColor4fv(hover.floats);
    if ( hovering ) hovering->RenderColorCoded();
    else if ( cousin && cousin->hovering ) cousin->hovering->RenderColorCoded();
   } else
   if ( selected || (cousin&&cousin->selected) ) {
    glDisable(GL_TEXTURE_2D);
    Blending(none);
    glColor4fv(select.floats);
    if ( selected ) selected->RenderColorCoded();
    else if ( cousin && cousin->selected ) cousin->selected->RenderColorCoded();
   }
  }
  fboScreen.Unbind();
  fboScreen.blend=additive;
  switch ( viewing ) {
   case LEFT: fboScreen.RenderReversed(x,y,x2,y2); break;
   case TOP: fboScreen.RenderReversed(x,y,x2,y2); break;
  }
 }

 void DrawTechs() {
  // Detect hovering over placed tech
  tech_hovering=null;
  int mx00=mx-w2; // Mouse position in screen space,
  int my00=my-h2; // window-relative (center is origin).
  // Draw the techs
  couplerCount=lightCount=emitterCount=jointCount=hardpointCount=0;
  EACH(techs->first,Tech,t) {
   GLImage *ico;
   switch ( t->type ) {
    case _NONE:      ico=library.find("data/images/icon/weapon/moves/null.png"); break;
    case _EMITTER:   ico=library.find("data/images/modeledit/emitter.png");      emitterCount++; break;
    case _COUPLER:   ico=library.find("data/images/modeledit/coupler.png");      couplerCount++; break;
    case _HARDPOINT: ico=library.find("data/images/modeledit/hardpoint.png");    hardpointCount++; break;
    case _JOINT:     ico=library.find("data/images/modeledit/joint.png");        jointCount++; break;
    case _LIGHT:     ico=t->spot ? library.find("data/images/modeledit/spot.png")
                                 : library.find("data/images/modeledit/lightbulb.png"); lightCount++;
                     break;
   }
   int tx,ty,tx2,ty2;
   switch ( viewing ) {
    case LEFT:
      tx=(int)-(t->source.x/5.0*(double)w2),
      ty=(int)-(t->source.z/5.0*(double)h2);
      tx2=tx+(int)(t->magnitude.x/5.0*(double)w2),
      ty2=ty+(int)(t->magnitude.z/5.0*(double)h2);
     break;
    case TOP:
      tx=(int)-(t->source.x/5.0*(double)w2),
      ty=(int)((t->source.y/5.0)*(double)h2);
      tx2=tx+(int)(t->magnitude.x/5.0*(double)w2),
      ty2=ty+(int)(t->magnitude.y/5.0*(double)h2);
     break;
   }
   if ( idistance(tx,ty,mx00,my00) <= 16+2 ) { tech_hovering=t; }
   if ( t == tech_selected ) drawing.Pick(green);
   else if ( t == tech_hovering ) drawing.Pick(magenta);
   else drawing.Pick(alabaster);
   int txw2=tx+w2,
       tyh2=ty+h2,
       tx2w2=tx2+w2,
       ty2h2=ty2+h2;
   Rectangle(this,drawing,txw2-16,tyh2-16,txw2+16,tyh2+16);
   StretchiNoAlpha(this,ico,
    (t->type==_LIGHT || t->type==_EMITTER) 
     ? t->color
     : drawing,additive,
    txw2-16,
    tyh2-16,
    32,32
   );
  }
 }

 void Render() {
  Vertexd vertex;
  int i,j;

  Blending(additive);
  // Draw a simple grid
  for ( i=0; i<10; i++ ) {
   for ( j=0; j<10; j++ ) {
    DrawLine(this,grid,
     0,(j*(h/10)),
     w,(j*(h/10)));
   }
   DrawLine(this,grid2,
    (i*(w/10)),0,
    (i*(w/10)),h);
  }
  // Draw the bisection line (ground / middle)
  DrawLine(this,crayons.Pick(gray),0,h2,w,h2);
  if ( input->KeyDown(DX_X) ) {   
   fboCDSScreen.Render(5,5,5+512,5+512);
   if ( mtx > 0 && mtx < fboCDSScreen.w && mty > 0 && mty < fboCDSScreen.h ) {
    Blending(none);
    DrawLine(crayons.Pick(alabaster),
     5+(int)(iratiof(mtx,fboCDSScreen.w)*512.0f),
     5+(int)(0),
     5+(int)(iratiof(mtx,fboCDSScreen.w)*512.0f),
     5+(int)(512)
    );
    DrawLine(crayons.Pick(alabaster),
     5+(int)(0),
     5+(int)(iratiof(mty,fboCDSScreen.h)*512.0f),
     5+(int)(512),
     5+(int)(iratiof(mty,fboCDSScreen.h)*512.0f)
    );
   }
  }
  DrawPrimitives();

  drawing.Pick(alabaster);
  glColor4fv( drawing.floats );
  Text( viewing == TOP ? "Top Down" : viewing == LEFT ? "From Right" : "From Left", x+20, y+20, 7, 10, false );
  switch ( viewing ) {
  case LEFT: Text( "Front -> Top ^", x+20, y2-10, 7, 10, false ); break;
  case TOP: Text( "Front ->", x+20, y2-10, 7, 10, false ); break;
  }
  
  DrawTechs();

  // Draw the 'cursor'
  DrawLine(this,select,mx,0,mx,h);
  DrawLine(this,select,0,my,w,my);
 }

 void MoveMatter() {
  Matter *m=selected;
  switch ( viewing ) {
   case 0: // Left
     if (!ALT)  m->position.x=(-fx/zoom);
     if (!CTRL) m->position.z=(fyinv/zoom);
    break;
   case 1: // Top
     if (!ALT)  m->position.x=(-fx/zoom);
     if (!CTRL) m->position.y=(fy/zoom);
    break;
   case 2: // Right
    break;
  }
 }
 
 void MoveTech() {
  if ( !tech_hovering ) return;
  moving_tech=tech_hovering;
   switch ( viewing ) {
    case LEFT: // Left
      moving_tech->source.x=-(fx/zoom);
      moving_tech->source.z=(fyinv/zoom);
     break;
    case TOP: // Top
      moving_tech->source.x=(-fx/zoom);
      moving_tech->source.y=(fy/zoom);
     break;
   }
   switch ( tech_hovering->type ) {
     case _NONE:      break;
     case _EMITTER:   break;
     case _COUPLER:   break;
     case _HARDPOINT: break;
     case _JOINT:     break;
     case _LIGHT:     break;
   }
 }
 
 void ScaleMatterBigger() {
  fx=iratiof(mx,w)-0.5f,
  fy=iratiof(my,h)-0.5f;
  Matter *m=selected;
  switch ( viewing ) {
   case 0:
     if ( !ALT ) {
      if ( m->scale.x < 5.0 ) m->scale.x+=0.1;
      else m->scale.x = 5.0;
     }
     if ( !CTRL ) {
      if ( m->scale.z < 5.0 ) m->scale.z+=0.1;
      else m->scale.z = 5.0;
     }
     if ( SHIFT ) {
      if ( m->scale.y < 5.0 ) m->scale.y+=0.1;
      else m->scale.y = 5.0;
     }
    break;
   case 1:
     if ( !ALT ) {
      if ( m->scale.x < 5.0 ) m->scale.x+=0.1;
      else m->scale.x = 5.0;
     }
     if ( !CTRL ) {
      if ( m->scale.y < 5.0 ) m->scale.y+=0.1;
      else m->scale.y = 5.0;
     }
     if ( SHIFT ) {
      if ( m->scale.z < 5.0 ) m->scale.z+=0.1;
      else m->scale.z = 5.0;
     }
    break;
   case 2:
     if ( !ALT ) {
      if ( m->scale.z < 5.0 ) m->scale.z+=0.1;
      else m->scale.z = 5.0;
     }
     if ( !CTRL ) {
      if ( m->scale.y < 5.0 ) m->scale.y+=0.1;
      else m->scale.y = 5.0;
     }
     if ( SHIFT ) {
      if ( m->scale.x < 5.0 ) m->scale.x+=0.1;
      else m->scale.x = 5.0;
     }
    break;
  }
 }
 
 void ModifyTechMagnitude() {  
  switch ( tech_selected->type ) {
    case _NONE:      break;
    case _EMITTER:/* switch ( viewing ) {
                    case LEFT:
                      tech_selected->magnitude.x=-tech_selected->source.x+(fx/zoom);
                      tech_selected->magnitude.z=-tech_selected->source.y+(fy/zoom);
                     break;
                    case TOP:
                      tech_selected->magnitude.x=-tech_selected->source.x+(fx/zoom);
                      tech_selected->magnitude.y=-tech_selected->source.y+(fyinv/zoom);
                     break;
                   } */
                  break;
    case _COUPLER:   break;
    case _HARDPOINT: break;
    case _JOINT:     break;
    case _LIGHT:     tech_selected->color.Any(); break;
  }
 }
 
 void ModifyTechParameters() {
  switch ( tech_selected->type ) {
   case _NONE:      break;
   case _EMITTER:   break;
   case _COUPLER:   break;
   case _HARDPOINT: break;
   case _JOINT:     break;
   case _LIGHT:     tech_selected->color.Any(); break;
  }
 }
 
 void ScaleMatterSmaller() {  
  float fx=iratiof(mx,w)-0.5f,
        fy=iratiof(my,h)-0.5f;
  Matter *m=selected;
  switch ( viewing ) {
   case LEFT: // Left
     if ( !ALT ) {
      if ( m->scale.x > 0.25 ) m->scale.x-=0.1;
      else m->scale.x = 0.25;
     }
     if ( !CTRL ) {
      if ( m->scale.z > 0.25 ) m->scale.z-=0.1;
      else m->scale.z = 0.25;
     }
     if ( SHIFT ) {
      if ( m->scale.y > 0.25 ) m->scale.y-=0.1;
      else m->scale.y = 0.25;
     }
    break;
   case TOP: // Top
     if ( !ALT ) {
      if ( m->scale.x > 0.25 ) m->scale.x-=0.1;
      else m->scale.x = 0.25;
     }
     if ( !CTRL ) {
      if ( m->scale.y > 0.25 ) m->scale.y-=0.1;
      else m->scale.y = 0.25;
     }
     if ( SHIFT ) {
      if ( m->scale.z > 0.25 ) m->scale.z-=0.1;
      else m->scale.z = 0.25;
     }
    break;
  }
 }
 
 void AddNewTech() {
  if ( tech_hovering || moving_tech ) return;
  if ( !techtypes->addingTech ) return;
  if ( techs->count > 20
   || ( (TechTypes) *(techtypes->binding) == _EMITTER && emitterCount >= 3 )
   || ( (TechTypes) *(techtypes->binding) == _LIGHT && lightCount >= 8 )
   || ( (TechTypes) *(techtypes->binding) == _COUPLER && couplerCount >= 1 )
   || ( (TechTypes) *(techtypes->binding) == _HARDPOINT && hardpointCount >= 10 ) ) {
   audio.manager.CueAndPlayIfNotPlaying("data/sounds/spoken/GRB-toomanytech.wav",1.0f,1.0f,false,true);
   return;
  }
  Tech *t=new Tech;
  techs->Append(t);
  t->type=(TechTypes) *(techtypes->binding);
  switch ( viewing ) {
   case LEFT: // Left
     t->source.x=-(fx/zoom);
     t->source.z=(fyinv/zoom);
    break;
   case TOP: // Top
     t->source.x=(-fx/zoom);
     t->source.y=(fy/zoom);
    break;
  }
  switch ( t->type ) {
    case _NONE:      break;
    case _EMITTER:   break;
    case _COUPLER:   break;
    case _HARDPOINT: break;
    case _JOINT:     break;
    case _LIGHT:     break;
  }
  moving_tech=t;
  tech_selected=t;
  tech_hovering=t;
 }
 
 void AddNewMatter() {
  Matter *m;
  if ( matters->count >= 10 ) return;   
  matters->Append(m=new Matter(ms->projection.m->ply,ms->projection.s));
  switch ( viewing ) {
   case LEFT: // Left
     m->position.x=(-fx/zoom);
     m->position.z=(fyinv/zoom);
    break;
   case TOP: // Top
     m->position.x=(-fx/zoom);
     m->position.y=(fy/zoom);
    break;
  }
  selected=m;
}

 void RotateMatter() {
  float fx=iratiof(mx,w)-0.5f,
        fy=iratiof(my,h)-0.5f;
  Matter *m=selected;
  switch ( viewing ) {
   case LEFT: // Left
     if ( ALT ) {
      if ( m->rotation.x < 360.0 ) m->rotation.x+=15.0;
      else m->rotation.x = 0.0;
     } else
     if ( CTRL ) {
      if ( m->rotation.z < 360.0 ) m->rotation.z+=15.0;
      else m->rotation.z = 0.0;
     } else {
      if ( m->rotation.y < 360.0 ) m->rotation.y+=15.0;
      else m->rotation.y = 0.0;
     }
    break;
   case TOP: // Top
     if ( ALT ) {
      if ( m->rotation.y < 360.0 ) m->rotation.y+=15.0;
      else m->rotation.y = 0.0;
     } else
     if ( CTRL ) {
      if ( m->rotation.x < 360.0 ) m->rotation.x+=15.0;
      else m->rotation.x = 0.0;
     } else {
      if ( m->rotation.z < 360.0 ) m->rotation.z+=15.0;
      else m->rotation.z = 0.0;
     }
    break;
  }
 }

 void RemoveMatter() {
  Matter *m=selected;
  if ( cousin && cousin->selected == selected ) {
   cousin->selected=null;
   cousin->hovering=null;
  }
  selected=hovering=null;
  matters->Remove(m);
  delete m;
 }

 void RemoveTech() {
  Tech *t=tech_selected;
  if ( cousin && tech_selected == cousin->tech_selected ) {
   cousin->tech_selected=null;
   cousin->tech_hovering=null;
  }
  tech_hovering=tech_selected=null;
  techs->Remove(t);
  delete t;
 }

 void Deselect() {
  ResetGUI();
//  moving_tech=null;
//  selected=null;
//  hovering=null;
//  tech_hovering=null;
//  tech_selected=null;
 }

 void LeftUp() {
  if ( !visible ) return;
  if (  leftUpDelay > 0.0f ) return;
  else leftUpDelay=0.25f;
  if ( ignoreLeftUp ) {
   ignoreLeftUp=false;
   return;
  }
  if ( clicked ) {
   clicked=false;
   return;
  }
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) {
   Deselect();
   return;
  }
  // Add a new tech after the user has selected a new tech brush
  else if ( !selected && !tech_selected && !tech_hovering ) { // Add a new matter
   AddNewMatter();
  } else selected=null;
  if ( tech_selected
   && (tech_selected->type != _LIGHT
    && tech_selected->type != _EMITTER) ) tech_selected=null;
  moving_tech=null;
 }

 void LeftDown() {
  if ( !visible ) return;
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) return;
  selected=hovering;
  if ( tech_selected && !techtypes->addingTech && !selected ) {
   ModifyTechMagnitude();
  } else
  if ( !tech_hovering && !clicked && techtypes->addingTech ) {
   AddNewTech();
   tech_selected=null;
   moving_tech=null;
   clicked=true;
   return;
  } else {
   if ( tech_hovering ) tech_selected=tech_hovering;
  }
 }

 void MouseMove() {
  if ( !visible ) return;
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) return;
  if ( tech_hovering && input->left ) { // Move a tech
   if ( moving_tech && moving_tech != tech_hovering ) tech_hovering=moving_tech;
   MoveTech();
   moving_tech=tech_hovering;
//   ignoreLeftUp=true;
  } else 
  // Matter manipulation space (inverted y)
  // Dragging, move a matter
  if ( !techtypes->addingTech && selected && input->left && input->wasLeft ) { // move selected to cursor position
   MoveMatter();
  }
 }
 void MiddleDown() {
  if ( !visible ) return;
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) return;
  selected=hovering;
  tech_selected=tech_hovering;
  if ( tech_selected ) { // Modify a tech
   switch ( tech_selected->type ) {
     case _NONE:      break;
     case _EMITTER:   tech_selected->color.Any(); break;
     case _COUPLER:   break;
     case _HARDPOINT: break;
     case _JOINT:     break;
     case _LIGHT:     tech_selected->color.Any(); break;
   }
  } else if ( selected ) { // Rotate a matter
   RotateMatter();
  }
 }
 void RightDown() {
  if ( !(tech_hovering || tech_selected) ) techtypes->addingTech=false;
 }
 void RightUp() {
  if ( !visible ) return;
  if ( rightUpDelay > 0.0f ) return;
  selected=hovering;
  tech_selected=tech_hovering;
  if ( tech_selected ) { // Remove a tech
   RemoveTech();
   rightUpDelay=0.5f;
  } else if ( selected ) { // Remove a matter
   RemoveMatter();
   rightUpDelay=0.5f;
  }
 }
 void WheelUp() {
  if ( !visible ) return;
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) return;
  selected=hovering;
  tech_selected=tech_hovering;
  if ( tech_selected ) { // Modify a tech
   ModifyTechParameters();
  } else if ( selected ) { // Scale a matter
   ScaleMatterBigger();
  }
 }
 void WheelDown() {
  if ( !visible ) return;
  Translate();
  if ( !WITHIN(mx,my,0,0,w,h) ) return;
  selected=hovering;
  tech_selected=tech_hovering;
  if ( tech_selected ) { // Modify a tech
   ModifyTechParameters();
  } else if ( selected ) { // Scale a matter
   ScaleMatterSmaller();
  }
 }
};