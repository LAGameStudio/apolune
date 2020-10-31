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

#include "DisplayList.h"
#include "Cartesian.h"

extern DisplayList quad;
void InitGLPrimitives();

struct GLPrimitive: public ListItem{};

struct GLTriangle : public GLPrimitive {
 float x,y,x2,y2,x3,y3;
 GLTriangle( float x, float y, float x2, float y2, float x3, float y3 ) {
  this->x=x;
  this->y=y;
  this->x2=x2;
  this->y2=y2;
  this->x3=x3;
  this->y3=y3;
 }
 virtual void Draw() {
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(x,  y);  // 3   2
  glTexCoord2f(0.5f, 0.5f); glVertex2f(x2, y2); //
  glTexCoord2f(1.0f, 1.0f); glVertex2f(x3, y3); // 0
  glEnd();
 }
};

struct GLRect : public GLPrimitive {
public:
 float x,y,x2,y2;
 GLRect( float x, float y, float x2, float y2 ) {
  this->x=x;
  this->y=y;
  this->x2=x2;
  this->y2=y2;
 }
 virtual void Draw() {
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(x,  y);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(x2, y);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(x2, y2);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(x,  y2);
  glEnd();
 }
};

struct GLRects : public LinkedList {
public:
 void Add( float x, float y, float x2, float y2 ) {
  Append( new GLRect(x,y,x2,y2) );
 }
 CLEARLISTRESET(GLRect);
 ~GLRects() { Clear(); }
};

struct GLQuad : public ListItem {
public:
 Cartesian a,b,c,d;
 virtual void Draw() {
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(a.fx, a.fy);
  glTexCoord2f(1.0f, 0.0f); glVertex2f(b.fx, b.fy);
  glTexCoord2f(1.0f, 1.0f); glVertex2f(c.fx, c.fy);
  glTexCoord2f(0.0f, 1.0f); glVertex2f(d.fx, d.fy);
  glEnd();
 }
};

struct GLQuads : public LinkedList {
public:
 void Draw() {
  glBegin(GL_QUADS);
  FOREACH(GLQuad,q) {
   glTexCoord2f(0.0f, 0.0f); glVertex2f(q->a.fx, q->a.fy);
   glTexCoord2f(1.0f, 0.0f); glVertex2f(q->b.fx, q->b.fy);
   glTexCoord2f(1.0f, 1.0f); glVertex2f(q->c.fx, q->c.fy);
   glTexCoord2f(0.0f, 1.0f); glVertex2f(q->d.fx, q->d.fy);
  }
  glEnd();
 }
 void DrawNoBeginEnd() {
  FOREACH(GLQuad,q) {
   glTexCoord2f(0.0f, 0.0f); glVertex2f(q->a.fx, q->a.fy);
   glTexCoord2f(1.0f, 0.0f); glVertex2f(q->b.fx, q->b.fy);
   glTexCoord2f(1.0f, 1.0f); glVertex2f(q->c.fx, q->c.fy);
   glTexCoord2f(0.0f, 1.0f); glVertex2f(q->d.fx, q->d.fy);
  }
 }
 CLEARLISTRESET(GLQuad);
 ~GLQuads() { Clear(); }
};

struct GLPrimitives : public LinkedList {
public:
 void Add( float x, float y, float x2, float y2, float x3, float y3 )  {
  Append( new GLTriangle(x,y,x2,y2,x3,y3) );
 }
 void Add( float x, float y, float x2, float y2 ) {
  Append( new GLRect(x,y,x2,y2) );
 }
 CLEARLISTRESET(GLPrimitive);
 ~GLPrimitives() { Clear(); }
};

//struct VertexColorQuad : public ListItem {
//};