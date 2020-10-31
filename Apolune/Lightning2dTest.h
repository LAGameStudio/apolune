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

#include "LimitedWrappingProce55Viewer2d.h"
#include "fx_Lightning.h"

class Lightning2dTest : public LimitedWrappingProce55Viewer2d {
public:
 int startX,startY,sizeW,sizeH;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  wrapLimit.SetRect(
   (startX=(-display->w)),
   (startY=(-display->h)),
   (sizeW=(display->w*3)),
   (sizeH=(display->h*3))
  );

  fx_Lightning2d *lightning=new fx_Lightning2d;
  lightning->x=display->w/2;
  lightning->y=100;
  lightning->delta.Setf(20.0f,40.0f);
  lightning->grains=20;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=1.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(skyBlue);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);
  lightning=new fx_Lightning2d;
  lightning->x=display->w/2;
  lightning->y=100;
  lightning->delta.Setf(20.0f,40.0f);
  lightning->grains=20;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=1.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(skyBlue);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);
  lightning=new fx_Lightning2d;
  lightning->x=display->w/2;
  lightning->y=100;
  lightning->delta.Setf(20.0f,40.0f);
  lightning->grains=20;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=1.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(skyBlue);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);

  lightning=new fx_Lightning2d;
  lightning->x=display->w/4;
  lightning->y=1;
  lightning->delta.Setf(20.0f,20.0f);
  lightning->grains=125;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=10.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(electricBlue);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);
  
  lightning=new fx_Lightning2d;
  lightning->x=display->w/2-display->w/4+75;
  lightning->y=100;
  lightning->delta.Setf(20.0f,40.0f);
  lightning->grains=20;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=1.0f;
  lightning->persist=2.5f;
  lightning->persist2=5.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(skyBlue);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);
  
  lightning=new fx_Lightning2d;
  lightning->x=display->w/2+display->w/4-50;
  lightning->y=display->h/4;
  lightning->delta.Setf(20.0f,20.0f);
  lightning->grains=32;
  lightning->duration=10.0f;
  lightning->waitLength=0.0f;
  lightning->thickness=5.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(orangeRed);
  lightning->contactTint.Pick(red255);
  lightning->startContact=true;
  Add(lightning);

  lightning=new fx_Lightning2d;
  lightning->x=display->w/2+display->w/4+50;
  lightning->y=display->h/4;
  lightning->delta.Setf(20.0f,20.0f);
  lightning->grains=32;
  lightning->duration=10.0f;
  lightning->waitLength=0.0f;
  lightning->thickness=5.0f;
  lightning->persist=1.0f;
  lightning->persist2=2.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(oliveDrab);
  lightning->startContact=true;
  lightning->endContact=true;
  lightning->contactTint.Pick(alabaster);
  lightning->flash=tweens.find("Humanized In Out Slow");
  Add(lightning);
  
  lightning=new fx_Lightning2d;
  lightning->x=display->w/4+display->w/2;
  lightning->y=1;
  lightning->delta.Setf(20.0f,90.0f);
  lightning->grains=30;
  lightning->duration=10.0f;
  lightning->waitLength=1.0f;
  lightning->thickness=30.0f;
  lightning->startExtents.SetRectf(0.0f,0.0f,0.0f,0.0f);
  lightning->tint.Pick(orange);
  lightning->flash=tweens.find("Sine");
  Add(lightning);
  
  Init();
 }
 void MoveBy(int xx, int yy) {
 }
};