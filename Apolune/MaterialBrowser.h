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