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

#include "Smoke2d.h"

class SmokeTest : public GLWindow {
public:
 Smoked2d vapor,gas,smallsmoulder,bubbles;
 PrecomputedSmokesd2d precomputedSmokes,precomputedSmokes2;
 void OnLoad() {
  background.Pick(clear);
  Anchor(0.0f,0.0f,0.0f,0.0f);
//  Gas();
  Vapor();
//  Vapori();
//  SmallSmoulder();
//  Bubbles();
//  CreatePrecomputedSmokes();
//  CreatePrecomputedSmokes2();
 }
 void Between() {
//  gas.Between();
  vapor.Between();
//  vapori.Between();
//  smallsmoulder.Between();
//  bubbles.Between();
//  precomputedSmokes.Between();
//  precomputedSmokes2.Between();
 }
 void Render() {
//  gas.Render(this);
  vapor.Render(this);
//  vapori.Render(this);
//  vapori.Render(this);
//  smallsmoulder.Render(this);
//  bubbles.Render(this);  
//  precomputedSmokes.Render(this);
//  precomputedSmokes2.Render(this);
 }
 void LeftDown() {  
 // precomputedSmokes2.PlayAt(input->mxi,input->myi);
 }
 void RightDown() { 
 // precomputedSmokes.PlayAt(input->mxi,input->myi);
 }
 void Gas() {
  Cartesiand gravity;
  gravity.x=0.0;
  gravity.y=-10.0;
  Cartesiand wind;
  wind.x=0.0;
  wind.y=-1.0;
  Images gasses;
  gasses.addIfUnique(library.Load("data/images/flares/smoke2.png"));
  gasses.addIfUnique(library.Load("data/images/flares/flare.png"));
  gasses.addIfUnique(library.Load("data/images/flares/smoke.png"));
  gasses.addIfUnique(library.Load("data/images/flares/neoflare1.png"));
  gas.Set( gravity, wind, 
    0.95, 5, 50, 0.1, 10.0, 0.0625, 0.125, 5.0, 10.0, 1.0, 4.0, 1.0, 0.5, 
    15,
    crayons.Pick(gray), crayons.Pick(charcoal),
    &gasses,
    w/2-w/4, h-h/4 );
 }
 void Vapor() {
  Cartesiand gravity;
  gravity.x=0.0;
  gravity.y=-0.0;
  Cartesiand wind;
  wind.x=1.0;
  wind.y=-1.0;
  vapor.Set( gravity, wind, 
    0.95, 1, 75, 0.1, 10.0, 0.25, 0.5, 5.0, 20.0, 1.0, 4.0, 1.0, 0.25, 
    15,
    crayons.Pick(green), crayons.Pick(charcoal),
    library.Load("data/images/flares/smoke2.png"),
    w/2, h-h/4 );
 }
 void SmallSmoulder() {
  Cartesiand gravity;
  gravity.x=0.0;
  gravity.y=-0.5;
  Cartesiand wind;
  wind.x=0.01;
  wind.y=-0.01;
  Crayons emissions;
  emissions.Hold(orangeRed);
  emissions.Hold(orange);
  emissions.Hold(red255);
  emissions.Hold(yellow);
  Crayons vapors;
  vapors.Hold(gray);
  vapors.Hold(alabaster);
  smallsmoulder.Set( gravity, wind, 
    0.50, 5, 50, 0.1, 10.0, 0.003125, 0.0625, 2.5, 5.0, 1.0, 4.0, 0.1, 0.25, 
    15,
    &emissions, &vapors,
    library.Load("data/images/flares/smoke2.png"),
    w/2+w/4, h-h/4 );
 }
 void Bubbles() { // Probably gonna make a special one
  Cartesiand gravity;
  gravity.x=0.0;
  gravity.y=-10.0;
  Cartesiand wind;
  wind.x=0.0;
  wind.y=-1.0;
  bubbles.Set( gravity, wind, 
    0.95, 5, 20, 0.1, 10.0, 0.003125, 0.0625, 5.0, 10.0, 1.0, 4.0, 1.0, 0.25, 
    15,
    crayons.Pick(alabaster), crayons.Pick(blue),
    library.Load("data/images/flares/shockwave2.png"),
    w/2-w/4, h/2 );
 }
 void CreatePrecomputedSmokes() {
  for ( int i=0; i<30; i++ ) {
   PrecomputedSmoked2d *smoke=new PrecomputedSmoked2d;
   smoke->gravity.x=0.0;
   smoke->gravity.y=-10.0;
   smoke->wind.x=0.0;
   smoke->wind.y=-1.0;
   smoke->thickness=5;
   smoke->limit=20;
   smoke->massLow=0.1;
   smoke->massHigh=10.0;
   smoke->sizeMin=0.25;
   smoke->sizeMax=0.5;
   smoke->lifespanMin=5.0;
   smoke->lifespanMax=20.0;
   smoke->fadeMin=1.0;
   smoke->fadeMax=4.0;
   smoke->maxVelocity=1.0;
   smoke->opacity=0.25;
   smoke->positionJitter=15;
   smoke->emissions.Hold(green);
   smoke->vapors.Hold(yellow);
   smoke->images.addIfUnique(library.Load("data/images/flares/smoke2.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/smoke.png"));
   smoke->Precompute();
   smoke->Rewind();
   precomputedSmokes.Append(smoke);
  }
 }

 void CreatePrecomputedSmokes2() {
  for ( int i=0; i<30; i++ ) {
   PrecomputedSmoked2d *smoke=new PrecomputedSmoked2d;
   smoke->gravity.x=0.0;
   smoke->gravity.y=-10.0;
   smoke->wind.x=0.0;
   smoke->wind.y=-1.0;
   smoke->thickness=5;
   smoke->limit=20;
   smoke->massLow=0.1;
   smoke->massHigh=10.0;
   smoke->sizeMin=0.25;
   smoke->sizeMax=0.5;
   smoke->lifespanMin=5.0;
   smoke->lifespanMax=20.0;
   smoke->fadeMin=1.0;
   smoke->fadeMax=4.0;
   smoke->maxVelocity=1.0;
   smoke->opacity=0.25;
   smoke->positionJitter=15;
   smoke->emissions.Hold(green);
   smoke->vapors.Hold(yellow);
   smoke->images.addIfUnique(library.Load("data/images/flares/smoke2.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/smoke.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/flare.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/neoflare2.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/neoflare2.png"));
   smoke->images.addIfUnique(library.Load("data/images/flares/neoflare14.png"));
   smoke->Precompute();
   smoke->Rewind();
   precomputedSmokes2.Append(smoke);
  }
 }
};
