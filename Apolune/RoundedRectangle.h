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

#include "Art.h"
#include "Vertex.h"
#include "Model.h"
#include "TextureLibrary.h"
#include "VBO.h"


class RoundedRectangle : public ListItem {
public:
 Zp<GLImage> image;
 Blends blend;
 Crayon border,inner,outer;
 Cartesian rect;
 Zint radius,precision;
 Zint segment_count;
 Zfloat lineWidth;
 VBOStaticVC interior;
 ImmediateModel borderLine;
 Zp<mPolygon> borderPoly;
 VBOStaticVCLines borderVBO;

 RoundedRectangle() {
  blend=none; 
  image = library.Load("data/images/elements/white.png");
 }
 RoundedRectangle( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h, int radius=16, int precision=8, float lineWidth=1.0f ) {
  blend=none; 
  image = library.Load("data/images/elements/white.png");
  Set(border,inner,outer,x,y,w,h,radius,precision,lineWidth);
 }
 void Clear() {
  borderLine.Clear();
  borderPoly=null;
  borderVBO.Clear();
  interior.Clear();
 }
 void Set( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h, int radius=16, int precision=8, float lineWidth=1.0f ) {
  Clear();
  this->border.fromCrayon(border);
  this->inner.fromCrayon(inner);
  this->outer.fromCrayon(outer);
  rect.SetRect(x,y,w,h);
  this->radius=radius;
  this->precision=precision;
  this->lineWidth=lineWidth;
  Calculate();
 }
 void Calculate();
 void Render();
 void RenderBorder();
};

class RoundedRectangles : public LinkedList {
public:
 RoundedRectangle *Create( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h,  int radius=16, int precision=8, float lineWidth=1.0f ) {
  RoundedRectangle *rr=new RoundedRectangle(border,inner,outer,x,y,w,h,radius,precision,lineWidth);
  Append(rr);
  return rr;
 }
 RoundedRectangle *Find( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h, int radius=16, int precision=8, float lineWidth=1.0f ) {
  FOREACH(RoundedRectangle,rr) {
   if ( rr->rect.x == x && rr->rect.y == y && rr->rect.w == w && rr->rect.h == h
    && rr->radius == radius && rr->precision == precision && rr->lineWidth == lineWidth
    && border.Equals(&rr->border) && inner.Equals(&rr->inner) && outer.Equals(&rr->outer) ) {
    return rr;
   }
  }
  return null;
 }
 RoundedRectangle *Render( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h, int radius=16, int precision=8, float lineWidth=1.0f ) {
  FOREACH(RoundedRectangle,rr) {
   if ( rr->rect.x == x && rr->rect.y == y && rr->rect.w == w && rr->rect.h == h
    && rr->radius == radius && rr->precision == precision && rr->lineWidth == lineWidth
    && border.Equals(&rr->border) && inner.Equals(&rr->inner) && outer.Equals(&rr->outer) ) {
    rr->Render();
    return rr;
   }
  }
  RoundedRectangle *rnew=Create( border,inner,outer,x,y,w,h,radius,precision,lineWidth );
  rnew->Render();
  return rnew;
 }
 CLEARLISTRESET(RoundedRectangle);
};

extern RoundedRectangles roundrects;  // application global, repeated locally