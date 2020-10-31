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
#include "GLImage.h"
#include "Art.h"

#include "TextureLibrary.h"


class RotatedTextureTest : public GLWindow
{
public:
 GLImage *i,*f,*g;
 float a;
 float fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4,whalf;
 Crayon color;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(black);
  a=0.0f;
  color.Pick(crimson);
  i=library.find("data/images/icons/commodity.png");
  f=library.find("data/images/elements/rounded_1024.png");
  g=library.find("data/images/elements/swab.png");
  whalf=64.0f;
  fx1=(float) w2;
  fy1=(float) h2;
  fx2=fx1/2;
  fy2=fy1/2;
  fx3=fx1+fx2;
  fy3=fy1+fy2;
  fx4=64;
  fy4=64;
 }
 void Between() {
  a+=360.0f/(float) FPS;
 }
 void Render() {
  RotatedRectf( this, i, color, additive, fx1,fy1, 128.0f, 64.0f, a );
  RotatedRectf( this, i, color, additive, fx2,fy2, 128.0f, 64.0f, a );
  RotatedRectf( this, i, color, additive, fx3,fy3, 64.0f, 64.0f, a );
  RotatedRectf( this, i, color, additive, fx4,fy4, 215.0f, 64.0f, a );
  LineRectf( this, f, color, additive, 0.0f,0.0f,(float) w, (float) h, 32);
  LineRectf( this, f, color, additive, 32,32,128,128, 32);
  RotatedIsotrapf( this, g, crayons.Pick(green), additive, 256.0f, 256.0f, 256.0f, 16.0f, 128.0f, a );
  DrawPixelf(this,crayons.Pick(alabaster),32,32);
  DrawPixelf(this,crayons.Pick(alabaster),128,128);
  LineRectf( this, f, color, additive, 32,32,128+64,128, 32);
  DrawPixelf(this,crayons.Pick(alabaster),32,32);
  DrawPixelf(this,crayons.Pick(alabaster),128+64,128);
  LineRectf( this, f, color, additive, (float) w-64, 64.0f, 64.0f, (float) h-64, 32);
  LineRectf( this, f, color, additive, (float) w-64, 0.0f, (float) w-64, (float) h-64, 32);
 }
};