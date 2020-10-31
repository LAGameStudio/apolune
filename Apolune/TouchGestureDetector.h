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
#include "Interface.h"

#define SWIPE_UP 0
#define SWIPE_DOWN 1
#define SWIPE_LEFT 2
#define SWIPE_RIGHT 3

class TouchGestureDetector {
public:
 Zbool touchDown,touchReleased;
 Zbool touching;
 Cartesian pointer,last,touched,swipes,swiped,released;
 Zbool swiping,wasSwiping;
 Zfloat swipeIntensityWhenDetected;
 Zint direction,directionLR,directionUD;
 Zdouble swipeAngle;
 Zint horizontal,vertical;
 Cartesian delta;
 Cartesians prior,previous;
 Zfloat SWIPE_SENSITIVITY_RADIUS;
 Zint MAX_PRIOR_GESTURE_POINTS;
 TouchGestureDetector() {
  Reset();
 }
 void Reset() {
  SWIPE_SENSITIVITY_RADIUS=48.0f;
  MAX_PRIOR_GESTURE_POINTS=1000;
  touchDown=false;
  touchReleased=false;
  touching=false;
  swiping=false;
  wasSwiping=false;
  horizontal=0;
  vertical=0;
  delta.Set(0,0);
  prior.Clear();
  previous.Clear();
 }
 std::string asString() {
  return FORMAT(
   "touchDown: %s   touchReleased: %s  touching: %s\n"
   "swiping: %s   wasSwiping:%s  swipeIntensity: %f\n"
   "pointer: %s    last: %s\n"
   "touched:  %s   swipes:  %s\n"
   "direction,LR,UD: %d %d %d   swipeAngle: %f\nhorizontal: %d vertical: %d"
   "prior detections in gesture: %d\nprevious gesture complexity: %d\nswipe radius: %1.2f   max gesture length: %d\nswiped: %s\ndelta: %s\n",
   touchDown ? "yes" : "no ",
   touchReleased ? "yes" : "no ",
   touching ? "yes" : "no ",
   swiping ? "yes" : "no ",
   wasSwiping ? "yes" : "no ",
   swipeIntensityWhenDetected.value,
   pointer.toString().c_str(),
   last.toString().c_str(),
   touched.toString().c_str(),
   swipes.toString().c_str(),
   direction.value, directionLR.value, directionUD.value,
   swipeAngle.value,
   (int) horizontal, (int) vertical,
   (int) prior.count,
   (int) previous.count,
   (float) SWIPE_SENSITIVITY_RADIUS,
   (int) MAX_PRIOR_GESTURE_POINTS,
   swiped.toString().c_str(),
   delta.toString().c_str()
  );
 }
 void Between() {
  last.Set(pointer.x,pointer.y);
  pointer.Set(inputPointer->mxi,inputPointer->myi);
  if ( inputPointer->left ) {
   if ( touchDown ) {
    if ( !swiping ) {
     float d=pointer.Distance(touched.x,touched.y);
     if ( d > SWIPE_SENSITIVITY_RADIUS ) {
      OnSwipeStart();
      swiping=true;
      swipeIntensityWhenDetected=d;
      swipes.SetLine(touched.x,touched.y,pointer.x,pointer.y); // Point of swipe detection
      swiped.Set(&swipes);
      swipeAngle=(double)swiped.LineAngle();
      directionLR= ( touched.x <= pointer.x ) ? SWIPE_RIGHT : SWIPE_LEFT;
      directionUD= ( touched.y <= pointer.y ) ? SWIPE_DOWN : SWIPE_UP;
      direction= ( abs(touched.x-pointer.x) > abs(touched.y-pointer.y) ) ? directionLR : directionUD;
     }
    }
    horizontal=pointer.x-touched.x;
    vertical=pointer.y-touched.y;
   } else {
    OnTouchDown();
    touched.Set(pointer.x,pointer.y); // Point initiated contact
    last.Set(pointer.x,pointer.y); // Set last to current here to eliminate previous detections
    touchDown=true;
    touching=true;
    horizontal=0;
    vertical=0;
    wasSwiping=false;
   }
  } else touchDown=false;
  touchReleased=inputPointer->leftReleased();
  if ( touchReleased ) {
   if ( swiping ) swiped.SetLine(touched.x,touched.y,pointer.x,pointer.y);
   touchDown=false;
   touching=false;
   wasSwiping=swiping;
   swiping=false;
   prior.Add(pointer.x,pointer.y);
   released.Set(pointer.x,pointer.y);
   previous.Clear();
   previous.Concat(&prior);
   if ( wasSwiping ) OnSwipeEnd();
   OnTouchUp();
  }
  if ( touchDown && prior.last ) {
   Cartesian *lastFrame=(Cartesian *) prior.last;
   delta.Set(pointer.x-lastFrame->x,pointer.y-lastFrame->y);
  } else delta.Set(0,0);
  if ( touchDown && prior.count < MAX_PRIOR_GESTURE_POINTS ) {
   prior.Add(pointer.x,pointer.y);
  }
 }
 virtual void OnTouchDown() {
 }
 virtual void OnTouchUp() {
 }
 virtual void OnSwipeStart() {
 }
 virtual void OnSwipeEnd() {
 }
};