/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once

#include "GLWindow.h"
#include "Proce55or.h"
#include "Chemistry.h"
#include "Primitives2d.h"
#include "Material.h"
#include "WorldShader.h"

class MaterialBrowser : public GLWindow {
public:
 Zpointer<Substance> selected;
 Indexed<Zpointer<Substance>> selectable;
 Indexed<Cartesian> rects;
 Zint boxw;
 Zbool dragging;
 Zint mx,my;
 Crayon hi,lo,white,a,b,c,d;

 MaterialBrowser() : GLWindow() {
  selectable.Size(5);
  rects.Size(5);
 }

 void OnLoad() {
  SetPositionAndSize(x,y,w,h);
  mx=my=-1;
  selected=(Substance *) substances.first;
  for ( int i=0; i<5; i++ ) {
   selectable[i]=(Substance *) substances.Element(i);
   rects[i].SetRect(i*boxw,h,boxw,h);
  }
  a.Pick(red255);
  b.Pick(blue);
  c.Pick(green);
  d.Pick(alabaster);
  lo.Pick(orange);
  hi.Pick(cyan);
  white.Pick(alabaster);
  boxw=w/5;
 }

 void Between() {
  mx=input->mxi-this->x;
  my=input->myi-this->y;
  if ( WITHIN(mx,my,this->x,this->y,this->x2,this->y2) ) {
   int selecting=mx/5;
   selected=selectable[selecting];
  }
 }

 void Render() {
 }
};