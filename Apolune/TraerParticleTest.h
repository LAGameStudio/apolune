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

#include "ModifiedEulerIntegrator.h"

class TraerParticleTest : public GLWindow {
public:
 ParticleSystem ps;
 Particle *a, *b, *c, *d;
 GLImage *A, *B, *C, *D;
 bool dragging;
 void OnLoad() {
  dragging=false;
  background.Pick(black);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  ps.Init(9.6f,0.0f,0.0f,0.2f);
  A=library.Load("data/images/flares/neoflare4.png");
  B=library.Load("data/images/flares/neoflare2.png");
  C=library.Load("data/images/flares/neoflare1.png");
  D=library.Load("data/images/flares/neoflare2.png");
  a=ps.makeParticle(1.0f,(float)(w2),(float)(h2),0.0f);
  b=ps.makeParticle(1.0f,(float)(w2),(float)(h2),0.0f);
  c=ps.makeParticle(2.0f,(float)(w2),(float)(h2),0.0f);
  d=ps.makeParticle(10.0f,(float)(w2+2),(float)(h2+2),0.0f);
  b->makeFixed();
  ps.makeSpring(a,b,0.5f, 0.1f, 10);
  ps.makeSpring(c,b,0.3f, 0.1f, 75);
  ps.makeSpring(d,b,0.001f, 0.01f, 300);
 }
 void BetweenFrames() {
  ps.step(1.0f/FPS);
 }
 void Render() { 
  DrawLine(this,crayons.Any(),w2-10,h2-10,w2+10,h2+10);
  DrawLine(this,crayons.Pick(red255),
   (int)(a->position.x), (int)(a->position.y) ,
   (int)(b->position.x), (int)(b->position.y) );
  DrawLine(this,crayons.Pick(yellow),
   (int)(c->position.x), (int)(c->position.y) ,
   (int)(b->position.x), (int)(b->position.y) );
  DrawLine(this,crayons.Pick(hotPink),
   (int)(d->position.x), (int)(d->position.y) ,
   (int)(b->position.x), (int)(b->position.y) );
  StretchiNoAlpha(this,A,crayons.Pick(red255),additive,   (int)(a->position.x) -5,(int)(a->position.y) -5,10,10);
  StretchiNoAlpha(this,B,crayons.Pick(green255),additive, (int)(b->position.x) -5,(int)(b->position.y) -5,10,10);
  StretchiNoAlpha(this,C,crayons.Pick(orange),additive,   (int)(c->position.x)-10,(int)(c->position.y)-10,20,20);
  StretchiNoAlpha(this,D,crayons.Pick(blue255),additive,   (int)(d->position.x)-20,(int)(d->position.y)-20,40,40);
 }
 void LeftDown( ) {
  dragging=true;
  a->makeFixed();
  c->makeFixed();
  d->makeFixed();
 }
 void MouseMove( ) {
  if ( dragging ) {
   a->position.Set((float) (input->mxi), (float) (input->myi),0.0f);
   c->position.Set((float) (input->mxi), (float) (input->myi),0.0f);
   d->position.Set((float) (input->mxi), (float) (input->myi),0.0f);
  }
 }
 void LeftUp( ) {
  dragging=false;
  a->makeFree();
  c->makeFree();
  d->makeFree();
 }
};

