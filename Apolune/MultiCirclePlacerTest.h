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
#include "MultiCirclePlacer.h"

class MultiCirclePlacerTest : public GLWindow {
public:
 MultiCirclePlacer placer;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  placer.tiers.Add(1,1.0,0.8,1.2,0.0);
  placer.tiers.Add(5,0.5,0.8,1.2,0.0);
  placer.tiers.Add(5,0.25,0.8,1.2,0.0);
  placer.tiers.Add(5,0.125,0.8,1.2,0.0);
  placer.tiers.Add(5,0.0625,0.8,1.2,0.0);
  placer.Place(256.0);
 }
 Zdouble keyDelay;
 void Between() {
  if ( keyDelay <= 0.0 )  {
   if ( input->KeyDown(DX_SPACE) ) {
    placer.ClearPlacements();
    placer.Place(256.0);
   }
  } else keyDelay-=FRAMETIME;
 }
 void Render() {
  placer.tiers.Debug(512.0,512.0);
 }
};

