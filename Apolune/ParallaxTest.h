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
#include <Windows.h>

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Display.h"
#include "Interface.h"

#include "Art.h"
#include "Presentation.h"
#include "Particles.h"
#include "TextureLibrary.h"

#include "CompositeImage.h"
#include "Parallax2D.h"
#include "VisualAid.h"
#include "fx_Glow.h"

#include "Globals.h"

class ParallaxTestBackgroundLayer : public Presentation {
 void OnLoad() {
  Slide *s;   
  VisualAidAdditive *v;
  int i;
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);

  letterbox=false;
  //debugmode=4;//debugmode=1;//2;//3;

  Add( s=new Slide(150.0f) );

  // Stellar background band (tinies) --------------------------------------------------------------------

  for ( i=0; i<50; i++ ) {
//   int r,g,b,c;
   int c=number_range(0,15);
   v=new VisualAidAdditive;
   v->image = flares.image(c); 
   v->delay=0.0f;
   v->duration=150.0f;
   v->in=float_range(5.0f,50.0f);
   v->out=v->duration-v->in;
   v->Opacity(0);
   v->Tints(crayons.Pick(black),crayons.Any(),crayons.Pick(black));   
   v->w=v->w2=v->h=v->h2=number_range(1,15);
   v->sx = number_range(-display->w,display->h);
   v->sy = number_range(0,display->h);
   v->dx=display->w+v->sx.abs()+number_range(0,200);
   // v->dy=display->h;
   s->fx->Add(v,this);
  }
  
  Init(false);
 }
};

class ParallaxTestLayer : public Presentation {
 void OnLoad() {
  Slide *s;   
  VisualAidAdditive *v;
  int i;
  Anchor(0.0f,0.0f,0.0f,0.0f);
//  SetPositionAndSize(-10000,-1000,30000,3000);
  background.Pick(clear);

  letterbox=false;
  //debugmode=4;//debugmode=1;//2;//3;

  Add( s=new Slide(150.0f) );

  // Stellar background band (tinies) --------------------------------------------------------------------

  for ( i=0; i<100; i++ ) {
//   int r,g,b,c;
   int c=number_range(0,15);
   v=new VisualAidAdditive;
   v->image = flares.image(c);
   v->delay=0.0f;
   v->duration=150.0f;
   v->in=float_range(0.0f,50.0f);
   v->out=v->duration-v->in;
   v->Opacity(0);
   v->Tints(crayons.Pick(black),crayons.Any(),crayons.Pick(black));   
   v->w=v->w2=v->h=v->h2=number_range(1,128);
   v->sx = number_range(0,w);
   v->sy = number_range(0,h);
   v->dx=display->w+v->sx.abs()+number_range(0,200);
   // v->dy=display->h;
   s->fx->Add(v,this);
  }
  
  Init(false);
 }
};

class ParallaxTest : public GLWindow {
public:
 ParallaxPresentation2d parallax;
 void OnLoad() {
  background.Pick(clear);
  Anchor(0.0f,0.0f,0.0f,0.0f);
 }
 void Between() {
  if ( input->KeyDown(DX_RIGHT) ) parallax.MoveBy(1,0);
  if ( input->KeyDown(DX_LEFT) ) parallax.MoveBy(-1,0);
  if ( input->KeyDown(DX_UP) ) parallax.MoveBy(0,-1);
  if ( input->KeyDown(DX_DOWN) ) parallax.MoveBy(0,1);
 }
};


class ParallaxTest2 : public GLWindow {
public:
 Parallax2di parallax;
 void OnLoad() {
  CompositeImage *i;
  int j;
  ParallaxLayer2di *layer;

  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);

  layer=&parallax.background;
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(red255);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(1.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(hotPink);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(2.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(blue255);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(4.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(lavender);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(8.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(azure);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(16.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(orangeRed);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(32.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(orange);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(64.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(green);
   layer->images.Append(i);
  }

  layer=parallax.AddLayer(128.0f);
  for ( j=0; j<100; j++ ) {
   i=new CompositeImage(library.Load("data/images/flares/flare.png"));
   i->position.w=64;
   i->position.h=64;
   i->position.x=128+j*32;
   i->position.y=128+(j%2*32);
   i->tint.Pick(yellow);
   layer->images.Append(i);
  }

  parallax.RenderTo(this);
 }
 void Between() {
  if ( input->KeyDown(DX_RIGHT) ) parallax.MoveBy(1,0);
  if ( input->KeyDown(DX_LEFT) ) parallax.MoveBy(-1,0);
  if ( input->KeyDown(DX_UP) ) parallax.MoveBy(0,-1);
  if ( input->KeyDown(DX_DOWN) ) parallax.MoveBy(0,1);
  parallax.Between();
 }
 void Render()  { parallax.Render(); }
};
