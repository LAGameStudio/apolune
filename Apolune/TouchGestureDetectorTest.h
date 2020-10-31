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
#include "UglyFont.h"
#include "TouchGestureDetector.h"
#include "Art.h"

class TouchGestureDetectorTest : public GLWindow {
public:
 TouchGestureDetector tgd;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
 }
 void Between() {
  tgd.Between();
  if ( input->KeyDown(LCtrl) ) {
   if ( input->wheelUp ) tgd.MAX_PRIOR_GESTURE_POINTS += 100;
   if ( input->wheelDown && tgd.MAX_PRIOR_GESTURE_POINTS > 0 ) tgd.MAX_PRIOR_GESTURE_POINTS -= 100;
  } else {
   if ( input->wheelUp ) tgd.SWIPE_SENSITIVITY_RADIUS += 8.0f;
   if ( input->wheelDown && tgd.SWIPE_SENSITIVITY_RADIUS > 0.0f ) tgd.SWIPE_SENSITIVITY_RADIUS -= 8.0f;
  }
 }
 void Render() {
  crayons.Pick(magenta).gl();
  DrawLine2Color(crayons.Pick(hotPink),crayons.Pick(brown),tgd.swiped.x,tgd.swiped.y,tgd.swiped.x2,tgd.swiped.y2);
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(green255),tgd.touched.x,tgd.touched.y,tgd.pointer.x,tgd.pointer.y);
  DrawNamedPoint(crayons.Pick(red255),"touched",tgd.touched.x,tgd.touched.y);
  DrawNamedPoint(crayons.Pick(green255),"pointer",tgd.touched.x,tgd.touched.y);
  DrawNamedPoint(crayons.Pick(orange),"swipes",tgd.swipes.x,tgd.swipes.y);
  DrawNamedPoint(crayons.Pick(gray),"last",tgd.last.x,tgd.last.y);
  if ( !input->KeyDown(LAlt) ) {
   EACH(tgd.prior.first,Cartesian,c) if ( c->next ) {
    Cartesian *n=(Cartesian *) c->next;
    DrawLine2Color(crayons.Pick(blue255),crayons.Pick(gold),c->x,c->y,n->x,n->y);
   }
   EACH(tgd.previous.first,Cartesian,c) if ( c->next ) {
    Cartesian *n=(Cartesian *) c->next;
    DrawLine2Color(crayons.Pick(cyan),crayons.Pick(peru),c->x,c->y,n->x,n->y);
   }
  }
  DrawCircle(tgd.touched.x,tgd.touched.y,tgd.SWIPE_SENSITIVITY_RADIUS,8);
  Text(tgd.asString().c_str(),16,20,12,16,2,false);
 }
};