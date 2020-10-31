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

#include "TextureLibrary.h"
#include "fx_FireAndForget.h"

#include "SpilledLiquid2d.h"

class GLFAFTester : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 int mx,my;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  Init();
 }
 void Between() {
  Cartesian move;
  if ( input->KeyDown(DX_LEFT) ) move.x=-1;
  if ( input->KeyDown(DX_RIGHT) ) move.x=1;
  if ( input->KeyDown(DX_UP) ) move.y=1;
  if ( input->KeyDown(DX_DOWN) ) move.y=-1;
  if ( move.x != 0 || move.y != 0 ) MoveBy(move.x,move.y);
  if ( input->left ) {
   MousePosition(&mx,&my);
   int quantity=2+upto(20);
   for ( int i=0; i<quantity; i++ ) { // Blob Creature
    SpilledLiquidBubbler *liq=new SpilledLiquidBubbler;
    liq->x=mx;
    liq->y=my;
    fx.Add(liq,this);
    liq->Setup();
   }
  }
  if ( input->right ) { // Blood splatter
   MousePosition(&mx,&my);
   int quantity=2+upto(20);
   for ( int i=0; i<quantity; i++ ) {
    SpilledLiquid2d *liq=new SpilledLiquid2d;
    liq->x=mx+(16-upto(32));
    liq->y=my+(16-upto(32));
    liq->x2=mx+(16-upto(32));
    liq->y2=my+(16-upto(32));
    liq->scale=float_range(1.0f/256.0f,1.0f/32.0f);
    liq->maxSpread=16.0f;
    fx.Add(liq,this);
    liq->Setup();
   }
  }
 }
 void LeftDown() {
 }
};

