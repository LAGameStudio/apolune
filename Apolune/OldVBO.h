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

#include "macros.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Strings.h"
#include "TexFont.h"

#include "GLSetup.h"
#include "GLImage.h"

#include "Cartesian.h"
#include "Vertex.h"
#include "Polygon.h"

#include "Material.h"

#include "dhpowareMath.h"


extern GLSetup gl;

class gCamera;
class Scene;

// Macro to return the byte offset into a buffer object in machine units.
// This is used primarily for Buffer Objects.
#if defined(BUFFER_OFFSET)
#undef BUFFER_OFFSET
#endif

#define BUFFER_OFFSET(bytes)  reinterpret_cast<const GLubyte *>(0) + bytes

//inline const GLubyte *BUFFER_OFFSET(size_t bytes);

enum TexCoordTypes {
 unary=1,
 flatland=2,
 projective=3,
 volumetric=4
};

// A "2 tex coord" VBO (8-float)
struct VBO_Vertex8	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
};

// A "PhongTexture" VBO (12-float)
struct VBO_Vertex12	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat color[4];
};

// A "WorldShaded" VBO (16-float)
struct VBO_Vertex16	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat color[4];
 GLfloat tangent[4];
};

// The Mother VBO
class VBO : public ListItem {
public:
 size_t stride;
 int sizeVertices, allocVertices;
	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
 bool uploaded;
 Vertices faceNormals;
 GLenum draw_method;
 GLvoid *data;

 Matrix4 modelMatrix;
 Matrix3 normalMatrix;

 VBO() : ListItem() { Constructor(); }
 void Constructor() {
  sizeVertices=allocVertices=0;
  vertexArrayObject=vertexBufferObject=0;
  uploaded=false;
  draw_method=GL_TRIANGLES;
  stride=0;
  data=null;
 }

 // Changes a single point in a "triangle face"
 virtual void ChangeVertex( Vertex *w, int i, int a, int b, int c ) {}
 // Changes all three points in a "triangle face"
 virtual void ChangeTriangle( Vertex *o, Vertex *p, Vertex *q, int a, int b, int c ) {}
 // Adds a triangle to the VBO before uploading.
 virtual void PushTriangle ( Vertex *a, Vertex *b, Vertex *c ) {}
 virtual void Render() {
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(draw_method, 0, sizeVertices);
  glBindVertexArray(0);
 }
 virtual void Clear() {}

 // "Static" member functions (static meaning - not virtual)
 // Calc Normal Matrix
 const float *calcNormalMatrix( Matrix4 viewMatrix ) 
 {
 	// The normal matrix is the transpose of the inverse of the upper 3x3
 	// sub-matrix of the model-view matrix. It is used to transform normals
 	// from model space into eye space.
 
 	Matrix4 modelViewMatrix = modelMatrix * viewMatrix;
 
 	normalMatrix[0][0] = modelViewMatrix[0][0];
 	normalMatrix[0][1] = modelViewMatrix[0][1];
 	normalMatrix[0][2] = modelViewMatrix[0][2];
 
 	normalMatrix[1][0] = modelViewMatrix[1][0];
 	normalMatrix[1][1] = modelViewMatrix[1][1];
 	normalMatrix[1][2] = modelViewMatrix[1][2];
 
 	normalMatrix[2][0] = modelViewMatrix[2][0];
 	normalMatrix[2][1] = modelViewMatrix[2][1];
 	normalMatrix[2][2] = modelViewMatrix[2][2];
 
 	normalMatrix = normalMatrix.inverse().transpose();
 
 	return &normalMatrix[0][0];
 }
 Strings log;
 void VBODebug( GLWindow *win ) {
  PrintTexFont( win, silverFont, crayons.Pick(green255), additive, win->w/2, 384, 10, 12, 2, &log);
 }
 void Bind() {
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
 }
 void Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 }

 virtual void Upload() {
  GL_Report("VBO:Upload() entering");
  if ( uploaded ) Release();
  GL_Report("VBO:Upload() Release()");
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  // Create the Vertex Buffer Object for the cube.
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, stride*sizeVertices, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  GL_Report("VBO:Upload()");
  uploaded=true;
 }

 void Reupload() {
  Release();
  Upload();
 }
 void Release() {
  if (vertexArrayObject)	 {	glDeleteVertexArrays(1, &vertexArrayObject);	vertexArrayObject  = 0; }
  if (vertexBufferObject) {	glDeleteBuffers(1, &vertexBufferObject);  	 	vertexBufferObject = 0; }
  uploaded=false;
 }
};

class VBO16 : public VBO {
public:
 VBO_Vertex16 *vertices;

 VBO16(void) {
  Constructor();
  stride=sizeof(VBO_Vertex16);
  vertices=null;
  data=vertices;
 }

 // Changes a single point in a "triangle face"
 void ChangeVertex( Vertex *w, int i, int a, int b, int c ) {
  static Vertex *n,*t;
  static Vertex A,B,C;

  vertices[i].pos[0]=(GLfloat) w->x; 
  vertices[i].pos[1]=(GLfloat) w->y; 
  vertices[i].pos[2]=(GLfloat) w->z; 
  
  A.Set( vertices[a].pos[0], vertices[a].pos[1], vertices[a].pos[2] );
  B.Set( vertices[b].pos[0], vertices[b].pos[1], vertices[b].pos[2] );
  C.Set( vertices[c].pos[0], vertices[c].pos[1], vertices[c].pos[2] );

  n=w->TriNormal(&A,&B,&C);
  t=w->calcTangentVector(&A,&B,&C,n);

  vertices[a].normal[0]=vertices[b].normal[0]=vertices[c].normal[0]=(GLfloat) n->x;
  vertices[a].normal[1]=vertices[b].normal[1]=vertices[c].normal[1]=(GLfloat) n->y;
  vertices[a].normal[2]=vertices[b].normal[2]=vertices[c].normal[2]=(GLfloat) n->z;

  vertices[a].tangent[0]=vertices[b].tangent[0]=vertices[c].tangent[0]=t->x;
  vertices[a].tangent[1]=vertices[b].tangent[1]=vertices[c].tangent[1]=t->y;
  vertices[a].tangent[2]=vertices[b].tangent[2]=vertices[c].tangent[2]=t->z;
  vertices[a].tangent[3]=vertices[b].tangent[3]=vertices[c].tangent[3]=t->w;  

  delete n;
  delete t;
 }

 // Changes all three points in a "triangle face"
 void ChangeTriangle( Vertex *o, Vertex *p, Vertex *q, int a, int b, int c ) {
  static Vertex *n,*t;
  static Vertex A,B,C;

  vertices[a].pos[0]=(GLfloat) o->x; vertices[b].pos[0]=(GLfloat) p->x; vertices[c].pos[0]=(GLfloat) q->x;  
  vertices[a].pos[1]=(GLfloat) o->y; vertices[b].pos[1]=(GLfloat) p->y; vertices[c].pos[1]=(GLfloat) q->y;  
  vertices[a].pos[2]=(GLfloat) o->z; vertices[b].pos[2]=(GLfloat) p->z; vertices[c].pos[2]=(GLfloat) q->z;  
  
  A.Set( vertices[a].pos[0], vertices[a].pos[1], vertices[a].pos[2] );
  B.Set( vertices[b].pos[0], vertices[b].pos[1], vertices[b].pos[2] );
  C.Set( vertices[c].pos[0], vertices[c].pos[1], vertices[c].pos[2] );

  n=o->TriNormal(&A,&B,&C);
  t=o->calcTangentVector(&A,&B,&C,n);

  vertices[a].normal[0]=vertices[b].normal[0]=vertices[c].normal[0]=(GLfloat) n->x;
  vertices[a].normal[1]=vertices[b].normal[1]=vertices[c].normal[1]=(GLfloat) n->y;
  vertices[a].normal[2]=vertices[b].normal[2]=vertices[c].normal[2]=(GLfloat) n->z;

  vertices[a].tangent[0]=vertices[b].tangent[0]=vertices[c].tangent[0]=t->x;
  vertices[a].tangent[1]=vertices[b].tangent[1]=vertices[c].tangent[1]=t->y;
  vertices[a].tangent[2]=vertices[b].tangent[2]=vertices[c].tangent[2]=t->z;
  vertices[a].tangent[3]=vertices[b].tangent[3]=vertices[c].tangent[3]=t->w;  

  delete n;
  delete t;
 }

 void PushTriangle ( Vertex *a, Vertex *b, Vertex *c ) { 
  Vertex *n=null;
  if ( a->normal ) n=a->normal;
  else n=c->TriNormal(a,b,c);
  Vertex *t = c->calcTangentVector(a,b,c,n);

  /// Cppcheck notes: Common realloc mistake: 'vertices' nulled but not freed upon failure
  /*
  MSVC 2015 Code Analyzer says:
  c:\users\moops\documents\visual studio 2015\projects\framework\framework\oldvbo.h(260): warning C6308: 'realloc' might return null pointer: assigning null pointer to 'vertices', which is passed as an argument to 'realloc', will cause the original memory block to be leaked.
  */
  vertices  = (VBO_Vertex16 *) realloc(vertices,  (allocVertices= stride * (sizeVertices + 3) ) );
  if ( vertices == NULL ) {
   OUTPUT("OldVBO::PushTriangle Could not re-allocate\n");
   return;
  }
  data = (GLvoid *) (vertices);

    vertices[sizeVertices].pos[0]=(GLfloat) a->x;
  vertices[sizeVertices].pos[1]=(GLfloat) a->y;
  vertices[sizeVertices].pos[2]=(GLfloat) a->z;

  vertices[sizeVertices].normal[0]=(GLfloat) n->x;
  vertices[sizeVertices].normal[1]=(GLfloat) n->y;
  vertices[sizeVertices].normal[2]=(GLfloat) n->z;

  vertices[sizeVertices].tangent[0]=t->x;
  vertices[sizeVertices].tangent[1]=t->y;
  vertices[sizeVertices].tangent[2]=t->z;
  vertices[sizeVertices].tangent[3]=t->w;

  vertices[sizeVertices].texCoord[0]=(GLfloat) a->r;
  vertices[sizeVertices].texCoord[1]=(GLfloat) a->q;

  vertices[sizeVertices].color[0]=(GLfloat) a->rgba[0];
  vertices[sizeVertices].color[1]=(GLfloat) a->rgba[1];
  vertices[sizeVertices].color[2]=(GLfloat) a->rgba[2];
  vertices[sizeVertices].color[3]=(GLfloat) a->rgba[3];

  vertices[sizeVertices+1].pos[0]=(GLfloat) b->x;
  vertices[sizeVertices+1].pos[1]=(GLfloat) b->y;
  vertices[sizeVertices+1].pos[2]=(GLfloat) b->z;

  vertices[sizeVertices+1].normal[0]=(GLfloat) n->x;
  vertices[sizeVertices+1].normal[1]=(GLfloat) n->y;
  vertices[sizeVertices+1].normal[2]=(GLfloat) n->z;

  vertices[sizeVertices+1].tangent[0]=t->x;
  vertices[sizeVertices+1].tangent[1]=t->y;
  vertices[sizeVertices+1].tangent[2]=t->z;
  vertices[sizeVertices+1].tangent[3]=t->w;

  vertices[sizeVertices+1].texCoord[0]=(GLfloat) b->r;
  vertices[sizeVertices+1].texCoord[1]=(GLfloat) b->q;

  vertices[sizeVertices+1].color[0]= (GLfloat) b->rgba[0];
  vertices[sizeVertices+1].color[1]= (GLfloat) b->rgba[1];
  vertices[sizeVertices+1].color[2]= (GLfloat) b->rgba[2];
  vertices[sizeVertices+1].color[3]= (GLfloat) b->rgba[3];

  vertices[sizeVertices+2].pos[0]=(GLfloat) c->x;
  vertices[sizeVertices+2].pos[1]=(GLfloat) c->y;
  vertices[sizeVertices+2].pos[2]=(GLfloat) c->z;

  vertices[sizeVertices+2].normal[0]=(GLfloat) n->x;
  vertices[sizeVertices+2].normal[1]=(GLfloat) n->y;
  vertices[sizeVertices+2].normal[2]=(GLfloat) n->z;

  vertices[sizeVertices+2].tangent[0]=t->x;
  vertices[sizeVertices+2].tangent[1]=t->y;
  vertices[sizeVertices+2].tangent[2]=t->z;
  vertices[sizeVertices+2].tangent[3]=t->w;

  vertices[sizeVertices+2].texCoord[0]=(GLfloat) c->r;
  vertices[sizeVertices+2].texCoord[1]=(GLfloat) c->q;

  vertices[sizeVertices+2].color[0]= (GLfloat) c->rgba[0];
  vertices[sizeVertices+2].color[1]= (GLfloat) c->rgba[1];
  vertices[sizeVertices+2].color[2]= (GLfloat) c->rgba[2];
  vertices[sizeVertices+2].color[3]= (GLfloat) c->rgba[3];

  sizeVertices +=3;
  if ( !a->normal ) delete n;
  delete t;
 }

// void Render( Vertex *offset, Vertex *position, Vertex *scale, Vertex *rotation, Quaternionf *q, gCamera *c, Scene *s );

 void Clear() {
  if ( uploaded ) Release();
  if ( vertices )  { free(vertices);  vertices=null;  }
  allocVertices=0;
 }

 ~VBO16(void) {
  Clear();
 }
};

class VBOs : public LinkedList {
public:
 void Render() {
  FOREACH(VBO,v) v->Render();
 }
 void Upload() {
  for ( VBO *v = (VBO *) first; v; v=(VBO *) v->next ) {
   v->Upload();
  }
 }
 CLEARLISTRESET(VBO);
 ~VBOs() { Clear(); }
};