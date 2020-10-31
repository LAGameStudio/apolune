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
#include "GLWindow.h"
#include "ColorField.h"
#include "ColorNetwork.h"
#include "fx_UglyText.h"

class ColorFieldTest : public GLWindow {
public:
 ColorField field;
 Crayon mixed;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 Zdouble keyDelay;
 void Between() {
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else
  if ( input->left ) {
   field.Add(crayons.Any(),input->mxi,input->myi);
   keyDelay=1.0;
  } else
  if ( input->KeyDown(DX_SPACE) ) {
   field.Clear();
   keyDelay=1.0;
  } else if ( input->KeyDown(DX_R) ) {
   field.Add(crayons.Pick(red255),input->mxi,input->myi);
   keyDelay=1.0;
  } else if ( input->KeyDown(DX_G) ) {
   field.Add(crayons.Pick(green255),input->mxi,input->myi);
   keyDelay=1.0;
  } else if ( input->KeyDown(DX_B) ) {
   field.Add(crayons.Pick(blue),input->mxi,input->myi);
   keyDelay=1.0;
  }
  mixed.fromCrayon(field.RequestSortedDistance(input->mxi,input->myi,true));
 }
 void Render() {
  Blending(transparency);
  Area(mixed,10,10,10+64,10+64);
  Blending(none);
  Rectangle(crayons.Pick(alabaster),10,10,10+64,10+64);
  Area(crayons.jam(0,mixed.rf,mixed.bf,mixed.gf,1.0f),74,10,74+64,10+64);
  Rectangle(crayons.Pick(alabaster),74,10,74+64,10+64);
  EACH(field.first,ColorFieldPoint,cfp) {
   Area(cfp->color,cfp->x-5,cfp->y-5,cfp->x+5,cfp->y+5);
   Text(
    FORMAT(buf,128,"%d %d %d %d",
     cfp->color.r,cfp->color.g,cfp->color.b,cfp->color.a),
    cfp->x,cfp->y+15,6,6,true
   );
  }
  field.box.Debug(crayons.Pick(hotPink),0,0);
  crayons.Pick(alabaster).gl();
  Text(
   FORMAT(buf,128,"%d %d %d %d (%1.2f %1.2f %1.2f %1.2f)",
    mixed.r,mixed.g,mixed.b,mixed.a,mixed.rf,mixed.gf,mixed.bf,mixed.af),
   10,display->h-30,8,10,false
  );
 }
};


class ColorGearTest : public GLWindow {
public:
 ColorGear gear;
 Crayon mixed;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 Zdouble keyDelay;
 void Between() {
  if ( keyDelay > 0.0 ) keyDelay-=FRAMETIME;
  else
  if ( input->left ) {
   gear.Add(crayons.Any());
   keyDelay=1.0;
  } else
  if ( input->KeyDown(DX_SPACE) ) {
   gear.Clear();
   keyDelay=1.0;
  }
  mixed.fromCrayon(gear.Request(input->mxi*10.0,input->myi*10.0));
 }
 void Render() {
  Blending(transparency);
  Area(mixed,10,10,10+64,10+64);
  Blending(none);
  Rectangle(crayons.Pick(alabaster),10,10,10+64,10+64);
  Area(crayons.jam(0,mixed.rf,mixed.bf,mixed.gf,1.0f),74,10,74+64,10+64);
  Rectangle(crayons.Pick(alabaster),74,10,74+64,10+64);
  crayons.Pick(alabaster).gl();
  int x=10;
  EACH(gear.first,ColorNode,c) {
   int x2=x+32;
   Area(c->color,x,100,x2,100+32);
   x=x2;
  }
  Text(
   FORMAT(buf,128,"%d %d %d %d (%1.2f %1.2f %1.2f %1.2f)",
    mixed.r,mixed.g,mixed.b,mixed.a,mixed.rf,mixed.gf,mixed.bf,mixed.af),
   10,display->h-30,8,10,false
  );
 }
};

