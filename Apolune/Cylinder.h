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

class Cylinder3d : public Primitive3d {
public:
 int numStrips;
 Cylinder3d() : Primitive3d() {
  numStrips=12;
 }
 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  Init();
  PolygonList *pList=new PolygonList;     
  mPolygon *p,*q,*r;
  Vertex *v;
  float oldangle=-1;
  float angle=-1;
  m->groups++;
  int group=m->groups;
//  int numStrips=12;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method=GL_QUAD_STRIP;

  q=new mPolygon; m->addPolygon(q); q->group=group; pList->Push(q); // top
  q->draw_method=GL_QUAD_STRIP;

  r=new mPolygon; m->addPolygon(r); r->group=group; pList->Push(r); // bottom
  r->draw_method=GL_QUAD_STRIP;

  for (int i=0; i < numStrips; i++) {
   angle = (float) ( 2. * (float)i * PI / (float)numStrips );
   if ( oldangle > 0 ) {
    v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y=-1; v->z= (float) ( 5*sin(oldangle) ); p->addVertex(v); // strip
    v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y= 1; v->z= (float) ( 5*sin(oldangle) ); p->addVertex(v); // strip
    v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y=-1; v->z= (float) ( 5*sin(angle) );    p->addVertex(v); // strip
    v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y= 1; v->z= (float) ( 5*sin(angle) );    p->addVertex(v); // strip
   } 
   v=new Vertex;  v->x= (float) ( 5*cos(angle) );    v->y= 1; v->z= (float) ( 5*sin(angle) );    q->addVertex(v); // strip
   v=new Vertex;  v->x= (float) ( 5*cos(angle) );    v->y=-1; v->z= (float) ( 5*sin(angle) );    r->addVertex(v); // strip
   oldangle=angle;
  }

  // Last fragment?
  v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y=-1; v->z= (float) ( 5*sin(oldangle) );   p->addVertex(v); // strip
  v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y= 1; v->z= (float) ( 5*sin(oldangle) );   p->addVertex(v); // strip
  v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y=-1; v->z= (float) ( 5*sin(angle) );      p->addVertex(v); // strip
  v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y= 1; v->z= (float) ( 5*sin(angle) );      p->addVertex(v); // strip

  return pList;
 }
};

extern Cylinder3d cylinder3d;