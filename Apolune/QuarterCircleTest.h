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

