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

#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"

class ZSortedFireAndForgetLimitedWrappingProce55Viewer2d : public FireAndForgetLimitedWrappingProce55Viewer2d {
public:
 void NonConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
  EACHN(fx.first,Proce55or,p, {
   p->Between();
   if ( p->Done() ) {
    fx.Remove(p);
    delete p;
   }
  });
  fx.ZSort();
 }  
 void ConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
//  conductor->Between();
  NonConductedBetweenFrames();
 }
};
