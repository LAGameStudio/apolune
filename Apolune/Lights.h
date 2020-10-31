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

#include "macros.h"
#include "moremath.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "Crayon.h"
#include "Vertex.h"
#include "Proce55Controller.h"
#include "Matrix4x4.h"
#include "Art.h"

/*
 * Virtualized GL Lighting Techniques
 */
#define MAX_GL_LIGHTS 8


#define MAX_LIGHTS 1000

enum LightTypes {
 directional_light=0,
 point_light=1,
 spot_light=2
};

class Light : public ListItem {
public:
 Proce55Controller controller;
 Crayon color;
 Blends blend;
 Vertex position;
 Vertex direction;
 Crayon diffuse;
 Crayon ambient;
 Crayon specular;
 float constantAttenuation;                
 float linearAttenuation;
 float quadraticAttenuation;  
 float innerCone,outerCone,cosInnerCone,cosOuterCone;
 float brightness;
 float radius;
 LightTypes type;
 bool lit;
 GLuint index;
 PROGRAMMABLE(Light,InitPtr,init,Init);
 Light() {
  type=point_light;
  lit=true;
  prev=next=null;
  constantAttenuation=0.0f;
  linearAttenuation=0.0f;
  quadraticAttenuation=0.0f;
  brightness=0.0f;
  radius=5.0f;
  init=&Light::InitAttenuated;
  blend=additive;
  innerCone=10.0f;
  outerCone=15.0f;
  SetSpot( innerCone, outerCone );
  ambient.Float(1.0f, 1.0f, 1.0f, 1.0f);
  diffuse.Float(1.0f, 1.0f, 1.0f, 1.0f);
	 specular.Float(1.0f, 1.0f, 1.0f, 1.0f);
  direction.Set(0.0f,0.0f,-1.0f);
 }
 void InitLinear() {
  glLightf(index, GL_CONSTANT_ATTENUATION, 0.0f);
  glLightf(index, GL_LINEAR_ATTENUATION, 0.4f);
  glLightf(index, GL_QUADRATIC_ATTENUATION, 0.0f);
  glLightfv(index, GL_SPECULAR, specular.floats);
 }
 void InitAttenuated() {
  glLightf(index, GL_CONSTANT_ATTENUATION, 0.0f);
  glLightf(index, GL_LINEAR_ATTENUATION, 0.0f);
  glLightf(index, GL_QUADRATIC_ATTENUATION, 0.1f);
  glLightfv(index, GL_SPECULAR, specular.floats);
 }
 void SetSpot( float innerAngle, float outerAngle ) {
  cosInnerCone=cosf((innerCone=innerAngle) * (3.14159265358979323846f / 180.0f));
  cosOuterCone=cosf((outerCone=outerAngle) * (3.14159265358979323846f / 180.0f));
 }
 float Relevance( Vertex *p ) {
  return p->distanceSquaredTo(&position) - radius;
 }
 VECTOR3D GetPosition() {
  VECTOR3D v;
  v.Set(position.x,position.y,position.z);
  return v;
 }

 ~Light() {
 }
};

class LightHandle : public ListItem {
public:
 Light *light;
 LightHandle() {
  next=prev=null;
  light=null;
 }
};

class LightList : public LinkedList {
public:
 void Add( Light *light ) {
  LightHandle *h=new LightHandle;
  h->light=light;
  Append(h);
 }
 SORTING(LightHandle,SortBrighter,{},{
  GLfloat diff = (B->light->lit ? B->light->brightness : 0.0f)
               - (A->light->lit ? A->light->brightness : 0.0f);  /* The brighter lights get sorted close to top of the list. */
  if (diff < 0) result=-1;
 },{});
 CLEARLISTRESET(LightHandle);
 ~LightList() { Clear(); }
};

class Lights : public LinkedList {
public:
 Light *current;
 GLfloat angle;
 int active;
 GLfloat  x,  y,  z;
 GLfloat ex, ey, ez;
 GLfloat nx, ny, nz;
 GLfloat dx, dy, dz;
 GLfloat quadraticAttenuation;
 GLfloat t;
 PROGRAMMABLE(Lights,MultiLightModelPtr,lightmodel,MultiLightModel);
 Vertex *w;
 Lights(void) {
  lightmodel=&Lights::MultiLightModelRawDistance;
  current=null;
  angle=0.0f;
  active=0;
  x=y=z=
  ex=ey=ez=
  nx=ny=ny=
  dx=dy=dz=0.0f;
  quadraticAttenuation=0.0f;
  t=0.0f;
  w=null;
 }
 void MultiLightModelRawDistance() {
  current->brightness=w->distanceSquaredTo(&current->position);
 }
 void MultiLightModelLambertianSquare() {  /* Lambertian surface-based brightness determination. */
  GLfloat d;
  GLfloat diffuseReflection;
  /* Determine eye point location (remember we can rotate by angle). */
  ex = 16.0f * sin(angle * (float) PI / 180.0f);
  ey = 1.0f;
  ez = 16.0f * -cos(angle * (float) PI / 180.0f);
  /* Calculated normalized object to eye position direction (nx,ny,nz). */
  nx = (ex - x);
  ny = (ey - y);
  nz = (ez - z);
  d = sqrt(nx * nx + ny * ny + nz * nz);
  nx /= d;
  ny /= d;
  nz /= d;
  /* True distance needed, take square root. */
  d = sqrt(current->quadraticAttenuation);
  /* Calculate normalized object to light postition direction (dx,dy,dz). */
  dx /= d;
  dy /= d;
  dz /= d;
  /* Dot product of object->eye and object->light source directions.
     OpenGL's lighting equations actually force the diffuse contribution
     to be zero if the dot product is less than zero.  For our purposes,
     that's too strict since we are approximating the entire object with
     a single object-to-eye normal. */
  diffuseReflection = nx * dx + ny * dy + nz * dz;
  /* Attenuate based on square of distance. */
  current->brightness = diffuseReflection / current->quadraticAttenuation;
//  } else {
//    /* Attenuate based on linear distance. */
//    current->brightness = diffuseReflection / d;
//  }
 }
 void LightModelLinear() {
  current->brightness = -quadraticAttenuation;
 }
 /*
 int Compare(ListItem *a, ListItem *b, void *value ) {
  Light *ld1 = (Light *) a;
  Light *ld2 = (Light *) b;
  GLfloat diff = (ld2->lit ? ld2->Relevance((Vertex *) value) : 0.0f) - (ld1->lit ? ld1->Relevance((Vertex *) value) : 0.0f);  // The relevant lights get sorted close to top of the list. 
  if (diff > 0)    return 1;
  if (diff < 0)    return -1;
  return 0;
 }*/
 SORTING(Light,SortBrighter,{},{
  GLfloat diff = (B->lit ? B->brightness : 0.0f) - (A->lit ? A->brightness : 0.0f);  /* The brighter lights get sorted close to top of the list. */
  if (diff < 0) result=-1;
 },{})
 void SetLightingProfile( float *ambient ) {
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
 }
 void MultiLightObjectAtVertex( Vertex *v ) {
  /*x = cos(t * 12.3f) * 2.0f;
  y = 0.0f;
  z = sin(t) * 7.0f;*/
  w=v;
  FOREACH(Light,current) {
   /* Calculate object to light position vector. */
   /*dx = (current->position.x - x);
   dy = (current->position.y - y);
   dz = (current->position.z - z);
   quadraticAttenuation = dx * dx + dy * dy + dz * dz;*/
   MultiLightModel();
  }
  this->SortBrighter();
 }
 CLEARLISTRESET(Light);
};
