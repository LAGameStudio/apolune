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

#include "Proce55or.h"
#include "ZSortedFAFZonedProce55Viewer2d.h"

class ZonedMovingProce55orDebugger : public Proce55or {
public:
 bool moving,hovering;
 int movebyx,movebyy;
 int mx,my;
 ZonedMovingProce55orDebugger( ZonedProce55orsGrid *zones ) : Proce55or() {
  x=(int)zones->extents.x+upto((int)zones->extents.w);
  y=(int)zones->extents.y+upto((int)zones->extents.h);
  w=256;
  h=256;
  x2=x+256;
  y2=y+256;
  movebyx=2-upto(5);
  movebyy=2-upto(5);
  mx=my=-1;
  moving=hovering=false;
 }
 virtual void Debug() {
  Cartesian extents; extents.SetRect(translated.x,translated.y,translated.w,translated.h);
  Crayon drawing;
  if ( moving ) drawing.Pick(green); else drawing.Pick(red255);
  if ( hovering ) drawing.ScaleByAlphaf(0.75f); else drawing.ScaleByAlphaf(0.25f);
  glColor4fv(drawing.floats);
  Blending(additive);
  Area(drawing,translated.x,translated.y,translated.x2,translated.y2);
  drawing.Any();
  Rectangle(drawing,&extents);
  drawing.Pick(alabaster);
  DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue),
   (double)translated.x,(double)translated.y,(double)translated.x2,(double)translated.y2);
  DrawLine2Color(crayons.Pick(yellow),crayons.Pick(green),
   (double)translated.x,(double)translated.y2,(double)translated.x2,(double)translated.y);
  Blending(additive);
  DrawLine2Color(crayons.Pick(green),crayons.Pick(black),(double)(translated.x+translated.w/2),(double)(translated.y+translated.h/2),(double)input.mxi,(double)input.myi);
  char buf[80];
  FORMAT(buf,80,"%d,%d\n%d,%d",(int) x,(int) y, (int) movebyx, (int) movebyy );
  Text(buf,crayons.Pick(green255),none,(double) (translated.x+w/2), (double) (translated.y+h/2), 10.0, 8.0, 4.0, 1.0,true );
 }
 void Between() {
  if ( moving ) {
   ZSortedFAFZonedProce55Viewer2d *p=(ZSortedFAFZonedProce55Viewer2d *) this->parent.pointer;
   this->MoveBy( movebyx, movebyy );
   p->grid.Rezone(this);
  }
  MouseMove();
 }
 void MouseMove() {
  MousePosition(&mx,&my);
  hovering= ( WITHIN(mx,my,translated.x,translated.y,translated.x2,translated.y2) );  
 }
 void LeftUp() {
  if ( hovering ) moving=!moving;
 }
 void RenderRelative( GLWindow *t, ScreenPosition *screen ) {
  Debug();
 }
 void Render( GLWindow *t ) {
  Debug();
 }
 // Y() must be overridden in any Proce55or used in a ZonedProce55orGrid
 int Y() {
  return translated.y2;
 }
};