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
#include "Vertex.h"
#include "DisplayList.h"

extern Vertexd up;

Vertices *CircleAboutAxis( int steps, Vertexd *axis );

class DLCircle2d {
public:
 DisplayList dlist;
 void Init() {
  Vertices circle;
  float delta=TWO_PIf/64.0f;
  float radius=256.0f;
  // Generate a set of triangular circles
  for ( float a=0.0f; a<TWO_PIf; a+=delta ) {
   circle.Add( (float) cosf(a) * radius, (float) sinf(a) * radius, 0.0f );
  }
  circle.Append( new Vertex( (Vertex *) circle.first ) );
  dlist.StartRecording();
  glBegin(GL_TRIANGLE_FAN);
  glVertex2i(0,0);
  EACH( circle.first, Vertex, v ) glVertex2f(v->x,v->y);
  glEnd();
  dlist.StopRecording();
 }
 void Render() {
  dlist.Call();
 }
};

extern DLCircle2d dlcircle2d;

void InitShapes();