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

#include "WrappingProce55Viewer2d.h"
#include "LimitedWrappingProce55Viewer2d.h"

class FireAndForgetLimitedWrappingProce55Viewer2d : public LimitedWrappingProce55Viewer2d {
public:
 PROGRAMMABLE(FireAndForgetLimitedWrappingProce55Viewer2d,BetweenFramesPtr,betweenframes,BetweenFrames);
 void Nothing() {}
 FireAndForgetLimitedWrappingProce55Viewer2d() : LimitedWrappingProce55Viewer2d() {
  this->classDescription="FireAndForgetLimitedWrappingProce55Viewer2d";
  wrapLimit.SetRect( (-displayPointer->w),(-displayPointer->h),(displayPointer->w*3),(displayPointer->h*3) );
  betweenframes=&FireAndForgetLimitedWrappingProce55Viewer2d::NonConductedBetweenFrames;
 }
 virtual void NonConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
  EACHN(fx.first,Proce55or,p, {
   if ( p->Done() ) {
    fx.Remove(p);
    delete p;
   } else p->Between();
  });
 }  
 virtual void ConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
  NonConductedBetweenFrames();
 }
};


class FireAndForgetWrappingProce55Viewer2d : public WrappingProce55Viewer2d {
public:
 PROGRAMMABLE(FireAndForgetWrappingProce55Viewer2d,BetweenFramesPtr,betweenframes,BetweenFrames);
 void Nothing() {}
 FireAndForgetWrappingProce55Viewer2d() : WrappingProce55Viewer2d() {
  this->classDescription="FireAndForgetWrappingProce55Viewer2d";
  wrapLimit.SetRect( (-displayPointer->w),(-displayPointer->h),(displayPointer->w*3),(displayPointer->h*3) );
  betweenframes=&FireAndForgetWrappingProce55Viewer2d::NonConductedBetweenFrames;
 }
 virtual void NonConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
  EACHN(fx.first,Proce55or,p, {
   p->Between();
   if ( p->Done() ) {
    fx.Remove(p);
    delete p;
   }
  });
 }  
 virtual void ConductedBetweenFrames(void)  { // Done this way to avoid delay/duration
  NonConductedBetweenFrames();
 }
};
