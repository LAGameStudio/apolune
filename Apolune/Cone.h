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

#include "ListItem.h"
#include "LinkedList.h"
#include "GLWindow.h"
#include "Cartesian.h"
#include "Display.h"

#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"
#include "OldVBO.h"
#include "Chemistry.h"
#include "Primitive3d.h"

class Cone3d : public Primitive3d {
public:
 int numStrips;
 Cone3d() : Primitive3d() {
  numStrips=16;
 }
 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
  mPolygon *p;
  Vertex *v;
  m->groups++;
  int group=m->groups;
  float angle;
  int i;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method=GL_TRIANGLE_FAN;

  v=new Vertex; v->x= 0; v->y= 10; v->z= 0; p->addVertex(v);
  for (i=0; i <= numStrips; i++) {
   angle = (float) ( 2.0f * (float) i * PI / (float) numStrips );
   v=new Vertex; v->x= (float) ( 5*cos(angle) ); v->y= 0; v->z= (float) ( 5*sin(angle) );
   p->addVertex(v); // strip
  }

  return pList;
 }
};

extern Cone3d cone3d;