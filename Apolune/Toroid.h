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

class Toroid3d : public Primitive3d {
public:
 int pts, segments; 
 float angle;
 float radius;
 float latheRadius; 
  // for optional helix
 bool isHelix;
 float helixOffset;
 Toroid3d() : Primitive3d() {
  pts=3;
  segments=5;
  angle = 0.0f;
  radius = 1.0f;
  latheRadius = 100.0f; 
  isHelix = false;
  helixOffset = 5.0f;
 }
 void VertexBufferAndImmediate() {
  mPolygon *p;
  Vertex *w;
  model.groups++;
  int group=model.groups;
  //vertices
  Indexed<Zfloat> vx1, vy1, vz1;
  Indexed<Zfloat> vx2, vy2, vz2;
  int i, j;
  // initialize point arrays
  vx1.Size(pts+1);
  vy1.Size(pts+1);
  vz1.Size(pts+1);
  vx2.Size(pts+1);
  vy2.Size(pts+1);
  vz2.Size(pts+1);
 
  // fill arrays
  for(i=0; i<=pts; i++){
   vx1[i] = (float) ( latheRadius + sin(deg2rad(angle))*radius );
   if (isHelix){
    vz1[i] = (float) ( cos(deg2rad(angle))*radius-(helixOffset*segments)/2 );
   } 
   else{
    vz1[i] = (float) ( cos(deg2rad(angle))*radius );
   }
   angle+=360.0f/pts;
  }
 
  // draw toroid
  Vertex *a,*b,*c,*d; a=b=c=d=null;
  float latheAngle = 0.0f;
  int k=0;
  for(i=0; i<=segments; i++){
   p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
   p->draw_method = GL_QUAD_STRIP;
   for(j=0; j<=pts; j++){
    if (i>0){
     w=new Vertex; w->x= vx2[j]; w->y=vy2[j]; w->z=vz2[j]; p->addVertex(w);
     if ( !a ) a=w;
     else c=w;
    }
    vx2[j] = (float) cos(deg2rad(latheAngle))*vx1[j];
    vy2[j] = (float) sin(deg2rad(latheAngle))*vx1[j];
    vz2[j] = vz1[j];
    // optional helix offset
    if (isHelix) vz1[j]+=helixOffset;
    w=new Vertex; w->x= vx2[j]*10; w->y=vy2[j]*10; w->z=vz2[j]*10; p->addVertex(w);
    if ( !b ) b=w;
    else d=w;
   }
   // create extra rotation for helix
   if (isHelix){ latheAngle+=720.0f/segments; } 
   else {        latheAngle+=360.0f/segments; }

   /*
    *   a    b 1,1
    *    +--+
    *    |\ |
    *    | \|
    *    +--+  
    *   d    c 1,0
    */

   if ( a && b && c && d ) {
    a->q=0.0; a->r=1.0f; 
    b->q=1.0; b->r=1.0f; 
    c->q=1.0; c->r=0.0f; 
    d->q=0.0; d->r=0.0f; 
    vbo16.PushTriangle( a,b,c );
    vbo16.PushTriangle( a,c,d );
    a=b=c=d=null;
   }

  } // for
 }
};

extern Toroid3d toroid3d;