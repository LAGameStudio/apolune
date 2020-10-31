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

#include "Spark2d.h"

class SparksTest : public GLWindow {
public:
 Sparksd2d sparks;
 Sparksd2d explosions;
 PrecomputedSparksd2d precomputedExplosions;
 void OnLoad() {
  background.Pick(clear);
  Anchor(0.0f,0.0f,0.0f,0.0f);
  Sparkd2d *spark=new Sparkd2d;
  spark->gravity.x=0.0;
  spark->gravity.y=9.6;
  spark->Create_MonocolorImage(
   5, 10, 0.5, 1.5, 0.1, 1.0, 0.5, 2.0, 0.1, 0.25, 1.0, 1, w2, h2,
   crayons.Pick(alabaster), crayons.Pick(blue), crayons.Pick(charcoal),
   library.Load("data/images/flares/flare.png")
  );
  sparks.Append(spark);
  sparks.Init();
  CreatePrecomputedExplosions();
 }
 void Between() {
  sparks.Between();
  explosions.Between();
  precomputedExplosions.Between();
 }
 void Render() {
  sparks.Render(this);
  explosions.Render(this);
  precomputedExplosions.Render(this);
 }
 void LeftDown() {  
  CreateExplosion();
 }
 void MiddleDown() {
  CreateSpark();
 }
 void RightDown() {
  precomputedExplosions.PlayAt(input->mxi,input->myi);
 }
 void CreateSpark() {
  Sparkd2d *spark;
  sparks.Append(spark=new Sparkd2d);
  spark->gravity.x=0.0;
  spark->gravity.y=9.6;
  spark->Create_MonocolorImage(
   5, 15, 0.5, 1.5, 0.1, 0.125, 0.25, 3.0, 0.1, 0.25, 50.0, 1, input->mxi, input->myi,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  spark->Init();
 }
 void CreateExplosion() {
  Sparkd2d *spark;
  explosions.Append(spark=new Sparkd2d);
  spark->gravity.x=0.0;
  spark->gravity.y=0.0;
  spark->Create_MonocolorImage(
   5, 50, 0.5, 1.5, 0.1, 0.25, 0.5, 1.0, 0.1, 0.25, 50.0, 1, input->mxi, input->myi,
   crayons.Pick(alabaster), crayons.Pick(orange), crayons.Pick(red255),
   library.Load("data/images/flares/flare.png")
  );
  spark->Init();
 }
 void CreatePrecomputedExplosions() {
  for ( int i=0; i<15; i++ ) {
   PrecomputedSparkd2d *explode=new PrecomputedSparkd2d;
   explode->countLow=5;
   explode->countHigh=50;
   explode->massLow=0.5;
   explode->massHigh=1.5;
   explode->sizeMin=0.1;
   explode->sizeMax=0.25;
   explode->lifespanMin=0.5;
   explode->lifespanMax=1.0;
   explode->fadeMin=0.1;
   explode->fadeMax=0.25;
   explode->maxVelocity=50.0;
   explode->positionJitter=1;
   explode->sparks.Hold(alabaster);
   explode->flames.Hold(orange);
   explode->ashes.Hold(red255);
   explode->images.addIfUnique(library.Load("data/images/flares/flare.png"));
   explode->images.addIfUnique(library.Load("data/images/flares/neoflare8.png"));
   explode->Precompute();
   explode->Rewind();
   precomputedExplosions.Append(explode);
  }
 }
};
