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
/***********************************************************************************************
 * ____ _  _ ___  _ ____ ____    _ _  _    ___ _  _ ____    ____ _  _ _   _ (c)2009-2011       *
 * |___ |\/| |__] | |__/ |___    | |\ |     |  |__| |___    [__  |_/   \_/  Lost Astronaut     *
 * |___ |  | |    | |  \ |___    | | \|     |  |  | |___    ___] | \_   |   Studios + Ganos.biz*
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once

#include "TextureLibrary.h"
#include "Tween.h"
#include "Slide.h"
#include "fx_Glow.h"
#include "VisualAid.h"
#include "LimitedProce55Viewer2d.h"
#include "WrappingProce55Viewer2d.h"
#include "LimitedWrappingProce55Viewer2d.h"
#include "TileMap2d.h"
#include "Parallax2D.h"
#include "fx_Shader2d.h"
#include "Skybox.h"
#include "Nebula4LightShader.h"

#include "Globals.h"
#include "Camera2d.h"

#include "Universe.h"

extern char *space_dust[15];
extern char *smoke_images[10];

/*
 "data/images/flares/ball3.png",
 "data/images/flares/ball1.png",
 "data/images/flares/ball2.png",
 "data/images/flares/ball3.png",
 "data/images/flares/ball4.png",
 "data/images/flares/ball5.png",
 "data/images/flares/ball6.png",
 "data/images/flares/ball7.png",
 "data/images/flares/ball8.png",
 "data/images/flares/ball9.png",
 "data/images/flares/ball10.png",
 "data/images/flares/ball11.png",
 "data/images/flares/ball12.png",
 "data/images/flares/base.png",
 "data/images/flares/flare.png",
 "data/images/flares/Purple_Flare.jpg"
 */

class StarDeformer : public fx_Shader2d {
public:
 Cartesian movement;
 StarDeformer() : fx_Shader2d() {
  movement.fx=0.0001f;
  movement.fy=0.0001f;
 }
 void Update() {
  undulation.fx+=movement.fx;
  undulation.fy+=movement.fy;
 }
};

class SolarSystemStarfieldLayer0 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=16.0f,maxiW=80.0f;

  int density=10;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;

  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   int ox=g->x=(int) (uniform() * (double) sizeW)+startX;
   int oy=g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= weapon_flares.Any(17,37); //"data/images/flares/flare.png" );
   g->tint.Any();
   int ow=g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  

   g=new fx_Glow2;
   g->x=ox;
   g->y=oy;
   g->flare= weapon_flares.Any(17,37); //"data/images/flares/flare.png" );
   g->tint.Pick(alabaster);
   g->w=g->w2=g->h=g->h2=ow;
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (5.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }
  Init();
 }

};

class SolarSystemStarfieldLayer1 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=5.0f,maxiW=64.0f;

  int density=10;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;

  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   int ox=g->x=(int) (uniform() * (double) sizeW)+startX;
   int oy=g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= weapon_flares.Any(17,37); //"data/images/flares/flare.png" );
   g->tint.Any();
   int ow=g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  

   g=new fx_Glow2;
   g->x=ox;
   g->y=oy;
   g->flare=weapon_flares.Any(17,37); //"data/images/flares/flare.png" );
   g->tint.Pick(alabaster);
   g->w=g->w2=g->h=g->h2=ow;
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (5.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }
  Init();
 }

};

class SolarSystemStarfieldLayer2 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.0f,maxiW=20.0f;

  int density=10;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;

  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Any();
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }

};

class SolarSystemStarfieldLayer3 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.2f,maxiW=20.0f;

  int density=10;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;
  
  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Pick(cyan);
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW/4.0f));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }
};


class SolarSystemStarfieldLayer4 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.2f,maxiW=20.0f;
  
  int density=25;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;
  
  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Pick(alabaster);
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW/4.0f));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }
};


class SolarSystemStarfieldLayer5 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.2f,maxiW=20.0f;


  int density=25;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;
  
  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Pick(electricBlue);
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW/3.0f));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }
};

class SolarSystemStarfieldLayer6 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.2f,maxiW=20.0f;

    int density=25;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;
  
  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Pick(brightCerulean);
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW/2.0f));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }
};

class SolarSystemStarfieldLayer7 : public WrappingProce55Viewer2d {
public:
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);

  int sizeW,sizeH,startX,startY;
  float miniW=1.2f,maxiW=20.0f;

  int density=25;
  /*
  switch ( (OptionsSetting) options->o_Starfield.value ) {
  case o_off: density=0; break;
  case o_lo:  density=10; break;
  case o_med: density=25; break;
  case o_hi:  density=50; break;
  }
  */

  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Glow2 *g;
  
  for ( int i=0; i<density; i++ ) { 
   g=new fx_Glow2;
   g->x=(int) (uniform() * (double) sizeW)+startX;
   g->y=(int) (uniform() * (double) sizeH)+startY;
   g->flare= library.find( "data/images/flares/flare.png" );
   g->tint.Pick(skyBlue);
   g->w=g->w2=g->h=g->h2=(int) (float_range(miniW,maxiW/2.0f));
   g->duration=10.0f;
   g->maxOp=127.0f;
   g->controller.period= (float) (10.0 * uniform() );
   g->controller.variables.NewfT("Z",127.0f,255.0f,float_range(0.25f,1.0f),tweens.randomNot1());
   Add(g);  
  }

  Init();
 }
};

class StarSimulations; // Forward declaration

class StarSimulation : public LimitedProce55Viewer2d {
public:
 Zpointer<fx_Shader2d> corona, photosphere, wr_corona, wr_photosphere, photosphere2;
 Zp<SolarSystem> system;
 Zp<Star> s;
 Zp<StarSimulations> stars;
 Zp<Cartesian> starting_position;
 Cartesian defaultStartingPosition;
 PROGRAMMABLE(StarSimulation,AddPtr,additional,Additional);
 Zfloat ofsx,ofsy;
 void OnLoad();
 Zfloat mbyx,mbyy;
 void MovePhoto2() {
  wr_photosphere->position.fx+=mbyx;
  wr_photosphere->position.fy+=mbyy;
 }
 void MoveWolfRayet() {
  if ( wr_corona ) {
   wr_corona->position.fx+=mbyx;
   wr_corona->offset.fx+=1.0f/360.0f; //?
   wr_corona->position.fy+=mbyy;
   wr_corona->offset.fy+=1.0f/360.0f; //?
  }
  wr_photosphere->position.fx+=mbyx;
  wr_photosphere->position.fy+=mbyy;  
 }
 void NoAdditional() {}
 void PreProcessing() {
 }
 void RepositionBy( float fx, float fy ) {
  mbyx=fx;
  mbyy=fy;
  if ( corona ) {
   corona->position.fx+=fx;
   corona->offset.fx+=1.0f/360.0f; //?
   corona->position.fy+=fy;
   corona->offset.fy+=1.0f/360.0f; //?
  }
  photosphere->position.fx+=fx;
  photosphere->position.fy+=fy;
  if ( photosphere2 ) {
   photosphere2->position.fx+=fx;
   photosphere2->position.fy+=fy;
  }
  Additional();
 }
};

class StarSimulationHandle : public ListItem {
public:
 Zpointer<StarSimulation> p;
};

class StarSimulations : public LinkedList {
public:
 CLEARLISTRESET(StarSimulationHandle);
 ~StarSimulations() { Clear(); }
};

class BlackholeSimulation : public LimitedProce55Viewer2d {
public:
 Zint seed;
 Zp<fx_Shader2d> hole;
 Zp<SolarSystem> system;
 void OnLoad() {
  seed=system->system_catalog_no;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);
  hole=new fx_Shader2d;
  hole->w=w;
  hole->h=h;
  hole->Select(monjori);
  hole->position.Set(-(float)w/2.0f+(float)w/4.0f,-(float)h/2.0f+(float)h/3.0f);
  hole->blend=additive;
  switch ( seed % 3 ) {
   case 0: hole->blend=additive; break;
   case 1: hole->blend=solarize; break;
   case 2: hole->blend=multiply; break;
  }
  Add(hole);
  VisualAid2 *va2=new VisualAid2;
  va2->w=w;
  va2->h=h;
  va2->image=library.find("data/images/elements/white.png");
  va2->Tint(crayons.Pick((Colors)(seed%100)));
  switch ( seed % 3 ) {
   case 0: va2->blend=additive; break;
   case 1: va2->blend=solarize; break;
   case 2: va2->blend=multiply; break;
   case 3: va2->blend=subtractive;  break;
   case 4: va2->blend=overlay;  break;
   case 5: va2->blend=saturate;  break;
   case 6: va2->blend=translucency;  break;
  }
  va2->duration=10.0f;
  Add(va2);
  Init();
 }
 void RepositionBy( float fx, float fy ) {
  hole->position.fx+=fx;
  hole->position.fy+=fy;
 }
};

class SolarSystemStarfieldLayerHandle : public ListItem {
public:
 Zpointer<WrappingProce55Viewer2d> p;
 SolarSystemStarfieldLayerHandle() : ListItem() {}
};

class SolarSystemStarfieldLayers : public LinkedList {
public:
 void Add( WrappingProce55Viewer2d *p ) {
  SolarSystemStarfieldLayerHandle *n=new SolarSystemStarfieldLayerHandle;
  n->p=p;
  Append(n);
 }
 CLEARLISTRESET(SolarSystemStarfieldLayerHandle);
 ~SolarSystemStarfieldLayers() { Clear(); }
};

class SolarSystemStarfield : public GLWindow {
public:
 Zbool useNebula4;
 Blends nebula4Blend;
 CubicSkyboxDefinitions heavens;
 StarSimulations stars;
 SolarSystemStarfieldLayers layers;
 Zp<BlackholeSimulation> hole;
 Zp<LimitedWrappingProce55Viewer2d> gasAndDust, gasAndDust2;
 Zint seed;
 Zp<SolarSystem> system;
 Zdouble zoom,zoomRate, movedx,movedy;

 Crayon tint,tint2;
 Zp<GLImage> atten2d,bgatten,box,attenuate;
 Blends attenBlend;

 Zpointer<Vertexd> velocity,camera;

 SolarSystemStarfield() : GLWindow() {
  attenBlend=additive;
  nebula4Blend=additive;
 }
 void OnLoad() {
  post=true;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
  movedx=0;
  movedy=0;
  atten2d=library.find("data/images/elements/attenOverlay.png");
  bgatten=library.find("data/images/elements/atten2D.png");
  box=library.find("data/images/elements/white.png");
 }

 void VelocityPlayerVehicle();

 void ChangeSystem( int sed );

 void Between() {
  float moveX=0;
  float moveY=0;
  if ( velocity ) {
   moveX+=(float) -velocity->x;
   moveY+=(float) -velocity->y;
  }
   double i=0.55;
   movedx+=moveX;
   movedy+=moveY;
   EACH(layers.first,SolarSystemStarfieldLayerHandle,p) {
    p->p->MoveBy((int)(moveX*i),(int)(moveY*i));
    i+=0.55;
   }
   if ( gasAndDust ) gasAndDust->MoveBy((int) moveX*3,(int) moveY*3);
   if ( gasAndDust2 ) gasAndDust2->MoveBy((int) moveX*3,(int) moveY*3);
  //}
  if ( input->KeyDown(DX_ADD) ) {
   zoom+=zoomRate;
//   maps.layers.Zoom(zoom,zoom);  
  }
  if ( input->KeyDown(DX_SUBTRACT) ) {
   zoom-=zoomRate;
   if ( zoom < zoomRate ) zoom=zoomRate;
//   maps.layers.Zoom(zoom,zoom);
  }
  if ( input->KeyDown(DX_NUMPAD5) ) {
   zoom=1.0f;
//   maps.layers.Zoom(zoom,zoom);
  }
 }

 void Render() {
  Blending(none);
  Area(this,crayons.Pick(black),0,0,w,h);
 }

 void RenderAfter() {
  heavens.Render();
  
  if ( attenuate ) {
   StretchiNoAlpha(this,attenuate,tint2,attenBlend,0,0,w,h);
  }

//  maps.layers.Render();
  /*
  char buf[128];
  sprintf_s(buf,"Position: %d,%d",movedx,movedy);
  WriteTexFont(this,&microFont,crayons.Pick(green),none,5,5,7,16,2,buf);
  if ( stars.first ) {
   StarSimulationHandle *p=(StarSimulationHandle *)stars.first;
   sprintf_s(buf,"Time: %f",p->p->photosphere->expired);
   WriteTexFont(this,&microFont,crayons.Pick(green),none,5,15,7,16,2,buf);
  }*/
  //EACH(stars.first,StarSimulationHandle,p) {
  // p->p->Render();
  //}
 }
};

