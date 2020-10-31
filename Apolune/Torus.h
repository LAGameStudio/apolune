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


class Torus3d : public Primitive3d {
public:
 int numc, numt;
 Torus3d() : Primitive3d() {
  numc=6;
  numt=8;
 }
 PolygonList *addTorus( ImmediateModel *m, Substance *s ) {
  PolygonList *pList=new PolygonList;     
  mPolygon *p;
  Vertex *v;
  m->groups++;
  int group=(m->groups);
 
 // int numc=8;
 // int numt=24;
  int i, j, k;
  double f, t, x, y, z, twopi;
 
  twopi = 2 * (double) /*M_*/PI;
  for (i = 0; i < numc; i++) {
   p=new mPolygon; m->addPolygon(p); p->group=group; pList->Push(p); 
   p->draw_method = GL_QUAD_STRIP;
   for (j = 0; j <= numt; j++) {
    for (k = 1; k >= 0; k--) {
     f = (i + k) % numc + 0.5f;
     t = j % numt;
     z = (1+0.1f*cos(f*twopi/numc))*cos(t*twopi/numt)*10.0f;
     y = (1+0.1f*cos(f*twopi/numc))*sin(t*twopi/numt)*10.0f;
     x = 0.1f * sin(f * twopi / numc);
     v=new Vertex; v->x= (float) x*10.0f; v->y= (float) y*10.0f; v->z= (float) z*10.0f; p->addVertex(v);
    }
   }
  }
 
  return pList;
 }
};

extern Torus3d torus3d;