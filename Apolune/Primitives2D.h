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

#include <malloc.h>

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

#include "Numbers.h"

#include "Primitive3d.h"

/*
 *   a    b 1,1 <- tc
 *    +--+
 *    |\ |
 *    | \|
 *    +--+  
 *   d    c 1,0
 */
class Plane2d : public Primitive3d {
public:
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  Vertex *a,*b,*c,*d;
  
  a=new Vertex; a->x=0.0f; a->y=1.0f; a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(a);
  b=new Vertex; b->x=1.0f; b->y=1.0f; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(b);
  c=new Vertex; c->x=1.0f; c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(c);
  d=new Vertex; d->x=0.0f; d->y=0.0f; d->z=0.0f; d->q=0.0; d->r=0.0f; p->addVertex(d);

  vbo16.PushTriangle( a,b,c );
  vbo16.PushTriangle( a,c,d );
 }
};

extern Plane2d plane;

/*
 * Draws an "up" arrow.
 *
 *   a    b 1,1 <- tc           
 *    +--+                          /\counter          /\b   
 *    |\ |                         /  \        or     /  \     
 *    | \|              then     a/____\b           a/____\clockwise    
 *    +--+                          
 *   d    c 1,0
 *
 * Based on 1 so max width is 1.0f, and tail should be <1.0f unless you want a monopoly house 
 */
class Arrow2d : public Primitive3d {
public:
 Euler e;
 float headTail;
 float headWidth,headLength,tailWidth,tailLength;
 bool clockwise;
 Arrow2d() : Primitive3d() {
  headTail=0.25f;
  headWidth=0.25f;
  headLength=0.25f;
  tailWidth=0.1f;
  tailLength=0.75f;
  clockwise=true;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;

  float head=headTail;
  float tail=1.0f-headTail;
 
  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  Vertex *a=null,*b=null,*c=null,*d=null;
  
  if ( clockwise ) {
   a=new Vertex; a->x=0.0f;      a->y=tail; a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(a);
   b=new Vertex; b->x=tailWidth; b->y=tail; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(b);
   c=new Vertex; c->x=tailWidth; c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(c);
   d=new Vertex; d->x=0.0f;      d->y=0.0f; d->z=0.0f; d->q=0.0; d->r=0.0f; p->addVertex(d);
  } else {
   d=new Vertex; a->x=0.0f;      a->y=tail; a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(d);
   c=new Vertex; b->x=tailWidth; b->y=tail; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(c);
   b=new Vertex; c->x=tailWidth; c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(b);
   a=new Vertex; d->x=0.0f;      d->y=0.0f; d->z=0.0f; d->q=0.0; d->r=0.0f; p->addVertex(a);
  }
  vbo16.PushTriangle( a,b,c );
  vbo16.PushTriangle( a,c,d );
  
  a=new Vertex; a->x=headWidth/2.0f; a->y=head; a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(a);
  if ( clockwise ) {
   b=new Vertex; b->x=headWidth; b->y=0.0f; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(b);
   c=new Vertex; c->x=0.0f; c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(c);
  } else {
   c=new Vertex; b->x=headWidth; b->y=0.0f; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(c);
   b=new Vertex; c->x=0.0f;      c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(b);
  }
  vbo16.PushTriangle( a,b,c );
 }
};

/* 
 * Draws a semicircle then closes the top with a triangle.
 * Can be rolled with the Euler.
 */
class Teardrop2d : public Primitive3d {
public:
 Euler e;
 float X,Y;
 float A;
 int segments;
 bool clockwise;
 Teardrop2d() : Primitive3d() {
  A=X=Y=1.0f;
  segments=8;
  clockwise=true;
 }

 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
  if ( segments < 6 ) segments=6;
//  if ( segments % 2 == 1 ) segments++; // requires even # of segments, making it perfectly symmetric

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);

  // Draw the semicircle

  float delta=PIf/(float) segments;
  Vertex *a,*b=null,*c=null;
  if ( !clockwise ) {
   for ( float t=0.0f; t<PIf; t+=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.5f; a->r=0.5f; p->addVertex(a); // center, point a
    b=new Vertex; b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X; b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t > PIf-delta/2.0f && t < PIf+delta/2.0f ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  } else {
   for ( float t=PIf; t<0.0f; t-=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.5f; a->r=0.5f; p->addVertex(a); // center, point a
    b=new Vertex;b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X;
    b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t < delta/2.0f && t >-(delta/2.0f) ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  }

  // Draw the back /\

  if ( clockwise ) {
   a=new Vertex; a->x=0.0f; a->y=A;    a->z=0.0f; a->q=0.0f; a->r=1.0f; p->addVertex(a);
   b=new Vertex; b->x=X;    b->y=0.0f; b->z=0.0f; b->q=1.0f; b->r=1.0f; p->addVertex(b);
   c=new Vertex; c->x=-X;   c->y=0.0f; c->z=0.0f; c->q=1.0f; c->r=0.0f; p->addVertex(c);
  } else {
   a=new Vertex; a->x=0.0f; a->y=A;    a->z=0.0f; a->q=0.0f; a->r=1.0f; p->addVertex(a);
   b=new Vertex; b->x=X;    b->y=0.0f; b->z=0.0f; b->q=1.0f; b->r=1.0f; p->addVertex(b);
   c=new Vertex; c->x=-X;   c->y=0.0f; c->z=0.0f; c->q=1.0f; c->r=0.0f; p->addVertex(c);
  }

  a->rotateAround(&origin,&e);
  b->rotateAround(&origin,&e);
  c->rotateAround(&origin,&e);
  vbo16.PushTriangle( a,b,c );
 }
};


/* 
 * Looks like a dome if counterclockwise,
 * a smiley if clockwise, unless the euler's roll is used.
 */
class Semicircle2d : public Primitive3d {
public:
 Euler e;
 float X,Y;
 int segments;
 bool clockwise;
 Semicircle2d() : Primitive3d() {
  X=Y=1.0f;
  segments=8;
  clockwise=true;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
  if ( segments < 6 ) segments=6;
//  if ( segments % 2 == 1 ) segments++; // requires even # of segments, making it perfectly symmetric

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);

  float delta=PIf/(float) segments;
  Vertex *a,*b=null,*c=null;
  if ( !clockwise ) {
   for ( float t=0.0f; t<PIf; t+=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.5f; a->r=0.5f; p->addVertex(a); // center, point a
    b=new Vertex; b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X; b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t > PIf-delta/2.0f && t < PIf+delta/2.0f ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  } else {
   for ( float t=PIf; t<0.0f; t-=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.5f; a->r=0.5f; p->addVertex(a); // center, point a
    b=new Vertex;b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X;
    b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t < delta/2.0f && t >-(delta/2.0f) ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  }
 }
};

/* 
 * Can use the euler's roll to rotate the drawing.
 * Centered on origin.
 * Default texture coords follow position.
 */
class Ellipse2d : public Primitive3d {
public:
 Euler e;
 float X,Y;
 int segments;
 bool clockwise;
 Ellipse2d() : Primitive3d() {
  X=Y=1.0f;
  segments=8;
  clockwise=true;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
  if ( segments < 6 ) segments=6;
//  if ( segments % 2 == 1 ) segments++; // requires even # of segments, making it perfectly symmetric

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);

  float delta=TWO_PIf/(float) segments;
  Vertex *a,*b=null,*c=null;
  if ( !clockwise ) {
   for ( float t=0.0f; t<TWO_PIf; t+=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.0f; a->r=0.0f; p->addVertex(a); // center, point a
    b=new Vertex; b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X; b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t > TWO_PIf-delta/2.0f && t < TWO_PIf+delta/2.0f ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  } else {
   for ( float t=TWO_PIf; t<0.0f; t-=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.0f; a->r=0.0f; p->addVertex(a); // center, point a
    b=new Vertex;b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t)*X;
    b->y=sinf(t)*Y; 
    t+=delta;
    c=new Vertex;  c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t < delta/2.0f && t >-(delta/2.0f) ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t)*X;
     c->y=sinf(t)*Y;
    }
    //a->rotateAround(&origin,&e);
    b->rotateAround(&origin,&e);
    c->rotateAround(&origin,&e);
    vbo16.PushTriangle( a,b,c );
   }
  }
 }
};


/*
 * Unit triangle with two provided sides A,B.
 *  0,A
 *  |\h     rotated by 135 degrees to be centered on origin and aligned thus   /\
 *  |_\ B,0 where hypotenuse is approximately parallel to x-axis
 * a
 * Like signs A, B draws counterclockwise.
 * Differing signs A, B draws clockwise.
 * Can be manipulated by the Euler;
 */
class Triangle2d : public Primitive3d {
public:
 Euler e;
 float A,B;
 Triangle2d() : Primitive3d() {
  A=B=1.0f;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  p->draw_method=GL_TRIANGLES;
  Vertex *a,*b,*c;  
  Euler d;//efault rotation
  d.roll=(double)(deg2rad(90.0f+45.0f));
  a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.0f; a->r=1.0f; p->addVertex(a); //a->rotateAround(&origin,&d);
  b=new Vertex; b->x=B;    b->y=0.0f; b->z=0.0f; b->q=1.0f; b->r=1.0f; p->addVertex(b); b->rotateAround(&origin,&d);  
  c=new Vertex; c->x=1.0f; c->y=A;    c->z=0.0f; c->q=1.0f; c->r=0.0f; p->addVertex(c); c->rotateAround(&origin,&d);

  //a->rotateAround(&origin,&e);
  b->rotateAround(&origin,&e);
  c->rotateAround(&origin,&e);
  vbo16.PushTriangle( a,b,c );
 }
};


/* Geometric Circle Approximation as a VBO
 * Build using triangles.
 *
 *        /\counter          /\b
 *       /  \        or     /  \ 
 *     a/____\b           a/____\clockwise
 */
class Circle2d : public Primitive3d {
public:
 int segments;
 bool clockwise;
 Circle2d() : Primitive3d() {
  segments=8;
  clockwise=true;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
  if ( segments < 6 ) segments=6;
//  if ( segments % 2 == 1 ) segments++; // requires even # of segments, making it perfectly symmetric

  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);

  float delta=TWO_PIf/(float) segments;
  Vertex *a,*b=null,*c=null;
  if ( !clockwise ) {
   for ( float t=0.0f; t<TWO_PIf; t+=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.0f; a->r=0.0f; p->addVertex(a); // center, point a
    b=new Vertex; b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t);
    b->y=sinf(t); 
    t+=delta;
    c=new Vertex; c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t > TWO_PIf-delta/2.0f && t < TWO_PIf+delta/2.0f ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t);
     c->y=sinf(t);
    }
    vbo16.PushTriangle( a,b,c );
   }
  } else {
   for ( float t=TWO_PIf; t<0.0f; t-=delta ) {
    a=new Vertex; a->x=0.0f; a->y=0.0f; a->z=0.0f; a->q=0.0f; a->r=0.0f; p->addVertex(a); // center, point a
    b=new Vertex; b->z=0.0f; b->q=b->x; b->r=b->y; p->addVertex(b); // point b
    b->x=cosf(t); b->y=sinf(t); 
    t+=delta;
    c=new Vertex; c->z=0.0f; c->q=c->x; c->R=c->y; p->addVertex(b);
    // Make absolutely sure the starting and ending vertices are the same.
    if ( t < delta/2.0f && t >-(delta/2.0f) ) {
     Vertex *f=(Vertex *) (p->points.first->next); // second point
     c->x=f->x;
     c->y=f->y;
    } else {
     c->x=cosf(t);
     c->y=sinf(t);
    }
    vbo16.PushTriangle( a,b,c );
   }
  }
 }
};

/* Not in unit 1.0
 *   a  ______ b
 *     /\    /
 *    /  \  /
 * d /____\/ c
 * 
 */
class Trapezoid2d : public Primitive3d {
public:
 Euler e;
 float skew;
 Trapezoid2d() : Primitive3d() {
  skew=0.5f;
 }
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
 
  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  Vertex *a,*b,*c,*d;
  
  a=new Vertex; a->x=0.0f+skew; a->y=1.0f; a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(a);
  b=new Vertex; b->x=1.0f+skew; b->y=1.0f; b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(b);
  c=new Vertex; c->x=1.0f; c->y=0.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(c);
  d=new Vertex; d->x=0.0f; d->y=0.0f; d->z=0.0f; d->q=0.0; d->r=0.0f; p->addVertex(d);

  a->rotateAround(&origin,&e);
  b->rotateAround(&origin,&e);
  c->rotateAround(&origin,&e);
  vbo16.PushTriangle( a,b,c );
  vbo16.PushTriangle( a,c,d );
 }
};


/* Diamond with center of 0,0
 *
 *         a 0,1
 *         /\
 *        /__\b 1,0 <-xy 1,1 <-tc
 * -1,0  d\  /
 *   xy    \/
 *          c 0,-1 1,0 
 */

class Diamond2d : public Primitive3d {
public:
 Euler e;
 void VertexBufferAndImmediate() {
  model.groups++;
  int group=model.groups;
 
  mPolygon *p=new mPolygon; model.addPolygon(p); p->group=group; pList->Push(p);
  Vertex *a,*b,*c,*d;
  
  a=new Vertex; a->x=0.0f;  a->y=1.0f;  a->z=0.0f; a->q=0.0; a->r=1.0f; p->addVertex(a);
  b=new Vertex; b->x=1.0f;  b->y=1.0f;  b->z=0.0f; b->q=1.0; b->r=1.0f; p->addVertex(b);
  c=new Vertex; c->x=0.0f;  c->y=-1.0f; c->z=0.0f; c->q=1.0; c->r=0.0f; p->addVertex(c);
  d=new Vertex; d->x=-1.0f; d->y=0.0f;  d->z=0.0f; d->q=0.0; d->r=0.0f; p->addVertex(d);

  a->rotateAround(&origin,&e);
  b->rotateAround(&origin,&e);
  c->rotateAround(&origin,&e);
  d->rotateAround(&origin,&e);
  vbo16.PushTriangle( a,b,c );
  vbo16.PushTriangle( a,c,d );
 }
};