#pragma once
#include "GLWindow.h"
#include "Model.h"

class QuarterCircleTest : public GLWindow {
public:
 ImmediateModel qC;
 ImmediateModel rectangle;
 Zint precision,radius;
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  Calculate();
  
  precision=8;
  radius=16;
 }
 void Calculate() {
  qC.Clear();
  
  Circle circle;
  circle.R=(double)radius;

  mPolygon *p=new mPolygon;
  p->draw_method=GL_TRIANGLE_FAN;
  qC.addPolygon(p);

  Vertex *cp=new Vertex;
  cp->Color(hotPink);
  p->addVertex(cp);

  Vertexd point;

  for ( int i=0; i<precision+1; i++ ) {
   circle.Point(iratiod(i,precision)*0.25,&point);
   Vertex *v=new Vertex;
  	v->Set( (float) point.x , (float) point.y );
  	v->q=(float)point.x / (float)radius;
  	v->r=(float)point.y / (float)radius;
   v->Color(crayons.Pick(green255));
   p->points.Append(v);
  }

 }
 void Between() {
  if ( input->wheelUp ) {
   radius+=1;
   Calculate();
  }
  if ( input->wheelDown ) {
   radius-=1;
   Calculate();
  }
 }
 void Render() {
  qC.Render2dVC((float)input->mxd*display->wf,(float)input->myd*display->hf,0.0f,1.0f,1.0f,0.0f);
 }
};

