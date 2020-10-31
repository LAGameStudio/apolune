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

class Tube3d : public Primitive3d {
public:
 int numStrips;
 Tube3d() : Primitive3d() {
  numStrips=16;
 }

 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
  mPolygon *p, *q, *r, *t;
  Vertex *v;
  m->groups++;
  int group=m->groups;

  float oldangle=-1;
  float angle=-1;
  int i;
//  int numStrips=12;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method=GL_QUAD_STRIP;

  q=new mPolygon; m->addPolygon(q); q->group=group; pList->Push(q);
  q->draw_method=GL_QUAD_STRIP;

  r=new mPolygon; m->addPolygon(r); q->group=group; pList->Push(r); // top
  t=new mPolygon; m->addPolygon(t); r->group=group; pList->Push(t); // bottom

  for (i=0; i <= numStrips; i++) {
      angle = (float) ( 2.0f * (float) i * PI / (float) numStrips );
      if ( oldangle > 0 ) { // draw interior and exterior panels

       // exterior
       v=new Vertex; v->x= (float) ( 10*cos(oldangle) ); v->y=-10; v->z= (float) ( 10*sin(oldangle) ); p->addVertex(v);
       v=new Vertex; v->x= (float) ( 10*cos(oldangle) ); v->y= 10; v->z= (float) ( 10*sin(oldangle) ); p->addVertex(v); 
       v=new Vertex; v->x= (float) ( 10*cos(angle) );    v->y=-10; v->z= (float) ( 10*sin(angle) );    p->addVertex(v);
       v=new Vertex; v->x= (float) ( 10*cos(angle) );    v->y= 10; v->z= (float) ( 10*sin(angle) );    p->addVertex(v); 

       // interior
       v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y=-1; v->z= (float) ( 5*sin(oldangle) ); q->addVertex(v);
       v=new Vertex; v->x= (float) ( 5*cos(oldangle) ); v->y= 1; v->z= (float) ( 5*sin(oldangle) ); q->addVertex(v); 
       v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y=-1; v->z= (float) ( 5*sin(angle) );    q->addVertex(v);
       v=new Vertex; v->x= (float) ( 5*cos(angle) );    v->y= 1; v->z= (float) ( 5*sin(angle) );    q->addVertex(v); 
      }

      v=new Vertex; v->x= (float) ( 10*cos(angle) ); v->y= 1; v->z= (float) ( 5*sin(angle) ); r->addVertex(v);
      v=new Vertex; v->x= (float) ( 5*cos(angle) );  v->y= 1; v->z= (float) ( 5*sin(angle) ); r->addVertex(v);

      v=new Vertex; v->x= (float) ( 10*cos(angle) ); v->y=-1; v->z= (float) ( 5*sin(angle) ); t->addVertex(v);
      v=new Vertex; v->x= (float) ( 5*cos(angle) );  v->y=-1; v->z= (float) ( 5*sin(angle) ); t->addVertex(v);

      oldangle=angle;
   }

//  v=VERTEX {}; v->x= 5*cos(oldangle); v->y=-1; v->z= 5*sin(oldangle); p->addVertex(v);
//  v=VERTEX {}; v->x= 5*cos(oldangle); v->y= 1; v->z= 5*sin(oldangle); p->addVertex(v);
//  v=VERTEX {}; v->x= 5*cos(angle);    v->y=-1; v->z= 5*sin(angle);    p->addVertex(v);
//  v=VERTEX {}; v->x= 5*cos(angle);    v->y= 1; v->z= 5*sin(angle);    p->addVertex(v);

  return pList;    
 }


};

extern Tube3d tube3d;