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
#include "Numbers.h"
#include "Proce55or.h"

class Line;
class Lines;

class fx_SectionalPercentages : public Proce55or {
public:
 Doubles values;
 Zint mx,my;
 fx_SectionalPercentages() : Proce55or() {}
 fx_SectionalPercentages( int divisions ) : Proce55or() {
  double dw=iratiod(1,divisions);
  unsigned int i;
  for ( i=0; i<(unsigned int)divisions; i++ ) values.Add((double)i*dw);
 }
 double AddDivision() {
  double dw=iratiod(1,values.count+1);
  double fw=dw/(double)values.count;
  EACH(values.first,Double,v) {
   v->d-=fw;
  }
  values.Add(dw);
  return dw;
 }
 virtual void WhenChanged() {}
 virtual void Setup() {}
 void Draw(GLWindow *surface) {}
 void LeftDown() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
 }
 void LeftUp() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
 }
 void RightDown() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
 }
 void RightUp() {
  mx=input.mxi-this->parent->x;
  my=input.myi-this->parent->y;
 }
};

