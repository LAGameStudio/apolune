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
#include <math.h>

#include "macros.h"
#include "moremath.h"

#include "GLWindow.h"
#include "GLImage.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Tween.h"
#include "Tweens.h"
#include "Crayon.h"
#include "Proce55Controller.h"
#include "Proce55or.h"
#include "Art.h"
#include "Audio.h"

#include "TextureLibrary.h"

#include "Interface.h"

extern Tweens tweens;
extern Interface input;

class fx_TweenSelect : public Proce55or {
public:
 bool hovering,clicking;
 Zpointer<Tween *>binding;
 int mx,my;
 float waiting;

 fx_TweenSelect() : Proce55or() {
  hovering=clicking=hidden=false;
  x2=y2=0;
  mx=my=-1;
  waiting=0.0f;
 }

 void Setup() { x2=x+w; y2=y+h; }

 void Render() {
  if ( hidden ) return;
  if ( *binding == null ) (*binding)=tweens.find("Humanized In Out");
  Blending(none);
  if ( this->parent )
  (*binding)->Render(this->parent->x+x+5,this->parent->y+y+5,w-10,h-10,crayons.Pick(alabaster)); //(clicking?crayons.Pick(alabaster):hovering?crayons.Pick(green):crayons.Pick(alabaster)));
  else
  (*binding)->Render(x+5,y+5,w-10,h-10,crayons.Pick(alabaster)); //(clicking?crayons.Pick(alabaster):hovering?crayons.Pick(green):crayons.Pick(alabaster)));
 }
 void Render(GLWindow *surface) {
  Draw(surface);
 }
 void Draw(GLWindow *surface) {
  Render();
 }

 virtual void OnSelect() {}

 void NextTween() {
  (*binding)=(Tween *) ( (*binding)->next ); 
  if ( !(*binding) ) (*binding)=(Tween *) tweens.first;
  OnSelect();
 }
 void PrevTween() {
  (*binding)=(Tween *) ( (*binding)->prev ); 
  if ( !(*binding) ) (*binding)=(Tween *) tweens.last;
  OnSelect();
 }

 void Between() {
  if ( hidden ) return;
  hovering=WITHIN(input.mxi,input.myi,x,y,x2,y2);
  if ( waiting > 0.0f ) waiting-=FRAMETIME;
  if ( waiting > 0.0f ) return;
  if ( !hovering ) return;
  if ( input.leftReleased() || input.wheelDown ) {
   NextTween();
  }
  if ( input.rightReleased() || input.wheelUp ) {
   PrevTween();
  }
 }
};


