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

#include "GLWindow.h"
#include "STLFile.h"
#include "Model.h"

class PointInTriangleTest : public GLWindow {
public:
 mPolygon tri;
 Zpointer<Vertex> a,b,c;
 Zbool inside;
 Vertex intersection,Ro,Rd;
 Zint result;
 Crayon color;
 STLFile source;
 ImmediateModel model;
 Zint intersections;  // pinpolys tested Ad Hoc profile
 PolygonList intersected;
 void OnLoad() {
  Fullscreen();
  background.Pick(clear);
  a=new Vertex(10.0f, 10.0f, 5.0f);// z=0 changed to 10
  b=new Vertex(500.0f, 500.0f, 9.0f);// z=0 changed to 10
  c=new Vertex(10.0f, 500.0f, 5.0f);// z=0 changed to 10
  a->Color(magenta);
  b->Color(chartreuse);
  c->Color(teal);
  tri.points.Append(a);
  tri.points.Append(b);
  tri.points.Append(c);
  source.Decode( "data/stl/leadscrew.stl" );
  source.toModel(&model);
  model.SetAlpha(0.2);
 }
 void Render() {
  tri.RenderImmediate2D();
  tri.points.DebugDraw(0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);
  if ( inside ) {
  }
  crayons.gl(magenta);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(500.0,700.0,0.0);
  model.Render2dXY(10.0f);
  EACH(intersected.first,PolygonHandle,pL) {
   mPolygon *p=pL->p;
   glBegin(p->draw_method);
   glNormal3f(p->normal->x,p->normal->y,p->normal->z);
   EACH(p->points.first,Vertex,v) {
    glColor4d(1.0,0,0,0.75);
    glTexCoord2f(v->q,v->r);
    glVertex2f(v->x*10.0f,v->y*10.0f);
   }
   glEnd();
  }
  glPopMatrix();
  Text(
   FORMAT("Ray Triangle:\nRo: %s -> Rd: %s\nRaycast result: %d\nPoint of intersection: %s\ninPoly2d? %s\nModel intersections: %d\nInside? %s",
    Ro.toString().c_str(),
    Rd.toString().c_str(),
    result.value,
    intersection.toString().c_str(),
    (inside?"yes":"no"),
    (int) intersections, AdHocPointInsideTest() ? "inside" : "outside"
   ).c_str(),
   crayons.Pick(macNcheese),transparency,512.0, 128.0, 8.0, 11.0, 4.0, 2.0, false );
 }
 bool AdHocPointInsideTest() {
  float testZ=1.0f;
  intersections=0;
  intersected.Clear();
  for ( mPolygon *p=(mPolygon *) model.polygons.first; p; p=(mPolygon *) p->next ) {
   p->Barycenter();
   if ( p->inPoly2d((float)(input->mxi),(float)(input->myi)) && (p->bary->z > testZ) ) {
    intersections+=1;
    intersected.Push(p);
   }
  }
  return (intersections % 2 == 1);
 }
 void Between() {
  inside = tri.inPoly2d((float)input->mxi,(float)input->myi);
  Ro.Set((float)input->mxi
	  ,(float)input->myi,-1.0f);
  Rd.Set(Ro.x,Ro.y,1000.0f);
  result = tri.RayTriangle( &Ro, &Rd, &intersection );
 }
};
