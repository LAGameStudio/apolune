#include "Shape.h"

// This value is set to 0,1,0 in main.cpp
Vertexd up;

Vertices *CircleAboutAxis( int steps, Vertexd *axis ) {
 Vertex *first;
 Vertices *circle=new Vertices;
 steps--;
 float a=0.0f, delta=TWO_PIf/(float) (steps);
 // First create the circle about the (0,1,0) axis
 for (int i = 0; i < steps; i++) {
  Vertex *v=new Vertex((float)cosf(a), 0.0f, (float)sinf(a));
  a -= delta;
  circle->Append(v);
 }
 first =(Vertex *) circle->first;
 Vertex *v=new Vertex( first->x, first->y, first->z );
 circle->Append(v);
 return circle;
}

DLCircle2d dlcircle2d;

void InitShapes() {
 dlcircle2d.Init();
}