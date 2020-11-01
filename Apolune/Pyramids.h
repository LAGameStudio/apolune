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

class Pyramid3d : public Primitive3d {
public:
 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
  mPolygon *p;
  Vertex *v;
  m->groups++;
  int group=m->groups;

  // sides
  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method = GL_TRIANGLES;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method = GL_TRIANGLES;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=  0; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method = GL_TRIANGLES;

  // bottom
  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y=-10; v->z= 10; p->addVertex(v);
  p->draw_method = GL_TRIANGLES;

  return pList;     
 }

};

extern Pyramid3d pyramid3d;

class Quadramid3d : public Primitive3d {
public:
 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
  mPolygon *p;
  Vertex *v;
  m->groups++;
  int group=m->groups;

  // sides
  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=-10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method=GL_TRIANGLES;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method=GL_TRIANGLES;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x= 10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method=GL_TRIANGLES;

  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=-10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x=  0; v->y= 10; v->z=  0; p->addVertex(v);
  p->draw_method=GL_TRIANGLES;
 
  //bottom
  p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
  v=new Vertex; v->x=-10; v->y=-10; v->z=-10; p->addVertex(v);
  v=new Vertex; v->x=-10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z= 10; p->addVertex(v);
  v=new Vertex; v->x= 10; v->y=-10; v->z=-10; p->addVertex(v);
  p->draw_method=GL_POLYGON;

  return pList;     
 }
};

extern Quadramid3d quadramid3d;

class Pentamid3d : public Primitive3d {
public:
 PolygonList *Immediate( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
//  mPolygon *p;
//  Vertex *v;   
//  int group=++m->groups;

  // bottom

  // sides
  return pList;
 }
};

extern Pentamid3d pentamid3d;